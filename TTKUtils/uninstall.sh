#!/bin/sh

username=`logname`
if [ -z "${username}" ]; then
  username=`whoami`
fi

if [ "${username}" = "root" ]; then
  homepath=/root
else
  homepath=/home/${username}
fi

rm -f ${homepath}/.local/share/applications/TTKMusicPlayer.desktop
rm -f ${homepath}/.local/share/metainfo/ttkmusicplayer.metainfo.xml
