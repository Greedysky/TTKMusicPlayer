#!/bin/sh

appname=$1

if [ -z "$appname" ]; then
  echo "No Run App Name Input!"
  return
fi

des="./"

deplist=$(ldd $appname | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des
