/***************************************************************************
                           regs.h  -  description
                             -------------------
    begin                : Wed May 15 2002
    copyright            : (C) 2002 by Pete Bernert
    email                : BlackDove@addcom.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version. See also the license.txt file for *
 *   additional informations.                                              *
 *                                                                         *
 ***************************************************************************/

//*************************************************************************//
// History of changes:
//
// 2004/04/04 - Pete
// - changed plugin to emulate PS2 spu
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

struct spu2_state_s;

void SoundOn(struct spu2_state_s *spu, int start,int end,unsigned short val);
void SoundOff(struct spu2_state_s *spu, int start,int end,unsigned short val);
void VolumeOn(struct spu2_state_s *spu, int start,int end,unsigned short val,int iRight);
void FModOn(struct spu2_state_s *spu, int start,int end,unsigned short val);
void NoiseOn(struct spu2_state_s *spu, int start,int end,unsigned short val);
void SetVolumeL(struct spu2_state_s *spu, unsigned char ch,short vol);
void SetVolumeR(struct spu2_state_s *spu, unsigned char ch,short vol);
void SetPitch(struct spu2_state_s *spu, int ch,unsigned short val);
void ReverbOn(struct spu2_state_s *spu, int start,int end,unsigned short val,int iRight);
void SetReverbAddr(struct spu2_state_s *spu, int core);

//EXPORT_GCC void CALLBACK SPU2write(unsigned long reg, unsigned short val);

