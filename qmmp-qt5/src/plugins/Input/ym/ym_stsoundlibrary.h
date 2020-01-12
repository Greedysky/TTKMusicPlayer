/*-----------------------------------------------------------------------------

	ST-Sound ( YM files player library )

	Main header to use the StSound "C" like API in your production.

-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ST-Sound, ATARI-ST Music Emulator
* Copyright (c) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
-----------------------------------------------------------------------------*/


#ifndef __STSOUNDLIBRARY__
#define __STSOUNDLIBRARY__

#include "ym_types.h"

typedef	void			YMMUSIC;

typedef struct
{
	ymchar	*	pSongName;
	ymchar	*	pSongAuthor;
	ymchar	*	pSongComment;
	ymchar	*	pSongType;
	ymchar	*	pSongPlayer;
	yms32		musicTimeInSec;		// keep for compatibility
	yms32		musicTimeInMs;
} ymMusicInfo_t;

#ifdef __cplusplus
extern "C"
{
#endif

// Create object
extern	YMMUSIC *		ymMusicCreate();

// Release object
extern	void			ymMusicDestroy(YMMUSIC *pMusic);

// Global settings
extern	void			ymMusicSetLowpassFiler(YMMUSIC *pMus,ymbool bActive);

// Functions
extern	ymbool			ymMusicLoad(YMMUSIC *pMusic,const char *fName);						// Method 1 : Load file using stdio library (fopen/fread, etc..)
extern	ymbool			ymMusicLoadMemory(YMMUSIC *pMusic,void *pBlock,ymu32 size);			// Method 2 : Load file from a memory block

extern	ymbool			ymMusicCompute(YMMUSIC *pMusic,ymsample *pBuffer,ymint nbSample);	// Render nbSample samples of current YM tune into pBuffer PCM 16bits mono sample buffer.

extern	void			ymMusicSetLoopMode(YMMUSIC *pMusic,ymbool bLoop);
extern	const char	*	ymMusicGetLastError(YMMUSIC *pMusic);
extern	int			ymMusicGetRegister(YMMUSIC *pMusic,ymint reg);
extern	void			ymMusicGetInfo(YMMUSIC *pMusic,ymMusicInfo_t *pInfo);
extern	void			ymMusicPlay(YMMUSIC *pMusic);
extern	void			ymMusicPause(YMMUSIC *pMusic);
extern	void			ymMusicStop(YMMUSIC *pMusic);
extern	ymbool			ymMusicIsOver(YMMUSIC *_pMus);

extern	void			ymMusicRestart(YMMUSIC *pMusic);

extern	ymbool			ymMusicIsSeekable(YMMUSIC *pMusic);
extern	ymu32			ymMusicGetPos(YMMUSIC *pMusic);
extern	void			ymMusicSeek(YMMUSIC *pMusic,ymu32 timeInMs);

#ifdef __cplusplus
}
#endif


#endif
