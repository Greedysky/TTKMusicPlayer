#ifndef TTKVERSION_H
#define TTKVERSION_H

/**
 * C
 *
 * opyright (c) 2015 - 2022 Greedysky Studio <greedysky@163.com>
 *
 * TTKMusicPlayer that imitation Kugou music, the music player use
 * of qmmp core library based on Qt for windows and linux
 */

/** \mainpage
 *
 * \section encoding Attention
 * 
 * This software is only for learning and communication, the songs' 
 * and playlists' copyright belongs to each music platform.
 *
 * \section result Features
 * TTKMusicPlayer can meet your most demands
 *
 *  Support for multiple music formats.
 *
 *  SID File(*.sid *.mus *.str *.prg *.P00)
 *
 *  MusePack File(*.mpc)
 *
 *  WavPack File(*.wv)
 *
 *  Ogg Vorbis File(*.ogg *.ogv)
 *
 *  Ogg Opus File(*.opus)
 *
 *  ADTS AAC File(*.aac)
 *
 *  MIDI File(*.mid)
 *
 *  PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)
 *
 *  CUE File(*.cue)
 *
 *  MPEG File(*.mp1 *.mp2 *.mp3)
 *
 *  Game Audio File(*.ay *.gbs *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)
 *
 *  FLAC File(*.flac *.oga)
 *
 *  Monkey's Audio File(*.ape)
 *
 *  FFmpeg File(*.wma *.ape *.tta *.m4a *.ra *.shn *.vqf *.ac3 *.tak *.dsf *.dsdiff *.webm)
 *
 *  OpenMPT File(*.669 *.ams *.amf *.dsm *.dmf *.dbm *.digi *.dtm *.far *.gdm *.it *.ice *.imf *.j2b *.mod *.mptm *.m15 *.mtm *.med *.mdl *.mt2 *.mms *.mo3 *.mmcmp *.nst *.okt *.okta *.pt36 *.ptm *.psm *.plm *.ppm *.s3m *.stm *.stk *.st26 *.sfx *.sfx2 *.stp *.ult *.umx *.wow *.xm *.xpk)
 *
 *  AdLib Sound File(*.a2m *.adl *.adlib *.amd *.as3m *.bam *.cmf *.d00 *.dfm *.dmo *.dro *.dtm *.hsc *.imf *.jbm *.laa *.lds *.m *.mad *.mkj *.msc *.mtk *.rad *.raw *.sa2 *.sat *.sng *.sqx *.xad *.xms *.xsm)
 *
 *  Another Slight Atari File(*.sap *.cm3 *.cmc *.cmr *.cms *.dmc *.dlt *.mpd *.mpt *.rmt *.tm2 *.tm8 *.tmc)
 *
 *  DTS Coherent Acoustics File(*.dts *.cpt)
 *
 *  Future Composer File(*.fc *.fc13 *.fc14 *.smod)
 *
 *  OptimFrog File(*.ofr *.ofs)
 *
 *  Audio Overload File(*.psf *.spu *.ssf *.qsf *.dsf *.minipsf *.minissf *.miniqsf *.minidsf *.2sf *.mini2sf *.gsf *.minigsf *.usf *.miniusf *.ncsf *.minincsf *.snsf *.minisnsf)
 *
 *  Atari ST(E) And Amiga File(*.sc68)
 *
 *  Speex Audio File(*.spx)
 *
 *  V2 Module Audio File(*.v2m)
 *
 *  AY/YM Audio File(*.vtx *.asc *.sqt *psg *.stc *.stp *.pt1 *.pt2 *.pt3 *.ay *.emul)
 *
 *  ST-Sound File(*.ym)
 *
 *  HVL Module Player File(*.ahx *.hvl)
 *
 *  BP SoundMon Audio File(*.bp *.bp3)
 *
 *  MDX Chiptunes Audio File(*.mdx *.m)
 *
 *  Cave Story's org Audio File(*.org)
 *
 *  Playlist formats(tkpl, m3u, m3u8, pls, wpl, xspf, asx, fpl, dbpl, csv, txt)
 *
 *  Junior fans interface. Interface cool, gorgeous pop. Maintaining a perfect performance under Vista and XP, refreshing Screenshots.
 *
 *  Search and high-speed downloads. Song search accuracy optimization, intelligent save bandwidth, you can disable the network function.
 *
 *  Perfect music. It supports local playback, compatible with all audio files, super perfect sound, music and more appealing.
 *
 *  User-friendly design. Attention to detail design, personalize, simple operation, found music more convenient.
 *
 *  Personalized cell phone ringtones. Ringtones DIY, so you have personalized ringtone.
 *
 *  Personalized skin. DIY your own personalized skin, creating a only part of your skin.
 *
 *  Music cube model. After entering the mini mode, it will remain in your desktop a "Clover" shaped icon, you can freely drag the desktop, you want to control the player, cut songs, play, lyrics control, volume adjustment in control.
 *
 *  Colorful custom skin. You can select skin color, set the transparency of freedom, and support transparent frosted glass (Windows Aero effects), music from becoming a type, colored.
 *
 *  Search Download Manager. Search Results tab merger - all of the search results tab concentrated under one label, make operation easier. 
 *
 *  Local music search, and cloud music library and interactive, custom ringtone.
 *
 *  Support converting different music formats, and can change the sound as like Tom Cat, also can enhance the sound.
 *
 *  Local music transferred to the cloud or mobile device.
 *
 *  Local music and the human voice humming recognition.
 *
 *  Music song, artist, album and playlist search supported.
 *
 *  Music DSP effects and Visual effects supported.
 *
 *  Music song spectrum(projectm, goom, sonique, florid, wavebar) and test supported.
 *
 *  Music replay gain scan and write supported.
 *
 *  Music screen saver supported.
 *
 */
 
/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

//update time 2022.01.02
#define TTKMUSIC_VERSION_STR    "2.15.7.0"
#define TTKMUSIC_VERSION_WSTR   L"2.15.7.0"
#define TTKMUSIC_VER_TIME_STR   "(2022/01/02)"

#define TTKMUSIC_MAJOR_VERSION 2
#define TTKMUSIC_MIDLE_VERSION 15
#define TTKMUSIC_MINOR_VERSION 7
#define TTKMUSIC_PATCH_VERSION 0

#define TTKMUSIC_VERSION 0x02F70

#define TTKCONFIG_VERSION_STR    "1.0.1"

#define TTK_VERSION_CHECK(major, middle, minor, patch) ((major<<12)|(middle<<8)|(minor<<4)|(patch))

#endif // TTKVERSION_H
