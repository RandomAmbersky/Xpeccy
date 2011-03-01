// NOTE	CHECKED. NO MINES HERE

// ld rp,nn
void npr01(Spec *p) {p->cpu->c = p->mem->rd(p->cpu->pc++); p->cpu->b = p->mem->rd(p->cpu->pc++);}
void npr11(Spec *p) {p->cpu->e = p->mem->rd(p->cpu->pc++); p->cpu->d = p->mem->rd(p->cpu->pc++);}
void npr21(Spec *p) {p->cpu->l = p->mem->rd(p->cpu->pc++); p->cpu->h = p->mem->rd(p->cpu->pc++);}
void npr31(Spec *p) {p->cpu->lsp = p->mem->rd(p->cpu->pc++); p->cpu->hsp = p->mem->rd(p->cpu->pc++);}
// inc rp
void npr03(Spec *p) {p->cpu->bc++;}
void npr13(Spec *p) {p->cpu->de++;}
void npr23(Spec *p) {p->cpu->hl++;}
void npr33(Spec *p) {p->cpu->sp++;}
// dec rp
void npr0B(Spec *p) {p->cpu->bc--;}
void npr1B(Spec *p) {p->cpu->de--;}
void npr2B(Spec *p) {p->cpu->hl--;}
void npr3B(Spec *p) {p->cpu->sp--;}

// ld r,n
void npr06(Spec *p) {p->cpu->b = p->mem->rd(p->cpu->pc++);}
void npr0E(Spec *p) {p->cpu->c = p->mem->rd(p->cpu->pc++);}
void npr16(Spec *p) {p->cpu->d = p->mem->rd(p->cpu->pc++);}
void npr1E(Spec *p) {p->cpu->e = p->mem->rd(p->cpu->pc++);}
void npr26(Spec *p) {p->cpu->h = p->mem->rd(p->cpu->pc++);}
void npr2E(Spec *p) {p->cpu->l = p->mem->rd(p->cpu->pc++);}
void npr36(Spec *p) {p->mem->wr(p->cpu->hl,p->mem->rd(p->cpu->pc++));}
void npr3E(Spec *p) {p->cpu->a = p->mem->rd(p->cpu->pc++);}
// inc r
void npr04(Spec *p) {p->cpu->b++; p->cpu->f = flag[p->cpu->b].inc | (p->cpu->f & FC);}
void npr0C(Spec *p) {p->cpu->c++; p->cpu->f = flag[p->cpu->c].inc | (p->cpu->f & FC);}
void npr14(Spec *p) {p->cpu->d++; p->cpu->f = flag[p->cpu->d].inc | (p->cpu->f & FC);}
void npr1C(Spec *p) {p->cpu->e++; p->cpu->f = flag[p->cpu->e].inc | (p->cpu->f & FC);}
void npr24(Spec *p) {p->cpu->h++; p->cpu->f = flag[p->cpu->h].inc | (p->cpu->f & FC);}
void npr2C(Spec *p) {p->cpu->l++; p->cpu->f = flag[p->cpu->l].inc | (p->cpu->f & FC);}
void npr34(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->hl) + 1; p->mem->wr(p->cpu->hl,p->cpu->x); p->cpu->f = flag[p->cpu->x].inc | (p->cpu->f & FC);}
void npr3C(Spec *p) {p->cpu->a++; p->cpu->f = flag[p->cpu->a].inc | (p->cpu->f & FC);}
// dec r
void npr05(Spec *p) {p->cpu->b--; p->cpu->f = flag[p->cpu->b].dec | (p->cpu->f & FC);}
void npr0D(Spec *p) {p->cpu->c--; p->cpu->f = flag[p->cpu->c].dec | (p->cpu->f & FC);}
void npr15(Spec *p) {p->cpu->d--; p->cpu->f = flag[p->cpu->d].dec | (p->cpu->f & FC);}
void npr1D(Spec *p) {p->cpu->e--; p->cpu->f = flag[p->cpu->e].dec | (p->cpu->f & FC);}
void npr25(Spec *p) {p->cpu->h--; p->cpu->f = flag[p->cpu->h].dec | (p->cpu->f & FC);}
void npr2D(Spec *p) {p->cpu->l--; p->cpu->f = flag[p->cpu->l].dec | (p->cpu->f & FC);}
void npr35(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->hl) - 1; p->mem->wr(p->cpu->hl,p->cpu->x); p->cpu->f = flag[p->cpu->x].dec | (p->cpu->f & FC);}
void npr3D(Spec *p) {p->cpu->a--; p->cpu->f = flag[p->cpu->a].dec | (p->cpu->f & FC);}

// rlca,rrca,rla,rra
void npr07(Spec *p) {p->cpu->a = flag[p->cpu->a].rlc.r; p->cpu->f = (p->cpu->f & (FS | FZ | FP)) | (p->cpu->a & (F5 | F3 | FC));}
void npr0F(Spec *p) {p->cpu->a = flag[p->cpu->a].rrc.r; p->cpu->f = (p->cpu->f & (FS | FZ | FP)) | (p->cpu->a & (F5 | F3)) | ((p->cpu->a & 128)?FC:0);}
void npr17(Spec *p) {bool z = (p->cpu->a & 128); p->cpu->a = flag[p->cpu->a].rl[p->cpu->f & FC].r; p->cpu->f = (p->cpu->f & (FS | FZ | FP)) | (p->cpu->a & (F5 | F3)) | (z?FC:0);}
void npr1F(Spec *p) {bool z = (p->cpu->a & 1); p->cpu->a = flag[p->cpu->a].rr[p->cpu->f & FC].r; p->cpu->f = (p->cpu->f & (FS | FZ | FP)) | (p->cpu->a & (F5 | F3)) | (z?FC:0);}

