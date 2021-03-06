#include <stdio.h>

#include "fdc.h"

#ifdef ISDEBUG
#define DBGOUT(args...) printf(args)
#else
#define DBGOUT(args...)
#endif

int seekADR(FDC*);	// from VG93: wait & read ADR mark in fdc->buf

// wait until all args is done
void uwargs(FDC* fdc) {
	if (fdc->comCnt == 0) {
		fdc->irq = 1;		// exec
		fdc->pos++;
	}
	fdc->wait = 1;
}

void ustp(FDC* fdc) {
	fdc->idle = 1;
	fdc->irq = 0;
	fdc->plan = NULL;
	fdc->drq = 1;
	fdc->dir = (fdc->resCnt > 0) ? 1 : 0;
}

void uResp(FDC* fdc, int len) {
	fdc->resPos = 0;
	fdc->resCnt = len;
	fdc->dir = 1;
}

void uSetDrive(FDC* fdc) {
	fdc->flp = fdc->flop[fdc->comBuf[0] & 1];
	fdc->trk = fdc->flp->trk;
	fdc->side = (fdc->comBuf[0] & 4) ? 0 : 1;
}

int uGetByte(FDC* fdc) {
	int res = 0;
	if (turbo) {
		if (!fdc->drq) {
			fdc->tmp = flpRd(fdc->flp);
			flpNext(fdc->flp, fdc->side);
			fdc->drq = 1;
			res = 1;
		}
		fdc->wait = 1;
	} else {
		if (fdc->drq) {
			fdc->sr1 |= 0x10;		// data lost
		}
		fdc->tmp = flpRd(fdc->flp);
		flpNext(fdc->flp, fdc->side);
		fdc->drq = 1;
		fdc->wait += BYTEDELAY;
		res = 1;
	}
	return res;
}

fdcCall utermTab[] = {&ustp,NULL};

void uTerm(FDC* fdc) {
	fdc->plan = utermTab;
	fdc->pos = 0;
	fdc->wait = 0;
}

// 00000011 [SRT:4.HUT:4, HLT:7.ND:1]
// specify

void uspcf00(FDC* fdc) {
	fdc->srt = (((fdc->comBuf[0] >> 4) & 0x0f) + 1) * 1000;
	fdc->hut = (((fdc->comBuf[0] & 0x0f) + 1) << 4) * 1000;
	fdc->hlt = ((fdc->comBuf[1] & 0xfe) + 2) * 1000;
	fdc->dma = (fdc->comBuf[1] & 1) ? 0 : 1;
	fdc->pos++;
}

fdcCall uSpecify[] = {&uwargs, &uspcf00, &uTerm};


typedef struct {
	int mask;
	int val;
	int argCount;
	fdcCall* plan;
} uCom;

// 00000100 [xxxxx.HD.DRV:2] R [ST3]
// drive status

void udrvst00(FDC* fdc) {
	uSetDrive(fdc);
	fdc->sr3 = (fdc->flp->protect << 6) |
		   (fdc->flp->insert << 5) |
		   ((fdc->flp->trk == 0) ? 0x10 : 0x00) |
		   (fdc->flp->doubleSide << 3) |
		   (fdc->comBuf[0] & 7);
	fdc->resBuf[0] = fdc->sr3;
	uResp(fdc, 1);
	fdc->pos++;
}

fdcCall uDrvStat[] = {&uwargs, &udrvst00, &uTerm};

// 00000111 [xxxxx0.DRV:2]
// recalibrate

void ucalib00(FDC* fdc) {
	int drv = fdc->comBuf[0] & 3;
	fdc->comBuf[0] |= 4;
	uSetDrive(fdc);
	fdc->cnt = 77;			// do 77 step back
	fdc->wait += turbo ? 1 : fdc->srt;
	fdc->state |= (1 << drv);	// set "flp is in seek mode"
	fdc->pos++;
}

