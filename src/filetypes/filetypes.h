//#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#ifdef WORDS_BIG_ENDIAN
	#include <endian.h>
#endif

#include "../libxpeccy/spectrum.h"

#ifdef _WIN32
#define SLASH "\\"
#endif

#ifdef __linux__
#define SLASH "/"
#endif

#define	TYP_SNA		0
#define	TYP_Z80 	1
#define	TYP_RZX 	2
#define TYPE_TRD	3
#define TYPE_SCL	4
#define TYPE_FDI	5
#define TYPE_UDI	6
#define TYPE_HOBETA	7
#define	TYPE_TAP	8
#define	TYPE_TZX	9

#define	ERR_OK		0
#define	ERR_CANT_OPEN	1	// can't open file

#define	ERR_RZX_SIGN	0x10	// rzx signature error
#define	ERR_RZX_CRYPT	0x11	// rzx is crypted
#define	ERR_RZX_UNPACK	0x12	// rzx unpacking error

#define	ERR_TAP_DATA	0x20	// can't save tap because of not-standart blocks

#define	ERR_TZX_SIGN	0x30	// tzx signature error
#define	ERR_TZX_UNKNOWN	0x31	// tzx unsupported block

#define	ERR_TRD_LEN	0x40	// incorrect trd lenght
#define	ERR_TRD_SIGN	0x41	// not trd image
#define	ERR_TRD_SNF	0x42	// can't save trd: wrong disk structure
#define	ERR_NOTRD	0x43	// this is not trd disk

#define	ERR_HOB_CANT	0x50	// can't create hobeta @ disk

#define	ERR_UDI_SIGN	0x60	// udi signature errror

#define	ERR_FDI_SIGN	0x70	// fdi signature error
#define	ERR_FDI_HEAD	0x71	// wrong fdi heads count

#define	ERR_SCL_SIGN	0x80	// scl signature error
#define	ERR_SCL_MANY	0x81	// too many files in scl

#define	ERR_DSK_SIGN	0x88	// dsk signature error

#define	ERR_RAW_LONG	0x90	// raw file too long

unsigned short getLEWord(std::ifstream*);
unsigned short getBEWord(std::ifstream*);
unsigned int getLength(std::ifstream*,unsigned char);
void putint(unsigned char*, unsigned int);

// snapshot

int loadSNA(ZXComp*,const char*);
int loadZ80(ZXComp*,const char*);
int loadRZX(ZXComp*,const char*);
int loadDUMP(ZXComp*, const char*, int);

int saveSNA(ZXComp*,const char*,bool);

// tape

TapeBlock tapDataToBlock(char*,int,int*);

int loadTAP(Tape*,const char*);
int loadTZX(Tape*,const char*);

int saveTAP(Tape*,const char*);

// disk

void loadBoot(Floppy*);

int loadRaw(Floppy*,const char*);
int loadHobeta(Floppy*,const char*);
int loadSCL(Floppy*,const char*);
int loadTRD(Floppy*,const char*);
int loadUDI(Floppy*,const char*);
int loadFDI(Floppy*,const char*);

int saveRawFile(Floppy*,int,const char*);
int saveHobetaFile(Floppy*,int,const char*);
int saveHobeta(TRFile,char*,const char*);
int saveSCL(Floppy*,const char*);
int saveTRD(Floppy*,const char*);
int saveUDI(Floppy*,const char*);

int loadDsk(Floppy*,const char*);

// spg

int loadSPG(ZXComp*,const char*);
