/* AY/YM emulator implementation. */

#include "ayemu.h"

#define debuglog stderr;

char *ayemu_err;

static const char VERSION[] = "libayemu 0.9";

const int MAGIC1 = 0xcdef;	/* for check ayemu_t structure inited */

enum {
/* Max amplitude value for stereo signal for avoiding for possible
    folowwing SSRC for clipping */
  AYEMU_MAX_AMP = 24575,
  AYEMU_DEFAULT_CHIP_FREQ = 1773400
};

/* sound chip volume envelops (will calculated by gen_env()) */
static int bEnvGenInit = 0;
static int Envelope [16][128];


/* AY volume table (c) by V_Soft and Lion 17 */
static int Lion17_AY_table [16] =
  { 0, 513, 828, 1239, 1923, 3238, 4926, 9110,
    10344, 17876, 24682, 30442, 38844, 47270, 56402, 65535};

/* YM volume table (c) by V_Soft and Lion 17 */
static int Lion17_YM_table [32] =
  { 0, 0, 190, 286, 375, 470, 560, 664,
    866, 1130, 1515, 1803, 2253, 2848, 3351, 3862,
    4844, 6058, 7290, 8559, 10474, 12878, 15297, 17787,
    21500, 26172, 30866, 35676, 42664, 50986, 58842, 65535};

/* AY volume table (c) by Hacker KAY */
static int KAY_AY_table [16] =
  { 0, 836, 1212, 1773, 2619, 3875, 5397, 8823,
    10392, 16706, 23339, 29292, 36969, 46421, 55195, 65535};

/* YM volume table (c) by Hacker KAY */
static int KAY_YM_table [32] =
  { 0, 0, 248, 450, 670, 826, 1010, 1239,
    1552, 1919, 2314, 2626, 3131, 3778, 4407, 5031,
    5968, 7161, 8415, 9622, 11421, 13689, 15957, 18280,
    21759, 26148, 30523, 34879, 41434, 49404, 57492, 65535};

/* default equlaizer (layout) settings for AY and YM, 7 stereo types */
static const int default_layout [2][7][6] = {
  {
    /* A_l, A_r,  B_l, B_r,  C_l, C_r */

    /* for AY */
    {100, 100, 100, 100, 100, 100},	// _MONO
    {100, 33, 70, 70, 33, 100},	   // _ABC
    {100, 33, 33, 100, 70, 70},	   // _ACB
    {70, 70, 100, 33, 33, 100},	   // _BAC
    {33, 100, 100, 33, 70, 70},	   // _BCA
    {70, 70, 33, 100, 100, 33},	   // _CAB
    {33, 100, 70, 70, 100, 33}},   // _CBA
  {
    /* for YM */
    {100, 100, 100, 100, 100, 100},	// _MONO
    {100, 5, 70, 70, 5, 100},	// _ABC
    {100, 5, 5, 100, 70, 70},	// _ACB
    {70, 70, 100, 5, 5, 100},	// _BAC
    {5, 100, 100, 5, 70, 70},	// _BCA
    {70, 70, 5, 100, 100, 5},	// _CAB
    {5, 100, 70, 70, 100, 5}}   // _CBA
};


static int check_magic(ayemu_ay_t *ay)
{
  if (ay->magic == MAGIC1)
    return 1;
  fprintf(stderr, "libayemu: passed pointer %p to uninitialized ayemu_ay_t structure\n", ay);
  return 0;
}


/* make chip hardware envelop tables.
    Will execute once before first use. */
static void gen_env()
{
  int env;
  int pos;
  int hold;
  int dir;
  int vol;

  for (env = 0; env < 16; env++) {
    hold = 0;
    dir = (env & 4)?  1 : -1;
    vol = (env & 4)? -1 : 32;
    for (pos = 0; pos < 128; pos++) {
      if (!hold) {
	vol += dir;
	if (vol < 0 || vol >= 32) {
	  if ( env & 8 ) {
	    if ( env & 2 ) dir = -dir;
	    vol = (dir > 0 )? 0:31;
	    if ( env & 1 ) {
	      hold = 1;
	      vol = ( dir > 0 )? 31:0;
	    }
	  } else {
	    vol = 0;
	    hold = 1;
	  }
	}
      }
      Envelope[env][pos] = vol;
    }
  }
  bEnvGenInit = 1;
}


