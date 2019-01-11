#!/bin/sh

routine="../TTKRoutine.sh"
appname=`basename $0 | sed s,\.sh$,,`  

if [ -z "$appname" ]; then
  echo "No Run App Name Input!"
  return
fi 

dirpath=`dirname $0`
tmp="${dirpath#?}"  

dirname=$dirpath
dirname=${dirname##*/}

if [ "${dirpath%$tmp}" != "/" ]; then  
  dirpath=$PWD/$dirpath  
fi  

$dirpath/$routine "$dirname/$appname"
