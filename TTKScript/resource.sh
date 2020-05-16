#!/bin/bash

SOURCE="$1/A_extras"
OUTPUT="$2"

#make output dir
`mkdir -p "$OUTPUT"`

#copy
`cp -rv "$SOURCE/TTKInit.sh" "$OUTPUT/TTKInit.sh"`
`cp -rv "$SOURCE/TTKMusicPlayer.sh" "$OUTPUT/../TTKMusicPlayer.sh"`
`cp -rv "$SOURCE/TTKRoutine.sh" "$OUTPUT/../TTKRoutine.sh"`
`cp -rv "$SOURCE/TTKConsole.sh" "$OUTPUT/TTKConsole.sh"`
`cp -rv "$SOURCE/TTKService.sh" "$OUTPUT/TTKService.sh"`
`cp -rv "$SOURCE/TTKRoutineCopy.sh" "$OUTPUT/TTKRoutineCopy.sh"`