// jr	mptr = addr when condition is true
void npr18(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); p->cpu->pc += (signed char)p->cpu->x; p->cpu->mptr = p->cpu->pc;}
void npr20(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); if (~p->cpu->f & FZ) {p->cpu->pc += (signed char)p->cpu->x; p->cpu->t += 5; p->cpu->mptr = p->cpu->pc;}}
void npr28(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); if (p->cpu->f & FZ) {p->cpu->pc += (signed char)p->cpu->x; p->cpu->t += 5; p->cpu->mptr = p->cpu->pc;}}
void npr30(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); if (~p->cpu->f & FC) {p->cpu->pc += (signed char)p->cpu->x; p->cpu->t += 5; p->cpu->mptr = p->cpu->pc;}}
void npr38(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); if (p->cpu->f & FC) {p->cpu->pc += (signed char)p->cpu->x; p->cpu->t += 5; p->cpu->mptr = p->cpu->pc;}}

// ld (bc|de),a; ld a,(bc|de)
void npr02(Spec *p) {p->mem->wr(p->cpu->bc,p->cpu->a); p->cpu->lptr = p->cpu->c + 1; p->cpu->hptr = p->cpu->a;}		// mptr = (a << 8) + ((bc + 1) & 0xff)
void npr12(Spec *p) {p->mem->wr(p->cpu->de,p->cpu->a); p->cpu->lptr = p->cpu->e + 1; p->cpu->hptr = p->cpu->a;}		// mptr = (a << 8) + ((de + 1) & 0xff)
void npr0A(Spec *p) {p->cpu->a = p->mem->rd(p->cpu->bc); p->cpu->mptr = p->cpu->bc + 1;}			// mptr = bc + 1
void npr1A(Spec *p) {p->cpu->a = p->mem->rd(p->cpu->de); p->cpu->mptr = p->cpu->de + 1;}			// mptr = de + 1
// ld (nn),[hl|a]; ld [hl|a],(nn)
void npr22(Spec *p) {p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->hptr = p->mem->rd(p->cpu->pc++); p->mem->wr(p->cpu->mptr++,p->cpu->l); p->mem->wr(p->cpu->mptr,p->cpu->h);}	// mptr = nn + 1
void npr2A(Spec *p) {p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->hptr = p->mem->rd(p->cpu->pc++); p->cpu->l = p->mem->rd(p->cpu->mptr++); p->cpu->h = p->mem->rd(p->cpu->mptr);}		// mptr = nn + 1
void npr32(Spec *p) {p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->hptr = p->mem->rd(p->cpu->pc++); p->mem->wr(p->cpu->mptr++,p->cpu->a); p->cpu->hptr = p->cpu->a;}				// mptr = (A << 8) + ((adr + 1) & 0xff)
void npr3A(Spec *p) {p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->hptr = p->mem->rd(p->cpu->pc++); p->cpu->a = p->mem->rd(p->cpu->mptr++);}						// mptr = nn + 1
// add hl,rp	mptr = hl + 1 (before adding)
void addHL(Spec *p,int op) {
	int z = p->cpu->hl;
	p->cpu->mptr = p->cpu->hl + 1;
	p->cpu->hl += op;
	p->cpu->f = (p->cpu->f & (FS | FZ | FP)) | (p->cpu->h & (F5 | F3)) | ((((z & 0xfff) + (op & 0xfff)) > 0xfff)?FH:0) | (((z + op) > 0xffff)?FC:0);
}
void npr09(Spec *p) {addHL(p,p->cpu->bc);}
void npr19(Spec *p) {addHL(p,p->cpu->de);}
void npr29(Spec *p) {addHL(p,p->cpu->hl);}
void npr39(Spec *p) {addHL(p,p->cpu->sp);}

// others (nop,exa,djnz,daa,cpl,scf,ccf
void npr00(Spec*) {}			// nop
void npr08(Spec *p) {int tmp = p->cpu->af; p->cpu->af = p->cpu->alt.af; p->cpu->alt.af = tmp;}	// ex af,af'
void npr10(Spec *p) {p->cpu->dlt = p->mem->rd(p->cpu->pc++); p->cpu->b--; if (p->cpu->b != 0) {p->cpu->pc += (signed char)p->cpu->dlt; p->cpu->t += 5; p->cpu->mptr = p->cpu->pc;}}	// mptr = addr when b!=0
void npr27(Spec *p) {
	switch (p->cpu->f & (FH | FN | FC)) {
		case 0: if ((p->cpu->a & 15) < 10) {
				if (p->cpu->a > 0x9f) {p->cpu->a += 0x60; p->cpu->f |= FC;}
			} else {
				if (p->cpu->a < 0x90) {p->cpu->a += 6; p->cpu->f &= ~FC;} else {p->cpu->a += 0x66; p->cpu->f |= FC;}
			} break;
		case FH: if (p->cpu->a < 0xa0) {p->cpu->a += 6; p->cpu->f &= ~FC;} else {p->cpu->a += 0x66; p->cpu->f |= FC;} break;
		case FC: p->cpu->a += (((p->cpu->a & 15) > 9) ? 0x66 : 0x60); p->cpu->f |= FC; break;
		case (FH | FC): p->cpu->a += 0x66; p->cpu->f |= FC; break;
//		case FN: break;
		case (FH | FN): p->cpu->a -= 0x06; p->cpu->f &= ~FC; break;
		case (FN | FC): p->cpu->a -= 0x60; p->cpu->f |= FC; break;
		case (FH | FN | FC): p->cpu->a -= 0x66; p->cpu->f |= FC; break;
	}
	p->cpu->f = (p->cpu->f & (FN | FC)) | (p->cpu->a & (FS | F5 | F3)) | ((p->cpu->a == 0)?FZ:0) | (parity(p->cpu->a)?FP:0);
}
void npr2F(Spec *p) {p->cpu->a ^= 255; p->cpu->f = (p->cpu->f & ~(F5 | F3)) | (p->cpu->a & (F5 | F3)) | FH | FN;}
void npr37(Spec *p) {p->cpu->f = (p->cpu->f & ~(FH | FN)) | (p->cpu->a & (F5 | F3)) | FC;}
void npr3F(Spec *p) {p->cpu->f = ((p->cpu->f & ~(FH | FN)) | ((p->cpu->f & FC)?FH:0) | (p->cpu->a & (F5 | F3))) ^ FC;}

