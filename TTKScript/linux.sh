#!/bin/bash

LCEXE="$1"
SOURCE="$2/TTKLanguage"
OUTPUT="$3/MLanguage"

#make output dir
`mkdir -p $OUTPUT`

#make
for i in $SOURCE/*.ts
do
  $LCEXE $i
done

#rename
for i in $SOURCE/*.qm
do
  `rename -vf 's/.qm/.ln/' $i`
done

#move
for i in $SOURCE/*.ln
do
  `mv $i $OUTPUT`
done

