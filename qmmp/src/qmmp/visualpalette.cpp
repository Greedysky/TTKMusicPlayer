#include "visualpalette.h"
#include <qmath.h>

#include <QColor>
#include <QVector>

#define GRADIENT_TABLE_SIZE 256

static bool globalTableInit = false;
static uint32_t globalTableGolors[GRADIENT_TABLE_SIZE];
static QVector<QColor> globalGolors = { QColor(0, 0, 0),
                                        QColor(0, 32, 100),
                                        QColor(0, 120, 160),
                                        QColor(128, 255, 120),
                                        QColor(255, 255, 0),
                                        QColor(255, 128, 0),
                                        QColor(255, 0, 0)
                                      };

void createGradientTable()
{
    int numbers = 6;
    for(int i = 0; i < GRADIENT_TABLE_SIZE; i++)
    {
        double position = (double)i/GRADIENT_TABLE_SIZE;
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

        globalTableGolors[i] = 0xFF0000;
        if(n < numbers)
        {
            globalTableGolors[i] = ((uint32_t)((globalGolors[n].red()) + f * ((globalGolors[n+1].red()) - (globalGolors[n].red()))) & 0xFF) << 16 |
                ((uint32_t)((globalGolors[n].green()) + f * ((globalGolors[n+1].green()) - (globalGolors[n].green()))) & 0xFF) << 8 |
                ((uint32_t)((globalGolors[n].blue()) + f * ((globalGolors[n+1].blue()) - (globalGolors[n].blue()))) & 0xFF) << 0;
        }
        else if(n == numbers)
        {
            globalTableGolors[i] = ((uint32_t)(globalGolors[n].red()) & 0xFF) << 16 |
                ((uint32_t)(globalGolors[n].green()) & 0xFF) << 8 |
                ((uint32_t)(globalGolors[n].blue()) & 0xFF) << 0;
        }
        else
        {
            globalTableGolors[i] = 0xFFFFFF;
        }
    }
}

// Modified version of Dan Bruton's algorithm:
// http://www.physics.sfasu.edu/astro/color/spectra.html
uint32_t spectrum(double level)
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
    uint32_t rr = (uint32_t) (r * cf + 0.5);
    uint32_t gg = (uint32_t) (g * cf + 0.5);
    uint32_t bb = (uint32_t) (b * cf + 0.5);
    return (rr << 16) + (gg << 8) + bb;
}

uint32_t spectrogram(double level)
{
    if(!globalTableInit)
    {
        createGradientTable();
        globalTableInit = true;
    }
    
    const int index = qBound(0, int(level * GRADIENT_TABLE_SIZE), GRADIENT_TABLE_SIZE - 1);
    return globalTableGolors[index];
}

// The default palette used by SoX and written by Rob Sykes.
uint32_t sox(double level)
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
    uint32_t rr = (uint32_t) (r * 255.0 + 0.5);
    uint32_t gg = (uint32_t) (g * 255.0 + 0.5);
    uint32_t bb = (uint32_t) (b * 255.0 + 0.5);
    return (rr << 16) + (gg << 8) + bb;
}

uint32_t mono(double level)
{
    uint32_t v = (uint32_t) (level * 255.0 + 0.5);
    return (v << 16) + (v << 8) + v;
}

namespace VisualPalette {
uint32_t renderPalette(Palette palette, double level)
{
    switch(palette)
    {
    case PALETTE_SPECTRUM:
        return spectrum(level);
    case PALETTE_SPECTROGRAM:
        return spectrogram(level);
    case PALETTE_SOX:
        return sox(level);
    case PALETTE_MONO:
        return mono(level);
    default:
        return 0;
    }
}
}
