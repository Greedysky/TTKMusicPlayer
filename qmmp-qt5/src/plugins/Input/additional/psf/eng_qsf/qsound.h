/*********************************************************

    Capcom Q-Sound system

*********************************************************/

#ifndef __QSOUND_H__
#define __QSOUND_H__

#define QSOUND_CLOCK    4000000   /* default 4MHz clock */

struct QSound_interface 
{
	int clock;			/* clock */
	char *sample_rom;		/* sample data */
};


/*
Two Q sound drivers:
DRIVER1 Based on the Amuse source
DRIVER2 Miguel Angel Horna (mahorna@teleline.es)
*/
#define QSOUND_DRIVER1	  1
/*
I don't know whether this system uses 8 bit or 16 bit samples.
If it uses 16 bit samples then the sample ROM loading macros need
to be modified to work with non-intel machines.
*/
#define QSOUND_8BIT_SAMPLES 1

/*
Debug defines
*/
#define LOG_WAVE	0
#define LOG_QSOUND  0

/* Typedefs & defines */

#define QSOUND_DRIVER2 !QSOUND_DRIVER1

#if QSOUND_8BIT_SAMPLES
/* 8 bit source ROM samples */
typedef signed char QSOUND_SRC_SAMPLE;
#define LENGTH_DIV 1
#else
/* 8 bit source ROM samples */
typedef signed short QSOUND_SRC_SAMPLE;
#define LENGTH_DIV 2
#endif

#define QSOUND_CLOCKDIV 166			 /* Clock divider */
#define QSOUND_CHANNELS 16
typedef INT16 QSOUND_SAMPLE;

struct QSOUND_CHANNEL
{
	int bank;	   /* bank (x16)	*/
	int address;	/* start address */
	int pitch;	  /* pitch */
	int reg3;	   /* unknown (always 0x8000) */
	int loop;	   /* loop address */
	int end;		/* end address */
	int vol;		/* master volume */
	int pan;		/* Pan value */
	int reg9;	   /* unknown */

	/* Work variables */
	int key;		/* Key on / key off */

#if QSOUND_DRIVER1
	int lvol;	   /* left volume */
	int rvol;	   /* right volume */
	int lastdt;	 /* last sample value */
	int offset;	 /* current offset counter */
#else
	QSOUND_SRC_SAMPLE *buffer;
	int factor;		   /*step factor (fixed point 8-bit)*/
	int mixl,mixr;		/*mixing factor (fixed point)*/
	int cursor;		   /*current sample position (fixed point)*/
	int lpos;			 /*last cursor pos*/
	int lastsaml;		 /*last left sample (to avoid any calculation)*/
	int lastsamr;		 /*last right sample*/
#endif
};


typedef struct {
    /* Private variables */
    struct QSound_interface intf;	/* Interface  */
    int qsound_stream;				/* Audio stream */
    struct QSOUND_CHANNEL qsound_channel[QSOUND_CHANNELS];
    int qsound_data;				  /* register latch data */
    QSOUND_SRC_SAMPLE *qsound_sample_rom;	/* Q sound sample ROM */

#if QSOUND_DRIVER1
    int qsound_pan_table[33];		 /* Pan volume table */
    float qsound_frq_ratio;		   /* Frequency ratio */
#endif

#if LOG_WAVE
    FILE *fpRawDataL;
    FILE *fpRawDataR;
#endif
} qsound_state_t;

qsound_state_t *qsound_sh_start(struct QSound_interface *qsintf);
void qsound_sh_stop(qsound_state_t *qs);

void qsound_data_h_w(qsound_state_t *qs, int data);
void qsound_data_l_w(qsound_state_t *qs, int data);
void qsound_cmd_w(qsound_state_t *qs, int data);
int qsound_status_r(qsound_state_t *qs);
void qsound_update(qsound_state_t *qs, int num, INT16 **buffer, int length);
void qsound_set_command(qsound_state_t *qs, int data, int value);

#endif /* __QSOUND_H__ */
