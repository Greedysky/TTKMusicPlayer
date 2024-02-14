#!/bin/sh

path=share/applications
mkdir $path

datapath=$path/TTKMusicPlayer.desktop
packpath="$PWD/$datapath"
userpath=~/.local/$datapath
iconpath="$PWD/share/pixmaps/ttkmusicplayer.png"
execpath="$PWD/../TTKMusicPlayer"

echo -n "[Desktop Entry]
Type=Application
Version=3.6.0.0
Name=TTKMusicPlayer
Comment=天天酷音
Icon=$iconpath
Exec=$execpath
Terminal=false
Keywords=player;audio;video;multimedia;
Categories=AudioVideo;Player;Audio;Qt;
X-KDE-StartupNotify=false
Name[zh_CN]=天天酷音\n" > $packpath

cp -rv $packpath $userpath
chmod +x $userpath
