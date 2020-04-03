/*

	SCSP (YMF292-F) header
*/

#ifndef _SCSP_H_
#define _SCSP_H_

#define MAX_SCSP	(2)

#define COMBINE_DATA(varptr)	(*(varptr) = (*(varptr) & mem_mask) | (data & ~mem_mask))

// convert AO types
typedef int8 data8_t;
typedef int16 data16_t;
typedef int32 data32_t;
typedef int offs_t;
struct m68ki_cpu_core_s;

struct SCSPinterface 
{
	int num;
	void *region[MAX_SCSP];
	int mixing_level[MAX_SCSP];			/* volume */
	void (*irq_callback[MAX_SCSP])(struct m68ki_cpu_core_s *cpu, int state);	/* irq callback */
	struct m68ki_cpu_core_s *cpu;
};

void *SCSP_Start(const void *config);
void SCSP_Exit (void *param);

void SCSP_Update(void *param, INT16 **inputs, INT16 **buf, int samples);

#define READ16_HANDLER(name)	data16_t name(void *param, offs_t offset, data16_t mem_mask)
#define WRITE16_HANDLER(name)	void     name(void *param, offs_t offset, data16_t data, data16_t mem_mask)

// SCSP register access
READ16_HANDLER( SCSP_0_r );
WRITE16_HANDLER( SCSP_0_w );
READ16_HANDLER( SCSP_1_r );
WRITE16_HANDLER( SCSP_1_w );

// MIDI I/O access (used for comms on Model 2/3)
WRITE16_HANDLER( SCSP_MidiIn );
READ16_HANDLER( SCSP_MidiOutR );

#endif