/**
 * \retval ayemu_init none.
 *
*/
void ayemu_init(ayemu_ay_t *ay)
{
  ay->default_chip_flag = 1;
  ay->ChipFreq = AYEMU_DEFAULT_CHIP_FREQ;
  ay->default_stereo_flag = 1;
  ay->default_sound_format_flag = 1;
  ay->dirty = 1;
  ay->magic = MAGIC1;

  ayemu_reset(ay);
}

/** Reset AY/YM chip.
 *
 * \arg \c ay - pointer to ayemu_ay_t structure.
 * \return none.
 */
void ayemu_reset(ayemu_ay_t *ay)
{
  if (!check_magic(ay)) return;

  ay->cnt_a = ay->cnt_b = ay->cnt_c = ay->cnt_n = ay->cnt_e = 0;
  ay->bit_a = ay->bit_b = ay->bit_c = ay->bit_n = 0;
  ay->env_pos = ay->EnvNum = 0;
  ay->Cur_Seed = 0xffff;
}


static void set_table_ay (ayemu_ay_t *ay, int tbl[16])
{
  int n;
  for (n = 0; n < 32; n++)
    ay->table[n] = tbl[n/2];
  ay->type = AYEMU_AY;
}

static void set_table_ym (ayemu_ay_t *ay, int tbl[32])
{
  int n;
  for (n = 0; n < 32; n++)
    ay->table[n] = tbl[n];
  ay->type = AYEMU_YM;
}


/** Set chip type. */
int ayemu_set_chip_type(ayemu_ay_t *ay, ayemu_chip_t type, int *custom_table)
{
if (!check_magic(ay))
		return 0;

  if (!(type == AYEMU_AY_CUSTOM || type == AYEMU_YM_CUSTOM) && custom_table != NULL) {
    ayemu_err = "For non-custom chip type 'custom_table' param must be NULL";
    return 0;
  }

  switch(type) {
  case AYEMU_AY:
  case AYEMU_AY_LION17:
    set_table_ay(ay, Lion17_AY_table);
    break;
  case AYEMU_YM:
  case AYEMU_YM_LION17:
    set_table_ym(ay, Lion17_YM_table);
    break;
  case AYEMU_AY_KAY:
    set_table_ay(ay, KAY_AY_table);
    break;
  case AYEMU_YM_KAY:
    set_table_ym(ay, KAY_YM_table);
    break;
  case AYEMU_AY_CUSTOM:
    set_table_ay(ay, custom_table);
    break;
  case AYEMU_YM_CUSTOM:
    set_table_ym(ay, custom_table);
    break;
  default:
    ayemu_err = "Incorrect chip type";
    return 0;
  }

  ay->default_chip_flag = 0;
  ay->dirty = 1;
  return 1;
}


/** Set chip frequency. */
void ayemu_set_chip_freq(ayemu_ay_t *ay, int chipfreq)
{
  if (!check_magic(ay)) return;

  ay->ChipFreq = chipfreq;
  ay->dirty = 1;
}

/*! Set output sound format
 * \arg \c ay - pointer to ayemu_t structure
 * \arg \c freq - sound freq (44100 for example)
 * \arg \c chans - number of channels (1-mono, 2-stereo)
 * \arg \c bits - now supported only 16 and 8.
 * \retval \b 1 on success, \b 0 if error occure
 */
int ayemu_set_sound_format (ayemu_ay_t *ay, int freq, int chans, int bits)
{
  if (!check_magic(ay))
    return 0;

  if (!(bits == 16 || bits == 8)) {
    ayemu_err = "Incorrect bits value";
    return 0;
  }
  else if (!(chans == 1 || chans == 2)) {
    ayemu_err = "Incorrect number of channels";
    return 0;
  }
  else if (freq < 50) {
    ayemu_err = "Incorrect output sound freq";
    return 0;
  }
  else {
    ay->sndfmt.freq = freq;
    ay->sndfmt.channels = chans;
    ay->sndfmt.bpc = bits;
  }

  ay->default_sound_format_flag = 0;
  ay->dirty = 1;
  return 1;
}


/*! Set amplitude factor for each of channels (A,B anc C, tone and noise).
 * Factor's value must be from (-100) to 100.
 * \arg ay - pointer to ayemu_t structure
 * \arg stereo_type - type of stereo
 * \arg custom_eq - NULL or pointer to custom table of mixer layout.
 * \retval 1 if OK, 0 if error occures.
 */
