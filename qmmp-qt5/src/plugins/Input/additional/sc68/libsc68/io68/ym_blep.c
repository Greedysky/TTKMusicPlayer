/*
 * @file    ym_blep.c
 * @brief   YM-2149 emulator - YM-2149 Band-Limited stEP synthesis engine
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 200?-2015 Antti Lankila
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ymemul.h"
#include "emu68/assert68.h"
#include <sc68/file68_msg.h>
#include <string.h>

extern int ym_cat;                      /* defined in ymemul.c */
extern const u16 * ym_envelops[16];     /* defined in ym_envelop.c */

enum {
  TONE_HI_MASK = 0x0f,
  NOISE_MASK   = 0x1f,
  CTL_ENV_MASK = 0x0f,
  BLEP_SIZE    = 1280,
  MAX_MIXBUF   = 2048,
};

const static s32 sine_integral[BLEP_SIZE] = {
  65536, 65536, 65536, 65536, 65536, 65536, 65535, 65536, 65536, 65535,
  65536, 65535, 65536, 65535, 65535, 65534, 65535, 65534, 65534, 65533,
  65533, 65532, 65531, 65530, 65530, 65528, 65527, 65526, 65524, 65522,
  65520, 65517, 65515, 65512, 65508, 65504, 65500, 65495, 65490, 65483,
  65478, 65469, 65462, 65453, 65443, 65432, 65420, 65408, 65393, 65378,
  65361, 65343, 65323, 65301, 65278, 65252, 65226, 65195, 65164, 65129,
  65092, 65052, 65010, 64963, 64915, 64862, 64805, 64746, 64681, 64613,
  64540, 64463, 64380, 64293, 64200, 64102, 63998, 63887, 63770, 63648,
  63518, 63380, 63236, 63084, 62924, 62756, 62580, 62394, 62199, 61996,
  61783, 61560, 61327, 61083, 60829, 60563, 60287, 60000, 59699, 59388,
  59064, 58727, 58378, 58016, 57641, 57252, 56850, 56434, 56004, 55560,
  55101, 54629, 54141, 53640, 53123, 52591, 52046, 51485, 50908, 50318,
  49713, 49092, 48456, 47807, 47143, 46464, 45771, 45064, 44342, 43609,
  42860, 42099, 41325, 40539, 39740, 38930, 38109, 37276, 36433, 35579,
  34718, 33846, 32966, 32078, 31183, 30282, 29374, 28461, 27544, 26622,
  25697, 24769, 23841, 22911, 21980, 21050, 20122, 19196, 18272, 17354,
  16438, 15529, 14626, 13731, 12843, 11964, 11095, 10236, 9390, 8554,
  7733, 6924, 6131, 5353, 4590, 3846, 3118, 2409, 1718, 1048, 397, -232,
  -841, -1426, -1991, -2532, -3050, -3543, -4013, -4459, -4879, -5275,
  -5646, -5990, -6310, -6605, -6873, -7115, -7333, -7525, -7692, -7832,
  -7950, -8041, -8108, -8152, -8171, -8168, -8142, -8093, -8023, -7932,
  -7820, -7689, -7538, -7369, -7182, -6979, -6759, -6525, -6275, -6013,
  -5737, -5451, -5154, -4846, -4530, -4206, -3876, -3539, -3198, -2853,
  -2504, -2155, -1803, -1453, -1103, -755, -410, -69, 268, 598, 924,
  1241, 1550, 1851, 2142, 2423, 2693, 2952, 3198, 3432, 3653, 3860,
  4054, 4232, 4396, 4545, 4678, 4797, 4899, 4986, 5057, 5112, 5150,
  5174, 5182, 5174, 5150, 5112, 5059, 4992, 4910, 4814, 4706, 4584,
  4450, 4305, 4148, 3981, 3803, 3617, 3421, 3218, 3007, 2790, 2566,
  2339, 2106, 1870, 1631, 1390, 1147, 905, 662, 421, 180, -57, -291,
  -522, -747, -969, -1183, -1392, -1593, -1788, -1973, -2150, -2319,
  -2477, -2625, -2764, -2892, -3008, -3114, -3208, -3291, -3362, -3420,
  -3468, -3503, -3526, -3538, -3537, -3525, -3501, -3467, -3420, -3364,
  -3297, -3219, -3132, -3035, -2929, -2815, -2693, -2562, -2425, -2282,
  -2132, -1976, -1817, -1652, -1483, -1313, -1138, -963, -786, -608,
  -430, -253, -76, 98, 270, 440, 605, 768, 926, 1078, 1226, 1368, 1503,
  1632, 1753, 1868, 1975, 2073, 2164, 2245, 2319, 2384, 2439, 2486,
  2523, 2551, 2571, 2580, 2581, 2573, 2556, 2530, 2496, 2453, 2402,
  2344, 2277, 2204, 2123, 2037, 1943, 1844, 1740, 1630, 1516, 1398,
  1276, 1151, 1024, 893, 762, 629, 495, 361, 227, 94, -38, -169, -298,
  -423, -547, -667, -783, -895, -1003, -1106, -1204, -1297, -1383,
  -1465, -1539, -1608, -1670, -1725, -1774, -1815, -1850, -1878, -1898,
  -1911, -1918, -1917, -1910, -1895, -1875, -1846, -1813, -1772, -1726,
  -1674, -1617, -1553, -1487, -1413, -1338, -1256, -1172, -1084, -994,
  -900, -804, -707, -608, -508, -407, -306, -205, -103, -4, 95, 193,
  289, 382, 473, 562, 647, 729, 808, 881, 953, 1018, 1080, 1137, 1189,
  1236, 1277, 1315, 1346, 1372, 1393, 1408, 1418, 1423, 1422, 1416,
  1404, 1388, 1367, 1341, 1309, 1275, 1235, 1190, 1143, 1092, 1036, 978,
  917, 854, 786, 719, 648, 575, 503, 428, 353, 278, 202, 127, 51, -23,
  -96, -168, -239, -308, -375, -440, -502, -561, -618, -672, -723, -770,
  -814, -853, -891, -922, -952, -976, -997, -1014, -1027, -1035, -1039,
  -1041, -1037, -1029, -1019, -1004, -985, -964, -938, -909, -878, -843,
  -806, -766, -723, -678, -632, -584, -533, -482, -429, -375, -321,
  -266, -211, -156, -100, -45, 9, 62, 115, 167, 217, 266, 312, 359, 401,
  442, 482, 518, 552, 583, 612, 639, 661, 682, 700, 714, 726, 734, 740,
  743, 742, 740, 733, 725, 714, 700, 683, 665, 643, 620, 594, 567, 538,
  507, 474, 441, 405, 369, 332, 294, 256, 216, 178, 138, 98, 60, 20,
  -18, -55, -93, -129, -164, -197, -231, -262, -291, -320, -347, -371,
  -395, -415, -435, -452, -467, -480, -491, -500, -507, -512, -515,
  -515, -513, -510, -505, -497, -489, -477, -464, -451, -434, -417,
  -398, -379, -357, -335, -311, -287, -263, -237, -210, -184, -158,
  -130, -103, -76, -49, -22, 4, 31, 56, 80, 105, 128, 151, 172, 192,
  212, 230, 247, 263, 277, 291, 302, 312, 321, 329, 334, 339, 343, 344,
  344, 343, 341, 337, 332, 325, 318, 310, 299, 289, 277, 264, 251, 236,
  221, 206, 189, 172, 156, 138, 120, 102, 84, 66, 48, 30, 12, -5, -22,
  -39, -55, -71, -86, -101, -114, -128, -141, -152, -162, -173, -182,
  -190, -197, -204, -209, -213, -217, -220, -221, -221, -222, -220,
  -218, -215, -212, -206, -202, -195, -189, -182, -173, -165, -156,
  -146, -136, -126, -115, -104, -93, -82, -71, -58, -47, -36, -24, -13,
  -2, 9, 20, 31, 40, 51, 59, 69, 77, 85, 92, 99, 106, 111, 117, 121,
  125, 129, 131, 134, 135, 137, 137, 136, 136, 134, 133, 130, 128, 124,
  120, 117, 111, 107, 101, 95, 90, 83, 77, 71, 63, 57, 49, 43, 35, 29,
  21, 14, 8, 0, -6, -13, -19, -25, -30, -37, -42, -47, -51, -56, -60,
  -64, -67, -70, -73, -75, -77, -78, -79, -81, -81, -80, -81, -80, -79,
  -77, -76, -74, -72, -70, -67, -64, -61, -57, -54, -51, -46, -43, -38,
  -35, -30, -27, -22, -18, -13, -10, -5, -1, 2, 7, 10, 14, 17, 20, 24,
  27, 29, 32, 34, 37, 38, 41, 41, 43, 45, 45, 45, 47, 46, 46, 46, 46,
  45, 45, 43, 43, 41, 39, 38, 36, 35, 32, 31, 28, 26, 24, 22, 19, 17,
  14, 12, 10, 7, 5, 2, 1, -2, -5, -6, -8, -10, -12, -14, -16, -17, -18,
  -20, -20, -22, -23, -24, -24, -25, -25, -25, -26, -26, -25, -26, -25,
  -24, -25, -23, -23, -22, -22, -20, -19, -19, -17, -16, -14, -14, -12,
  -11, -10, -8, -7, -5, -5, -3, -1, -1, 1, 2, 3, 4, 6, 6, 7, 8, 9, 10,
  11, 11, 11, 12, 13, 13, 13, 13, 14, 13, 14, 14, 13, 13, 14, 12, 13,
  12, 11, 12, 10, 10, 10, 9, 8, 8, 7, 6, 5, 5, 4, 4, 3, 2, 1, 1, 0, -1,
  -1, -2, -2, -3, -4, -3, -5, -5, -5, -5, -6, -6, -6, -7, -6, -7, -7,
  -7, -7, -7, -6, -7, -7, -6, -6, -7, -5, -6, -6, -5, -4, -5, -4, -4,
  -4, -3, -3, -2, -2, -2, -1, -1, -1, 0, 0, 0, 0, 1, 1, 2, 1, 2, 2, 3,
  2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 4, 3, 3, 3, 2, 3, 3, 2, 3, 2,
  2, 2, 1, 2, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, -1, -1,
  -1, -1, -1, -2, -1, -1, -2, -1, -2, -1, -2, -1, -2, -1, -1, -2, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, 0, -1, 0, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1,
  0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0,
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

static void ym2149_new_output_level(ym_t * const ym)
{
  ym_blep_t *blep = &ym->emu.blep;

  u32 i;
  s16 output;

  u16 dacstate = 0;
  for (i = 0; i < 3; i ++) {
    u16 mask = blep->tonegen[i].tonemix | blep->tonegen[i].flip_flop;
    mask &= blep->tonegen[i].noisemix | blep->noise_output;
    dacstate |= mask &
      ((blep->env_output & blep->tonegen[i].envmask)
       | blep->tonegen[i].volmask);
  }

  assert( (dacstate & 0x7fff) == dacstate );
  output = (ym->ymout5[dacstate] + 1) >> 1;

  if (output != blep->global_output_level) {
    /* Add a new blep before the others */
    blep->blep_idx -= 1;
    blep->blep_idx &= MAX_BLEPS - 1;
    blep->blepstate[blep->blep_idx].stamp = blep->time;
    blep->blepstate[blep->blep_idx].level = blep->global_output_level - output;
    blep->global_output_level = output;
  }
}

static void ym2149_clock(ym_t * const ym, cycle68_t cycles)
{
  ym_blep_t *blep = &ym->emu.blep;
  const u16 * const waveform =
    ym_envelops[ym->reg.name.env_shape & CTL_ENV_MASK];

  while (cycles) {
    int iter = cycles;
    int i;
    int change = 0;

    for (i = 0; i < 3; i ++) {
      if (iter > blep->tonegen[i].count)
        iter = blep->tonegen[i].count;
    }

    if (iter > blep->noise_count)
      iter = blep->noise_count;

    if (iter > blep->env_count)
      iter = blep->env_count;

    cycles -= iter;
    blep->time += iter;

    /* Clock subsystems forward */
    for (i = 0; i < 3; i++) {
      blep->tonegen[i].count -= iter;
      assert(blep->tonegen[i].count >= 0);
      if (blep->tonegen[i].count == 0) {
        blep->tonegen[i].flip_flop = ~blep->tonegen[i].flip_flop;
        blep->tonegen[i].count = blep->tonegen[i].event;
        change = 1;
      }
    }

    blep->noise_count -= iter;
    assert(blep->noise_count >= 0);
    if (blep->noise_count == 0) {
      u16 new_noise;
      blep->noise_state =
        (blep->noise_state >> 1) |
        (((blep->noise_state ^ (blep->noise_state >> 2)) & 1) << 16);
      blep->noise_count = blep->noise_event;

      new_noise = blep->noise_state & 1 ? 0xffff : 0x0000;
      change = change || blep->noise_output != new_noise;
      blep->noise_output = new_noise;
    }

    blep->env_count -= iter;
    assert(blep->env_count >= 0);
    if (blep->env_count == 0) {
      u16 new_env = waveform[blep->env_state];
      if (++ blep->env_state == 96)
        blep->env_state = 32;
      blep->env_count = blep->env_event;

      change = change || new_env != blep->env_output;
      blep->env_output = new_env;
    }

    if (change)
      ym2149_new_output_level(ym);
  }
}

static s32 ym2149_output(ym_t * const ym, const u8 subsample)
{
  ym_blep_t *blep = &ym->emu.blep;
  u32 i = blep->blep_idx;
  s32 output = 0;

  /* Workaround bug #30: 
   * https://sourceforge.net/p/sc68/bugs/30/
   */
  blep->blepstate[(i-1)&(MAX_BLEPS-1)].stamp = blep->time - BLEP_SIZE;
  
  while (1) {
    u16 age = blep->time - blep->blepstate[i].stamp;
    if (age >= BLEP_SIZE-1)
      break;
    /* JOS says that we should have several subphases of SINC for
     * this, and we should then interpolate between them linearly.
     * What I got here is better than nothing, though. */
    output += ((sine_integral[age] * (256 - subsample)
                + sine_integral[age+1] * subsample
                + 128) >> 8) * blep->blepstate[i].level;
    i = (i + 1) & (MAX_BLEPS - 1);
  }
  /* Terminate the blep train by keeping the last stamp invalid. */
  blep->blepstate[i].stamp = blep->time - BLEP_SIZE;

  return ((output + (1 << 15)) >> 16) + blep->global_output_level;
}

static s32 highpass(ym_t * const ym, s32 output)
{
  ym_blep_t *blep = &ym->emu.blep;

  blep->hp = (blep->hp * 511 + (output << 6) + (1 << 8)) >> 9;
  output -= (blep->hp + (1 << 5)) >> 6;

  if (output > 32767)
    output = 32767;
  if (output < -32768)
    output = -32768;

  return output;
}

/* Run output synthesis for some clocks */
static int mix_to_buffer(ym_t * const ym, cycle68_t cycles, s32 *output)
{
  ym_blep_t *blep = &ym->emu.blep;

  u32 len = 0;
  while (cycles) {
    cycle68_t iter = cycles;
    u8 makesample = 0;
    if (iter > blep->cycles_to_next_sample >> 8) {
      iter = blep->cycles_to_next_sample >> 8;
      makesample = 1;
    }

    /* Simulate ym2149 for iter clocks */
    ym2149_clock(ym, iter);
    cycles -= iter;
    blep->cycles_to_next_sample -= iter << 8;

    /* Generate output.
     * To improve accuracy, we interpolate the sinc table. */
    if (makesample) {
      assert(blep->cycles_to_next_sample <= 0xff);
      output[len++] =
        highpass(ym, ym2149_output(ym, blep->cycles_to_next_sample));
      assert(len < MAX_MIXBUF);
      blep->cycles_to_next_sample += blep->cycles_per_sample;
    }
  }
  return len;
}

/* Mix for ymcycles cycles. */
static int run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
  ym_blep_t *blep = &ym->emu.blep;

  u32 len = 0, voice;
  s32 newevent;

  /* Walk  the static list of allocated events */
  cycle68_t currcycle = 0;
  ym_event_t *event;
  for (event = ym->event_buf; event < ym->event_ptr; event++) {
    assert( event->ymcycle <= ymcycles );

    /* Mix up to this cycle, update state */
    len += mix_to_buffer(ym, event->ymcycle - currcycle, output + len);
    ym->reg.index[event->reg] = event->val;

    /* Update various internal variables in response to writes.
     * unfortunately pointers don't work for this, so... */
    switch (event->reg) {
    case 0: /* per_x_lo, per_x_hi */
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      voice = event->reg >> 1;
      newevent = ym->reg.index[voice << 1]
        | ((ym->reg.index[(voice << 1) + 1] & TONE_HI_MASK) << 8);
      if (newevent == 0)
        newevent = 1;
      newevent <<= 3;

      /* The chip performs count >= event. If the condition is
       * true, event triggers immediately. However, I have inverted
       * the count to occur towards zero. Changes in event must
       * therefore affect the prevailing count. If new event time
       * is greater than current, the count must increase as the
       * current state will be delayed. */
      blep->tonegen[voice].count += newevent - blep->tonegen[voice].event;
      blep->tonegen[voice].event = newevent;
      /* I do not deal with negative counts in the hot path. */
      if (blep->tonegen[voice].count < 0)
        blep->tonegen[voice].count = 0;
      break;

    case 6: /* per_noise */
      newevent = ym->reg.name.per_noise & NOISE_MASK;
      if (newevent == 0)
        newevent = 1;
      newevent <<= 4;

      blep->noise_count += newevent - blep->noise_event;
      blep->noise_event = newevent;
      if (blep->noise_count < 0)
        blep->noise_count = 0;
      break;

    case 7: /* mixer */
      blep->tonegen[0].tonemix  = event->val &  1 ? 0xffff : 0;
      blep->tonegen[1].tonemix  = event->val &  2 ? 0xffff : 0;
      blep->tonegen[2].tonemix  = event->val &  4 ? 0xffff : 0;
      blep->tonegen[0].noisemix = event->val &  8 ? 0xffff : 0;
      blep->tonegen[1].noisemix = event->val & 16 ? 0xffff : 0;
      blep->tonegen[2].noisemix = event->val & 32 ? 0xffff : 0;
      break;

    case 8: /* volume */
    case 9:
    case 10:
      voice = event->reg - 8;
      blep->tonegen[voice].envmask = (event->val & 0x10)
        ? 0x1f << (voice*5) : 0;
      blep->tonegen[voice].volmask = (event->val & 0x10)
        ? 0 : (((event->val & 0xf) << 1) | 1) << (voice*5);
      break;

    case 11: /* per_env_lo, per_env_hi */
    case 12:
      newevent = ym->reg.name.per_env_lo | (ym->reg.name.per_env_hi << 8);
      if (newevent == 0)
        newevent = 1;
      newevent <<= 3;

      blep->env_count += newevent - blep->env_event;
      blep->env_event = newevent;
      if (blep->env_count < 0)
        blep->env_count = 0;
      break;

    case 13: /* env_shape */
      blep->env_state = 0;
      break;
    }

    ym2149_new_output_level(ym);

    currcycle = event->ymcycle;
  }

  /* Reset event list. */
  ym->event_ptr = ym->event_buf;

  /* Mix stuff outside writes */
  len += mix_to_buffer(ym, ymcycles - currcycle, output + len);

  return len;
}

