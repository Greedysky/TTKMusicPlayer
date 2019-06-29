#!/bin/sh
APP="TTKMusicplayer"
APP_VER="2.9.0.0"

DEV_PATH="$1"
PACK_PATH="ttkmusicplayer"
LIB_PATH="$PACK_PATH/usr/lib"
BIN_PATH="$PACK_PATH/usr/bin"

rm -rf ~/$PACK_PATH

mkdir -p ~/$PACK_PATH
mkdir -p ~/$BIN_PATH
mkdir -p ~/$LIB_PATH
mkdir -p ~/$PACK_PATH/usr/share/applications
mkdir -p ~/$PACK_PATH/usr/share/pixmaps
mkdir -p ~/$PACK_PATH/usr/share/mime/packages
mkdir -p ~/$PACK_PATH/usr/share/metainfo

cp $DEV_PATH/deploy/ttkmusicplayer.png ~/$LIB_PATH
cp $DEV_PATH/deploy/ttkmusicplayer.png ~/$PACK_PATH/usr/share/pixmaps
cp $DEV_PATH/deploy/ttkmusicplayer.desktop ~/$PACK_PATH/usr/share/applications
cp $DEV_PATH/deploy/ttkmusicplayer.appdata.xml ~/$PACK_PATH/usr/share/metainfo

chmod -x ~/$LIB_PATH/ttkmusicplayer.png
chmod -x ~/$PACK_PATH/usr/share/pixmaps/ttkmusicplayer.png
chmod -x ~/$PACK_PATH/usr/share/applications/ttkmusicplayer.desktop
chmod -x ~/$PACK_PATH/usr/share/metainfo/ttkmusicplayer.appdata.xml
