#include "visualpalette.h"

#include <QColor>
#include <QVector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif
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
        double position = i * 1.0 / GRADIENT_TABLE_SIZE;
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
    cf *= 255;

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r * cf + 0.5);
    const uint32_t gc = (uint32_t)(g * cf + 0.5);
    const uint32_t bc = (uint32_t)(b * cf + 0.5);
    return (rc << 16) + (gc << 8) + bc;
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
    level *= 256;
    constexpr double key = 1.5;
    const double h = M_PI * 2 * level / 256;
    const double s = (level <= 127) ? (key * level) : (key * (256 - level));

    double r = level - s * std::cos(h) * 0.201424 + s * std::sin(h) * 0.612372;
    double g = level - s * std::cos(h) * 0.201424 - s * std::sin(h) * 0.612372;
    double b = level + s * std::cos(h) * 0.402848 + s * std::sin(h) * 0.0;

    if(r < 0) r = 0; else if(r >= 256) r = 255;
    if(g < 0) g = 0; else if(g >= 256) g = 255;
    if(b < 0) b = 0; else if(b >= 256) b = 255;

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r);
    const uint32_t gc = (uint32_t)(g);
    const uint32_t bc = (uint32_t)(b);
    return (rc << 16) + (gc << 8) + bc;
}

// The default palette used by SoX and written by Rob Sykes.
static uint32_t sox(double level)
{
    double r = 0.0;
    if(level >= 0.13 && level < 0.73)
    {
        r = std::sin((level - 0.13) / 0.60 * M_PI / 2.0);
    }
    else if(level >= 0.73)
    {
        r = 1.0;
    }

    double g = 0.0;
    if(level >= 0.6 && level < 0.91)
    {
        g = std::sin((level - 0.6) / 0.31 * M_PI / 2.0);
    }
    else if(level >= 0.91)
    {
        g = 1.0;
    }

    double b = 0.0;
    if(level < 0.60)
    {
        b = 0.5 * std::sin(level / 0.6 * M_PI);
    }
    else if(level >= 0.78)
    {
        b = (level - 0.78) / 0.22;
    }

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r * 255 + 0.5);
    const uint32_t gc = (uint32_t)(g * 255 + 0.5);
    const uint32_t bc = (uint32_t)(b * 255 + 0.5);
    return (rc << 16) + (gc << 8) + bc;
}

static uint32_t magma(double level)
{
    const double x = level;
    const double x2 = x * x;
    const double x3 = x * x2;
    const double x4 = x * x3;
    const double x5 = x * x4;
    const double x6 = x * x5;
    const double x7 = x * x6;
    const double x8 = x * x7;

    // found using https://arachnoid.com/polysolve/
    double r = -2.1104070317295411e-002 + 1.0825531148278227e+000 * x -7.2556742716785472e-002 * x2 + 6.1700693562312701e+000 * x3 -1.1408475082678258e+001 * x4 + 5.2341915705822935e+000 * x5;
    if(r < 0.0)
    {
        r = 0.0; // small correction
    }

    double g = (-9.6293819919380796e-003 + 8.1951407027674095e-001 * x -2.9094991522336970e+000 * x2 + 5.4475501043849874e+000 * x3 -2.3446957347481536e+000 * x4);
    if(g < 0.0)
    {
        g = 0.0; // small correction
    }

    double b = 3.4861713828180638e-002 - 5.4531128070732215e-001 * x + 4.9397985434515761e+001 * x2 -3.4537272622690250e+002 * x3 + 1.1644865375431577e+003 * x4 -2.2241373781645634e+003 * x5 +
               2.4245808412415154e+003 * x6 -1.3968425226952077e+003 * x7 + 3.2914755310075969e+002 * x8;

    // clip
    if(r > 1.0)
    {
        r = 1.0;
    }

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r * 255 + 0.5);
    const uint32_t gc = (uint32_t)(g * 255 + 0.5);
    const uint32_t bc = (uint32_t)(b * 255 + 0.5);
    return (rc << 16) + (gc << 8) + bc;
}