// ld b,r
void npr40(Spec*) {}
void npr41(Spec *p) {p->cpu->b = p->cpu->c;}
void npr42(Spec *p) {p->cpu->b = p->cpu->d;}
void npr43(Spec *p) {p->cpu->b = p->cpu->e;}
void npr44(Spec *p) {p->cpu->b = p->cpu->h;}
void npr45(Spec *p) {p->cpu->b = p->cpu->l;}
void npr46(Spec *p) {p->cpu->b = p->mem->rd(p->cpu->hl);}
void npr47(Spec *p) {p->cpu->b = p->cpu->a;}
// ld c,r
void npr48(Spec *p) {p->cpu->c = p->cpu->b;}
void npr49(Spec*) {}
void npr4A(Spec *p) {p->cpu->c = p->cpu->d;}
void npr4B(Spec *p) {p->cpu->c = p->cpu->e;}
void npr4C(Spec *p) {p->cpu->c = p->cpu->h;}
void npr4D(Spec *p) {p->cpu->c = p->cpu->l;}
void npr4E(Spec *p) {p->cpu->c = p->mem->rd(p->cpu->hl);}
void npr4F(Spec *p) {p->cpu->c = p->cpu->a;}
// ld d,r
void npr50(Spec *p) {p->cpu->d = p->cpu->b;}
void npr51(Spec *p) {p->cpu->d = p->cpu->c;}
void npr52(Spec*) {}
void npr53(Spec *p) {p->cpu->d = p->cpu->e;}
void npr54(Spec *p) {p->cpu->d = p->cpu->h;}
void npr55(Spec *p) {p->cpu->d = p->cpu->l;}
void npr56(Spec *p) {p->cpu->d = p->mem->rd(p->cpu->hl);}
void npr57(Spec *p) {p->cpu->d = p->cpu->a;}
// ld e,r
void npr58(Spec *p) {p->cpu->e = p->cpu->b;}
void npr59(Spec *p) {p->cpu->e = p->cpu->c;}
void npr5A(Spec *p) {p->cpu->e = p->cpu->d;}
void npr5B(Spec*) {}
void npr5C(Spec *p) {p->cpu->e = p->cpu->h;}
void npr5D(Spec *p) {p->cpu->e = p->cpu->l;}
void npr5E(Spec *p) {p->cpu->e = p->mem->rd(p->cpu->hl);}
void npr5F(Spec *p) {p->cpu->e = p->cpu->a;}
// ld h,r
void npr60(Spec *p) {p->cpu->h = p->cpu->b;}
void npr61(Spec *p) {p->cpu->h = p->cpu->c;}
void npr62(Spec *p) {p->cpu->h = p->cpu->d;}
void npr63(Spec *p) {p->cpu->h = p->cpu->e;}
void npr64(Spec*) {}
void npr65(Spec *p) {p->cpu->h = p->cpu->l;}
void npr66(Spec *p) {p->cpu->h = p->mem->rd(p->cpu->hl);}
void npr67(Spec *p) {p->cpu->h = p->cpu->a;}
// ld l,r
void npr68(Spec *p) {p->cpu->l = p->cpu->b;}
void npr69(Spec *p) {p->cpu->l = p->cpu->c;}
void npr6A(Spec *p) {p->cpu->l = p->cpu->d;}
void npr6B(Spec *p) {p->cpu->l = p->cpu->e;}
void npr6C(Spec *p) {p->cpu->l = p->cpu->h;}
void npr6D(Spec*) {}
void npr6E(Spec *p) {p->cpu->l = p->mem->rd(p->cpu->hl);}
void npr6F(Spec *p) {p->cpu->l = p->cpu->a;}
// ld (hl),r
void npr70(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->b);}
void npr71(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->c);}
void npr72(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->d);}
void npr73(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->e);}
void npr74(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->h);}
void npr75(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->l);}
void npr76(Spec *p) {if (!p->istrb) p->cpu->pc--;}
void npr77(Spec *p) {p->mem->wr(p->cpu->hl,p->cpu->a);}
// ld a,r
void npr78(Spec *p) {p->cpu->a = p->cpu->b;}
void npr79(Spec *p) {p->cpu->a = p->cpu->c;}
void npr7A(Spec *p) {p->cpu->a = p->cpu->d;}
void npr7B(Spec *p) {p->cpu->a = p->cpu->e;}
void npr7C(Spec *p) {p->cpu->a = p->cpu->h;}
void npr7D(Spec *p) {p->cpu->a = p->cpu->l;}
void npr7E(Spec *p) {p->cpu->a = p->mem->rd(p->cpu->hl);}
void npr7F(Spec*) {}

