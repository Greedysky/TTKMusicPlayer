/***************************************************************************

  Capcom System QSound(tm)
  ========================

  Driver by Paul Leaman (paul@vortexcomputing.demon.co.uk)
		and Miguel Angel Horna (mahorna@teleline.es)

  A 16 channel stereo sample player.

  QSpace position is simulated by panning the sound in the stereo space.

  Register
  0	 xxbb	xx = unknown bb = start high address
  1	 ssss	ssss = sample start address
  2	 pitch
  3	 unknown (always 0x8000)
  4	 loop offset from end address
  5	 end
  6	 master channel volume
  7	 not used
  8	 Balance (left=0x0110  centre=0x0120 right=0x0130)
  9	 unknown (most fixed samples use 0 for this register)

  Many thanks to CAB (the author of Amuse), without whom this probably would
  never have been finished.

  If anybody has some information about this hardware, please send it to me
  to mahorna@teleline.es or 432937@cepsz.unizar.es.
  http://teleline.terra.es/personal/mahorna

***************************************************************************/

#include <math.h>
#include "cpuintrf.h"
#include "qsound.h"

#if 0
#if QSOUND_DRIVER2
void setchannel(int channel,signed short *buffer,int length,int vol,int pan);
void setchloop(int channel,int loops,int loope);
void stopchan(int channel);
void calcula_mix(int channel);
#endif
#endif

qsound_state_t *qsound_sh_start( struct QSound_interface *qsintf )
{
    qsound_state_t *qs = malloc (sizeof (qsound_state_t));
    memset (qs, 0, sizeof (qsound_state_t));
    memcpy (&qs->intf, qsintf, sizeof (struct QSound_interface));

	int i;

	qs->qsound_sample_rom = (QSOUND_SRC_SAMPLE *)qs->intf.sample_rom;

	memset(qs->qsound_channel, 0, sizeof(qs->qsound_channel));

#if QSOUND_DRIVER1
	qs->qsound_frq_ratio = ((float)qs->intf.clock / (float)QSOUND_CLOCKDIV) /
						(float) 44100;
	qs->qsound_frq_ratio *= 16.0;

	/* Create pan table */
	for (i=0; i<33; i++)
	{
		qs->qsound_pan_table[i]=(int)((256/sqrt(32)) * sqrt(i));
	}
#else
	i=0;
#endif

#if LOG_QSOUND
	logerror("Pan table\n");
	for (i=0; i<33; i++)
		logerror("%02x ", qs->qsound_pan_table[i]);
#endif
#if 0
	{
		/* Allocate stream */
#define CHANNELS ( 2 )
		char buf[CHANNELS][40];
		const char *name[CHANNELS];
		int  vol[2];
		name[0] = buf[0];
		name[1] = buf[1];
		sprintf( buf[0], "%s L", sound_name(msound) );
		sprintf( buf[1], "%s R", sound_name(msound) );
		vol[0]=MIXER(intf->mixing_level[0], MIXER_PAN_LEFT);
		vol[1]=MIXER(intf->mixing_level[1], MIXER_PAN_RIGHT);
		qsound_stream = stream_init_multi(
			CHANNELS,
			name,
			vol,
			Machine->sample_rate,
			0,
			qsound_update );
	}
#endif
#if LOG_WAVE
	qs->fpRawDataR=fopen("qsoundr.raw", "w+b");
	qs->fpRawDataL=fopen("qsoundl.raw", "w+b");
	if (!qs->fpRawDataR || !qs->fpRawDataL)
	{
		return NULL;
	}
#endif

	return qs;
}

void qsound_sh_stop (qsound_state_t *qs)
{
    if (qs) {
#if LOG_WAVE
        if (qs->fpRawDataR)
        {
            fclose(qs->fpRawDataR);
        }
        if (qs->fpRawDataL)
        {
            fclose(qs->fpRawDataL);
        }
#endif
        free (qs);
    }
}

void qsound_data_h_w(qsound_state_t *qs, int data)
{
	qs->qsound_data=(qs->qsound_data&0xff)|(data<<8);
}

void qsound_data_l_w(qsound_state_t *qs, int data)
{
	qs->qsound_data=(qs->qsound_data&0xff00)|data;
}

void qsound_cmd_w(qsound_state_t *qs, int data)
{
//	printf("QS: cmd %x, data %x\n", data, qs->qsound_data);
	qsound_set_command(qs, data, qs->qsound_data);
}

