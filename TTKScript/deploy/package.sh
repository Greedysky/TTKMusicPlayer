#!/bin/sh

path=share/applications/TTKMusicPlayer.desktop
packpath="$PWD/$path"
userpath=~/.local/$path
iconpath="$PWD/share/pixmaps/ttkmusicplayer.png"
execpath="$PWD/../TTKMusicPlayer"

echo -n "[Desktop Entry]
Type=Application
Version=2.13.4.0
Name=天天酷音
Comment=天天酷音
Icon=$iconpath
Exec=$execpath
Terminal=false
Categories=Development;
Name[zh_CN]=TTKMusicPlayer\n" > $packpath

cp -rv $packpath $userpath
chmod +x $userpath