// add a,r
void npr80(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->b][0]; p->cpu->a += p->cpu->b;}
void npr81(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->c][0]; p->cpu->a += p->cpu->c;}
void npr82(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->d][0]; p->cpu->a += p->cpu->d;}
void npr83(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->e][0]; p->cpu->a += p->cpu->e;}
void npr84(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->h][0]; p->cpu->a += p->cpu->h;}
void npr85(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->l][0]; p->cpu->a += p->cpu->l;}
void npr86(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->hl); p->cpu->f = flag[p->cpu->a].add[p->cpu->x][0]; p->cpu->a += p->cpu->x;}
void npr87(Spec *p) {p->cpu->f = flag[p->cpu->a].add[p->cpu->a][0]; p->cpu->a += p->cpu->a;}
// adc a,r
void adcXX(Spec *p, unsigned char op) {
	int z = (p->cpu->f & FC)?1:0;
	p->cpu->f = flag[p->cpu->a].add[op][z];
	p->cpu->a += (op + z);
}
void npr88(Spec *p) {adcXX(p,p->cpu->b);}
void npr89(Spec *p) {adcXX(p,p->cpu->c);}
void npr8A(Spec *p) {adcXX(p,p->cpu->d);}
void npr8B(Spec *p) {adcXX(p,p->cpu->e);}
void npr8C(Spec *p) {adcXX(p,p->cpu->h);}
void npr8D(Spec *p) {adcXX(p,p->cpu->l);}
void npr8E(Spec *p) {adcXX(p,p->mem->rd(p->cpu->hl));}
void npr8F(Spec *p) {adcXX(p,p->cpu->a);}
// sub r
void npr90(Spec *p) {p->cpu->f = flag[p->cpu->a].sub[p->cpu->b][0]; p->cpu->a -= p->cpu->b;}
void npr91(Spec *p) {p->cpu->f = flag[p->cpu->a].sub[p->cpu->c][0]; p->cpu->a -= p->cpu->c;}
void npr92(Spec *p) {p->cpu->f = flag[p->cpu->a].sub[p->cpu->d][0]; p->cpu->a -= p->cpu->d;}
void npr93(Spec *p) {p->cpu->f = flag[p->cpu->a].sub[p->cpu->e][0]; p->cpu->a -= p->cpu->e;}
void npr94(Spec *p) {p->cpu->f = flag[p->cpu->a].sub[p->cpu->h][0]; p->cpu->a -= p->cpu->h;}
void npr95(Spec *p) {p->cpu->f = flag[p->cpu->a].sub[p->cpu->l][0]; p->cpu->a -= p->cpu->l;}
void npr96(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->hl); p->cpu->f = flag[p->cpu->a].sub[p->cpu->x][0]; p->cpu->a -= p->cpu->x;}
void npr97(Spec *p) {p->cpu->f = FZ | FN; p->cpu->a = 0;}
// sbc a,r
void sbcXX(Spec *p,unsigned char op) {
	int z = (p->cpu->f & FC)?1:0;
	p->cpu->f = flag[p->cpu->a].sub[op][z];
	p->cpu->a -= (op + z);
}
void npr98(Spec *p) {sbcXX(p,p->cpu->b);}
void npr99(Spec *p) {sbcXX(p,p->cpu->c);}
void npr9A(Spec *p) {sbcXX(p,p->cpu->d);}
void npr9B(Spec *p) {sbcXX(p,p->cpu->e);}
void npr9C(Spec *p) {sbcXX(p,p->cpu->h);}
void npr9D(Spec *p) {sbcXX(p,p->cpu->l);}
void npr9E(Spec *p) {sbcXX(p,p->mem->rd(p->cpu->hl));}
void npr9F(Spec *p) {sbcXX(p,p->cpu->a);}

// and r
void nprA0(Spec *p) {p->cpu->a &= p->cpu->b; p->cpu->f = flag[p->cpu->a].andf;}
void nprA1(Spec *p) {p->cpu->a &= p->cpu->c; p->cpu->f = flag[p->cpu->a].andf;}
void nprA2(Spec *p) {p->cpu->a &= p->cpu->d; p->cpu->f = flag[p->cpu->a].andf;}
void nprA3(Spec *p) {p->cpu->a &= p->cpu->e; p->cpu->f = flag[p->cpu->a].andf;}
void nprA4(Spec *p) {p->cpu->a &= p->cpu->h; p->cpu->f = flag[p->cpu->a].andf;}
void nprA5(Spec *p) {p->cpu->a &= p->cpu->l; p->cpu->f = flag[p->cpu->a].andf;}
void nprA6(Spec *p) {p->cpu->a &= p->mem->rd(p->cpu->hl); p->cpu->f = flag[p->cpu->a].andf;}
void nprA7(Spec *p) {p->cpu->f = flag[p->cpu->a].andf;}
// xor r
void nprA8(Spec *p) {p->cpu->a ^= p->cpu->b; p->cpu->f = flag[p->cpu->a].orf;}
void nprA9(Spec *p) {p->cpu->a ^= p->cpu->c; p->cpu->f = flag[p->cpu->a].orf;}
void nprAA(Spec *p) {p->cpu->a ^= p->cpu->d; p->cpu->f = flag[p->cpu->a].orf;}
void nprAB(Spec *p) {p->cpu->a ^= p->cpu->e; p->cpu->f = flag[p->cpu->a].orf;}
void nprAC(Spec *p) {p->cpu->a ^= p->cpu->h; p->cpu->f = flag[p->cpu->a].orf;}
void nprAD(Spec *p) {p->cpu->a ^= p->cpu->l; p->cpu->f = flag[p->cpu->a].orf;}
void nprAE(Spec *p) {p->cpu->a ^= p->mem->rd(p->cpu->hl); p->cpu->f = flag[p->cpu->a].orf;}
void nprAF(Spec *p) {p->cpu->a = 0; p->cpu->f = FZ | FP;}
// or r
void nprB0(Spec *p) {p->cpu->a |= p->cpu->b; p->cpu->f = flag[p->cpu->a].orf;}
void nprB1(Spec *p) {p->cpu->a |= p->cpu->c; p->cpu->f = flag[p->cpu->a].orf;}
void nprB2(Spec *p) {p->cpu->a |= p->cpu->d; p->cpu->f = flag[p->cpu->a].orf;}
void nprB3(Spec *p) {p->cpu->a |= p->cpu->e; p->cpu->f = flag[p->cpu->a].orf;}
void nprB4(Spec *p) {p->cpu->a |= p->cpu->h; p->cpu->f = flag[p->cpu->a].orf;}
void nprB5(Spec *p) {p->cpu->a |= p->cpu->l; p->cpu->f = flag[p->cpu->a].orf;}
void nprB6(Spec *p) {p->cpu->a |= p->mem->rd(p->cpu->hl); p->cpu->f = flag[p->cpu->a].orf;}
void nprB7(Spec *p) {p->cpu->f = flag[p->cpu->a].orf;}
// cp r
void nprB8(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->b];}
void nprB9(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->c];}
void nprBA(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->d];}
void nprBB(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->e];}
void nprBC(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->h];}
void nprBD(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->l];}
void nprBE(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->mem->rd(p->cpu->hl)];}
void nprBF(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->cpu->a];}		// flag[p->cpu->a].cp[p->cpu->a]