int qsound_status_r(qsound_state_t *qs)
{
	/* Port ready bit (0x80 if ready) */
	return 0x80;
}

void qsound_set_command(qsound_state_t *qs, int data, int value)
{
	int ch=0,reg=0;
	if (data < 0x80)
	{
		ch=data>>3;
		reg=data & 0x07;
	}
	else
	{
		if (data < 0x90)
		{
			ch=data-0x80;
			reg=8;
		}
		else
		{
			if (data >= 0xba && data < 0xca)
			{
				ch=data-0xba;
				reg=9;
			}
			else
			{
				/* Unknown registers */
				ch=99;
				reg=99;
			}
		}
	}

	switch (reg)
	{
		case 0: /* Bank */
			ch=(ch+1)&0x0f;	/* strange ... */
			qs->qsound_channel[ch].bank=(value&0x7f)<<16;
			qs->qsound_channel[ch].bank /= LENGTH_DIV;
#ifdef MAME_DEBUG
			if (!value & 0x8000)
			{
				char baf[40];
				sprintf(baf,"Register3=%04x",value);
				usrintf_showmessage(baf);
			}
#endif

			break;
		case 1: /* start */
//			printf("QS: key on ch %02d\n", ch);
			qs->qsound_channel[ch].address=value;
			qs->qsound_channel[ch].address/=LENGTH_DIV;
			break;
		case 2: /* pitch */
#if QSOUND_DRIVER1
			qs->qsound_channel[ch].pitch=(long)
					((float)value * qs->qsound_frq_ratio );
			qs->qsound_channel[ch].pitch/=LENGTH_DIV;
#else
			qs->qsound_channel[ch].factor=((float) (value*(6/LENGTH_DIV)) /
									  (float) Machine->sample_rate)*256.0;

#endif
			if (!value)
			{
				/* Key off */
//				printf("QS: key off ch %02d\n", ch);
				qs->qsound_channel[ch].key=0;
			}
			break;
		case 3: /* unknown */
			qs->qsound_channel[ch].reg3=value;
#ifdef MAME_DEBUG
			if (value != 0x8000)
			{
				char baf[40];
				sprintf(baf,"Register3=%04x",value);
				usrintf_showmessage(baf);
			}
#endif
			break;
		case 4: /* loop offset */
			qs->qsound_channel[ch].loop=value/LENGTH_DIV;
			break;
		case 5: /* end */
			qs->qsound_channel[ch].end=value/LENGTH_DIV;
			break;
		case 6: /* master volume */
			if (value==0)
			{
				/* Key off */
				qs->qsound_channel[ch].key=0;
			}
			else if (qs->qsound_channel[ch].key==0)
			{
				/* Key on */
				qs->qsound_channel[ch].key=1;
#if QSOUND_DRIVER1
				qs->qsound_channel[ch].offset=0;
				qs->qsound_channel[ch].lastdt=0;
#else
				qs->qsound_channel[ch].cursor=qs->qsound_channel[ch].address <<8 ;
				qs->qsound_channel[ch].buffer=qsound_sample_rom+
										 qs->qsound_channel[ch].bank;
#endif
			}
			qs->qsound_channel[ch].vol=value;
#if QSOUND_DRIVER2
			calcula_mix(ch);
#endif
			break;

		case 7:  /* unused */
#ifdef MAME_DEBUG
			{
				char baf[40];
				sprintf(baf,"UNUSED QSOUND REG 7=%04x",value);
				usrintf_showmessage(baf);
			}
#endif

			break;
		case 8:
			{
#if QSOUND_DRIVER1
			   int pandata=(value-0x10)&0x3f;
			   if (pandata > 32)
			   {
					pandata=32;
			   }
			   qs->qsound_channel[ch].rvol=qs->qsound_pan_table[pandata];
			   qs->qsound_channel[ch].lvol=qs->qsound_pan_table[32-pandata];
#endif
			   qs->qsound_channel[ch].pan = value;
#if QSOUND_DRIVER2
			   calcula_mix(ch);
#endif
			}
			break;
		 case 9:
			qs->qsound_channel[ch].reg9=value;
/*
#ifdef MAME_DEBUG
			{
				char baf[40];
				sprintf(baf,"QSOUND REG 9=%04x",value);
				usrintf_showmessage(baf);
			}
#endif
*/
			break;
	}
#if LOG_QSOUND
	logerror("QSOUND WRITE %02x CH%02d-R%02d =%04x\n", data, ch, reg, value);
#endif
}

