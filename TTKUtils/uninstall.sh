#!/bin/sh

username=`logname`
if [ "${username}" = "root" ]; then
  homepath=/root
else
  homepath=/home/${username}
fi

rm -f ${homepath}/.local/share/applications/TTKMusicPlayer.desktop
rm -f ${homepath}/.local/share/metainfo/ttkmusicplayer.metainfo.xml
