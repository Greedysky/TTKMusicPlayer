/*

	Sega/Yamaha AICA emulation	
*/

#ifndef _AICA_H_
#define _AICA_H_

#define MAX_AICA	(2)

#define COMBINE_DATA(varptr)	(*(varptr) = (*(varptr) & mem_mask) | (data & ~mem_mask))

// convert AO types
typedef int8 data8_t;
typedef int16 data16_t;
typedef int32 data32_t;
typedef int offs_t;
struct sARM7;

struct AICAinterface 
{
	int num;
	struct sARM7 *cpu;
	void *region[MAX_AICA];
	int mixing_level[MAX_AICA];			/* volume */
	void (*irq_callback[MAX_AICA])(struct sARM7 *cpu, int state);	/* irq callback */
};

struct _AICA;

int AICA_sh_start(struct AICAinterface *intf);
void AICA_sh_stop(void);
void scsp_stop(void);

#define READ16_HANDLER(name)	data16_t name(struct _AICA *AICA, offs_t offset, data16_t mem_mask)
#define WRITE16_HANDLER(name)	void     name(struct _AICA *AICA, offs_t offset, data16_t data, data16_t mem_mask)

// AICA register access
READ16_HANDLER( AICA_0_r );
WRITE16_HANDLER( AICA_0_w );
READ16_HANDLER( AICA_1_r );
WRITE16_HANDLER( AICA_1_w );

// MIDI I/O access (used for comms on Model 2/3)
WRITE16_HANDLER( AICA_MidiIn );
READ16_HANDLER( AICA_MidiOutR );

void *aica_start(const void *config);
void aica_stop(struct _AICA *AICA);
void AICA_Update(struct _AICA *AICA, void *param, INT16 **inputs, INT16 **buf, int samples);

#endif