#if QSOUND_DRIVER1

/* Driver 1 - based on the Amuse source */

void qsound_update(qsound_state_t *qs, int num, INT16 **buffer, int length )
{
	int i,j;
	int rvol, lvol, count;
	struct QSOUND_CHANNEL *pC=&qs->qsound_channel[0];
	QSOUND_SRC_SAMPLE * pST;
	QSOUND_SAMPLE  *datap[2];

	datap[0] = buffer[0];
	datap[1] = buffer[1];
	memset( datap[0], 0x00, length * sizeof(QSOUND_SAMPLE) );
	memset( datap[1], 0x00, length * sizeof(QSOUND_SAMPLE) );


	for (i=0; i<QSOUND_CHANNELS; i++)
	{
		if (pC->key)
		{
			QSOUND_SAMPLE *pOutL=datap[0];
			QSOUND_SAMPLE *pOutR=datap[1];
			pST=qs->qsound_sample_rom+pC->bank;

			rvol=(pC->rvol*pC->vol)>>(8*LENGTH_DIV);
			lvol=(pC->lvol*pC->vol)>>(8*LENGTH_DIV);

			for (j=length-1; j>=0; j--)
			{
				count=(pC->offset)>>16;
				pC->offset &= 0xffff;
				if (count)
				{
					pC->address += count;
					if (pC->address >= pC->end)
					{
						if (!pC->loop)
						{
							/* Reached the end of a non-looped sample */
							pC->key=0;
							break;
						}
						/* Reached the end, restart the loop */
						pC->address = (pC->end - pC->loop) & 0xffff;
					}
					pC->lastdt = pST[pC->address];
				}

				(*pOutL) += ((pC->lastdt * lvol) >> 6);
				(*pOutR) += ((pC->lastdt * rvol) >> 6);
				pOutL++;
				pOutR++;
				pC->offset += pC->pitch;
			}
		}
		pC++;
	}

#if LOG_WAVE
	fwrite(datap[0], length*sizeof(QSOUND_SAMPLE), 1, fpRawDataL);
	fwrite(datap[1], length*sizeof(QSOUND_SAMPLE), 1, fpRawDataR);
#endif
}

#else

/* ----------------------------------------------------------------
		QSound Sample Mixer (Slow)
		Miguel Angel Horna mahorna@teleline.es

 ------------------------------------------------------------------ */


void calcula_mix(int channel)
{
	int factl,factr;
	struct qs->qsound_channel *pC=&qs->qsound_channel[channel];
	int vol=pC->vol>>5;
	int pan=((pC->pan&0xFF)-0x10)<<3;
	pC->mixl=vol;
	pC->mixr=vol;
	factr=pan;
	factl=255-factr;
	pC->mixl=(pC->mixl * factl)>>8;
	pC->mixr=(pC->mixr * factr)>>8;
#if QSOUND_8BIT_SAMPLES
	pC->mixl<<=8;
	pC->mixr<<=8;
#endif
}

void qsound_update(int num,void **buffer,int length)
{
	int i,j;
	QSOUND_SAMPLE *bufL,*bufR, sample;
	struct qs->qsound_channel *pC=qs->qsound_channel;

	memset( buffer[0], 0x00, length * sizeof(QSOUND_SAMPLE) );
	memset( buffer[1], 0x00, length * sizeof(QSOUND_SAMPLE) );

	for(j=0;j<qs->qsound_channelS;++j)
	{
		  bufL=(QSOUND_SAMPLE *) buffer[0];
		  bufR=(QSOUND_SAMPLE *) buffer[1];
		  if(pC->key)
		  {
				for(i=0;i<length;++i)
				{
						   int pos=pC->cursor>>8;
						   if(pos!=pC->lpos)	/*next sample*/
						   {
								sample=pC->buffer[pos];
								pC->lastsaml=(sample*pC->mixl)>>8;
								pC->lastsamr=(sample*pC->mixr)>>8;
								pC->lpos=pos;
						   }
						   (*bufL++)+=pC->lastsaml;
						   (*bufR++)+=pC->lastsamr;
						   pC->cursor+=pC->factor;
						   if(pC->loop && (pC->cursor>>8) > pC->end)
						   {
								 pC->cursor=(pC->end-pC->loop)<<8;
						   }
						   else if((pC->cursor>>8) > pC->end)
								   pC->key=0;
				 }
		  }
		  pC++;
	 }
}
#endif


/**************** end of file ****************/