static int reset(ym_t * const ym, const cycle68_t ymcycle)
{
  ym_blep_t *blep = &ym->emu.blep;

  u32 tmp = blep->cycles_per_sample;
  memset(blep, 0, sizeof(ym_blep_t));

  blep->cycles_per_sample = tmp;
  blep->noise_state = 1;
  blep->time = BLEP_SIZE;
  blep->tonegen[0].event = 8;
  blep->tonegen[1].event = 8;
  blep->tonegen[2].event = 8;
  blep->noise_event = 16;
  blep->env_event = 8;

  return 0;
}

/* Get required length of buffer at run(s32 *output) (number of frames). */
static int buffersize(const ym_t * const ym, const cycle68_t ymcycles)
{
  return MAX_MIXBUF;
}

static int sampling_rate(ym_t * const ym, const int hz)
{
  ym_blep_t *blep = &ym->emu.blep;
  blep->cycles_per_sample = (ym->clock << 8) / hz;
  return hz;
}

int ym_blep_setup(ym_t * const ym)
{
  ym->cb_cleanup       = 0;
  ym->cb_reset         = reset;
  ym->cb_run           = run;
  ym->cb_buffersize    = buffersize;
  ym->cb_sampling_rate = sampling_rate;
  return 0;
}

void ym_blep_add_options(void)
{
}
