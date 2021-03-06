// emulation thread (non-GUI)

#include "ethread.h"
#include "xgui/xgui.h"
#include "xcore/sound.h"

unsigned char* blkData = NULL;
extern unsigned char scrn[1024 * 512 * 3];

xThread::xThread() {
	sndNs = 0;
	fast = 0;
	finish = 0;
	mtx.lock();
}

void xThread::tapeCatch() {
	int blk = comp->tape->block;
	if (blk >= comp->tape->blkCount) return;
	if (conf->tape.fast && comp->tape->blkData[blk].hasBytes) {
		unsigned short de = comp->cpu->de;
		unsigned short ix = comp->cpu->ix;
		TapeBlockInfo inf = tapGetBlockInfo(comp->tape,blk);
		blkData = (unsigned char*)realloc(blkData,inf.size + 2);
		tapGetBlockData(comp->tape,blk,blkData);
		if (inf.size == de) {
			for (int i = 0; i < de; i++) {
				memWr(comp->mem,ix,blkData[i + 1]);
				ix++;
			}
			comp->cpu->ix = ix;
			comp->cpu->de = 0;
			comp->cpu->hl = 0;
			tapNextBlock(comp->tape);
		} else {
			comp->cpu->hl = 0xff00;
		}
		comp->cpu->pc = 0x5df;
	} else {
		if (conf->tape.autostart)
			emit tapeSignal(TW_STATE,TWS_PLAY);
	}
}

void xThread::emuCycle() {
//	int endBuf = 0;
	comp->frmStrobe = 0;
	sndNs = 0;
	conf->snd.fill = 1;
	do {
		// exec 1 opcode (+ INT, NMI)
		sndNs += compExec(comp);
		if (comp->frmStrobe && !fast) {
			comp->frmStrobe = 0;
			memcpy(scrn, comp->vid->scrimg, comp->vid->vBytes);
		}
		// if need - request sound buffer update
		if (sndNs > nsPerSample) {
			sndSync(comp, 0, fast);
			sndNs -= nsPerSample;
		}
		// tape trap
		// pc =	comp->cpu->pc;
		if ((comp->mem->map[0].type == MEM_ROM) && comp->rom && !comp->dos) {		// FIXME: shit
			if (comp->cpu->pc == 0x56b) tapeCatch();
			if ((comp->cpu->pc == 0x5e2) && conf->tape.autostart)
				emit tapeSignal(TW_STATE,TWS_STOP);
		}
	} while (!comp->brk && conf->snd.fill); //(!comp->brk && !comp->frmStrobe); conf->snd.fill
	comp->nmiRequest = 0;
}

void xThread::run() {
	do {
		if (!fast) mtx.lock();		// wait until unlocked (MainWin::onTimer() or at exit)
		if (finish) break;
		if (comp->rzx.start) {
			comp->rzx.start = 0;
			comp->rzx.play = 1;
			comp->rzx.fCount = 0;
			comp->rzx.fCurrent = 0;
			rewind(comp->rzx.file);
			rzxGetFrame(comp);
		}
		if (!block && !comp->brk) {
			emuCycle();
			if (comp->brk) {
				emit dbgRequest();
			}
		}
	} while (1);
	mtx.unlock();
	exit(0);
}
