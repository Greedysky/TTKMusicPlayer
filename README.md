# TTKMusicPlayer
[简体中文 Chinese](README_cn.md) | [繁体中文 Chinese](README_tc.md)

![TTKMusicPlayer](https://img.shields.io/badge/Greedysky-TTKMusicPlayer-green.svg?style=flat-square)
![Version](https://img.shields.io/github/v/release/Greedysky/TTKMusicPlayer?style=flat-square&label=Version)
![License](https://img.shields.io/badge/License-GPL%20V3-yellowgreen.svg?style=flat-square)
![License](https://img.shields.io/badge/License-LGPL%20V3-yellow.svg?style=flat-square)

![LOGO](TTKResource/logo_banner.png?raw=true)

### **|[About](#usage)|[License](#license)|[Download](#download)|[Features](#features)|[Screenshots](#screenshots)|[Copyright](#copyright)|[Contribute](#how-to-contribute)|**

Usage
--------
**TTKMusicPlayer imitates Kugou music, uses of qmmp core library, based on Qt for windows and linux.**

**Compiler tutorial I have written in the wiki.(See here <u>https://github.com/Greedysky/TTKMusicPlayer/wiki</u> )**

### Attention: This software is only for learning and communication, the songs' and playlists' copyright belongs to each music platform.

## CI Status
 * Qt stable version is tested on Qt4.x for windows-mingw 32bit
 * Qt stable version is tested on Qt4.x for ubuntu-linux-gcc 64bit
 * Qt stable version is tested on Qt5.x for windows-mingw 32bit
 * Qt stable version is tested on Qt5.x for ubuntu-linux-gcc 64bit

| Platform | Qt & Compiler     | Status                                                                 |
| :---:    | :---:             | :---:                                                                  |
| Linux    | 4.x GCC & 5.x GCC | ![Linux](https://img.shields.io/badge/build-passing-brightgreen.svg)   |
| Windows  | 4.x GCC & 5.x GCC | ![Windows](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| Windows  | MSVC              | ![Windows](https://img.shields.io/badge/build-unknown-lightgrey.svg)   |
| OS X     | Clang             | ![OSX](https://img.shields.io/badge/build-unknown-lightgrey.svg)       |

# For Developers

License
--------
TTKMusicPlayer is free software licensed under the term of [Licence GPL v3](LICENSE). If you use TTKMusicPlayer or its constituent libraries, you must adhere to the terms of the license in question.

API Document
--------
You Can See Here: **<u>https://greedysky.github.io/ttkdocs</u>**

Download
--------
Gitee URL: **<u>https://gitee.com/Greedysky/TTKMusicplayer</u>**

Github URL: **<u>https://github.com/Greedysky/TTKMusicplayer</u>**

Qmmp Source Player URL: **<u>https://qmmp.ylsoftware.com</u>**

Qmmp Source Mirror URL: **<u>https://github.com/Greedysky/qmmp</u>**

Qmmp Source Modified URL: **<u>https://github.com/Greedysky/TTKMusicPlayer/tree/plugins</u>**

Qmmp Extra Plugins URL: **<u>https://github.com/TTK-qmmp</u>**

Qmmp Core Modified Library URL: **<u>http://pan.baidu.com/s/1bv4iSY</u>**

Player Base URL: **<u>http://pan.baidu.com/s/1gdB5QLH</u>**

Skin Download URL: **<u>http://pan.baidu.com/s/1mh7AeG4</u>**

Executable Download URL: **<u>http://pan.baidu.com/s/1kVOHN0b</u>**

Features
--------
TTKMusicPlayer can meet your most demands
 * Support for multiple music formats.
 * SID File(*.sid *.psid *.mus *.str *.p00 *.prg *.c64)
 * MusePack File(*.mpc)
 * WavPack File(*.wv)
 * Ogg Vorbis File(*.ogg *.ogv)
 * Ogg Opus File(*.opus)
 * ADTS AAC File(*.aac)
 * MIDI File(*.mid *.mus *.hmp *.hmi *.xmi)
 * PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)
 * CUE File(*.cue)
 * MPEG File(*.mp1 *.mp2 *.mp3)
 * Game Audio File(*.ay *.gbs *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)
 * FLAC File(*.flac *.oga)
 * Monkey's Audio File(*.ape)
 * FFmpeg File(*.wma *.ape *.tta *.m4a *.m4b *.aac *.ra *.shn *.ac3 *.mka *.vqf *.tak *.spx *.adx *.aix *.wve *.sln *.paf *.pvf *.ircam *.gsm *.avr *.amr *.dsf *.dsdiff *.webm *.3g2 *.3gp *.asf *.avi *.f4v *.flv *.hevc *.m4v *.mov *.mp4 *.mpeg *.rm *.swf *.vob *.wtv)
 * OpenMPT File(*.669 *.ams *.amf *.c67 *.dsm *.dmf *.dbm *.digi *.dtm *.far *.fmt *.gdm *.it *.ice *.imf *.j2b *.mod *.mptm *.m15 *.mtm *.med *.mmd *.mmd0 *.mmd1 *.mmd2 *.mmd3 *.mdl *.mt2 *.mms *.mo3 *.mmcmp *.nst *.okt *.okta *.pt36 *.ptm *.psm *.plm *.ppm *.s3m *.stm *.stk *.st26 *.sfx *.sfx2 *.stp *.ult *.umx *.wow *.xm *.xpk *.mdr *.s3r *.xmr *.itr *.mdz *.s3z *.xmz *.itz *.mdgz *.s3gz *.xmgz *.itgz *.mdbz *.s3bz *.xmbz *.itbz)
 * Unix Amiga Delitracker Emulator File(*.aam *.amc *.aon *.aon8 *.aps *.ash *.ast *.avp *.bd *.bds *.bsi *.bss *.bye *.cm *.core *.cust *.dh *.dl *.dlm1 *.dlm2 *.dln *.dm *.dm2 *.dmu *.doda *.dsc *.dsr *.dss *.dw *.dz *.ea *.ems *.emsv6 *.ex *.fp *.fred *.fw *.glue *.gmc *.gray *.hd *.hip *.hip7 *.hipc *.hot *.ims *.is *.is20 *.jam *.jcb *.jcbo *.jd *.jmf *.jo *.jpo *.jt *.kh *.kim *.kris *.lion *.lme *.ma *.mc *.mcmd *.mco *.md *.mii *.mk2 *.mkii *.ml *.mm8 *.mmdc *.mok *.mon *.mosh *.mso *.mug *.mug2 *.mw *.ntp *.pap *.pn *.ps *.psa *.psf *.pt *.puma *.pvp *.rh *.rho *.riff *.rmc *.s7g *.sa *.sas *.sb *.sc *.scn *.scr *.sct *.scumm *.sdr *.sg *.sid1 *.sid2 *.sm *.sm3 *.smn *.smpro *.sng *.snk *.soc *.sog *.spl *.sqt *.ss *.sun *.syn *.synmod *.tcb *.tf *.tfmx *.thx *.tits *.tme *.tro *.tronic *.tw *.ufo *.vss *.wb *.smus *.snx *.tiny *.adsc *.dat *.dum *.osp *.dns *.jpn *.jpnd *.max *.mcr *.mfp *.npp *.pat *.qpa *.qts *.sjs *.thm *.tmk *.tpu *.uds)
 * RSN Archive File(*.rsn *.rps *.rdc *.rds *.rgs *.r64)
 * 7z Archive File(*.7z *.xz *.bz2 *.gz *.tar *.zip *.wim *.ar *.arj *.cab *.chm *.cpio *.cramfs *.dmg *.ext *.fat *.gpt *.hfs *.ihex *.iso *.lzh *.lzma *.mbr *.msi *.nsis *.ntfs *.qcow2 *.rar *.rpm *.squashfs *.udf *.uefi *.vdi *.vhd *.vmdk *.xar *.z)
 * AdLib Sound File(*.a2m *.adl *.adlib *.agd *.amd *.as3m *.bam *.bmf *.cff *.cmf *.d00 *.dfm *.dmo *.dro *.dtm *.got *.ha2 *.hsc *.hsq *.hsp *.imf *.ims *.jbm *.laa *.lds *.m *.mad *.mdi *.mkj *.msc *.mtk *.mus *.rac *.rad *.raw *.rix *.s3m *.sa2 *.sat *.sdb *.sng *.sop *.sqx *.xad *.xms *.xsm *.vgm *.vgz *.wlf *.ksm *.rol *.sci)
 * XMP File(*.abk *.dsym *.emod *.fnk *.gtk *.ims *.liq *.mfp *.mgt *.rtm *.tcb *.xmf)
 * Another Slight Atari File(*.cm3 *.cmc *.cmr *.cms *.dlt *.dmc *.fc *.mpd *.mpt *.rmt *.sap *.tm2 *.tm8 *.tmc)
 * DTS Coherent Acoustics File(*.dts *.cpt)
 * Future Composer File(*.fc *.fc13 *.fc14 *.smod)
 * OptimFrog File(*.ofr *.ofs)
 * Audio Overload File(*.pcm *.msu *.spu *.psf *.ssf *.qsf *.dsf *.psf2 *.minipsf *.minissf *.miniqsf *.minidsf *.minipsf2 *.2sf *.mini2sf *.gsf *.minigsf *.usf *.miniusf *.ncsf *.minincsf *.snsf *.minisnsf)
 * Quite OK Audio File(*.qoa)
 * Buzzic Audio File(*.buz *buz2)
 * S98 Audio File(*.s98 *.cym *.mym *.x1f)
 * Atari ST(E) And Amiga File(*.sc68 *.snd *.sndh)
 * Speex Audio File(*.spx)
 * V2 Player File(*.v2m)
 * AY/YM Audio File(*.asc *.ay *.emul *.sqt *.st13 *.stc *.stp *.stp2 *.psc *.psg *.pt1 *.pt2 *.pt3 *.vtx *.zxs)
 * ST-Sound File(*.ym)
 * SunVox Audio File(*.sunvox)
 * Hively Audio File(*.ahx *.hvl)
 * BP SoundMon Audio File(*.bp *.bp3)
 * Ken Silverman's Audio File(*.kdm *.ksm *.sm *.snd)
 * XMDX Game Audio File(*.mdx *.pdx *.m *.mub *.muc *.vgs *.bgm *.mml)
 * Cave Story's org Audio File(*.org *.pttune *.ptcop)
 * Final Music System Tracker Module File(*.tfm *.tfmx *.mdat *.tfx)
 * Playlist formats(tkpl, m3u, m3u8, pls, wpl, xspf, jspf, asx, fpl, dbpl, csv, txt)
 * Junior fans interface. Interface cool, gorgeous pop. Maintaining a perfect performance under Vista and XP, refreshing Screenshots.
 * Search and high-speed downloads. Song search accuracy optimization, intelligent save bandwidth, you can disable the network function.
 * It supports local playback, compatible with all audio files, super perfect sound, music and more appealing.
 * User-friendly design. Attention to detail design, personalize, simple operation, found music more convenient.
 * Personalized cell phone ringtones. Ringtones DIY, so you have personalized ringtones.
 * Personalized skin. DIY your own personalized skin, creating a only part of your skin.
 * Music cube model. After entering the mini mode, it will remain in your desktop a "Clover" shaped icon, you can freely drag the desktop, you want to control the player, cut songs, play, lyrics control, volume adjustment in control.
 * Colorful custom skin. You can select skin color, set the transparency of freedom, and support transparent frosted glass (Windows Aero effects).
 * Search Download Manager. Search Results tab merger - all of the search results tab concentrated under one label, make operation easier.
 * Local music search, and cloud music library and interactive.
 * Support converting different music formats, and can change the sound as like Tom Cat, also can enhance the sound.
 * Local music transferred to the cloud or mobile device.
 * Local music and the human voice humming recognition.
 * Music song, artist, album, playlist, toplist and movie search supported.
 * Music DSP effects and Visual effects supported.
 * Music song spectrum(projectm, goom, sonique, florid, wavebar) and test supported.
 * Music replay gain scan and write supported.
 * Music screen saver supported.
 * MPRIS D-Bus interface specification supported.
 * MMS and online fm radio supported.

Screenshots
--------
### Index
![Index](https://github.com/Greedysky/Resource/blob/master/Screen/1.jpg?raw=true)

### Search
![Search](https://github.com/Greedysky/Resource/blob/master/Screen/2.jpg?raw=true)

### Lrc
![Lrc](https://github.com/Greedysky/Resource/blob/master/Screen/3.jpg?raw=true)

### Desktop Lrc
![Desktop Lrc](https://github.com/Greedysky/Resource/blob/master/Screen/4.jpg?raw=true)

### Remote Controller
![Remote Controller](https://github.com/Greedysky/Resource/blob/master/Screen/5.jpg?raw=true)

### Lrc Display Expand
![Lrc Display Expand](https://github.com/Greedysky/Resource/blob/master/Screen/6.jpg?raw=true)

### Movie
![Movie](https://github.com/Greedysky/Resource/blob/master/Screen/7.jpg?raw=true)

### Video Barrage
![Video Barrage](https://github.com/Greedysky/Resource/blob/master/Screen/8.jpg?raw=true)

### Magic Music
![Magic Music](https://github.com/Greedysky/Resource/blob/master/Screen/9.jpg?raw=true)

### Other Tools
![Other Tools](https://github.com/Greedysky/Resource/blob/master/Screen/10.jpg?raw=true)

### Spectrum
![Spectrum](https://github.com/Greedysky/Resource/blob/master/Screen/11.jpg?raw=true)

### Other Functions
![Other Functions](https://github.com/Greedysky/Resource/blob/master/Screen/12.jpg?raw=true)

Copyright
--------
 * This file is part of the TTK Music Player project.
 * Copyright (C) 2015 - 2024 Greedysky Studio.
 * Mail: greedysky@163.com.

How To Contribute
--------
 * Fork this project on github and make a branch. Commit in that branch, and push, then create a pull request to be reviewed and merged.
 * Create an issue if you have any problem when using project or you find a bug, etc.
 * What you can do: translation, write document, wiki, find or fix bugs, give your idea for this project etc.
 * If you want to join the project developed together, please send e-mail to me.
