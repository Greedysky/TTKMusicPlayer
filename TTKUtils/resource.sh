#!/bin/sh

source="$1/_extras"
output="$2"

#make output dir
mkdir -p "${output}"

#copy
cp -rv "${source}/TTKInit.sh" "${output}/TTKInit.sh"
cp -rv "${source}/TTKMusicPlayer.sh" "${output}/../TTKMusicPlayer.sh"
cp -rv "${source}/TTKRoutine.sh" "${output}/../TTKRoutine.sh"
cp -rv "${source}/TTKConsole.sh" "${output}/TTKConsole.sh"
cp -rv "${source}/TTKService.sh" "${output}/TTKService.sh"
cp -rv "${source}/TTKRoutineCopy.sh" "${output}/TTKRoutineCopy.sh"

#chmod
chmod +x "${output}/TTKInit.sh"
chmod +x "${output}/../TTKMusicPlayer.sh"
chmod +x "${output}/../TTKRoutine.sh"
chmod +x "${output}/TTKConsole.sh"
chmod +x "${output}/TTKService.sh"
chmod +x "${output}/TTKRoutineCopy.sh"
