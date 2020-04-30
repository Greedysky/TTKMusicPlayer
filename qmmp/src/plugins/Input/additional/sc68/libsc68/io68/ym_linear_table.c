/*
 * @file    ym_linear_table.c
 * @brief   YM-2149 emulator - Linear Volume Table
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* Table of 5 bit D/A output level for 1 channel.
 *
 * Correct formula is: 2^-((31-vol)/4.0). Is is almost the same as
 * -1.5dB by step.
 */
static const u16 ymout1c5bit[32] =

#if 0
/* -1.5dB by step */
{
  310,   369,   438,   521,   619,   735,   874,   1039,
  1234,  1467,  1744,  2072,  2463,  2927,  3479,  4135,
  4914,  5841,  6942,  8250,  9806,  11654, 13851, 16462,
  19565, 23253, 27636, 32845, 39037, 46395, 55141, 65535
}
#elif 0
/* -1.5dB by step, rescale in range [0..65535] */
{
  0,     59,    128,   212,   310,   427,   566,   732,
  928,   1162,  1440,  1770,  2163,  2629,  3184,  3843,
  4625,  5557,  6663,  7977,  9541,  11398, 13605, 16229,
  19346, 23052, 27456, 32690, 38911, 46304, 55092, 65535
}
#elif 0
/* divide by 2 every 4 steps. */
{
  304,   362,   431,   512,   609,   724,   861,   1024,
  1218,  1448,  1722,  2048,  2435,  2896,  3444,  4096,
  4871,  5793,  6889,  8192,  9742,  11585, 13777, 16384,
  19484, 23170, 27554, 32768, 38967, 46340, 55108, 65535
}
#else
/* divide by 2 every 4 steps, rescale in range [0..65535] */
{
  0,     58,    127,   209,   306,   422,   559,   723,
  918,   1149,  1424,  1752,  2141,  2604,  3154,  3809,
  4588,  5514,  6615,  7924,  9481,  11333, 13535, 16154,
  19269, 22972, 27377, 32615, 38843, 46251, 55059, 65535
}
#endif
;


/* Create a linear 3 channels 5 bit per channels DAC table.
 */
void ym_create_5bit_linear_table(s16 * out, unsigned int level)
{
  int i;
  const unsigned int min = ymout1c5bit[00];
  const unsigned int max = ymout1c5bit[31];
  const unsigned int div = max-min ? max-min : 1;
  const int center = ( level + 1 ) >> 1;
  for (i=0; i<32*32*32; ++i) {
    int tmp =
      (  (unsigned int) ymout1c5bit[0x1F & (i>>10)] +
         (unsigned int) ymout1c5bit[0x1F & (i>> 5)] +
         (unsigned int) ymout1c5bit[0x1F & (i>> 0)] -
         3u * min ) / 3u * level / div;
    out[i] = tmp - center;
  }
  TRACE68(ym_cat,
          "ym-2149: volume model -- *linear* -- [%d..%d]\n",
          out[0],out[0x7FFF]);
}