int ayemu_set_stereo(ayemu_ay_t *ay, ayemu_stereo_t stereo_type, int *custom_eq)
{
  int i;
  int chip;

  if (!check_magic(ay))
    return 0;

  if (stereo_type != AYEMU_STEREO_CUSTOM && custom_eq != NULL) {
    ayemu_err = "Stereo type not custom, 'custom_eq' parametr must be NULL";
    return 0;
  }

  chip = (ay->type == AYEMU_AY)? 0 : 1;

  switch(stereo_type) {
  case AYEMU_MONO:
  case AYEMU_ABC:
  case AYEMU_ACB:
  case AYEMU_BAC:
  case AYEMU_BCA:
  case AYEMU_CAB:
  case AYEMU_CBA:
    for (i = 0 ; i < 6 ; i++)
      ay->eq[i] = default_layout[chip][stereo_type][i];
    break;
  case AYEMU_STEREO_CUSTOM:
    for (i = 0 ; i < 6 ; i++)
      ay->eq[i] = custom_eq[i];
    break;
  default:
    ayemu_err = "Incorrect stereo type";
    return 0;
  }

  ay->default_stereo_flag = 0;
  ay->dirty = 1;
  return 1;
}


#if 0
#define WARN_IF_REGISTER_GREAT_THAN(r,m) \
if (*(regs + r) > m) \
   fprintf(stderr, "ayemu_set_regs: warning: possible bad register data- R%d > %d\n", r, m)
#else
#define WARN_IF_REGISTER_GREAT_THAN(r,m)
#endif


/** Assign values for AY registers.
 *
 * You must pass array of char [14] to this function
 */
void ayemu_set_regs(ayemu_ay_t *ay, ayemu_ay_reg_frame_t regs)
{
  if (!check_magic(ay)) return;

  WARN_IF_REGISTER_GREAT_THAN(1,15);
  WARN_IF_REGISTER_GREAT_THAN(3,15);
  WARN_IF_REGISTER_GREAT_THAN(5,15);
  WARN_IF_REGISTER_GREAT_THAN(8,31);
  WARN_IF_REGISTER_GREAT_THAN(9,31);
  WARN_IF_REGISTER_GREAT_THAN(10,31);

  ay->regs.tone_a  = regs[0] + ((regs[1]&0x0f) << 8);
  ay->regs.tone_b  = regs[2] + ((regs[3]&0x0f) << 8);
  ay->regs.tone_c  = regs[4] + ((regs[5]&0x0f) << 8);

  ay->regs.noise = regs[6] & 0x1f;

  ay->regs.R7_tone_a  = ! (regs[7] & 0x01);
  ay->regs.R7_tone_b  = ! (regs[7] & 0x02);
  ay->regs.R7_tone_c  = ! (regs[7] & 0x04);

  ay->regs.R7_noise_a = ! (regs[7] & 0x08);
  ay->regs.R7_noise_b = ! (regs[7] & 0x10);
  ay->regs.R7_noise_c = ! (regs[7] & 0x20);

  ay->regs.vol_a = regs[8]  & 0x0f;
  ay->regs.vol_b = regs[9]  & 0x0f;
  ay->regs.vol_c = regs[10] & 0x0f;
  ay->regs.env_a = regs[8]  & 0x10;
  ay->regs.env_b = regs[9]  & 0x10;
  ay->regs.env_c = regs[10] & 0x10;
  ay->regs.env_freq = regs[11] + (regs[12] << 8);

  if (regs[13] != 0xff) {                   /* R13 = 255 means continue curent envelop */
    ay->regs.env_style = regs[13] & 0x0f;
    ay->env_pos = ay->cnt_e = 0;
  }
}


static void prepare_generation(ayemu_ay_t *ay)
{
  int vol, max_l, max_r;

  if (!ay->dirty) return;

  if (!bEnvGenInit) gen_env ();

  if (ay->default_chip_flag) ayemu_set_chip_type(ay, AYEMU_AY, NULL);

  if (ay->default_stereo_flag) ayemu_set_stereo(ay, AYEMU_ABC, NULL);

  if (ay->default_sound_format_flag) ayemu_set_sound_format(ay, 44100, 2, 16);

  ay->ChipTacts_per_outcount = ay->ChipFreq / ay->sndfmt.freq / 8;

  {  /* GenVols */
    int n, m;
    int vol;
    for (n = 0; n < 32; n++) {
      vol = ay->table[n];
      for (m=0; m < 6; m++)
	ay->vols[m][n] = (int) (((double) vol * ay->eq[m]) / 100);
    }
  }

  /* динамическая настройка глобального коэффициента усиления
     подразумевается, что в vols [x][31] лежит самая большая громкость
     TODO: Сделать проверку на это ;-)
  */
  max_l = ay->vols[0][31] + ay->vols[2][31] + ay->vols[3][31];
  max_r = ay->vols[1][31] + ay->vols[3][31] + ay->vols[5][31];
  vol = (max_l > max_r) ? max_l : max_r;  // =157283 on all defaults
  ay->Amp_Global = ay->ChipTacts_per_outcount *vol / AYEMU_MAX_AMP;

  ay->dirty = 0;
}


