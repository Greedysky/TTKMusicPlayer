#ifndef MUSICVERSION_H
#define MUSICVERSION_H

/**
 * C
 *
 * opyright (c) 2015 - 2019 Greedysky Studio <greedysky@163.com>
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
 *  SID File(*.sid *.mus *.str *.prg *.P00) on linux
 *
 *  MusePack File(*.mpc)
 *
 *  WavPack File(*.wv)
 *
 *  Ogg Vorbis File(*.ogg)
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
 *  MPEG File(*.mp1 *.mp2 *.mp3 *.wav)
 *
 *  Game File(*.ay *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)
 *
 *  FLAC File(*.flac *.oga)
 *
 *  Monkey's Audio File(*.ape)
 *  FFmpeg File(*.wma *.ape *.tta *.m4a *.ra *.shn *.vqf *.ac3)
 *
 *  ModPlug File(*.mod *.s3m *.xm *.it *.669 *.amf *.ams *.dbm *.dmf *.dsm *.far,mdl *.med *.mtm *.okt *.ptm *.stm *.ult *.umx *.mt2 *.psm *.mdz *.s3z *.xmz *.itz *.mdr *.s3r *.xmr *.itr *.dgz *.s3gz *.xmgz *.itgz)
 *
 *  Playlist formats(ttklist, m3u, m3u8, pls, wpl, xspf)
 *
 *  Junior fans interface. Interface cool, gorgeous pop. Maintaining a perfect performance under Vista and XP, refreshing Screenshots.
 *
 *  Search and high-speed downloads. Song search accuracy optimization, intelligent save bandwidth, you can disable the network function, 8 source download speed.
 *
 *  Perfect music. It supports local playback, compatible with all audio files, super perfect sound, music and more appealing.
 *
 *  User-friendly design. Attention to detail design, personalize, simple operation, found music more convenient.
 *
 *  Personalized cell phone ringtones. Ringtones DIY, so you have personalized ringtones Yang Hyun.
 *
 *  Audio format I choose. I made my songs, I decided to sound quality.
 *
 *  Personalized skin. DIY your own personalized skin, creating a only part of your skin.
 *
 *  Music cube model. After entering the mini mode, it will remain in your desktop a "Clover" shaped icon, you can freely drag the desktop, you want to control the player, cut songs, play, lyrics control, volume adjustment in control.
 *
 *  Colorful custom skin. You can select skin color, set the transparency of freedom, and support transparent frosted glass (Windows Aero effects), music from becoming a type, colored.
 *
 *  Search Download Manager. Search Results tab merger - all of the search results tab concentrated under one label, make operation easier. Speed flip - drop-down slider, you can auto-flip, no longer need to click the "Next" a one roll.
 *
 *  Local music search, and cloud music library and interactive, custom ringtone.
 *
 *  Support converting different music formats, and can change the sound as like Tom Cat, also can enhance the sound.
 *
 *  Local music transferred to the cloud or mobile device.
 *
 *  Local music and the human voice humming recognition.
 *
 *  Music song, artist, album and playlist searcg supported.
 *
 *  Music song spectrum and test supported.
 *
 */
 
/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

//update time 2019.06.29
#define TTKMUSIC_VERSION_STR    "2.9.0.0"
#define TTKMUSIC_VERSION_WSTR   L"2.9.0.0"
#define TTKMUSIC_VER_TIME_STR   "(2019/06/29)"

#define TTKMUSIC_MAJOR_VERSION 2
#define TTKMUSIC_MIDLE_VERSION 9
#define TTKMUSIC_MINOR_VERSION 0
#define TTKMUSIC_PATCH_VERSION 0

#define TTKMUSIC_VERSION 0x02900

#define TTKMUSIC_VERSION_CHECK(major, middle, minor, patch) ((major<<12)|(middle<<8)|(minor<<4)|(patch))


#endif // MUSICVERSION

