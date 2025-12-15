# TTKMusicPlayer
[英文 English](README.md) | [繁体中文 Chinese](README_tc.md)

![TTKMusicPlayer](https://img.shields.io/badge/Greedysky-TTKMusicPlayer-green.svg?style=flat-square)
![版本](https://img.shields.io/github/v/release/Greedysky/TTKMusicPlayer?style=flat-square&label=Version)
![许可](https://img.shields.io/badge/License-GPL%20V3-yellowgreen.svg?style=flat-square)
![许可](https://img.shields.io/badge/License-LGPL%20V3-yellow.svg?style=flat-square)

![徽标](TTKResource/logo_banner.png?raw=true)

### **|[关于](README_cn.md#使用方法)|[许可证](README_cn.md#许可证)|[下载](README_cn.md#下载)|[特性](README_cn.md#特性)|[快照](README_cn.md#快照)|[版权](README_cn.md#版权)|[贡献](README_cn.md#贡献)|**

使用方法
--------
**音乐播放器模仿酷狗，基于Qt和Qmmp库开发，支持Windows和Linux平台。**

**编译教程我已经写在wiki上了(看这里<u>https://github.com/Greedysky/TTKMusicPlayer/wiki</u> )**

### 本项目代码不用于商用，仅限研究和学习使用，资源版权归个音乐平台所有。

## 构建状态
 * Qt stable version is tested on Qt4.x for windows-mingw 32bit
 * Qt stable version is tested on Qt4.x for ubuntu-linux-gcc 64bit
 * Qt stable version is tested on Qt5.x for windows-mingw 32bit
 * Qt stable version is tested on Qt5.x for windows-mingw 64bit
 * Qt stable version is tested on Qt5.x for ubuntu-linux-gcc 64bit
 * Qt stable version is tested on Qt6.x for windows-mingw 64bit
 * Qt stable version is tested on Qt6.x for ubuntu-linux-gcc 64bit

| Platform | Qt & Compiler               | Status                                                                 |
| :---:    | :---:                       | :---:                                                                  |
| Linux    | 4.x GCC & 5.x GCC & 6.x GCC | ![Linux](https://img.shields.io/badge/build-passing-brightgreen.svg)   |
| Windows  | 4.x GCC & 5.x GCC & 6.x GCC | ![Windows](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| Windows  | 5.x MSVC & 6.x MSVC         | ![Windows](https://img.shields.io/badge/build-unknown-lightgrey.svg)   |
| OS X     | Clang                       | ![OSX](https://img.shields.io/badge/build-passing-brightgreen.svg)     |

# 开发者

许可证
--------
TTKMusicPlayer是基于[Licence GPL v3](LICENSE)条款授权的免费软件。如果您使用TTKMusicPlayer或者它的动态库，则务必添加相关许可证到项目中。

API 文档
--------
文档请看这里: **<u>https://greedysky.github.io/ttkdocs</u>**

下载
--------
Gitee镜像下载地址: **<u>https://gitee.com/Greedysky/TTKMusicplayer</u>**

Github镜像下载地址: **<u>https://github.com/Greedysky/TTKMusicplayer</u>**

Qmmp原版下载地址: **<u>https://qmmp.ylsoftware.com</u>**

Qmmp原版镜像下载地址: **<u>https://github.com/Greedysky/qmmp</u>**

Qmmp修改版下载地址: **<u>https://github.com/Greedysky/TTKMusicPlayer/tree/plugins</u>**

Qmmp更多插件下载地址: **<u>https://github.com/TTK-qmmp</u>**

Qmmp修改版二进制库下载地址: **<u>http://pan.baidu.com/s/1bv4iSY</u>**

播放器及其他依赖下载地址: **<u>http://pan.baidu.com/s/1gdB5QLH</u>**

皮肤下载地址: **<u>http://pan.baidu.com/s/1mh7AeG4</u>**

二进制下载地址: **<u>http://pan.baidu.com/s/1kVOHN0b</u>**

二进制下载Github地址: **<u>https://github.com/Greedysky/TTKMusicPlayer/releases</u>**

持续集成下载地址: **<u>https://github.com/Greedysky/TTKMusicPlayer/releases/tag/continuous-build</u>**

特性
--------
TTKMusicPlayer的一些特性
 * 支持绝大多数的音频播放格式。
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
 * Game Audio File(*.ay *.gbs *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz *.mgs *.bgm *.opx *.mpk *.mbm *.gbr *.mus *.nsd *.sgc)
 * FLAC File(*.flac *.oga)
 * Monkey's Audio File(*.ape)
 * FFmpeg File(*.wma *.ape *.tta *.m4a *.m4b *.aac *.ra *.shn *.ac3 *.mka *.vqf *.tak *.spx *.adx *.aix *.wve *.sln *.paf *.pvf *.ircam *.gsm *.avr *.amr *.dsf *.dsdiff *.webm *.3g2 *.3gp *.asf *.avi *.f4v *.flv *.hevc *.m4v *.mov *.mp4 *.mpeg *.rm *.swf *.vob *.wtv)
 * OpenMPT File(*.669 *.ams *.amf *.c67 *.dsm *.dmf *.dbm *.digi *.dtm *.far *.flx *.ft *.gdm *.it *.ice *.imf *.j2b *.mod *.mptm *.m15 *.mtm *.med *.mmd *.mmd0 *.mmd1 *.mmd2 *.mmd3 *.mdl *.mt2 *.mms *.mo3 *.mmcmp *.nst *.oct *.okt *.okta *.pt36 *.ptm *.psm *.plm *.ppm *.s3m *.stm *.stk *.st26 *.sfx *.sfx2 *.stp *.ult *.umx *.unic *.wow *.xm *.xpk *.mdr *.s3r *.xmr *.itr *.mdz *.s3z *.xmz *.itz *.mdgz *.s3gz *.xmgz *.itgz *.mdbz *.s3bz *.xmbz *.itbz)
 * Unix Amiga Delitracker Emulator File(*.aam *.ac1d *.amc *.aon *.aon8 *.aps *.ash *.ast *.avp *.bd *.bds *.bsi *.bss *.bye *.cm *.core *.cust *.dh *.di *.dl *.dlm1 *.dlm2 *.dln *.dm *.dm2 *.dmu *.doda *.dsc *.dsr *.dss *.dw *.dz *.ea *.ems *.emsv6 *.ex *.fc-m *.fp *.fred *.fw *.glue *.gmc *.gray *.hd *.hip *.hip7 *.hipc *.hot *.ims *.is *.is20 *.jam *.jcb *.jcbo *.jd *.jmf *.jo *.jpo *.js *.jt *.kh *.kim *.kris *.lion *.lme *.ma *.mc *.mcmd *.mco *.md *.mdst *.mii *.mk2 *.mkii *.ml *.mm8 *.mmdc *.mok *.mon *.mosh *.mso *.mtp2 *.mug *.mug2 *.mw *.ntp *.np2 *.np3 *.one *.p4x *.p5x *.p6x *.pap *.pn *.pp21 *.pp30 *.pr1 *.prom *.prun *.pru2 *.ps *.psa *.psf *.pt *.ptm *.puma *.pvp *.rh *.rho *.riff *.rmc *.s7g *.sa *.sa_old *.sas *.sb *.sc *.scn *.scr *.sct *.scumm *.sdr *.sg *.sid1 *.sid2 *.skyt *.sm *.sm3 *.smn *.smpro *.sng *.snk *.soc *.sog *.spl *.sqt *.ss *.sun *.syn *.synmod *.tcb *.tf *.tfmx *.thx *.tit *.tits *.tme *.tp2 *.tp3 *.tro *.tronic *.tw *.two *.ufo *.unic *.vss *.wb *.xann *.ymst *.cin *.smus *.snx *.tiny *.adsc *.dat *.dum *.osp;alp.* dns.* jpn.* jpnd.* max.* mcr.* mfp.* midi.* npp.* pat.* qpa.* qts.* rjp.* sjs.* thm.* tmk.* tpu.* uds.*)
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
 * TFMX Related Audio File(*.tfm *.tfmx *.mdat *.tfx *.hip *.hipc *.hip7 *.mcmd)
 * Jaytrax Audio File(*.jxs)
 * SBStudio PAC Audio File(*.pac)
 * 支持的播放列表格式(tkpl, m3u, m3u8, pls, wpl, xspf, jspf, asx, fpl, dbpl, csv, txt)
 * 华丽流星炫酷的界面设计，跨平台有良好的性能表现。
 * 搜索和高速下载。歌曲搜索精度优化，智能节省带宽，可以禁用网络功能，支持多个国内音乐平台曲库。
 * 支持本地播放，兼容所有音频文件，超级完美的声音，音乐和更吸引人。
 * 友好的用户设计。注重细节设计，个性化，操作简单，发现音乐更方便。
 * 个性化手机铃声。铃声DIY，让你拥有个性化的铃声。
 * 个性化皮肤。DIY你自己的个性化皮肤，创造属于你的专属皮肤。
 * 音乐立方体模型。进入迷你模式后，它会在你的桌面上保留一个“三叶草”形状的图标，你可以自由拖动桌面，你想控制播放器，剪切歌曲，播放，控制歌词，控制音量。
 * 彩色定制皮肤。您可以选择肤色，设置自由的透明度，并支持透明磨砂玻璃（Windows Aero效果）。
 * 搜索下载管理器。搜索结果标签合并-所有的搜索结果标签集中在一个标签下，使操作更容易。
 * 本地音乐搜索，云音乐库和互动。
 * 支持不同音乐格式的转换，可以像汤姆猫一样改变声音，也可以增强声音。
 * 本地音乐传输到云或移动设备。
 * 本地音乐和人声哼唱识别。
 * 支持音乐歌曲、艺术家、专辑、播放列表、热门列表和电影搜索。
 * 支持音乐数字信号处理效果和视觉效果。
 * 音乐歌曲谱（projectm，goom，sonique，florid，wavebar）支持。
 * 支持音乐重放扫描和写入。
 * 支持音乐屏幕保护程序。
 * 支持MPRIS总线接口规范。
 * 支持MMS和在线调频收音机。

快照
--------
### 首页
![首页](https://github.com/Greedysky/Resource/blob/master/Screen/1.jpg?raw=true)

### 搜索
![搜索](https://github.com/Greedysky/Resource/blob/master/Screen/2.jpg?raw=true)

### 歌词
![歌词](https://github.com/Greedysky/Resource/blob/master/Screen/3.jpg?raw=true)

### 桌面歌词
![桌面歌词](https://github.com/Greedysky/Resource/blob/master/Screen/4.jpg?raw=true)

### 遥控器
![遥控器](https://github.com/Greedysky/Resource/blob/master/Screen/5.jpg?raw=true)

### 歌词拓展显示
![歌词拓展显示](https://github.com/Greedysky/Resource/blob/master/Screen/6.jpg?raw=true)

### 视频
![视频](https://github.com/Greedysky/Resource/blob/master/Screen/7.jpg?raw=true)

### 视频弹幕
![视频弹幕](https://github.com/Greedysky/Resource/blob/master/Screen/8.jpg?raw=true)

### 音效
![音效](https://github.com/Greedysky/Resource/blob/master/Screen/9.jpg?raw=true)

### 工具
![工具](https://github.com/Greedysky/Resource/blob/master/Screen/10.jpg?raw=true)

### 音乐频谱
![音乐频谱](https://github.com/Greedysky/Resource/blob/master/Screen/11.jpg?raw=true)

### 其他功能
![其他功能](https://github.com/Greedysky/Resource/blob/master/Screen/12.jpg?raw=true)

版权
--------
 * This file is part of the TTK Music Player project.
 * Copyright (C) 2015 - 2026 Greedysky Studio.
 * Mail: greedysky@163.com.

贡献
--------
 * Fork this project on github and make a branch. Commit in that branch, and push, then create a pull request to be reviewed and merged.
 * Create an issue if you have any problem when using project or you find a bug, etc.
 * What you can do: translation, write document, wiki, find or fix bugs, give your idea for this project etc.
 * If you want to join the project developed together, please send e-mail to me.