/*! Generate sound.
 * Fill sound buffer with current register data
 * Return value: pointer to next data in output sound buffer
 * \retval \b 1 if OK, \b 0 if error occures.
 */
void *ayemu_gen_sound(ayemu_ay_t *ay, void *buff, size_t sound_bufsize)
{
  int mix_l, mix_r;
  int tmpvol;
  int m;
  int snd_numcount;
  unsigned char *sound_buf = buff;

  if (!check_magic(ay))
    return 0;

  prepare_generation(ay);

  snd_numcount = sound_bufsize / (ay->sndfmt.channels * (ay->sndfmt.bpc >> 3));
  while (snd_numcount-- > 0) {
    mix_l = mix_r = 0;
		
    for (m = 0 ; m < ay->ChipTacts_per_outcount ; m++) {
      if (++ay->cnt_a >= ay->regs.tone_a) {
	ay->cnt_a = 0;
	ay->bit_a = ! ay->bit_a;
      }
      if (++ay->cnt_b >= ay->regs.tone_b) {
	ay->cnt_b = 0;
	ay->bit_b = ! ay->bit_b;
      }
      if (++ay->cnt_c >= ay->regs.tone_c) {
	ay->cnt_c = 0;
	ay->bit_c = ! ay->bit_c;
      }
			
      /* GenNoise (c) Hacker KAY & Sergey Bulba */
      if (++ay->cnt_n >= (ay->regs.noise * 2)) {
	ay->cnt_n = 0;
	ay->Cur_Seed = (ay->Cur_Seed * 2 + 1) ^ \
	  (((ay->Cur_Seed >> 16) ^ (ay->Cur_Seed >> 13)) & 1); 
	ay->bit_n = ((ay->Cur_Seed >> 16) & 1);
      }
			
      if (++ay->cnt_e >= ay->regs.env_freq) {
	ay->cnt_e = 0;
	if (++ay->env_pos > 127)
	  ay->env_pos = 64;
      }

#define ENVVOL Envelope [ay->regs.env_style][ay->env_pos]

      if ((ay->bit_a | !ay->regs.R7_tone_a) & (ay->bit_n | !ay->regs.R7_noise_a)) {
	tmpvol = (ay->regs.env_a)? ENVVOL : ay->regs.vol_a * 2 + 1;
	mix_l += ay->vols[0][tmpvol];
	mix_r += ay->vols[1][tmpvol];
      }
			
      if ((ay->bit_b | !ay->regs.R7_tone_b) & (ay->bit_n | !ay->regs.R7_noise_b)) {
	tmpvol =(ay->regs.env_b)? ENVVOL :  ay->regs.vol_b * 2 + 1;
	mix_l += ay->vols[2][tmpvol];
	mix_r += ay->vols[3][tmpvol];
      }
			
      if ((ay->bit_c | !ay->regs.R7_tone_c) & (ay->bit_n | !ay->regs.R7_noise_c)) {
	tmpvol = (ay->regs.env_c)? ENVVOL : ay->regs.vol_c * 2 + 1;
	mix_l += ay->vols[4][tmpvol];
	mix_r += ay->vols[5][tmpvol];
      }			
    } /* end for (m=0; ...) */
		
    mix_l /= ay->Amp_Global;
    mix_r /= ay->Amp_Global;
		
    if (ay->sndfmt.bpc == 8) {
      mix_l = (mix_l >> 8) | 128; /* 8 bit sound */
      mix_r = (mix_r >> 8) | 128;
      *sound_buf++ = mix_l;
      if (ay->sndfmt.channels != 1)
	*sound_buf++ = mix_r;
    } else {
      *sound_buf++ = mix_l & 0x00FF; /* 16 bit sound */
      *sound_buf++ = (mix_l >> 8);
      if (ay->sndfmt.channels != 1) {
	*sound_buf++ = mix_r & 0x00FF;
	*sound_buf++ = (mix_r >> 8);
      }
    }
  }
  return sound_buf;
}

/** Free all data allocated by emulator
 *
 * For now it do nothing.
 */
void ayemu_free (ayemu_ay_t *ay)
{
  /* nothing to do here */
  return;
}
