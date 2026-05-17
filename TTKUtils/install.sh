#!/bin/sh

dirpath=`dirname $0`
tmp="${dirpath#?}"

if [ "${dirpath%${tmp}}" != "/" ]; then
  dirpath=$PWD/${dirpath}
fi

username=`logname`
if [ "${username}" = "root" ]; then
  homepath=/root
else
  homepath=/home/${username}
fi

mkdir -p ${homepath}/.local/share/icons
mkdir -p ${homepath}/.local/share/metainfo
mkdir -p ${homepath}/.local/share/applications

sh ${dirpath}/deploy/make_desktop.sh
cp -rf ${dirpath}/share/icons ${homepath}/.local/share
cp -rf ${dirpath}/share/metainfo ${homepath}/.local/share
