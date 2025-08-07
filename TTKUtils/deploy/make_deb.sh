#!/bin/sh

packname=TTKMusicPlayer
packvern=4.2.0.0
rootpath=data/opt
datapath=data/DEBIAN

fullname=$1
if [ $# -eq 0 ]; then
  fullname=${packname}_${packvern}
fi

mkdir -p ${rootpath}
mkdir -p ${datapath}

# create control file
echo -n "Package: ${packname}
Version: ${packvern}
Section: multimedia
Priority: optional
Architecture: amd64
Installed-Size: $(du -s ${packname} | sed -e 's/[ \t].*//')
Maintainer: Greedysky <greedysky@163.com>
Homepage: https://github.com/Greedysky/${packname}
Description: TTK Music Player
 TTKMusicPlayer imitates Kugou UI, the music player uses of 
 qmmp core library based on Qt for windows and linux.\n" > ${datapath}/control

# create postinst file
echo -n "#!/bin/bash
sh /opt/${packname}/install.sh\n" > ${datapath}/postinst
chmod +x ${datapath}/postinst

# create prerm file
echo -n "#!/bin/bash
sh /opt/${packname}/uninstall.sh\n" > ${datapath}/prerm
chmod +x ${datapath}/prerm

# create data path
mv ${packname} ${rootpath}

# create install package
dpkg -b data ${fullname}.deb
