# TTKMusicPlayer
[简体中文 Chinese](README_cn.md) | [英文 English](README.md)

![TTKMusicPlayer](https://img.shields.io/badge/Greedysky-TTKMusicPlayer-green.svg?style=flat-square)
![版本](https://img.shields.io/github/v/release/Greedysky/TTKMusicPlayer?style=flat-square&label=Version)
![許可](https://img.shields.io/badge/License-GPL%20V3-yellowgreen.svg?style=flat-square)
![許可](https://img.shields.io/badge/License-LGPL%20V3-yellow.svg?style=flat-square)

![徽標](TTKResource/logo_banner.png?raw=true)

### **|[關於](README_tc.md#使用方法)|[許可證](README_tc.md#許可證)|[下載](README_tc.md#下載)|[特性](README_tc.md#特性)|[快照](README_tc.md#快照)|[版權](README_tc.md#版權)|[貢獻](README_tc.md#貢獻)|**

使用方法
--------
**音樂播放器模仿酷狗，基於Qt和Qmmp庫開發，支持Windows和Linux平臺。**

**編譯教程我已經寫在wiki上了(看這裏<u>https://github.com/Greedysky/TTKMusicPlayer/wiki</u> )**

### 本項目代碼不用於商用，僅限研究和學習使用，資源版權歸個音樂平臺所有。

## 構建狀態
 * Qt stable version is tested on Qt4.x for windows-mingw 32bit
 * Qt stable version is tested on Qt4.x for ubuntu-linux-gcc 64bit
 * Qt stable version is tested on Qt5.x for windows-mingw 32bit
 * Qt stable version is tested on Qt5.x for windows-mingw 64bit
 * Qt stable version is tested on Qt5.x for ubuntu-linux-gcc 64bit

| Platform | Qt & Compiler     | Status                                                                 |
| :---:    | :---:             | :---:                                                                  |
| Linux    | 4.x GCC & 5.x GCC | ![Linux](https://img.shields.io/badge/build-passing-brightgreen.svg)   |
| Windows  | 4.x GCC & 5.x GCC | ![Windows](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| Windows  | MSVC              | ![Windows](https://img.shields.io/badge/build-unknown-lightgrey.svg)   |
| OS X     | Clang             | ![OSX](https://img.shields.io/badge/build-passing-brightgreen.svg)     |

# 開發者

許可證
--------
TTKMusicPlayer是基於[Licence GPL v3](LICENSE)條款授權的免費軟件。如果您使用TTKMusicPlayer或者它的動態庫，則務必添加相關許可證到項目中。

API 文檔
--------
文檔請看這裏: **<u>https://greedysky.github.io/ttkdocs</u>**

下載
--------
Gitee鏡像下載地址: **<u>https://gitee.com/Greedysky/TTKMusicplayer</u>**

Github鏡像下載地址: **<u>https://github.com/Greedysky/TTKMusicplayer</u>**

Qmmp原版下載地址: **<u>https://qmmp.ylsoftware.com</u>**

Qmmp原版鏡像下載地址: **<u>https://github.com/Greedysky/qmmp</u>**

Qmmp修改版下載地址: **<u>https://github.com/Greedysky/TTKMusicPlayer/tree/plugins</u>**

Qmmp更多插件下载地址: **<u>https://github.com/TTK-qmmp</u>**

Qmmp修改版二進制庫下載地址: **<u>http://pan.baidu.com/s/1bv4iSY</u>**

播放器及其他依賴下載地址: **<u>http://pan.baidu.com/s/1gdB5QLH</u>**

皮膚下載地址: **<u>http://pan.baidu.com/s/1mh7AeG4</u>**

二進制下載地址: **<u>http://pan.baidu.com/s/1kVOHN0b</u>**

特性
--------
TTKMusicPlayer的壹些特性
 * 支持絕大多數的音頻播放格式。
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
 * Future Composer File(*.fc *.fc13 *.fc14 *.fc3 *.fc4 *.smod)
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
 * TFMX Module File(*.tfm *.tfmx *.mdat *.tfx *.hip *.hipc *.hip7 *.mcmd)
 * 支持的播放列表格式(tkpl, m3u, m3u8, pls, wpl, xspf, jspf, asx, fpl, dbpl, csv, txt)
 * 華麗流星炫酷的界面設計，跨平臺有良好的性能表現。
 * 搜索和高速下載。歌曲搜索精度優化，智能節省帶寬，可以禁用網絡功能，支持多個國內音樂平臺曲庫。
 * 支持本地播放，兼容所有音頻文件，超級完美的聲音，音樂和更吸引人。
 * 友好的用戶設計。註重細節設計，個性化，操作簡單，發現音樂更方便。
 * 個性化手機鈴聲。鈴聲DIY，讓妳擁有個性化的鈴聲。
 * 個性化皮膚。DIY妳自己的個性化皮膚，創造屬於妳的專屬皮膚。
 * 音樂立方體模型。進入迷妳模式後，它會在妳的桌面上保留壹個“三葉草”形狀的圖標，妳可以自由拖動桌面，妳想控制播放器，剪切歌曲，播放，控制歌詞，控制音量。
 * 彩色定制皮膚。您可以選擇膚色，設置自由的透明度，並支持透明磨砂玻璃（Windows Aero效果）。
 * 搜索下載管理器。搜索結果標簽合並-所有的搜索結果標簽集中在壹個標簽下，使操作更容易。
 * 本地音樂搜索，雲音樂庫和互動。
 * 支持不同音樂格式的轉換，可以像湯姆貓壹樣改變聲音，也可以增強聲音。
 * 本地音樂傳輸到雲或移動設備。
 * 本地音樂和人聲哼唱識別。
 * 支持音樂歌曲、藝術家、專輯、播放列表、熱門列表和電影搜索。
 * 支持音樂數字信號處理效果和視覺效果。
 * 音樂歌曲譜（projectm，goom，sonique，florid，wavebar）支持。
 * 支持音樂重放掃描和寫入。
 * 支持音樂屏幕保護程序。
 * 支持MPRIS總線接口規範。
 * 支持MMS和在線調頻收音機。

快照
--------
### 首頁
![首頁](https://github.com/Greedysky/Resource/blob/master/Screen/1.jpg?raw=true)

### 搜索
![搜索](https://github.com/Greedysky/Resource/blob/master/Screen/2.jpg?raw=true)

### 歌詞
![歌詞](https://github.com/Greedysky/Resource/blob/master/Screen/3.jpg?raw=true)

### 桌面歌詞
![桌面歌詞](https://github.com/Greedysky/Resource/blob/master/Screen/4.jpg?raw=true)

### 遙控器
![遙控器](https://github.com/Greedysky/Resource/blob/master/Screen/5.jpg?raw=true)

### 歌詞拓展顯示
![歌詞拓展顯示](https://github.com/Greedysky/Resource/blob/master/Screen/6.jpg?raw=true)

### 視頻
![視頻](https://github.com/Greedysky/Resource/blob/master/Screen/7.jpg?raw=true)

### 視頻彈幕
![視頻彈幕](https://github.com/Greedysky/Resource/blob/master/Screen/8.jpg?raw=true)

### 音效
![音效](https://github.com/Greedysky/Resource/blob/master/Screen/9.jpg?raw=true)

### 工具
![工具](https://github.com/Greedysky/Resource/blob/master/Screen/10.jpg?raw=true)

### 音樂頻譜
![音樂頻譜](https://github.com/Greedysky/Resource/blob/master/Screen/11.jpg?raw=true)

### 其他功能
![其他功能](https://github.com/Greedysky/Resource/blob/master/Screen/12.jpg?raw=true)

版權
--------
 * This file is part of the TTK Music Player project.
 * Copyright (C) 2015 - 2025 Greedysky Studio.
 * Mail: greedysky@163.com.

貢獻
--------
 * Fork this project on github and make a branch. Commit in that branch, and push, then create a pull request to be reviewed and merged.
 * Create an issue if you have any problem when using project or you find a bug, etc.
 * What you can do: translation, write document, wiki, find or fix bugs, give your idea for this project etc.
 * If you want to join the project developed together, please send e-mail to me.
