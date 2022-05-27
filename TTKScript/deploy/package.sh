#!/bin/sh

path=share/applications/TTKMusicPlayer.desktop
packpath="$PWD/$path"
userpath=~/.local/$path
iconpath="$PWD/share/pixmaps/ttkmusicplayer.png"
execpath="$PWD/../TTKMusicPlayer"

echo -n "[Desktop Entry]
Type=Application
Version=2.15.10.0
Name=TTKMusicPlayer
Comment=天天酷音
Icon=$iconpath
Exec=$execpath
Terminal=false
Categories=Development;
Name[zh_CN]=天天酷音\n" > $packpath

cp -rv $packpath $userpath
chmod +x $userpath
