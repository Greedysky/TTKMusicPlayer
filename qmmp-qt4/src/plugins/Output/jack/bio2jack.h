/*
 * Copyright 2003-2004 Chris Morgan <cmorgan@alum.wpi.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _H_JACK_OUT_H
#define _H_JACK_OUT_H

#include <jack/jack.h>

#ifdef __cplusplus
extern "C" {
#else
#define bool long
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ERR_SUCCESS                           0
#define ERR_OPENING_JACK                      1
#define ERR_RATE_MISMATCH                     2
#define ERR_BYTES_PER_OUTPUT_FRAME_INVALID    3
#define ERR_BYTES_PER_INPUT_FRAME_INVALID     4
#define ERR_TOO_MANY_OUTPUT_CHANNELS          5
#define ERR_PORT_NAME_OUTPUT_CHANNEL_MISMATCH 6
#define ERR_PORT_NOT_FOUND                    7
#define ERR_TOO_MANY_INPUT_CHANNELS           8
#define ERR_PORT_NAME_INPUT_CHANNEL_MISMATCH  9

enum status_enum { PLAYING, PAUSED, STOPPED, CLOSED, RESET };
enum pos_enum    { BYTES, MILLISECONDS };

#define PLAYED          1 /* played out of the speakers(estimated value but should be close */
#define WRITTEN_TO_JACK 2 /* amount written out to jack */
#define WRITTEN         3 /* amount written to the bio2jack device */

/**********************/
/* External functions */
void JACK_Init(void); /* call this before any other bio2jack calls */
void JACK_DoSampleRateConversion(bool value); /* whether the next device that's Open()d should do
                                                 sample rate conversion if necessary */
void JACK_SetSampleRateConversionFunction(int converter); /* which SRC converter function should be used
                                                             for the next Open()d device */
int  JACK_Open(int *deviceID, unsigned int bits_per_sample, unsigned long *rate, int channels); /* Note: defaults to 0 input channels
												   if you need input (record) use OpenEx
												   instead */
int  JACK_OpenEx(int *deviceID, unsigned int bits_per_channel,
                 unsigned long *rate,
                 unsigned int input_channels, unsigned int output_channels,
                 const char **jack_port_name, unsigned int jack_port_name_count,
                 unsigned long jack_port_flags);
int  JACK_Close(int deviceID); /* return 0 for success */
void JACK_Reset(int deviceID); /* free all buffered data and reset several values in the device */
long JACK_Write(int deviceID, unsigned char *data, unsigned long bytes); /* returns the number of bytes written */
long JACK_Read(int deviceID, unsigned char *data, unsigned long bytes); /* returns the number of bytes read */

/* state setting values */
/* set/get the written/played/buffered value based on a byte or millisecond input value */
long JACK_GetPosition(int deviceID, enum pos_enum position, int type);
void JACK_SetPosition(int deviceID, enum pos_enum position, long value);

long JACK_GetJackLatency(int deviceID); /* deprectated, you probably want JACK_GetJackOutputLatency */
long JACK_GetJackOutputLatency(int deviceID); /* return the output latency in frames */
long JACK_GetJackInputLatency(int deviceID); /* return the input latency in frames */

int JACK_SetState(int deviceID, enum status_enum state); /* playing, paused, stopped */
enum status_enum JACK_GetState(int deviceID);

long JACK_GetMaxOutputBufferedBytes(int deviceID);
long JACK_GetMaxInputBufferedBytes(int deviceID);

/* bytes that jack requests during each callback */
unsigned long JACK_GetJackBufferedBytes(int deviceID);

/* Properties of the jack driver */

/* linear means 0 volume is silence, 100 is full volume */
/* dbAttenuation means 0 volume is 0dB attenuation */
/* Bio2jack defaults to linear */
/* Note: volume controls only effect output channels for now */
enum JACK_VOLUME_TYPE { linear, dbAttenuation };
enum JACK_VOLUME_TYPE JACK_SetVolumeEffectType(int deviceID,
                                               enum JACK_VOLUME_TYPE type);

int  JACK_SetAllVolume(int deviceID, unsigned int volume); /* returns 0 on success */
int  JACK_SetVolumeForChannel(int deviceID, unsigned int channel, unsigned int volume);
void JACK_GetVolumeForChannel(int deviceID, unsigned int channel, unsigned int *volume);


unsigned long JACK_GetOutputBytesPerSecond(int deviceID); /* bytes_per_output_frame * sample_rate */
unsigned long JACK_GetInputBytesPerSecond(int deviceID);  /* bytes_per_input_frame * sample_rate */
unsigned long JACK_GetBytesStored(int deviceID);          /* bytes currently buffered in the output buffer */
unsigned long JACK_GetBytesFreeSpace(int deviceID);       /* bytes of free space in the output buffer */
unsigned long JACK_GetBytesUsedSpace(int deviceID);       /* bytes of space used in the input buffer */
unsigned long JACK_GetBytesPerOutputFrame(int deviceID);
unsigned long JACK_GetBytesPerInputFrame(int deviceID);

/* Note: these will probably be removed in a future release */
int  JACK_GetNumInputChannels(int deviceID);
int  JACK_GetNumOutputChannels(int deviceID);

long JACK_GetSampleRate(int deviceID); /* samples per second */

void JACK_SetClientName(char *name); /* sets the name that bio2jack will use when
                                        creating a new jack client.  name_%pid%_%deviceID%%counter%
                                        will be used
                                        NOTE: this defaults to name = bio2jack
                                        NOTE: we limit the size of the client name to
                                           jack_client_name_size() */

enum JACK_PORT_CONNECTION_MODE
{
    CONNECT_ALL,    /* connect to all avaliable ports */
    CONNECT_OUTPUT, /* connect only to the ports we need for output */
    CONNECT_NONE    /* don't connect to any ports */
};

/* set the mode for port connections */
/* defaults to CONNECT_ALL */ 
void JACK_SetPortConnectionMode(enum JACK_PORT_CONNECTION_MODE mode);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef JACK_OUT_H */
