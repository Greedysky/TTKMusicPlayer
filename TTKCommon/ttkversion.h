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
 * Support for multiple music formats.
 *
 * SID File(*.sid *.psid *.mus *.str *.p00 *.prg *.c64)
 *
 * MusePack File(*.mpc)
 *
 * WavPack File(*.wv)
 *
 * Ogg Vorbis File(*.ogg *.ogv)
 *
 * Ogg Opus File(*.opus)
 *
 * ADTS AAC File(*.aac)
 *
 * MIDI File(*.mid *.mus *.hmp *.hmi *.xmi)
 *
 * PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)
 *
 * CUE File(*.cue)
 *
 * MPEG File(*.mp1 *.mp2 *.mp3)
 *
 * Game Audio File(*.ay *.gbs *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)
 *
 * FLAC File(*.flac *.oga)
 *
 * Monkey's Audio File(*.ape)
 *
 * FFmpeg File(*.wma *.ape *.tta *.m4a *.m4b *.aac *.ra *.shn *.ac3 *.mka *.vqf *.tak *.spx *.adx *.aix *.wve *.sln *.paf *.pvf *.ircam *.gsm *.avr *.amr *.dsf *.dsdiff *.webm *.3g2 *.3gp *.asf *.avi *.f4v *.flv *.hevc *.m4v *.mov *.mp4 *.mpeg *.rm *.swf *.vob *.wtv)
 *
 * OpenMPT File(*.669 *.ams *.amf *.dsm *.dmf *.dbm *.digi *.dtm *.far *.gdm *.it *.ice *.imf *.j2b *.mod *.mptm *.m15 *.mtm *.med *.mmd *.mmd0 *.mmd1 *.mmd2 *.mmd3 *.mdl *.mt2 *.mms *.mo3 *.mmcmp *.nst *.okt *.okta *.pt36 *.ptm *.psm *.plm *.ppm *.s3m *.stm *.stk *.st26 *.sfx *.sfx2 *.stp *.ult *.umx *.wow *.xm *.xpk *.mdr *.s3r *.xmr *.itr *.mdz *.s3z *.xmz *.itz *.mdgz *.s3gz *.xmgz *.itgz *.mdbz *.s3bz *.xmbz *.itbz)
 *
 * Unix Amiga Delitracker Emulator File(*.aam *.amc *.aon *.aps *.ash *.ast *.avp *.bd *.bds *.bsi *.bss *.bye *.cm *.core *.cust *.dat *.dh *.dl *.dln *.dm *.dm2 *.dmu *.doda *.dsc *.dsr *.dss *.dum *.dw *.dz *.ea *.ems *.emsv6 *.ex *.fp *.fred *.fw *.glue *.gmc *.gray *.hd *.hip *.hip7 *.hipc *.hot *.is *.is20 *.jam *.jcb *.jcbo *.jd *.jmf *.jo *.jpo *.jt *.kh *.kim *.kris *.lion *.lme *.ma *.mc *.mcmd *.mco *.md *.mii *.mk2 *.mkii *.ml *.mm8 *.mmdc *.mok *.mon *.mosh *.mso *.mug *.mw *.ntp *.osp *.pap *.pn *.ps *.psa *.psf *.pt *.puma *.pvp *.rh *.rho *.riff *.rmc *.s7g *.sa *.sas *.sb *.sc *.scn *.scr *.sct *.scumm *.sdr *.sg *.sid1 *.sid2 *.sm *.sm3 *.smn *.smpro *.sng *.snk *.soc *.sog *.spl *.sqt *.ss *.sun *.syn *.synmod *.tf *.thx *.tits *.tme *.tro *.tw *.ufo *.vss *.wb)
 *
 * Archive File(*.7z *.xz *.bz2 *.gz *.tar *.zip *.wim *.ar *.arj *.cab *.chm *.cpio *.cramfs *.dmg *.ext *.fat *.gpt *.hfs *.ihex *.iso *.lzh *.lzma *.mbr *.msi *.nsis *.ntfs *.qcow2 *.rar *.rpm *.squashfs *.udf *.uefi *.vdi *.vhd *.vmdk *.xar *.z *.rsn *.rps *.rdc *.rds *.rgs *.r64)
 *
 * AdLib Sound File(*.a2m *.adl *.adlib *.amd *.as3m *.bam *.cmf *.d00 *.dfm *.dmo *.dro *.dtm *.hsc *.imf *.jbm *.laa *.lds *.m *.mad *.mkj *.msc *.mtk *.rad *.raw *.sa2 *.sat *.sng *.sqx *.xad *.xms *.xsm)
 *
 * XMP File(*.abk *.dsym *.emod *.fnk *.gtk *.ims *.liq *.mfp *.mgt *.rtm *.tcb)
 *
 * Another Slight Atari File(*.cm3 *.cmc *.cmr *.cms *.dlt *.dmc *.fc *.mpd *.mpt *.rmt *.sap *.tm2 *.tm8 *.tmc)
 *
 * DTS Coherent Acoustics File(*.dts *.cpt)
 *
 * Future Composer File(*.fc *.fc13 *.fc14 *.smod)
 *
 * OptimFrog File(*.ofr *.ofs)
 *
 * Audio Overload File(*.pcm *.msu *.spu *.psf *.ssf *.qsf *.dsf *.psf2 *.minipsf *.minissf *.miniqsf *.minidsf *.minipsf2 *.2sf *.mini2sf *.gsf *.minigsf *.usf *.miniusf *.ncsf *.minincsf *.snsf *.minisnsf)
 *
 * S98 Audio File(*.s98 *.cym *.mym *.x1f)
 *
 * Atari ST(E) And Amiga File(*.sc68 *.snd *.sndh)
 *
 * Speex Audio File(*.spx)
 *
 * V2 Player File(*.v2m)
 *
 * AY/YM Audio File(*.asc *.ay *.emul *.sqt *.st13 *.stc *.stp *.stp2 *.psc *.psg *.pt1 *.pt2 *.pt3 *.vtx *.zxs)
 *
 * ST-Sound File(*.ym)
 *
 * SunVox Audio File(*.sunvox)
 *
 * Hively Audio File(*.ahx *.hvl)
 *
 * BP SoundMon Audio File(*.bp *.bp3)
 *
 * MDX Game Audio File(*.mdx *.pdx *.m *.mub *.muc *.vgs *.bgm *.mml)
 *
 * Cave Story's org Audio File(*.org *.pttune *.ptcop)
 *
 * Final Music System Tracker Module File(*.tfmx *.tfx *.tfm *.mdat)
 *
 * Playlist formats(tkpl, m3u, m3u8, pls, wpl, xspf, asx, fpl, dbpl, csv, txt)
 *
 * Junior fans interface. Interface cool, gorgeous pop. Maintaining a perfect performance under Vista and XP, refreshing Screenshots.
 *
 * Search and high-speed downloads. Song search accuracy optimization, intelligent save bandwidth, you can disable the network function.
 *
 * Perfect music. It supports local playback, compatible with all audio files, super perfect sound, music and more appealing.
 *
 * User-friendly design. Attention to detail design, personalize, simple operation, found music more convenient.
 *
 * Personalized cell phone ringtones. Ringtones DIY, so you have personalized ringtone.
 *
 * Personalized skin. DIY your own personalized skin, creating a only part of your skin.
 *
 * Music cube model. After entering the mini mode, it will remain in your desktop a "Clover" shaped icon, you can freely drag the desktop, you want to control the player, cut songs, play, lyrics control, volume adjustment in control.
 *
 * Colorful custom skin. You can select skin color, set the transparency of freedom, and support transparent frosted glass (Windows Aero effects), music from becoming a type, colored.
 *
 * Search Download Manager. Search Results tab merger - all of the search results tab concentrated under one label, make operation easier. 
 *
 * Local music search, and cloud music library and interactive, custom ringtone.
 *
 * Support converting different music formats, and can change the sound as like Tom Cat, also can enhance the sound.
 *
 * Local music transferred to the cloud or mobile device.
 *
 * Local music and the human voice humming recognition.
 *
 * Music song, artist, album and playlist search supported.
 *
 * Music DSP effects and Visual effects supported.
 *
 * Music song spectrum(projectm, goom, sonique, florid, wavebar) and test supported.
 *
 * Music replay gain scan and write supported.
 *
 * Music screen saver supported.
 *
 * MPRIS D-Bus interface specification supported.
 *
 * MMS and online fm radio supported.
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

//update time 2022.07.01
#define TTK_VERSION             0x02FB0
#define TTK_VERSION_STR         "2.15.11.0"
#define TTK_VERSION_TIME_STR    "(2022/07/01)"

#define TTK_MAJOR_VERSION 2
#define TTK_MIDLE_VERSION 15
#define TTK_MINOR_VERSION 11
#define TTK_PATCH_VERSION 0

#define TTK_CONFIG_VERSION_STR	"1.0.1"

#define TTK_RC_FILEVERSION      2,15,11,0
#define TTK_RC_PRODUCTVERSION   TTK_VERSION_STR
#define TTK_RC_COMPANY          "Greedysky Studio, Ltd."
#define TTK_RC_COPYRIGHT        "Copyright (C) 2022 Greedysky Studio"

#define TTK_VERSION_CHECK(major, middle, minor, patch) ((major<<12)|(middle<<8)|(minor<<4)|(patch))

#endif // TTKVERSION_H
