# TTKMusicPlayer
[简体中文 Chinese](./README_cn.md) | [繁体中文 Chinese](./README_tc.md)

![TTKMusicplayer](https://img.shields.io/badge/Greedysky-TTKMusicPlayer-green.svg?style=flat-square)
![Version](https://img.shields.io/badge/Version-2.15.7.0-blue.svg?style=flat-square)
![License](https://img.shields.io/badge/License-GPL%20V3-yellowgreen.svg?style=flat-square)

![LOGO](https://github.com/Greedysky/Resource/blob/master/Screen/logo_pic.png?raw=true)

### **|[About](https://github.com/Greedysky/TTKMusicplayer#usage)|[License](https://github.com/Greedysky/TTKMusicplayer#license)|[Download](https://github.com/Greedysky/TTKMusicplayer#download)|[Features](https://github.com/Greedysky/TTKMusicplayer#features)|[Screenshots](https://github.com/Greedysky/TTKMusicplayer#screenshots)|[Copyright](https://github.com/Greedysky/TTKMusicplayer#copyright)|[Contribute](https://github.com/Greedysky/TTKMusicplayer#how-to-contribute)|**

Usage
----
**The music player use of qmmp core library based on Qt. Compiler tutorial I have written in the wiki.(See here <u>https://github.com/Greedysky/TTKMusicplayer/wiki</u> )**

### Attention: This software is only for learning and communication, the songs' and playlists' copyright belongs to each music platform.
 
## CI Status
 * Qt develop version be tested on Qt4.8.7 for windows-mingw 32bit
 * Qt develop version be tested on Qt4.8.7 for linux-gcc 64bit
 * Qt develop version be tested on Qt5.x for windows-mingw 32bit
 * Qt develop version be tested on Qt5.x for linux-gcc 64bit
 
| Platform | Qt & Compiler       | Status                                                                                      |
| :---:    | :---:               | :---:                                                                                       |
| Linux    | 4.8.7 GCC & 5.x GCC    | ![Linux](https://img.shields.io/badge/build-passing-brightgreen.svg)                            |
| Windows  | 4.8.7 GCC & 5.x GCC       | ![Windows](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| Windows  | 5.x MSVC       | ![Windows](https://img.shields.io/badge/build-unknown-lightgrey.svg) |
| OS X     | 5.x Clang     | ![OSX](https://img.shields.io/badge/build-unknown-lightgrey.svg)                           |
 
# For Developers

License
---
TTKMusicPlayer is free software licensed under the term of [Licence GPL v3](https://github.com/Greedysky/TTKMusicplayer/blob/master/LICENSE). If you use TTKMusicPlayer or its constituent libraries, you must adhere to the terms of the license in question.

API Document
---
You Can See Here: **<u>https://greedysky.github.io/ttkdocs</u>**

Download
---
OSChina: **<u>http://git.oschina.net/greedysky/Musicplayer</u>**

Qmmp Source Player URL: **<u>https://qmmp.ylsoftware.com</u>**

Qmmp Source Mirror URL: **<u>https://github.com/Greedysky/qmmp</u>**

Qmmp Source Modified URL: **<u>https://github.com/Greedysky/TTKMusicplayer/tree/plugins</u>**

Qmmp Extra Plugins URL: **<u>https://github.com/TTK-qmmp</u>**

Qmmp Core Modified Library URL: **<u>http://pan.baidu.com/s/1bv4iSY</u>**

Player Base URL: **<u>http://pan.baidu.com/s/1gdB5QLH</u>**

Skin Download URL: **<u>http://pan.baidu.com/s/1mh7AeG4</u>**

Executable Download URL: **<u>http://pan.baidu.com/s/1kVOHN0b</u>**

Features
-------
TTKMusicPlayer can meet your most demands
 * Support for multiple music formats.
 * SID File(*.sid *.mus *.str *.prg *.P00)
 * MusePack File(*.mpc)
 * WavPack File(*.wv)
 * Ogg Vorbis File(*.ogg *.ogv)
 * Ogg Opus File(*.opus)
 * ADTS AAC File(*.aac)
 * MIDI File(*.mid)
 * PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)
 * CUE File(*.cue)
 * MPEG File(*.mp1 *.mp2 *.mp3)
 * Game Audio File(*.ay *.gbs *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)
 * FLAC File(*.flac *.oga)
 * Monkey's Audio File(*.ape)
 * FFmpeg File(*.wma *.ape *.tta *.m4a *.ra *.shn *.vqf *.ac3 *.tak *.dsf *.dsdiff *.webm)
 * OpenMPT File(*.669 *.ams *.amf *.dsm *.dmf *.dbm *.digi *.dtm *.far *.gdm *.it *.ice *.imf *.j2b *.mod *.mptm *.m15 *.mtm *.med *.mdl *.mt2 *.mms *.mo3 *.mmcmp *.nst *.okt *.okta *.pt36 *.ptm *.psm *.plm *.ppm *.s3m *.stm *.stk *.st26 *.sfx *.sfx2 *.stp *.ult *.umx *.wow *.xm *.xpk)
 * AdLib Sound File(*.a2m *.adl *.adlib *.amd *.as3m *.bam *.cmf *.d00 *.dfm *.dmo *.dro *.dtm *.hsc *.imf *.jbm *.laa *.lds *.m *.mad *.mkj *.msc *.mtk *.rad *.raw *.sa2 *.sat *.sng *.sqx *.xad *.xms *.xsm)
 * Another Slight Atari File(*.sap *.cm3 *.cmc *.cmr *.cms *.dmc *.dlt *.mpd *.mpt *.rmt *.tm2 *.tm8 *.tmc)
 * DTS Coherent Acoustics File(*.dts *.cpt)
 * Future Composer File(*.fc *.fc13 *.fc14 *.smod)
 * OptimFrog File(*.ofr *.ofs)
 * Audio Overload File(*.psf *.spu *.ssf *.qsf *.dsf *.minipsf *.minissf *.miniqsf *.minidsf *.2sf *.mini2sf *.gsf *.minigsf *.usf *.miniusf *.ncsf *.minincsf *.snsf *.minisnsf)
 * Atari ST(E) And Amiga File(*.sc68)
 * Speex Audio File(*.spx)
 * V2 Module Audio File(*.v2m)
 * AY/YM Audio File(*.vtx *.asc *.sqt *psg *.stc *.stp *.pt1 *.pt2 *.pt3 *.ay *.emul)
 * ST-Sound File(*.ym)
 * HVL Module Player File(*.ahx *.hvl)
 * BP SoundMon Audio File(*.bp *.bp3)
 * MDX Chiptunes Audio File(*.mdx *.m)
 * Cave Story's org Audio File(*.org)
 * Playlist formats(tkpl, m3u, m3u8, pls, wpl, xspf, asx, fpl, dbpl, csv, txt)
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
 

Screenshots
----
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
-------
 * This file is part of the TTK Music Player project.
 * Copyright (c) 2015 - 2022 Greedysky Studio.
 * Mail: greedysky@163.com.
 
How To Contribute
-------
 * Fork this project on github and make a branch. Commit in that branch, and push, then create a pull request to be reviewed and merged.
 * Create an issue if you have any problem when using project or you find a bug, etc.
 * What you can do: translation, write document, wiki, find or fix bugs, give your idea for this project etc.
 * If you want to join the project developed together, please send e-mail to me.
