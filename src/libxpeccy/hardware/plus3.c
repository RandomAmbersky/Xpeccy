#include "../spectrum.h"

// out

/*
void p3OutFE(ZXComp* comp, unsigned short port, unsigned char val) {
	comp->vid->nextbrd = val & 0x07;
	if (!comp->vid->border4t)
		comp->vid->brdcol = val & 0x07;
	comp->beeplev = (val & 0x10) ? 1 : 0;
	comp->tape->levRec = (val & 0x08) ? 1 : 0;
}
*/

void p3Out1FFD(Computer* comp, unsigned short port, unsigned char val) {
	comp->p1FFD = val;
	comp->dif->fdc->flp->motor = (val & 8) ? 1 : 0;
	pl2MapMem(comp);
}

void p3Out7FFD(Computer* comp, unsigned short port, unsigned char val) {
	if (comp->p7FFD & 0x20) return;
	comp->rom = (val & 0x10) ? 1 : 0;
	comp->p7FFD = val;
	comp->vid->curscr = (val & 0x08) ? 7 : 5;
	pl2MapMem(comp);
}

xPort p3PortMap[] = {
	{0xc002,0x7ffd,2,2,2,NULL,	p3Out7FFD},
	{0xc002,0xbffd,2,2,2,NULL,	xOutBFFD},
	{0xc002,0xfffd,2,2,2,xInFFFD,	xOutFFFD},
	{0xf002,0x1ffd,2,2,2,NULL,	p3Out1FFD},
	{0x0003,0x00fe,2,2,2,xInFE,	xOutFE},
	{0x0000,0x0000,2,2,2,NULL,	NULL}
};

void pl3Out(Computer* comp, unsigned short port, unsigned char val, int dos) {
	difOut(comp->dif, port, val, 0);
	hwOut(p3PortMap, comp, port, val, 0);
}

unsigned char pl3In(Computer* comp, unsigned short port, int dos) {
	unsigned char res = 0xff;
	if (difIn(comp->dif, port, &res, 0)) return res;
	res = hwIn(p3PortMap, comp, port, 0);
	return res;
}
