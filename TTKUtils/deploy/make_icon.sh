#!/bin/bash

module="$1"
utilspath="$2"
sharepath="$3"

iconsizes="16 32 48 64 128 256 512"
iconsource="${utilspath}/deploy/share/pixmaps/${module}.png"

for size in ${iconsizes}; do
  icondir="${sharepath}/icons/hicolor/${size}x${size}/apps"
  mkdir -p "${icondir}"
  cp "${iconsource}" "${icondir}/${module}.png"
done
