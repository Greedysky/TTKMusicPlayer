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

#ifndef AY_H_
#define AY_H_

#define AY_TEMP_BUFFER_SIZE 4096

enum
{
    AY_CHNL_A_FINE = 0, 
    AY_CHNL_A_COARSE, 
    AY_CHNL_B_FINE, 
    AY_CHNL_B_COARSE, 
    AY_CHNL_C_FINE, 
    AY_CHNL_C_COARSE, 
    AY_NOISE_PERIOD, 
    AY_MIXER, 
    AY_CHNL_A_VOL, 
    AY_CHNL_B_VOL, 
    AY_CHNL_C_VOL, 
    AY_ENV_FINE, 
    AY_ENV_COARSE, 
    AY_ENV_SHAPE, 
    AY_GPIO_A, 
    AY_GPIO_B
};

struct init_mix_levels
{
    float a_left;
    float a_right;
    float b_left;
    float b_right;
    float c_left;
    float c_right;
};

typedef enum _ay_mix_types {
  AY_ABC = 0,
  AY_ACB,
  AY_BAC,
  AY_BCA,
  AY_CAB,
  AY_CBA
} AYMixTypes;

class ay
{
public:
    ay();
    ~ay();
    void ayReset();
    void ayWrite(unsigned char reg, unsigned char val);
    unsigned char ayRead(unsigned char reg);
    unsigned long ayProcess(unsigned char *stream, unsigned long len);
    void chnlMute(unsigned long chnl, bool mute)
    {
        switch(chnl)
        {
            case 0:
                chnl_mute0 = !mute;
                break;
            case 1:
                chnl_mute1 = !mute;
                break;
            case 2:
                chnl_mute2 = !mute;
                break;
            default:
                break;
        }
    }
    ;
    bool chnlMuted(unsigned long chnl)
    {
        switch(chnl)
        {
            case 0:
                return chnl_mute0;
            case 1:
                return chnl_mute1;
            case 2:
                return chnl_mute2;
            default:
                return false;
        }
    }
    ;
    float GetVolume(unsigned long chnl)
    {
        switch(chnl)
        {
            case 0:
                return volume0;
            case 1:
                return volume1;
            case 2:
                return volume2;
            default:
                break;
        }
        return 0.0;
    }
    ;
    void SetVolume(unsigned long chnl, float new_volume)
    {
        new_volume = new_volume > 1 ? 1 : (new_volume < 0 ? 0 : new_volume);
        switch(chnl)
        {
            case 0:
                volume0 = new_volume;
                break;
            case 1:
                volume1 = new_volume;
                break;
            case 2:
                volume2 = new_volume;
                break;
            default:
                break;
        }

    }
    ;
    const unsigned char *GetRegs()
    {
        return regs;
    }
    ;

    void SetMixType(AYMixTypes mixType);
    AYMixTypes GetMixType(void);

    void SetParameters(AYSongInfo *_songinfo = 0);
    void ayBeeper(bool on);
	unsigned long chip_nr;
private:
    static const float init_levels_ay[32];
    static const float init_levels_ym[32];
    float levels_ay[32];
    float levels_ym[32];
    float *levels;
    unsigned char regs[16];
    long noise_period;
    long chnl_period0, chnl_period1, chnl_period2;
    long tone_period_init0, tone_period_init1, tone_period_init2;
    long noise_period_init;
    long chnl_trigger0, chnl_trigger1, chnl_trigger2;
    unsigned long noise_trigger;
    unsigned long noise_reg;
    long env_period_init;
    long env_period;
    long env_type;
    long env_type_old;
    long env_vol;
    unsigned char env_step;
    bool chnl_mute0, chnl_mute1, chnl_mute2;
    unsigned long ay_tacts;
    double ay_tacts_f;
    unsigned long ay_tacts_counter;
    float volume_divider;
    void setEnvelope();
    void updateEnvelope();
    float volume0, volume1, volume2;
    AYSongInfo *songinfo;
    unsigned long int_counter;
    unsigned long int_limit;
    long z80_per_sample;
    long int_per_z80;
    long z80_per_sample_counter;
    long int_per_z80_counter;
    unsigned long frame_size;
    void ayStep(float &s0, float &s1, float &s2);
    static const init_mix_levels mix_levels[];
    AYMixTypes mix_levels_nr;
    float a_left, a_right, b_left, b_right, c_left, c_right;
    Filter3 flt;
    unsigned long flt_state;
    unsigned long flt_state_limit;
    unsigned long src_remaining;
    float ay_temp_buffer_in[AY_TEMP_BUFFER_SIZE];
    float ay_temp_buffer_out[AY_TEMP_BUFFER_SIZE];
    
    //beeper stuff
    float beeper_volume;
    bool beeper_oldval;
};

#endif /*AY_H_*/