// ret cc	mptr = ret.addr when condition is true
void nprC9(Spec *p) {p->cpu->lpc = p->mem->rd(p->cpu->sp++); p->cpu->hpc = p->mem->rd(p->cpu->sp++); p->cpu->mptr = p->cpu->pc;}
void nprC0(Spec *p) {if (!(p->cpu->f & FZ)) {nprC9(p); p->cpu->t += 6;}}
void nprC8(Spec *p) {if (p->cpu->f & FZ) {nprC9(p); p->cpu->t += 6;}}
void nprD0(Spec *p) {if (!(p->cpu->f & FC)) {nprC9(p); p->cpu->t += 6;}}
void nprD8(Spec *p) {if (p->cpu->f & FC) {nprC9(p); p->cpu->t += 6;}}
void nprE0(Spec *p) {if (!(p->cpu->f & FP)) {nprC9(p); p->cpu->t += 6;}}
void nprE8(Spec *p) {if (p->cpu->f & FP) {nprC9(p); p->cpu->t += 6;}}
void nprF0(Spec *p) {if (!(p->cpu->f & FS)) {nprC9(p); p->cpu->t += 6;}}
void nprF8(Spec *p) {if (p->cpu->f & FS) {nprC9(p); p->cpu->t += 6;}}
// jp cc
void jumpif(Spec *p, bool cnd) {p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->hptr = p->mem->rd(p->cpu->pc++); if (cnd) p->cpu->pc = p->cpu->mptr;}
void nprC3(Spec *p) {jumpif(p, true);}
void nprC2(Spec *p) {jumpif (p, !(p->cpu->f & FZ));}
void nprCA(Spec *p) {jumpif(p, p->cpu->f & FZ);}
void nprD2(Spec *p) {jumpif(p, !(p->cpu->f & FC));}
void nprDA(Spec *p) {jumpif(p, p->cpu->f & FC);}
void nprE2(Spec *p) {jumpif(p, !(p->cpu->f & FP));}
void nprEA(Spec *p) {jumpif(p, p->cpu->f & FP);}
void nprF2(Spec *p) {jumpif(p, !(p->cpu->f & FS));}
void nprFA(Spec *p) {jumpif(p, p->cpu->f & FS);}
// call cc
void callif(Spec *p, bool cnd) {
	p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->hptr = p->mem->rd(p->cpu->pc++);
	if (cnd) {p->cpu->t += 7; p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = p->cpu->mptr;}
}
void nprCD(Spec *p) {callif(p, true);}
void nprC4(Spec *p) {callif(p, !(p->cpu->f & FZ));}
void nprCC(Spec *p) {callif(p, p->cpu->f & FZ);}
void nprD4(Spec *p) {callif(p, !(p->cpu->f & FC));}
void nprDC(Spec *p) {callif(p, p->cpu->f & FC);}
void nprE4(Spec *p) {callif(p, !(p->cpu->f & FP));}
void nprEC(Spec *p) {callif(p, p->cpu->f & FP);}
void nprF4(Spec *p) {callif(p, !(p->cpu->f & FS));}
void nprFC(Spec *p) {callif(p, p->cpu->f & FS);}
// pop
void nprC1(Spec *p) {p->cpu->c = p->mem->rd(p->cpu->sp++); p->cpu->b = p->mem->rd(p->cpu->sp++);}
void nprD1(Spec *p) {p->cpu->e = p->mem->rd(p->cpu->sp++); p->cpu->d = p->mem->rd(p->cpu->sp++);}
void nprE1(Spec *p) {p->cpu->l = p->mem->rd(p->cpu->sp++); p->cpu->h = p->mem->rd(p->cpu->sp++);}
void nprF1(Spec *p) {p->cpu->f = p->mem->rd(p->cpu->sp++); p->cpu->a = p->mem->rd(p->cpu->sp++);}
// push
void nprC5(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->b); p->mem->wr(--p->cpu->sp,p->cpu->c);}
void nprD5(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->d); p->mem->wr(--p->cpu->sp,p->cpu->e);}
void nprE5(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->h); p->mem->wr(--p->cpu->sp,p->cpu->l);}
void nprF5(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->a); p->mem->wr(--p->cpu->sp,p->cpu->f);}
// ariphmetic / logic
void nprC6(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); p->cpu->f = flag[p->cpu->a].add[p->cpu->x][0]; p->cpu->a += p->cpu->x;}
void nprCE(Spec *p) {adcXX(p,p->mem->rd(p->cpu->pc++));}
void nprD6(Spec *p) {p->cpu->x = p->mem->rd(p->cpu->pc++); p->cpu->f = flag[p->cpu->a].sub[p->cpu->x][0]; p->cpu->a -= p->cpu->x;}
void nprDE(Spec *p) {sbcXX(p,p->mem->rd(p->cpu->pc++));}
void nprE6(Spec *p) {p->cpu->a &= p->mem->rd(p->cpu->pc++); p->cpu->f = flag[p->cpu->a].andf;}
void nprEE(Spec *p) {p->cpu->a ^= p->mem->rd(p->cpu->pc++); p->cpu->f = flag[p->cpu->a].orf;}
void nprF6(Spec *p) {p->cpu->a |= p->mem->rd(p->cpu->pc++); p->cpu->f = flag[p->cpu->a].orf;}
void nprFE(Spec *p) {p->cpu->f = flag[p->cpu->a].cp[p->mem->rd(p->cpu->pc++)];}
// rst n	mptr = adr
void nprC7(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x00; p->cpu->mptr = p->cpu->pc;}
void nprCF(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x08; p->cpu->mptr = p->cpu->pc;}
void nprD7(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x10; p->cpu->mptr = p->cpu->pc;}
void nprDF(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x18; p->cpu->mptr = p->cpu->pc;}
void nprE7(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x20; p->cpu->mptr = p->cpu->pc;}
void nprEF(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x28; p->cpu->mptr = p->cpu->pc;}
void nprF7(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x30; p->cpu->mptr = p->cpu->pc;}
void nprFF(Spec *p) {p->mem->wr(--p->cpu->sp,p->cpu->hpc); p->mem->wr(--p->cpu->sp,p->cpu->lpc); p->cpu->pc = 0x38; p->cpu->mptr = p->cpu->pc;}
// di,ei
void nprF3(Spec *p) {p->cpu->iff1 = p->cpu->iff2 = false;}
void nprFB(Spec *p) {p->cpu->nextei = true;}
// exchange
void nprD9(Spec *p) {
	p->cpu->adr = p->cpu->bc; p->cpu->bc = p->cpu->alt.bc; p->cpu->alt.bc = p->cpu->adr;
	p->cpu->adr = p->cpu->de; p->cpu->de = p->cpu->alt.de; p->cpu->alt.de = p->cpu->adr;
	p->cpu->adr = p->cpu->hl; p->cpu->hl = p->cpu->alt.hl; p->cpu->alt.hl = p->cpu->adr;}
