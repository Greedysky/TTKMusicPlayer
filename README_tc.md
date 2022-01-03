# TTKMusicPlayer
[简体中文 Chinese](./README_cn.md) | [英文 English](./README.md)

![TTKMusicplayer](https://img.shields.io/badge/Greedysky-TTKMusicPlayer-green.svg?style=flat-square)
![版本](https://img.shields.io/badge/Version-2.15.7.0-blue.svg?style=flat-square)
![許可](https://img.shields.io/badge/License-GPL%20V3-yellowgreen.svg?style=flat-square)

![徽標](https://github.com/Greedysky/Resource/blob/master/Screen/logo_pic.png?raw=true)

### **|[關於](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#使用方法)|[許可證](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#許可證)|[下載](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#下載)|[特性](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#特性)|[快照](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#快照)|[版權](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#版權)|[貢獻](https://github.com/Greedysky/TTKMusicplayer/blob/master/README_tc.md#貢獻)|**

使用方法
----
**音樂播放器基於qmmp和Qt庫開發. 編譯教程我已經寫在wiki上了.(看這裏<u>https://github.com/Greedysky/TTKMusicplayer/wiki</u> )**

### 本項目代碼不用於商用，僅限研究和學習使用，資源版權歸個音樂平臺所有。
 
## 構建狀態
 * 基於Qt4.8.7的windows-mingw 32bit版本測試通過
 * 基於Qt4.8.7的linux-gcc 64bit版本測試通過
 * 基於Qt5.x的windows-mingw 32bit版本測試通過
 * 基於Qt5.x的linux-gcc 64bit版本測試通過
 
| 平臺 | Qt & 編譯器       | 狀態                                                                                      |
| :---:    | :---:               | :---:                                                                                       |
| Linux    | 4.8.7 GCC & 5.x GCC    | ![Linux](https://img.shields.io/badge/build-passing-brightgreen.svg)                            |
| Windows  | 4.8.7 GCC & 5.x GCC       | ![Windows](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| Windows  | 5.x MSVC       | ![Windows](https://img.shields.io/badge/build-unknown-lightgrey.svg) |
| OS X     | 5.x Clang     | ![OSX](https://img.shields.io/badge/build-unknown-lightgrey.svg)                           |
 
# 開發者

許可證
---
TTKMusicPlayer是基於[license GPL v3](https://github.com/Greedysky/TTKMusicplayer/blob/master/LICENSE)條款授權的免費軟件。如果您使用TTKMusicPlayer或者它的動態庫，則務必添加相關許可證到項目中。

API 文檔
---
文檔請看這裏: **<u>https://greedysky.github.io/ttkdocs</u>**

下載
---
OSChina: **<u>http://git.oschina.net/greedysky/Musicplayer</u>**

Qmmp原版下載地址: **<u>https://qmmp.ylsoftware.com</u>**

Qmmp原版鏡像下載地址: **<u>https://github.com/Greedysky/qmmp</u>**

Qmmp修改版下載地址: **<u>https://github.com/Greedysky/TTKMusicplayer/tree/plugins</u>**

Qmmp更多插件下载地址: **<u>https://github.com/TTK-qmmp</u>**

Qmmp修改版二進制庫下載地址: **<u>http://pan.baidu.com/s/1bv4iSY</u>**

播放器及其他依賴下載地址: **<u>http://pan.baidu.com/s/1gdB5QLH</u>**

播放器皮膚下載地址: **<u>http://pan.baidu.com/s/1mh7AeG4</u>**

播放器二進制下載地址: **<u>http://pan.baidu.com/s/1kVOHN0b</u>**

特性
-------
TTKMusicPlayer的壹些特性
 * 支持絕大多數的音頻播放格式。
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
 * 支持的播放列表格式(tkpl, m3u, m3u8, pls, wpl, xspf, asx, fpl, dbpl, csv, txt)
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
 

快照
----
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
-------
 * This file is part of the TTK Music Player project.
 * Copyright (c) 2015 - 2022 Greedysky Studio.
 * Mail: greedysky@163.com.
 
貢獻
-------
 * Fork this project on github and make a branch. Commit in that branch, and push, then create a pull request to be reviewed and merged.
 * Create an issue if you have any problem when using project or you find a bug, etc.
 * What you can do: translation, write document, wiki, find or fix bugs, give your idea for this project etc.
 * If you want to join the project developed together, please send e-mail to me.
