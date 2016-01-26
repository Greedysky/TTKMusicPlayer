#!/bin/sh

# based on ffmpeg version.sh script
# check for SVN revision number
MYDIR=$(dirname $0)
revision=`cd $MYDIR && LC_ALL=C svn info 2> /dev/null | grep Revision | cut -d' ' -f2`
test $revision && revision=r$revision
test $revision && echo $revision
