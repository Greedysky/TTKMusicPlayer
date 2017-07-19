#ifndef SPEKPALETTE_H
#define SPEKPALETTE_H

#include <stdint.h>

typedef enum palette {
    PALETTE_SPECTRUM,
    PALETTE_SOX,
    PALETTE_MONO,
    PALETTE_COUNT,
    PALETTE_DEFAULT = PALETTE_SOX,
}Palette;

uint32_t spek_palette(Palette palette, double level);

#endif