void ucalib01(FDC* fdc) {
	if (fdc->flp->trk == 0) {
		fdc->sr0 &= 0x0f;
		fdc->sr0 |= 0x20;	// SR0:0010xxxx
		fdc->state &= 0xf0;
		fdc->pos++;
	} else if (fdc->cnt > 0) {
		flpStep(fdc->flp, FLP_BACK);
		fdc->wait += turbo ? 1 : fdc->srt;
		fdc->cnt--;
	} else {
		fdc->sr0 &= 0x0f;
		fdc->sr0 |= 0x70;	// SR0:0111xxxxx
		fdc->state &= 0xf0;
		fdc->pos++;
	}
}

fdcCall uCalib[] = {&uwargs,&ucalib00,&ucalib01,&uTerm};

// 00001000 R [ST0],[PCN]
// sense interrupt status

void usint00(FDC* fdc) {
	fdc->state &= 0xf0;		// clear b0..3 of main status register
	fdc->resBuf[0] = fdc->sr0;
	fdc->resBuf[1] = fdc->flp->trk;
	uResp(fdc, 2);
	fdc->sr0 |= 0x80;
	fdc->pos++;
}

fdcCall uSenseInt[] = {&uwargs,&usint00,&uTerm};

// 00001111 [xxxxx.H.DRV:2,NCN]
// seek

void useek00(FDC* fdc) {
	uSetDrive(fdc);
	fdc->trk = fdc->flp->trk;
	fdc->wait += turbo ? 1 : fdc->srt;
	fdc->pos++;
}

void useek01(FDC* fdc) {
	if (fdc->trk == fdc->comBuf[1]) {
		fdc->sr0 &= 0x0f;
		fdc->sr0 |= 0x20;
		fdc->pos++;
	} else if (fdc->trk < fdc->comBuf[1]) {
		flpStep(fdc->flp, FLP_FORWARD);
		fdc->wait += turbo ? 1 : fdc->srt;
		fdc->trk++;
	} else {
		flpStep(fdc->flp, FLP_BACK);
		fdc->wait += turbo ? 1 : fdc->srt;
		fdc->trk--;
	}
}

void useek02(FDC* fdc) {
	fdc->trk = fdc->flp->trk;
	fdc->pos++;
}

fdcCall uSeek[] = {&uwargs,&useek00,&useek01,&useek02,&uTerm};

// 0.MF.001010 [xxxxx.H.DRV:2] R [ST0,ST1,ST2,C,H,R,N]
// read ID

void urdaRS(FDC* fdc) {
	fdc->resBuf[0] = fdc->sr0;
	fdc->resBuf[1] = fdc->sr1;
	fdc->resBuf[2] = fdc->sr2;
	fdc->resBuf[3] = fdc->buf[0];	// C
	fdc->resBuf[4] = fdc->buf[1];	// H
	fdc->resBuf[5] = fdc->buf[2];	// R
	fdc->resBuf[6] = fdc->buf[3];	// N
	uResp(fdc, 7);
	fdc->pos++;
}

void urda00(FDC* fdc) {
	uSetDrive(fdc);
	if (!fdc->flp->insert) {
		fdc->sr0 |= 0x48;
		urdaRS(fdc);
		uTerm(fdc);
	} else {
		fdc->cnt = 2;
		fdc->pos++;
	}
}

void urda01(FDC* fdc) {
	int res = seekADR(fdc);
	if (res == 2) {
		fdc->cnt--;
		if (fdc->cnt == 0) {
			fdc->sr1 |= 0x01;	// missed ADR
			fdc->sr0 |= 0x40;	// exit code 01
			fdc->pos++;
		}
	} else if (res == 1) {
		fdc->pos++;
	}
}

fdcCall uReadID[] = {&uwargs,&urda00,&urda01,&urdaRS,&uTerm};

// MT.MF.SK.00110 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,DTL] R [SR0,SR1,SR2,C,H,R,N]
// MT.MF.SK.01000 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,DTL] R [SR0,SR1,SR2,C,H,R,N]
// read [deleted] data

