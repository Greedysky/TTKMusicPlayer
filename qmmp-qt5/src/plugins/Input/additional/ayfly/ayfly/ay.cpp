/***************************************************************************
 *   Copyright (C) 2008 by Deryabin Andrew   				               *
 *   andrew@it-optima.ru                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ayfly.h"

#define TACTS_MULT (unsigned long)800
#define VOL_BEEPER (15000)

const float ay::init_levels_ay[] =
{ 0, 836, 1212, 1773, 2619, 3875, 5397, 8823, 10392, 16706, 23339, 29292, 36969, 46421, 55195, 65535 };

const float ay::init_levels_ym[] =
{ 0, 0, 0xF8, 0x1C2, 0x29E, 0x33A, 0x3F2, 0x4D7, 0x610, 0x77F, 0x90A, 0xA42, 0xC3B, 0xEC2, 0x1137, 0x13A7, 0x1750, 0x1BF9, 0x20DF, 0x2596, 0x2C9D, 0x3579, 0x3E55, 0x4768, 0x54FF, 0x6624, 0x773B, 0x883F, 0xA1DA, 0xC0FC, 0xE094, 0xFFFF };
#define TONE_ENABLE(ch) ((regs [AY_MIXER] >> (ch)) & 1)
#define NOISE_ENABLE(ch) ((regs [AY_MIXER] >> (3 + (ch))) & 1)
#define TONE_PERIOD(ch) (((((regs [((ch) << 1) + 1]) & 0xf) << 8)) | (regs [(ch) << 1]))
#define NOISE_PERIOD (regs [AY_NOISE_PERIOD] & 0x1f)
#define CHNL_VOLUME(ch) (regs [AY_CHNL_A_VOL + (ch)] & 0xf)
#define CHNL_ENVELOPE(ch) (regs [AY_CHNL_A_VOL + (ch)] & 0x10)
#define ENVELOPE_PERIOD (((regs [AY_ENV_COARSE]) << 8) | regs [AY_ENV_FINE])

const init_mix_levels ay::mix_levels[] =
{
{ 1.0, 0.33, 0.67, 0.67, 0.33, 1.0 }, //AY_ABC
{ 1.0, 0.33, 0.33, 1.0, 0.67, 0.67 }, //AY_ACB
{ 0.67, 0.67, 1.0, 0.33, 0.33, 1.0 }, //AY_BAC
{ 0.33, 1.0, 1.0, 0.33, 0.67, 0.67 }, //AY_BCA
{ 0.67, 0.67, 0.33, 1.0, 1.0, 0.33 }, //AY_CAB
{ 0.33, 1.0, 0.67, 0.67, 1.0, 0.33 }  //AY_CBA
};

ay::ay()
{
    for(unsigned long i = 0; i < sizeof_array(ay::levels_ay); i++)
    {
        ay::levels_ay[i] = (ay::init_levels_ay[i / 2]) / (float)6;
        ay::levels_ym[i] = (ay::init_levels_ym[i]) / (float)6;
    }
    songinfo = 0;
	chip_nr = 0;
    ayReset();
}

ay::~ay()
{
}

void ay::SetMixType(AYMixTypes mixType)
{
    if (mixType >= 0 && mixType < sizeof(mix_levels) / sizeof(mix_levels[0]))
    {
        a_left = mix_levels[mixType].a_left;
        a_right = mix_levels[mixType].a_right;
        b_left = mix_levels[mixType].b_left;
        b_right = mix_levels[mixType].b_right;
        c_left = mix_levels[mixType].c_left;
        c_right = mix_levels[mixType].c_right;

        mix_levels_nr = mixType;
    }
}

AYMixTypes ay::GetMixType(void)
{
  return mix_levels_nr;
}

void ay::SetParameters(AYSongInfo *_songinfo)
{
    if(_songinfo == 0 && songinfo == 0)
        return;
    if((_songinfo != songinfo) && (_songinfo != 0))
        songinfo = _songinfo;
    if(songinfo->sr == 0 || songinfo->int_freq == 0)
        return;
    ay_tacts_f = ((double)songinfo->ay_freq * songinfo->ay_oversample) / (double)songinfo->sr / (double)8;
    flt_state_limit = ((double)songinfo->ay_freq * TACTS_MULT * songinfo->ay_oversample) / (double)songinfo->sr / (double)8;
    ay_tacts = ay_tacts_f;
    if((ay_tacts_f - ay_tacts) >= 0.5)
        ay_tacts++;
    levels = songinfo->chip_type == 0 ? ay::levels_ay : ay::levels_ym;
    float int_limit_f = ((float)songinfo->sr * TACTS_MULT) / (float)songinfo->int_freq * ay_tacts_f;
    int_limit = int_limit_f;
    if(int_limit_f - int_limit >= 0.5)
        int_limit++;
    frame_size = songinfo->sr / songinfo->int_freq;
    if(songinfo->is_z80)
    {
        float z80_per_sample_f = ((float)songinfo->z80_freq * TACTS_MULT) / songinfo->sr / ay_tacts_f;
        z80_per_sample = z80_per_sample_f;
        if(z80_per_sample_f - z80_per_sample)
            z80_per_sample++;
        float int_per_z80_f = ((float)songinfo->z80_freq * TACTS_MULT) / (float)songinfo->int_freq;
        int_per_z80 = int_per_z80_f;
        if((int_per_z80_f - int_per_z80) >= 0.5)
            int_per_z80++;

    }
    flt_state = flt_state_limit + TACTS_MULT;
    src_remaining = 0;
    FilterOpts fopts;
    fopts.Fs = songinfo->ay_freq * songinfo->ay_oversample / 8;
    fopts.f0 = (float)songinfo->sr / (float)4;
    fopts.Q = 1;
    fopts.type = LPF;
    flt.Init(&fopts);
    SetMixType(songinfo->mix_levels_nr);
}

void ay::ayReset()
{
    //init regs with defaults
    int_limit = 0;
    int_counter = 0;
    z80_per_sample_counter = 0;
    int_per_z80_counter = 0;
    memset(regs, 0, sizeof(regs));
    regs[AY_GPIO_A] = regs[AY_GPIO_B] = 0xff;
    chnl_period0 = chnl_period1 = chnl_period2 = 0;
    tone_period_init0 = tone_period_init1 = tone_period_init2 = 0;
    chnl_mute0 = chnl_mute1 = chnl_mute2 = false;
    env_type = 0;
    env_vol = 0;
    chnl_trigger0 = chnl_trigger1 = chnl_trigger2 = 0;
    noise_reg = 0x1;
    noise_trigger = 1;
    noise_period = 1;

    volume0 = volume1 = volume2 = 1;
    env_type_old = -1;
    env_step = 0;
    ay_tacts_counter = 0;

    beeper_volume = 0;
    beeper_oldval = false;

    SetParameters(0);
    setEnvelope();

}

void ay::ayWrite(unsigned char reg, unsigned char val)
{
    regs[reg & 0xf] = val;
    switch(reg)
    {
        case AY_CHNL_A_COARSE:
        case AY_CHNL_A_FINE:
            tone_period_init0 = TONE_PERIOD(0) * songinfo->ay_oversample;
            break;
        case AY_CHNL_B_COARSE:
        case AY_CHNL_B_FINE:
            tone_period_init1 = TONE_PERIOD(1) * songinfo->ay_oversample;
            break;
        case AY_CHNL_C_COARSE:
        case AY_CHNL_C_FINE:
            tone_period_init2 = TONE_PERIOD(2) * songinfo->ay_oversample;
            break;
        case AY_NOISE_PERIOD:
            noise_period_init = NOISE_PERIOD * 2 * songinfo->ay_oversample;
            break;
        case AY_MIXER:
            break;
        case AY_CHNL_A_VOL:
        case AY_CHNL_B_VOL:
        case AY_CHNL_C_VOL:
            break;
        case AY_ENV_SHAPE:
            setEnvelope();
            break;
        case AY_ENV_FINE:
        case AY_ENV_COARSE:
            env_period_init = ENVELOPE_PERIOD * songinfo->ay_oversample;
            break;
        default:
            break;
    }
	if(songinfo->aywrite_callback)
		songinfo->aywrite_callback(songinfo, chip_nr, reg, val);
}

unsigned char ay::ayRead(unsigned char reg)
{
    reg &= 0xf;
    return regs[reg];
}

void ay::setEnvelope()
{
    env_type = regs[AY_ENV_SHAPE];
    switch(env_type)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 9:
            env_step = 0;
            env_vol = 31;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 15:
            env_step = 1;
            env_vol = 0;
            break;
        case 8:
            env_step = 2;
            env_vol = 31;
            break;
        case 10:
            env_step = 3;
            env_vol = 31;
            break;
        case 11:
            env_step = 4;
            env_vol = 31;
            break;
        case 12:
            env_step = 5;
            env_vol = 0;
            break;
        case 13:
            env_step = 6;
            env_vol = 0;
            break;
        case 14:
            env_step = 7;
            env_vol = 0;
            break;
        default:
            env_step = 8;
            env_vol = 0;
            break;
    }

}

inline void ay::updateEnvelope()
{
    env_period++;
    if(env_period >= env_period_init)
    {
        env_period = 0;
        switch(env_step)
        {
            case 0:
                if(--env_vol == 0)
                    env_step = 8;
                break;
            case 1:
                if(++env_vol == 32)
                {
                    env_vol = 0;
                    env_step = 8;
                }
                break;
            case 2:
                if(--env_vol == -1)
                    env_vol = 31;
                break;
            case 3:
                if(--env_vol == -1)
                {
                    env_vol = 0;
                    env_step = 7;
                }
                break;
            case 4:
                if(--env_vol == -1)
                {
                    env_vol = 31;
                    env_step = 8;
                }
                break;
            case 5:
                if(++env_vol == 32)
                    env_vol = 0;
                break;
            case 6:
                if(++env_vol == 31)
                    env_step = 8;
                break;
            case 7:
                if(++env_vol == 32)
                {
                    env_vol = 31;
                    env_step = 3;
                }
                break;
            default:
                break;

        }
    }
}

inline void ay::ayStep(float &s0, float &s1, float &s2)
{
    s0 = s1 = s2 = 0;
    if(++chnl_period0 >= tone_period_init0)
    {
        chnl_period0 -= tone_period_init0;
        chnl_trigger0 ^= 1;
    }
    if(++chnl_period1 >= tone_period_init1)
    {
        chnl_period1 -= tone_period_init1;
        chnl_trigger1 ^= 1;
    }
    if(++chnl_period2 >= tone_period_init2)
    {
        chnl_period2 -= tone_period_init2;
        chnl_trigger2 ^= 1;
    }

    if(++noise_period >= noise_period_init)
    {
        noise_period = 0;
        noise_reg = (noise_reg >> 1) ^ ((noise_reg & 1) ? 0x14000 : 0);
        noise_trigger = noise_reg & 1;
    }
    updateEnvelope();

    if((chnl_trigger0 | TONE_ENABLE(0)) & (noise_trigger | NOISE_ENABLE(0)) & !chnl_mute0)
        s0 = (CHNL_ENVELOPE(0) ? ay::levels[env_vol] : ay::levels[CHNL_VOLUME(0) * 2]) * volume0;
    if((chnl_trigger1 | TONE_ENABLE(1)) & (noise_trigger | NOISE_ENABLE(1)) & !chnl_mute1)
        s1 = (CHNL_ENVELOPE(1) ? ay::levels[env_vol] : ay::levels[CHNL_VOLUME(1) * 2]) * volume1;
    if((chnl_trigger2 | TONE_ENABLE(2)) & (noise_trigger | NOISE_ENABLE(2)) & !chnl_mute2)
        s2 = (CHNL_ENVELOPE(2) ? ay::levels[env_vol] : ay::levels[CHNL_VOLUME(2) * 2]) * volume2;
}

unsigned long ay::ayProcess(unsigned char *stream, unsigned long len)
{
    unsigned long to_process = (len >> 1);
    float s0, s1, s2;

    unsigned long i = 0;
    short *stream16 = (short *)stream;
    while(i < to_process)
    {
        s0 = s1 = s2 = 0;
        if(!songinfo->empty_song)
        {
            if(songinfo->is_z80)
            {
                while(z80_per_sample_counter < z80_per_sample)
                {
                    int tstates = z80ex_step(songinfo->z80ctx) * TACTS_MULT;
                    z80_per_sample_counter += tstates;
                    int_per_z80_counter += tstates;
                    if(int_per_z80_counter > int_per_z80)
                    {
                        tstates = z80ex_int(songinfo->z80ctx) * TACTS_MULT;
                        z80_per_sample_counter += tstates;
                        int_per_z80_counter = tstates;
                        if(++songinfo->timeElapsed >= songinfo->Length)
                        {
                            songinfo->timeElapsed = songinfo->Loop;
                            if(songinfo->e_callback)
                                songinfo->stopping = songinfo->e_callback(songinfo->e_callback_arg);
                        }
                    }

                }
                z80_per_sample_counter -= z80_per_sample;
            }
            else
            {
                int_counter += TACTS_MULT;
                if(int_counter > int_limit)
                {
                    int_counter -= int_limit;
                    ay_softexec(songinfo);
                }
            }
        }
        else
        {
            if(songinfo->empty_callback)
            {
                int_counter += TACTS_MULT;
                if(int_counter > int_limit)
                {
                    int_counter -= int_limit;
                    songinfo->empty_callback(songinfo);
                }

            }
        }

        if(++chnl_period0 >= tone_period_init0)
        {
            chnl_period0 -= tone_period_init0;
            chnl_trigger0 ^= 1;
        }
        if(++chnl_period1 >= tone_period_init1)
        {
            chnl_period1 -= tone_period_init1;
            chnl_trigger1 ^= 1;
        }
        if(++chnl_period2 >= tone_period_init2)
        {
            chnl_period2 -= tone_period_init2;
            chnl_trigger2 ^= 1;
        }

        if(++noise_period >= noise_period_init)
        {
            noise_period = 0;
            noise_reg = (noise_reg >> 1) ^ ((noise_reg & 1) ? 0x14000 : 0);
            noise_trigger = noise_reg & 1;
        }
        updateEnvelope();

        if((chnl_trigger0 | TONE_ENABLE(0)) & (noise_trigger | NOISE_ENABLE(0)) & !chnl_mute0)
            s0 = (CHNL_ENVELOPE(0) ? ay::levels[env_vol] : ay::levels[CHNL_VOLUME(0) * 2]) * volume0;
        if((chnl_trigger1 | TONE_ENABLE(1)) & (noise_trigger | NOISE_ENABLE(1)) & !chnl_mute1)
            s1 = (CHNL_ENVELOPE(1) ? ay::levels[env_vol] : ay::levels[CHNL_VOLUME(1) * 2]) * volume1;
        if((chnl_trigger2 | TONE_ENABLE(2)) & (noise_trigger | NOISE_ENABLE(2)) & !chnl_mute2)
            s2 = (CHNL_ENVELOPE(2) ? ay::levels[env_vol] : ay::levels[CHNL_VOLUME(2) * 2]) * volume2;

        if(songinfo->is_ts)
        {
            float s3, s4, s5;
            songinfo->ay8910[1].ayStep(s3, s4, s5);
            s0 = (s0 + s3) / 2;
            s1 = (s1 + s4) / 2;
            s2 = (s2 + s5) / 2;
        }

        if(songinfo->stopping)
            break;

        float left = s0 * a_left + s1 * b_left + s2 * c_left + beeper_volume;
        float right = s0 * a_right + s1 * b_right + s2 * c_right + beeper_volume;

        flt.Process2(left, right);
        if(flt_state > flt_state_limit)
        {
            flt_state -= flt_state_limit;
            stream16[i] = left;
            stream16[i + 1] = right;
            i += 2;
        }
        flt_state += TACTS_MULT;

    }
    return (i << 1);
}

void ay::ayBeeper(bool on)
{
    if(beeper_oldval == on)
        return;
    beeper_volume = on ? -VOL_BEEPER : 0;
    beeper_oldval = on;
}

