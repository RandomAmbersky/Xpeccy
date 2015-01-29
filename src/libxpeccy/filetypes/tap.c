#include <stdlib.h>
#include "filetypes.h"

TapeBlock tapDataToBlock(char* data,int len,int* sigLens) {
	TapeBlock block;
	int i;
	char* ptr = data;
	char tmp = data[0];		// block type
	block.plen = sigLens[0];
	block.s1len = sigLens[1];
	block.s2len = sigLens[2];
	block.len0 = sigLens[3];
	block.len1 = sigLens[4];
	block.pdur = (sigLens[6] == -1) ? ((tmp == 0) ? 8063 : 3223) : sigLens[6];
	block.breakPoint = 0;
	block.hasBytes = 1;
	block.isHeader = (tmp == 0) ? 1 : 0;
	block.sigCount = 0;
	block.sigData = NULL;
	//block.data.clear();
	for (i = 0; i < (int)block.pdur; i++)
		blkAddPulse(&block,block.plen);
	if (block.s1len != 0)
		blkAddPulse(&block,block.s1len);
	if (block.s2len != 0)
		blkAddPulse(&block,block.s2len);
	block.dataPos = block.sigCount;
	for (i = 0; i < len; i++) {
		blkAddByte(&block,*ptr,0,0);
		ptr++;
	}
//	printf("tapDataToBlock: %i bytes -> %i signals\t datapos = %i\n",len,block.sigCount,block.dataPos);
	return block;
}

int loadTAP(Tape* tape, const char* name) {
	FILE* file = fopen(name, "rb");
	if (!file) return ERR_CANT_OPEN;

	unsigned short len;
	char blockBuf[0x10000];
	TapeBlock block;
	int sigLens[] = {PILOTLEN,SYNC1LEN,SYNC2LEN,SIGN0LEN,SIGN1LEN,SYNC3LEN,-1};
	tapEject(tape);

	while (!feof(file)) {
		len = fgetwLE(file);
		if (!feof(file)) {
			fread(blockBuf, len, 1, file);
			block = tapDataToBlock(blockBuf, len, sigLens);
			blkAddPulse(&block, sigLens[5]);
			blkAddPause(&block, (block.pdur == 8063) ? 500 : 1000);		// pause
			tapAddBlock(tape, block);
			blkClear(&block);
		}
	}
	fclose(file);

	tape->isData = 1;
	tape->path = (char*)realloc(tape->path,sizeof(char) * (strlen(name) + 1));
	strcpy(tape->path,name);

	return ERR_OK;
}

int saveTAP(Tape* tape, const char* name) {
	if (!tape->isData) return ERR_TAP_DATA;
	FILE* file = fopen(name, "wb");
	if (!file) return ERR_CANT_OPEN;

	TapeBlockInfo inf;
	unsigned char blockData[0x100000];

	for (int i = 0; i < tape->blkCount; i++) {
		inf = tapGetBlockInfo(tape, i);
		inf.size += 2;	// +flag +crc
		tapGetBlockData(tape, i, blockData);
		fputwLE(file, inf.size);
		fwrite((char*)blockData, inf.size, 1, file);
	}
	fclose(file);

	tape->path = (char*)realloc(tape->path,sizeof(char) * (strlen(name) + 1));
	strcpy(tape->path,name);

	return ERR_OK;
}