void ureadRS(FDC* fdc) {
	fdc->resBuf[0] = fdc->sr0;
	fdc->resBuf[1] = fdc->sr1;
	fdc->resBuf[2] = fdc->sr2;
	fdc->resBuf[3] = fdc->flp->trk;
	fdc->resBuf[4] = fdc->side;
	fdc->resBuf[5] = fdc->sec;
	fdc->resBuf[6] = fdc->buf[3];
	uResp(fdc, 7);
	fdc->pos++;
}

// wait ADR
void uread01(FDC* fdc) {
	int res = seekADR(fdc);
	if (res == 2) {
		fdc->cnt--;
		if (fdc->cnt < 1) {
			fdc->sr0 |= 0x40;	// error
			fdc->sr1 |= 0x04;	// no data
			uTerm(fdc);
		}
	} else if (res == 1) {
		if (fdc->sec == fdc->buf[2]) {		// compare sec.nr
			fdc->cnt = 52;
			fdc->pos++;
		}
	}
}

int ureadCHK(FDC* fdc, int rt, int wt) {
	fdc->tmp = flpRd(fdc->flp);
	flpNext(fdc->flp, fdc->side);
	fdc->wait += turbo ? 1 : BYTEDELAY;
	if (fdc->flp->field == rt) return 1;
	if ((fdc->flp->field == wt) && (~fdc->com & 0x20)) {
		fdc->sr2 |= 0x40;
		return 1;
	}
	fdc->cnt--;
	if (fdc->cnt > 0) return 0;
	fdc->sr0 |= 0x40;
	fdc->sr1 |= 0x04;		// no data
	return 2;
}

void uread02(FDC* fdc) {		// for read data
	int res = (fdc->com & 0x08) ? ureadCHK(fdc,3,2) : ureadCHK(fdc,2,3);
	if (res == 1) {
		fdc->pos++;
		fdc->wait = 0;
	} else if (res == 2) {
		ureadRS(fdc);
		uTerm(fdc);
	}
}

void uread02d(FDC* fdc) {		// for read deleted data
	int res = ureadCHK(fdc,3,2);
	if (res == 1) {
		fdc->pos++;
		fdc->wait = 0;
	} else if (res == 2) {
		ureadRS(fdc);
		uTerm(fdc);
	}
}

void uread03(FDC* fdc) {
	if (fdc->comBuf[4] & 3) {
		fdc->cnt = 0x80 << (fdc->comBuf[4] & 3);
	} else {
		fdc->cnt = fdc->comBuf[7] + 1;			// DTL
	}
	fdc->state |= 0x10;		// wr/rd operation
	fdc->drq = 0;
	fdc->dir = 1;
	fdc->wait = BYTEDELAY;
	fdc->pos++;
}

void uread04(FDC* fdc) {
	if (!uGetByte(fdc)) return;
	fdc->data = fdc->tmp;
	fdc->cnt--;
	if (fdc->cnt < 1) {
		fdc->pos++;
		fdc->wait = BYTEDELAY;
	}
}

// check EOT, MT
void uread05(FDC* fdc) {
	fdc->state &= ~0x10;
	fdc->sec++;
	if (fdc->sr2 & 0x40) {				// SR2:CM flag set - not right sector & SK=0
		fdc->pos++;
	} else if (fdc->sec > fdc->comBuf[5]) {		// check EOT
		if ((fdc->com & 0x80) && (fdc->side == 1)) {		// multitrack (side 1->0 only)
			fdc->side = 0;
			fdc->cnt = 2;
			fdc->sec = 1;
			fdc->pos = 0;
		} else {
			fdc->pos++;
		}
	} else {
		fdc->cnt = 2;
		fdc->pos = 0;
	}
}

fdcCall uReadD01[] = {&uread01,&uread02,&uread03,&uread04,&uread05,&ureadRS,&uTerm};

