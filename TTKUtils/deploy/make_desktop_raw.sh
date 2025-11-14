#!/bin/sh

packpath="$1"
iconpath="$2"
execpath="$3"

echo -n "[Desktop Entry]
Type=Application
Version=1.0
Name=TTKMusicPlayer
Name[zh_CN]=天天酷音
Name[zh_TW]=天天酷音
GenericName=Audio player
GenericName[zh_CN]=音乐播放器
GenericName[zh_TW]=音樂播放器
Comment=Listen to music
Comment[zh_CN]=倾听音乐
Comment[zh_TW]=聆聽音樂
Icon=${iconpath}
Exec=${execpath}
Terminal=false
Keywords=player;audio;video;multimedia;sound;musicplayer;
Categories=AudioVideo;Player;Audio;Qt;Network;
MimeType=application/x-ogg;audio/mp3;audio/mpeg;audio/flac;audio/x-flac;audio/opus;audio/x-mp3;audio/x-mpeg;audio/x-ms-wma;audio/x-musepack;audio/x-tta;audio/x-wavpack;application/ogg;audio/x-vorbis+ogg;audio/x-scpls;audio/x-mpegurl;audio/x-it;audio/x-mod;audio/x-s3m;audio/x-stm;audio/x-xm;audio/x-ape;application/x-cue;x-content/audio-cdda;audio/x-ffmpeg-shorten;audio/aac;audio/aacp;audio/x-aac;audio/m4a;audio/x-m4a;audio/aiff;audio/basic;audio/mid;audio/mp2;audio/mp4;audio/wav;audio/3gpp;audio/3gpp2;audio/vnd.rn-realaudio;audio/ac3;audio/eac3;audio/dts;audio/true-hd;audio/x-matroska;audio/speech;audio/webm;
X-KDE-StartupNotify=false\n" > ${packpath}