/* based on the code by Linas Vepstas January 16 1994 : void make_cmap (void) */
static uint32_t linas(double level)
{
    double r = 0.0, g = 0.0, b = 0.0;
    // gradient with 4 segments 0-0.25-0.5-0.75-1.0
    if(level < 0.25)
    {
        /* ramp from black to blue = (0.0; 0.25)) */
        r = 0;
        g = 0;
        b = 708 * level;
    }
    else if(level < 0.5)
    {
        /* ramp down from blue, up to green = (0.25; 0.5) */
        r = 0;
        g = -177 + 708 * level;
        b = 354 - 708 * level;
    }
    else if(level < 0.75)
    {
        /* ramp from green to yellow = (0.5; 0.75) */
        r = -420 + 840 * level;
        g = 219 - 84 * level;
        b = 0;
    }
    else
    {
        /* ramp from yellow to red (pink) = (0.75; 1.0) */
        r = 84 + 168 * level;
        g = 516 - 480 * level;
        b = -57 + 76 * level;
    }

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r);
    const uint32_t gc = (uint32_t)(g);
    const uint32_t bc = (uint32_t)(b);
    return (rc << 16) + (gc << 8) + bc;
}

/*
https://arxiv.org/abs/1108.5083
A colour scheme for the display of astronomical intensity images by D. A. Green
*/
static uint32_t cubeHelix(double level)
{
    /* gamma for gray scale and cubehelix palettes only */
    constexpr double gamma = 1.5;
    /* control parameters for the cubehelix palette scheme */
    //set palette cubehelix start 0.5 cycles -1.5 saturation 1
    //set palette gamma 1.5
    constexpr double cubehelix_start = 0.5;	/* offset (radians) from colorwheel 0 */
    constexpr double cubehelix_cycles = -1.5;	/* number of times round the colorwheel */
    constexpr double cubehelix_saturation = 1.0;	/* color saturation */
    double gray = level;

    /*
     Petr Mikulik, December 1998 -- June 1999
    * Copyright: open source as much as possible
    */
    // /* Map gray in [0,1] to color components according to colorMode */
    // function color_components_from_gray
    // from gnuplot/src/getcolor.c
    const double phi = 2. * M_PI * (cubehelix_start / 3. + gray * cubehelix_cycles);

    // gamma correction
    gray = std::pow(gray, 1 / gamma);

    const double a = cubehelix_saturation * gray * (1. - gray) / 2.;

    // compute
    const double r = gray + a * (-0.14861 * std::cos(phi) + 1.78277 * std::sin(phi));
    const double g = gray + a * (-0.29227 * std::cos(phi) - 0.90649 * std::sin(phi));
    const double b = gray + a * ( 1.97294 * std::cos(phi));

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r * 255 + 0.5);
    const uint32_t gc = (uint32_t)(g * 255 + 0.5);
    const uint32_t bc = (uint32_t)(b * 255 + 0.5);
    return (rc << 16) + (gc << 8) + bc;
}

/*
It is orange-blue gradient from the [Fractalizer program](http://www.fractalizer.de/en/).
*/
static uint32_t fractalizer(double level)
{
    const double p = level * 5; //relative posiion
    // 2 points defining line
    constexpr double y0 = 80.0 / 255;
    constexpr double x0 = 1.0;
    constexpr double y1 = 1.0;
    constexpr double x1 = 2.0;
    constexpr double y2 = 128.0 / 255;
    constexpr double x2 = 2.0;
    double r = 0.0, g = 0.0, b = 0.0;

    // gradient with 5 segments
    if(level < 0.2)
    {
        // 1/5 from black orange
        r = p;
        g = p * y0;
        b = 0.0;
    }
    else if(level < 0.4)
    {
        // 2/5 from orange to yellow
        r = 1.0;
        g = (y1 - y0) / (x1 - x0) * p + y0 - (y1 - y0) / (x1 - x0) * x0;
        b = y2 * (p - 1.0);
    }
    else if(level  < 0.6)
    {
        // 3/5 from yellow to white
        r = 1.0;
        g = 1.0;
        b = (1.0 - y2) * p + y2 - (1.0 - y2) * x2;
    }
    else if(level < 0.8)
    {
        // 4/5 from white to blue
        r = 4.0 - p;
        g = 4.0 - p;
        b = 1.0;
    }
    else
    {
        // 5/5 from blue to black
        r = 0.0;
        g = 0.0;
        b = 5.0 - p;
    }

    // Pack RGB values into a 32-bit uint.
    const uint32_t rc = (uint32_t)(r * 255 + 0.5);
    const uint32_t gc = (uint32_t)(g * 255 + 0.5);
    const uint32_t bc = (uint32_t)(b * 255 + 0.5);
    return (rc << 16) + (gc << 8) + bc;
}

static uint32_t mono(double level)
{
    const uint32_t v = (uint32_t)(level * 255 + 0.5);
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
    case PALETTE_MAGMA: return magma(level);
    case PALETTE_LINAS: return linas(level);
    case PALETTE_CUBEHELIX: return cubeHelix(level);
    case PALETTE_FRACTALIZER: return fractalizer(level);
    case PALETTE_MONO: return mono(level);
    default: return 0;
    }
}
}