void uread00(FDC* fdc) {
	uSetDrive(fdc);
	fdc->sec = fdc->comBuf[3];		// R, sec.num
	if (!fdc->flp->insert) {
		fdc->sr0 |= 0x48;	// not ready
		ureadRS(fdc);		// prepare resp
		uTerm(fdc);		// terminate
	} else {
		fdc->cnt = 2;
		fdc->wait += turbo ? 1 : fdc->hlt;	// load head
		fdc->pos++;
	}
}

void uread00a(FDC* fdc) {
	fdc->plan = uReadD01;
	fdc->pos = 0;
}

fdcCall uRdData[] = {&uwargs,&uread00,&uread00a};

// 0.MF.SK.00010 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,DTL] R [SR0,SR1,SR2,C,H,R,N]
// read track

void urdtrkRS(FDC* fdc) {
	fdc->resBuf[0] = fdc->sr0;
	fdc->resBuf[1] = fdc->sr1;
	fdc->resBuf[2] = fdc->sr2;
	fdc->resBuf[3] = fdc->flp->trk;
	fdc->resBuf[4] = fdc->side;
	fdc->resBuf[5] = fdc->sec;
	fdc->resBuf[6] = fdc->buf[3];
	uResp(fdc, 7);
	fdc->pos++;
}

void urdtrk00(FDC* fdc) {
	uSetDrive(fdc);
	fdc->sec = fdc->comBuf[3];		// R, sec.num
	if (!fdc->flp->insert) {
		fdc->sr0 |= 0x48;	// not ready
		urdtrkRS(fdc);		// prepare resp
		uTerm(fdc);		// terminate
	} else {
		fdc->sec = 0;
		fdc->sr1 |= 0x05;	// MA (reset on 1st found sector), ND (reset if specified sector found)
		fdc->pos++;
		fdc->wait = 0;
	}
}

// wait IDX
void urdtrk01(FDC* fdc) {
	if (seekADR(fdc) == 2)
		fdc->pos++;
}

// find ADR
void urdtrk02(FDC* fdc) {
	int res = seekADR(fdc);
	if (res == 2) {				// 2nd IDX
		if (fdc->sr1 & 0x01)		// if no ADR was found, error
			fdc->sr0 |= 0x40;
		urdtrkRS(fdc);
		uTerm(fdc);
	} else if (res == 1) {
		fdc->sr1 &= ~0x01;			// reset MA
		if (fdc->buf[2] == fdc->comBuf[3]) {	// if specified sector found, reset ND
			fdc->sr1 &= ~0x04;
		}
		fdc->cnt = 53;
		fdc->pos++;
	}
}

// find DATA
void urdtrk03(FDC* fdc) {
	int res = ureadCHK(fdc,2,3);
	if (res == 2) {				// IDX : end
		urdtrkRS(fdc);
		uTerm(fdc);
	} else {
		fdc->pos++;
		fdc->wait = 0;
	}
}

// uread03: prepare to data transfer
// uread04: transfer data FDD->FDC->CPU
// go next sector
void urdtrk04(FDC* fdc) {
	fdc->sec++;
	if (fdc->sec >= fdc->comBuf[5]) {	// EOT
		fdc->pos++;
	} else {
		fdc->pos = 3;
	}
}

fdcCall uRdTrk[] = {&uwargs, &urdtrk00, &urdtrk01, &urdtrk02, &urdtrk03, &uread03, &uread04, &urdtrk04, &urdtrkRS, &uTerm};

// MT.MF.SK.10001 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,STP] R [SR0,SR1,SR2,C,H,R,N]	scan equal
// MT.MF.SK.11001 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,STP] R [SR0,SR1,SR2,C,H,R,N]	scan low or equal
// MT.MF.SK.11101 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,STP] R [SR0,SR1,SR2,C,H,R,N]	scan high or equal

// uread00 : prepare FDC, fdc->sec = R
// uread01 : find ADR for sector C in 2 turns
// find DATA
void uscan00(FDC* fdc) {
	int res = ureadCHK(fdc,2,3);
	if (res == 1) {
		fdc->pos++;
		fdc->wait = 0;
	} else if (res == 2) {
		ureadRS(fdc);
		uTerm(fdc);
	}
}

