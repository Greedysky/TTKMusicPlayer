#include "visualpalette.h"
#include <qmath.h>

#include <QColor>
#include <QVector>

#define GRADIENT_TABLE_SIZE 256

static bool __globalTableInit__ = false;
static uint32_t __globalTableGolors__[GRADIENT_TABLE_SIZE];
static QVector<QColor> __globalGolors__ = { QColor(0, 0, 0),
                                            QColor(0, 32, 100),
                                            QColor(0, 120, 160),
                                            QColor(128, 255, 120),
                                            QColor(255, 255, 0),
                                            QColor(255, 128, 0),
                                            QColor(255, 0, 0)
                                          };

static void createGradientTable()
{
    constexpr int numbers = 6;
    for(int i = 0; i < GRADIENT_TABLE_SIZE; ++i)
    {
        double position = (double)i / GRADIENT_TABLE_SIZE;
        /* if position > 1 then we have repetition of colors it maybe useful    */
        if(position > 1.0)
        {
            if(position - int(position) == 0.0)
            {
                position = 1.0;
            }
            else
            {
                position = position - int(position);
            }
        }

        const double m = numbers * position;
        const int n = (int)m; // integer of m
        const double f = m - n;  // fraction of m

        __globalTableGolors__[i] = 0xFF0000;
        if(n < numbers)
        {
            __globalTableGolors__[i] = ((uint32_t)((__globalGolors__[n].red()) + f * ((__globalGolors__[n+1].red()) - (__globalGolors__[n].red()))) & 0xFF) << 16 |
                ((uint32_t)((__globalGolors__[n].green()) + f * ((__globalGolors__[n+1].green()) - (__globalGolors__[n].green()))) & 0xFF) << 8 |
                ((uint32_t)((__globalGolors__[n].blue()) + f * ((__globalGolors__[n+1].blue()) - (__globalGolors__[n].blue()))) & 0xFF) << 0;
        }
        else if(n == numbers)
        {
            __globalTableGolors__[i] = ((uint32_t)(__globalGolors__[n].red()) & 0xFF) << 16 |
                ((uint32_t)(__globalGolors__[n].green()) & 0xFF) << 8 |
                ((uint32_t)(__globalGolors__[n].blue()) & 0xFF) << 0;
        }
        else
        {
            __globalTableGolors__[i] = 0xFFFFFF;
        }
    }
}

// Modified version of Dan Bruton's algorithm:
// http://www.physics.sfasu.edu/astro/color/spectra.html
static uint32_t spectrum(double level)
{
    level *= 0.6625;
    double r = 0.0, g = 0.0, b = 0.0;
    if(level >= 0 && level < 0.15)
    {
        r = (0.15 - level) / (0.15 + 0.075);
        g = 0.0;
        b = 1.0;
    }
    else if(level >= 0.15 && level < 0.275)
    {
        r = 0.0;
        g = (level - 0.15) / (0.275 - 0.15);
        b = 1.0;
    }
    else if(level >= 0.275 && level < 0.325)
    {
        r = 0.0;
        g = 1.0;
        b = (0.325 - level) / (0.325 - 0.275);
    }
    else if(level >= 0.325 && level < 0.5)
    {
        r = (level - 0.325) / (0.5 - 0.325);
        g = 1.0;
        b = 0.0;
    }
    else if(level >= 0.5 && level < 0.6625)
    {
        r = 1.0;
        g = (0.6625 - level) / (0.6625 - 0.5f);
        b = 0.0;
    }

    // Intensity correction.
    double cf = 1.0;
    if(level >= 0.0 && level < 0.1)
    {
        cf = level / 0.1;
    }
    cf *= 255.0;

    // Pack RGB values into a 32-bit uint.
    const uint32_t rr = (uint32_t) (r * cf + 0.5);
    const uint32_t gg = (uint32_t) (g * cf + 0.5);
    const uint32_t bb = (uint32_t) (b * cf + 0.5);
    return (rr << 16) + (gg << 8) + bb;
}

static uint32_t perceptual(double level)
{
    if(!__globalTableInit__)
    {
        createGradientTable();
        __globalTableInit__ = true;
    }

    const int index = qBound(0, int(level * GRADIENT_TABLE_SIZE), GRADIENT_TABLE_SIZE - 1);
    return __globalTableGolors__[index];
}

static uint32_t rainbow(double level)
{
    float R, G, B, I, H, S, key = 1.5;
    level *= 256;
    I = level;
    H = M_PI * 2 * level / 256;

    (level <= 127) ? S = key * level : S = key * (256 - level);

    R = I - S * cos(H) * 0.201424 + S * sin(H) * 0.612372;
    G = I - S * cos(H) * 0.201424 - S * sin(H) * 0.612372;
    B = I + S * cos(H) * 0.402848 + S * sin(H) * 0.0;

    if(R < 0) R = 0; else if(R >= 256) R = 255;
    if(G < 0) G = 0; else if(G >= 256) G = 255;
    if(B < 0) B = 0; else if(B >= 256) B = 255;

    const uint32_t rr = (uint32_t) (R);
    const uint32_t gg = (uint32_t) (G);
    const uint32_t bb = (uint32_t) (B);
    return (rr << 16) + (gg << 8) + bb;
}

// The default palette used by SoX and written by Rob Sykes.
static uint32_t sox(double level)
{
    double r = 0.0;
    if(level >= 0.13 && level < 0.73)
    {
        r = sin((level - 0.13) / 0.60 * M_PI / 2.0);
    }
    else if(level >= 0.73)
    {
        r = 1.0;
    }

    double g = 0.0;
    if(level >= 0.6 && level < 0.91)
    {
        g = sin((level - 0.6) / 0.31 * M_PI / 2.0);
    }
    else if(level >= 0.91)
    {
        g = 1.0;
    }

    double b = 0.0;
    if(level < 0.60)
    {
        b = 0.5 * sin(level / 0.6 * M_PI);
    }
    else if(level >= 0.78)
    {
        b = (level - 0.78) / 0.22;
    }

    // Pack RGB values into a 32-bit uint.
    const uint32_t rr = (uint32_t) (r * 255.0 + 0.5);
    const uint32_t gg = (uint32_t) (g * 255.0 + 0.5);
    const uint32_t bb = (uint32_t) (b * 255.0 + 0.5);
    return (rr << 16) + (gg << 8) + bb;
}

static uint32_t mono(double level)
{
    const uint32_t v = (uint32_t) (level * 255.0 + 0.5);
    return (v << 16) + (v << 8) + v;
}

namespace VisualPalette {
uint32_t renderPalette(Palette palette, double level)
{
    switch(palette)
    {
    case PALETTE_SPECTRUM: return spectrum(level);
    case PALETTE_PERCEPTUAL: return perceptual(level);
    case PALETTE_RAINBOW: return rainbow(level);
    case PALETTE_SOX: return sox(level);
    case PALETTE_MONO: return mono(level);
    default: return 0;
    }
}
}
