#ifndef _XPGSOUND
#define _XPGSOUND

#ifdef __cplusplus
extern "C" {
#endif

#include "sndcommon.h"

#include "../cpu/cpu.h"
#include "../memory.h"

// gs stereo
enum {
	GS_MONO = 0,
	GS_12_34
};
// gs cpu freq
#define	GS_FRQ	12.0

#include "sndcommon.h"

typedef struct {
	unsigned enable:1;
	unsigned reset:1;

	CPU* cpu;
	Memory* mem;
	unsigned char pb3_gs;	// gs -> zx
	unsigned char pb3_zx;	// zx -> gs
	unsigned char pbb_zx;
	unsigned char rp0;
	unsigned char pstate;	// state register (d0, d7)
	int vol1,vol2,vol3,vol4;
	int ch1,ch2,ch3,ch4;
	int cnt;
	int stereo;
	int sync;
	long counter;
} GSound;

GSound* gsCreate();
void gsDestroy(GSound*);
void gsReset(GSound*);
void gsSync(GSound*);
sndPair gsGetVolume(GSound*);

int gsIn(GSound*, int, unsigned char*);
int gsOut(GSound*, int, unsigned char);

void gsSetRom(GSound*,int,char*);

#ifdef __cplusplus
}
#endif

#endif