void nprEB(Spec *p) {p->cpu->adr = p->cpu->de; p->cpu->de = p->cpu->hl; p->cpu->hl = p->cpu->adr;}
// other
void nprD3(Spec *p) {p->cpu->hptr = p->cpu->a; p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->io->out(p->cpu->mptr,p->cpu->a); p->cpu->lptr++;}	// mptr = (a << 8) + ((n + 1) & FF)
void nprDB(Spec *p) {p->cpu->hptr = p->cpu->a; p->cpu->lptr = p->mem->rd(p->cpu->pc++); p->cpu->a = p->io->in(p->cpu->mptr); p->cpu->mptr++;}	// mptr = (a << 8) + n + 1
void nprE3(Spec *p) {p->cpu->lptr = p->mem->rd(p->cpu->sp++); p->cpu->hptr = p->mem->rd(p->cpu->sp); p->mem->wr(p->cpu->sp--,p->cpu->h); p->mem->wr(p->cpu->sp,p->cpu->l); p->cpu->hl = p->cpu->mptr;}	// ex (sp),rp	mptr = rp after operation
void nprE9(Spec *p) {p->cpu->pc = p->cpu->hl;}
void nprF9(Spec *p) {p->cpu->sp = p->cpu->hl;}
// prefix
void nprDD(Spec *p) {p->cpu->mod = 1;}		// DD: ix
void nprFD(Spec *p) {p->cpu->mod = 2;}		// FD: iy
void nprED(Spec *p) {p->cpu->mod = 8;}		// ED
void nprCB(Spec *p) {				// CB (DDCB,FDCB)
	p->cpu->mod |= 4;
	if (p->cpu->mod != 4) {
		p->cpu->dlt = p->mem->rd(p->cpu->pc++);
		p->cpu->r = ((p->cpu->r - 1) & 127) | (p->cpu->r & 128);
	}
}

//==================