// prepare compare
void uscan01(FDC* fdc) {
	fdc->sr2 &= 0xf3;
	fdc->sr2 |= 0x08;	// SH=1, SN=0
	fdc->cnt = 0x80 << (fdc->buf[3] & 3);
	fdc->drq = 0;
	fdc->dir = 1;
	fdc->wait = BYTEDELAY;
	fdc->pos++;
}

// compare whole sector
void uscan02(FDC* fdc) {
	if (!uGetByte(fdc)) return;
	if (fdc->tmp != fdc->data)
		fdc->sr2 &= ~0x08;	// tmp!=data : SH=0 for all
	switch(fdc->com & 0x0c) {
		case 0x00:
			if (fdc->tmp != fdc->data)
				fdc->sr2 |= 0x04;	// SN=1 : not meet
			break;
		case 0x80:
			if (fdc->tmp > fdc->data)
				fdc->sr2 |= 0x04;
			break;
		case 0xc0:
			if (fdc->tmp < fdc->data)
				fdc->sr2 |= 0x04;
			break;
	}
	fdc->cnt--;
	if (fdc->cnt < 1) {
		fdc->pos++;
		fdc->wait = BYTEDELAY;
	}
}

// check end
void uscan03(FDC* fdc) {
	if ((fdc->sr2 & 0x08) || (fdc->sr2 & 0x04)) {	// scan succes OR deleted sector found
		fdc->pos++;
	} else {
		fdc->sec += fdc->comBuf[7];		// STP
		if (fdc->sec >= fdc->comBuf[5]) {	// check EOT
			fdc->pos++;
		} else {
			fdc->cnt = 2;
			fdc->pos = 2;
		}
	}
}

fdcCall uScan[] = {&uwargs, &uread00, &uread01, &uscan00, &uscan01, &uscan02, &uscan03, &ureadRS, &uTerm};

// MT.MF.SK.00101 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,DTL] R [SR0,SR1,SR2,C,H,R,N]
// MT.MF.SK.01001 [xxxxx.HD.DRV:2,C,H,R,N,EOT,GPL,DTL] R [SR0,SR1,SR2,C,H,R,N]
// write [deleted] data

void uwrdat00(FDC* fdc) {
	uSetDrive(fdc);
	fdc->sec = fdc->comBuf[3];		// R, sec.num
	if (!fdc->flp->insert) {
		fdc->sr0 |= 0x48;		// not ready
		ureadRS(fdc);
		uTerm(fdc);
	} else {
		fdc->sr0 |= 0x40;
		fdc->sr1 |= 0x02;		// TRICK : force NW flag (write protect), termination
		ureadRS(fdc);
		uTerm(fdc);
	}
}

fdcCall uWrData[] = {&uwargs,&uwrdat00,&uTerm};

// 0.MF.001101 [xxxxx.H.DRV:1, N, SC, GPL, D] R [SR0,SR1,SR2,C,H,R,N]

fdcCall uFormat[] = {&uwargs,&uwrdat00,&uTerm};

// invalid op : R [SR0 = 0x80]

void uinv00(FDC* fdc) {
	fdc->sr0 = 0x80;
	fdc->resBuf[0] = fdc->sr0;
	uResp(fdc, 1);
	fdc->pos++;
}

fdcCall uInvalid[] = {&uwargs,&uinv00,&uTerm};

// common

