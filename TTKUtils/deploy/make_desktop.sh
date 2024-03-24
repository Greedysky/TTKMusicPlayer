#!/bin/sh

path=share/applications
if [ ! -d $path ]; then
  mkdir $path
fi

datapath=$path/TTKMusicPlayer.desktop
packpath="$PWD/$datapath"
userpath=~/.local/$datapath
iconpath="$PWD/share/pixmaps/ttkmusicplayer.png"
execpath="$PWD/../TTKMusicPlayer"

echo -n "[Desktop Entry]
Type=Application
Version=3.6.0.0
Name=TTKMusicPlayer
Name[zh_CN]=天天酷音
Name[zh_TW]=天天酷音
GenericName=Audio player
GenericName[zh_CN]=音乐播放器
GenericName[zh_TW]=音樂播放器
Comment=Listen to music
Comment[zh_CN]=倾听音乐
Comment[zh_TW]=聆聽音樂
Icon=$iconpath
Exec=$execpath
Terminal=false
Keywords=player;audio;video;multimedia;sound;musicplayer;
Categories=AudioVideo;Player;Audio;Qt;
MimeType=application/x-ogg;audio/mp3;audio/mpeg;audio/flac;audio/opus;audio/x-mp3;audio/x-mpeg;audio/x-ms-wma;audio/x-musepack;audio/x-tta;audio/x-wavpack;application/ogg;audio/x-vorbis+ogg;audio/x-scpls;audio/x-mpegurl;audio/x-it;audio/x-mod;audio/x-s3m;audio/x-stm;audio/x-xm;audio/x-ape;application/x-cue;x-content/audio-cdda;audio/x-ffmpeg-shorten;audio/aac;audio/x-aac;audio/m4a;audio/x-m4a;
X-KDE-StartupNotify=false\n" > $packpath

cp -rv $packpath $userpath
chmod +x $userpath