ZOp nopref[256]={
	ZOp(&npr00,4,"nop"),
	ZOp(&npr01,10,"ld bc,:2"),
	ZOp(&npr02,7,"ld (bc),a"),
	ZOp(&npr03,6,"inc bc"),
	ZOp(&npr04,4,"inc b"),
	ZOp(&npr05,4,"dec b"),
	ZOp(&npr06,7,"ld b,:1"),
	ZOp(&npr07,4,"rlca"),

	ZOp(&npr08,4,"ex af,af'"),
	ZOp(&npr09,11,"add hl,bc"),
	ZOp(&npr0A,7,"ld a,(bc)"),
	ZOp(&npr0B,6,"dec bc"),
	ZOp(&npr0C,4,"inc c"),
	ZOp(&npr0D,4,"dec c"),
	ZOp(&npr0E,7,"ld c,:1"),
	ZOp(&npr0F,4,"rrca"),

	ZOp(&npr10,8,"djnz :3"),
	ZOp(&npr11,10,"ld de,:2"),
	ZOp(&npr12,7,"ld (de),a"),
	ZOp(&npr13,6,"inc de"),
	ZOp(&npr14,4,"inc d"),
	ZOp(&npr15,4,"dec d"),
	ZOp(&npr16,7,"ld d,:1"),
	ZOp(&npr17,4,"rla"),

	ZOp(&npr18,12,"jr :3"),
	ZOp(&npr19,11,"add hl,de"),
	ZOp(&npr1A,7,"ld a,(de)"),
	ZOp(&npr1B,6,"dec de"),
	ZOp(&npr1C,4,"inc e"),
	ZOp(&npr1D,4,"dec e"),
	ZOp(&npr1E,7,"ld e,:1"),
	ZOp(&npr1F,4,"rra"),

	ZOp(&npr20,7,"jr nz,:3"),
	ZOp(&npr21,10,"ld hl,:2"),
	ZOp(&npr22,16,"ld (:2),hl"),
	ZOp(&npr23,6,"inc hl"),
	ZOp(&npr24,4,"inc h"),
	ZOp(&npr25,4,"dec h"),
	ZOp(&npr26,7,"ld h,:1"),
	ZOp(&npr27,4,"daa"),

	ZOp(&npr28,7,"jr z,:3"),
	ZOp(&npr29,11,"add hl,hl"),
	ZOp(&npr2A,16,"ld hl,(:2)"),
	ZOp(&npr2B,6,"dec hl"),
	ZOp(&npr2C,4,"inc l"),
	ZOp(&npr2D,4,"dec l"),
	ZOp(&npr2E,7,"ld l,:1"),
	ZOp(&npr2F,4,"cpl"),

	ZOp(&npr30,7,"jr nc,:3"),
	ZOp(&npr31,10,"ld sp,:2"),
	ZOp(&npr32,13,"ld (:2),a"),
	ZOp(&npr33,6,"inc sp"),
	ZOp(&npr34,11,"inc (hl)"),
	ZOp(&npr35,11,"dec (hl)"),
	ZOp(&npr36,10,"ld (hl),:1"),
	ZOp(&npr37,4,"scf"),

	ZOp(&npr38,7,"jr c,:3"),
	ZOp(&npr39,11,"add hl,sp"),
	ZOp(&npr3A,13,"ld a,(:2)"),
	ZOp(&npr3B,6,"dec sp"),
	ZOp(&npr3C,4,"inc a"),
	ZOp(&npr3D,4,"dec a"),
	ZOp(&npr3E,7,"ld a,:1"),
	ZOp(&npr3F,4,"ccf"),

	ZOp(&npr40,4,"ld b,b"),
	ZOp(&npr41,4,"ld b,c"),
	ZOp(&npr42,4,"ld b,d"),
	ZOp(&npr43,4,"ld b,e"),
	ZOp(&npr44,4,"ld b,h"),
	ZOp(&npr45,4,"ld b,l"),
	ZOp(&npr46,7,"ld b,(hl)"),
	ZOp(&npr47,4,"ld b,a"),

	ZOp(&npr48,4,"ld c,b"),
	ZOp(&npr49,4,"ld c,c"),
	ZOp(&npr4A,4,"ld c,d"),
	ZOp(&npr4B,4,"ld c,e"),
	ZOp(&npr4C,4,"ld c,h"),
	ZOp(&npr4D,4,"ld c,l"),
	ZOp(&npr4E,7,"ld c,(hl)"),
	ZOp(&npr4F,4,"ld c,a"),

	ZOp(&npr50,4,"ld d,b"),
	ZOp(&npr51,4,"ld d,c"),
	ZOp(&npr52,4,"ld d,d"),
	ZOp(&npr53,4,"ld d,e"),
	ZOp(&npr54,4,"ld d,h"),
	ZOp(&npr55,4,"ld d,l"),
	ZOp(&npr56,7,"ld d,(hl)"),
	ZOp(&npr57,4,"ld d,a"),

	ZOp(&npr58,4,"ld e,b"),
	ZOp(&npr59,4,"ld e,c"),
	ZOp(&npr5A,4,"ld e,d"),
	ZOp(&npr5B,4,"ld e,e"),
	ZOp(&npr5C,4,"ld e,h"),
	ZOp(&npr5D,4,"ld e,l"),
	ZOp(&npr5E,7,"ld e,(hl)"),
	ZOp(&npr5F,4,"ld e,a"),

	ZOp(&npr60,4,"ld h,b"),
	ZOp(&npr61,4,"ld h,c"),
	ZOp(&npr62,4,"ld h,d"),
	ZOp(&npr63,4,"ld h,e"),
	ZOp(&npr64,4,"ld h,h"),
	ZOp(&npr65,4,"ld h,l"),
	ZOp(&npr66,7,"ld h,(hl)"),
	ZOp(&npr67,4,"ld h,a"),

	ZOp(&npr68,4,"ld l,b"),
	ZOp(&npr69,4,"ld l,c"),
	ZOp(&npr6A,4,"ld l,d"),
	ZOp(&npr6B,4,"ld l,e"),
	ZOp(&npr6C,4,"ld l,h"),
	ZOp(&npr6D,4,"ld l,l"),
	ZOp(&npr6E,7,"ld l,(hl)"),
	ZOp(&npr6F,4,"ld l,a"),

	ZOp(&npr70,7,"ld (hl),b"),
	ZOp(&npr71,7,"ld (hl),c"),
	ZOp(&npr72,7,"ld (hl),d"),
	ZOp(&npr73,7,"ld (hl),e"),
	ZOp(&npr74,7,"ld (hl),h"),
	ZOp(&npr75,7,"ld (hl),l"),
	ZOp(&npr76,4,"halt"),
	ZOp(&npr77,7,"ld (hl),a"),

	ZOp(&npr78,4,"ld a,b"),
	ZOp(&npr79,4,"ld a,c"),
	ZOp(&npr7A,4,"ld a,d"),
	ZOp(&npr7B,4,"ld a,e"),
	ZOp(&npr7C,4,"ld a,h"),
	ZOp(&npr7D,4,"ld a,l"),
	ZOp(&npr7E,7,"ld a,(hl)"),
	ZOp(&npr7F,4,"ld a,a"),

	ZOp(&npr80,4,"add a,b"),
	ZOp(&npr81,4,"add a,c"),
	ZOp(&npr82,4,"add a,d"),
	ZOp(&npr83,4,"add a,e"),
	ZOp(&npr84,4,"add a,h"),
	ZOp(&npr85,4,"add a,l"),
	ZOp(&npr86,7,"add a,(hl)"),
	ZOp(&npr87,4,"add a,a"),

	ZOp(&npr88,4,"adc a,b"),
	ZOp(&npr89,4,"adc a,c"),
	ZOp(&npr8A,4,"adc a,d"),
	ZOp(&npr8B,4,"adc a,e"),
	ZOp(&npr8C,4,"adc a,h"),
	ZOp(&npr8D,4,"adc a,l"),
	ZOp(&npr8E,7,"adc a,(hl)"),
	ZOp(&npr8F,4,"adc a,a"),

	ZOp(&npr90,4,"sub b"),
	ZOp(&npr91,4,"sub c"),
	ZOp(&npr92,4,"sub d"),
	ZOp(&npr93,4,"sub e"),
	ZOp(&npr94,4,"sub h"),
	ZOp(&npr95,4,"sub l"),
	ZOp(&npr96,7,"sub (hl)"),
	ZOp(&npr97,4,"sub a"),

	ZOp(&npr98,4,"sbc a,b"),
	ZOp(&npr99,4,"sbc a,c"),
	ZOp(&npr9A,4,"sbc a,d"),
	ZOp(&npr9B,4,"sbc a,e"),
	ZOp(&npr9C,4,"sbc a,h"),
	ZOp(&npr9D,4,"sbc a,l"),
	ZOp(&npr9E,7,"sbc a,(hl)"),
	ZOp(&npr9F,4,"sbc a,a"),

	ZOp(&nprA0,4,"and b"),
	ZOp(&nprA1,4,"and c"),
	ZOp(&nprA2,4,"and d"),
	ZOp(&nprA3,4,"and e"),
	ZOp(&nprA4,4,"and h"),
	ZOp(&nprA5,4,"and l"),
	ZOp(&nprA6,7,"and (hl)"),
	ZOp(&nprA7,4,"and a"),

	ZOp(&nprA8,4,"xor b"),
	ZOp(&nprA9,4,"xor c"),
	ZOp(&nprAA,4,"xor d"),
	ZOp(&nprAB,4,"xor e"),
	ZOp(&nprAC,4,"xor h"),
	ZOp(&nprAD,4,"xor l"),
	ZOp(&nprAE,7,"xor (hl)"),
	ZOp(&nprAF,4,"xor a"),

	ZOp(&nprB0,4,"or b"),
	ZOp(&nprB1,4,"or c"),
	ZOp(&nprB2,4,"or d"),
	ZOp(&nprB3,4,"or e"),
	ZOp(&nprB4,4,"or h"),
	ZOp(&nprB5,4,"or l"),
	ZOp(&nprB6,7,"or (hl)"),
	ZOp(&nprB7,4,"or a"),

	ZOp(&nprB8,4,"cp b"),
	ZOp(&nprB9,4,"cp c"),
	ZOp(&nprBA,4,"cp d"),
	ZOp(&nprBB,4,"cp e"),
	ZOp(&nprBC,4,"cp h"),
	ZOp(&nprBD,4,"cp l"),
	ZOp(&nprBE,7,"cp (hl)"),
	ZOp(&nprBF,4,"cp a"),

	ZOp(&nprC0,5,"ret nz"),		// [+6]
	ZOp(&nprC1,10,"pop bc"),
	ZOp(&nprC2,10,"jp nz,:2"),
	ZOp(&nprC3,10,"jp :2"),
	ZOp(&nprC4,10,"call nz,:2"),	// [+7]
	ZOp(&nprC5,11,"push bc"),
	ZOp(&nprC6,7,"add a,:1"),
	ZOp(&nprC7,11,"rst #00"),

	ZOp(&nprC8,5,"ret z"),		// [+7]
	ZOp(&nprC9,10,"ret"),
	ZOp(&nprCA,10,"jp z,:2"),
	ZOp(&nprCB,4,"#CB",true),
	ZOp(&nprCC,10,"call z,:2"),	// [+7]
	ZOp(&nprCD,10,"call :2"),	// [+7]: acts like CALL true,nn
	ZOp(&nprCE,7,"adc a,:1"),
	ZOp(&nprCF,11,"rst #08"),

	ZOp(&nprD0,5,"ret nc"),		// [+6]
	ZOp(&nprD1,10,"pop de"),
	ZOp(&nprD2,10,"jp nc,:2"),
	ZOp(&nprD3,11,"out (:1),a"),
	ZOp(&nprD4,10,"call nc,:2"),	// [+7]
	ZOp(&nprD5,11,"push de"),
	ZOp(&nprD6,7,"sub :1"),
	ZOp(&nprD7,11,"rst #10"),

	ZOp(&nprD8,5,"ret c"),		// [+6]
	ZOp(&nprD9,4,"exx"),
	ZOp(&nprDA,10,"jp c,:2"),
	ZOp(&nprDB,11,"in a,(:1)"),
	ZOp(&nprDC,10,"call c,:2"),	// [+7]
	ZOp(&nprDD,4,"#DD",true),
	ZOp(&nprDE,7,"sbc a,:1"),
	ZOp(&nprDF,11,"rst #18"),

	ZOp(&nprE0,5,"ret po"),		// [+6]
	ZOp(&nprE1,10,"pop hl"),
	ZOp(&nprE2,10,"jp po,:2"),
	ZOp(&nprE3,19,"ex (sp),hl"),
	ZOp(&nprE4,10,"call c,:2"),	// [+7]
	ZOp(&nprE5,11,"push hl"),
	ZOp(&nprE6,7,"and :1"),
	ZOp(&nprE7,11,"rst #20"),

	ZOp(&nprE8,5,"ret pe"),		// [+6]
	ZOp(&nprE9,4,"jp (hl)"),
	ZOp(&nprEA,10,"jp pe,:2"),
	ZOp(&nprEB,4,"ex de,hl"),
	ZOp(&nprEC,10,"call pe,:2"),	// [+7]
	ZOp(&nprED,4,"#ED",true),
	ZOp(&nprEE,7,"xor :1"),
	ZOp(&nprEF,11,"rst #28"),

	ZOp(&nprF0,5,"ret p"),		// [+6]
	ZOp(&nprF1,10,"pop af"),
	ZOp(&nprF2,10,"jp p,:2"),
	ZOp(&nprF3,4,"di"),
	ZOp(&nprF4,10,"call p,:2"),	// [+7]
	ZOp(&nprF5,11,"push af"),
	ZOp(&nprF6,7,"or :1"),
	ZOp(&nprF7,11,"rst #30"),

	ZOp(&nprF8,5,"ret m"),		// [+6]
	ZOp(&nprF9,6,"ld sp,hl"),
	ZOp(&nprFA,10,"jp m,:2"),
	ZOp(&nprFB,4,"ei"),
	ZOp(&nprFC,10,"call m,:2"),	// [+7]
	ZOp(&nprFD,4,"#FD",true),
	ZOp(&nprFE,7,"cp :1"),
	ZOp(&nprFF,11,"rst #38")
};