uCom uComTab[] = {
	{0x1f, 0x06, 8, uRdData},	// xxx00110 read data
	{0x1f, 0x0c, 8, uRdData},	// xxx01100 read deleted data
	{0x3f, 0x05, 8, uWrData},	// xx000101 write data
	{0x3f, 0x09, 8, uWrData},	// xx001001 write deleted data
	{0x9f, 0x02, 8, uRdTrk},	// 0xx00010 read track		TODO:check
	{0xbf, 0x0a, 1, uReadID},	// 0x001010 read ID
	{0xbf, 0x0d, 5, uFormat},	// 0x001101 format track
	{0x1f, 0x11, 8, uScan},		// xxx10001 scan equal		TODO:check scan commands
	{0x1f, 0x19, 8, uScan},		// xxx11001 scan low or equal
	{0x1f, 0x1d, 8, uScan},		// xxx11101 scan high or equal
	{0xff, 0x07, 1, uCalib},	// 00000111 recalibrate
	{0xff, 0x08, 0, uSenseInt},	// 00001000 sense interrupt status
	{0xff, 0x03, 2, uSpecify},	// 00000011 specify
	{0xff, 0x04, 1, uDrvStat},	// 00000100 sense drive status
	{0xff, 0x0f, 2, uSeek},		// 00001111 seek
	{0x00, 0x00, 0, uInvalid}	// -------- invalid op
};

void uWrite(FDC* fdc, int adr, unsigned char val) {
	if (adr != FDC_DATA) return;
	if (fdc->idle) {			// 1st byte, command
		// DBGOUT("updCom %.2X\n",val);
		fdc->com = val;
		int idx = 0;
		while (1) {
			if ((val & uComTab[idx].mask) == uComTab[idx].val) {
				fdc->comCnt = uComTab[idx].argCount;
				fdc->plan = uComTab[idx].plan;
				// if (!fdc->plan) DBGOUT("Command %.2X not implemented\n",fdc->com);
				// assert(fdc->plan);
				fdc->pos = 0;
				fdc->comPos = 0;
				fdc->idle = 0;
				fdc->drq = 1;	// wait for args
				fdc->dir = 0;	//	from cpu
				fdc->wait = 0;
				if (fdc->com != 0x08) {
					fdc->sr0 = 0x00;
					fdc->sr1 = 0x00;
					fdc->sr2 = 0x00;
				}
				break;
			}
			idx++;
		}
	} else if (fdc->comCnt > 0) {		// command
		// DBGOUT("arg %.2X\n",val);
		fdc->comBuf[fdc->comPos] = val;
		fdc->comPos++;
		fdc->comCnt--;
		if (fdc->comCnt == 0) {
			fdc->irq = 1;		// exec
			fdc->drq = 0;
		}
	} else if (fdc->drq && !fdc->dir) {	// execution
		fdc->data = val;
		fdc->drq = 0;
	}
}

unsigned char uRead(FDC* fdc, int adr) {
	unsigned char res = 0xff;
	switch (adr) {
		case FDC_COM:
			if (fdc->idle) {
				res = 0x80 | ((fdc->resCnt > 0) ? 0x40 : 0x00);
			} else {
				res = (fdc->state & 0x1f) | (fdc->drq << 7) | (fdc->dir << 6) | (fdc->irq << 5);
			}
			break;
		case FDC_DATA:
			if (fdc->drq && fdc->dir) {
				if (fdc->irq) {			// execution
					res = fdc->data;
					fdc->drq = 0;
					//DBGOUT("%.2X ",res);
				} else if (fdc->resCnt > 0) {	// result
					res = fdc->resBuf[fdc->resPos];
					fdc->resPos++;
					fdc->resCnt--;
					if (fdc->resCnt == 0)
						fdc->dir = 0;
					//DBGOUT("resp : %.2X\n",res);
				} else {			// other
					res = 0xff;
				}
			}
			break;
	}
	return res;
}

void uReset(FDC* fdc) {
	fdc->idle = 1;
	fdc->drq = 1;
	fdc->dir = 1;
	fdc->plan = NULL;
	fdc->wait = 0;
	fdc->trk = 0;
	fdc->sec = 0;
	fdc->data = 0;
	fdc->state = 0;
	fdc->srt = 0;
	fdc->hlt = 0;
	fdc->hut = 0;
	fdc->resCnt = 0;
	fdc->resPos = 0;
}
