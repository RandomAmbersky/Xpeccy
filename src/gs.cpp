#include "gs.h"

//extern GS* gs;

Z80EX_BYTE gsmemrd(Z80EX_CONTEXT*,Z80EX_WORD adr,int,void* ptr) {
	GS* comp = (GS*)ptr;
	return comp->mem->rd(adr);
}

void gsmemwr(Z80EX_CONTEXT*,Z80EX_WORD adr,Z80EX_BYTE val,void* ptr) {
	GS* comp = (GS*)ptr;
	comp->mem->wr(adr,val);
}

Z80EX_BYTE gsiord(Z80EX_CONTEXT*,Z80EX_WORD port,void* ptr) {
	GS* comp = (GS*)ptr;
	return comp->in(port);
}

void gsiowr(Z80EX_CONTEXT*,Z80EX_WORD port,Z80EX_BYTE val,void* ptr) {
	GS* comp = (GS*)ptr;
	comp->out(port,val);
}

Z80EX_BYTE gsintrq(Z80EX_CONTEXT*,void*) {
	return 0xff;
}

GS::GS() {
//	sys = new ZXBase(this);
	void* ptr = (void*)this;
	cpu = z80ex_create(&gsmemrd,ptr,&gsmemwr,ptr,&gsiord,ptr,&gsiowr,ptr,&gsintrq,ptr);
	mem = new Memory(MEM_GS);
	mem->pt0 = &mem->rom[0][0];
	mem->pt1 = &mem->ram[0][0];
	mem->pt2 = &mem->ram[0][0];
	mem->pt3 = &mem->ram[1][0];
	t = cnt = 0;
	pstate = 0x7e;
	flags = GS_ENABLE;
	stereo = GS_12_34;
	counter = 0;
	ch1 = ch2 = ch3 = ch4 = 0;
	vol1 = vol2 = vol3 = vol4 = 0;
}

void GS::reset() {
	z80ex_reset(cpu);
}

GSData GS::getvol() {
	GSData res;
	res.l = res.r = 0;
	if (~flags & GS_ENABLE) return res;
	switch (stereo) {
		case GS_MONO:
			res.l = res.r = ((ch1 * vol1 + ch2 * vol2 + ch3 * vol3 + ch4 * vol4) >> 9);
			break;
		case GS_12_34:
			res.l = ((ch1 * vol1 + ch2 * vol2) >> 8);
			res.r = ((ch3 * vol3 + ch4 * vol4) >> 8);
			break;
	}
	return res;
}

void GS::sync(uint32_t tk) {
	int res;
	double ln = (tk - t) * GS_FRQ / 7.0;		// scale to GS ticks;
	counter += ln;
	t = tk;
	if (~flags & GS_ENABLE) return;
	while (counter > 0) {
		res = 0;
		do {
			res += z80ex_step(cpu);
		} while (z80ex_last_op_type(cpu) != 0);
		counter -= res;
		cnt += res;
		if (cnt > 320) {	// 12MHz CLK, 37.5KHz INT -> int in each 320 ticks
			cnt -= 320;
			res = z80ex_int(cpu);
			cnt += res;
			counter -= res;
		}
	}
}

// external in/out

bool GS::extin(int prt,uint8_t* val) {
	if (~flags & GS_ENABLE) return false;	// gs disabled
	if ((prt & 0x44) != 0) return false;	// port don't catched
	if (prt & 8) {
		*val = pstate;
	} else {
		*val = pb3_gs;
		pstate &= 0x7f;		// reset b7,state
	}
	return true;
}

bool GS::extout(int prt,uint8_t val) {
	if (~flags & GS_ENABLE) return false;
	if ((prt & 0x44) != 0) return false;
	if (prt & 8) {
		pbb_zx = val;
		pstate |= 1;
	} else {
		pb3_zx = val;
		pstate |= 0x80;	// set b7,state
	}
	return true;
}

// internal in/out

uint8_t GS::in(uint16_t prt) {
	uint8_t res = 0xff;
	prt &= 0x0f;
	switch (prt) {
		case 0: break;
		case 1: res = pbb_zx; break;
		case 2: pstate &= 0x7f; res = pb3_zx; break;
		case 3: pstate |= 0x80; break;
		case 4: res = pstate; break;
		case 5: pstate &= 0xfe; break;
		case 6: break;
		case 7: break;
		case 8: break;
		case 9: break;
		case 10: if (rp0 & 0x01) pstate &= 0x7f; else pstate |= 0x80; break;
		case 11: if (vol1 & 0x20) pstate |= 1; else pstate &= 0xfe; break;
	}
	return res;
}

void GS::out(uint16_t prt,uint8_t val) {
	prt &= 0x0f;
	switch (prt) {
		case 0: rp0 = val;
			val &= 7;
			if ((val & 7) == 0) {
				mem->pt2 = &mem->rom[0][0];
				mem->pt3 = &mem->rom[1][0];
			} else {
				mem->pt2 = &mem->ram[val*2 - 2][0];
				mem->pt3 = &mem->ram[val*2 - 1][0];
			}
			break;
		case 1: break;
		case 2: pstate &= 0x7f; break;
		case 3: pstate |= 0x80; pb3_gs = val; break;
		case 4: break;
		case 5: pstate &= 0xfe; break;
		case 6: vol1 = val & 0x3f; break;
		case 7: vol2 = val & 0x3f; break;
		case 8: vol3 = val & 0x3f; break;
		case 9: vol4 = val & 0x3f; break;
		case 10: if (rp0 & 1) pstate &= 0x7f; else pstate |= 0x80; break;
		case 11: if (vol1 & 64) pstate |= 1; else pstate &= 0xfe; break;
	}
}
