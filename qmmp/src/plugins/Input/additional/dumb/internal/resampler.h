#ifndef _RESAMPLER_H_
#define _RESAMPLER_H_

// Ugglay
#ifdef RESAMPLER_DECORATE
#undef PASTE
#undef EVALUATE
#define PASTE(a,b) a ## b
#define EVALUATE(a,b) PASTE(a,b)
#define resampler_init EVALUATE(RESAMPLER_DECORATE,_resampler_init)
#define resampler_create EVALUATE(RESAMPLER_DECORATE,_resampler_create)
#define resampler_delete EVALUATE(RESAMPLER_DECORATE,_resampler_delete)
#define resampler_dup EVALUATE(RESAMPLER_DECORATE,_resampler_dup)
#define resampler_dup_inplace EVALUATE(RESAMPLER_DECORATE,_resampler_dup_inplace)
#define resampler_set_quality EVALUATE(RESAMPLER_DECORATE,_resampler_set_quality)
#define resampler_get_free_count EVALUATE(RESAMPLER_DECORATE,_resampler_get_free_count)
#define resampler_write_sample EVALUATE(RESAMPLER_DECORATE,_resampler_write_sample)
#define resampler_write_sample_fixed EVALUATE(RESAMPLER_DECORATE,_resampler_write_sample_fixed)
#define resampler_set_rate EVALUATE(RESAMPLER_DECORATE,_resampler_set_rate)
#define resampler_ready EVALUATE(RESAMPLER_DECORATE,_resampler_ready)
#define resampler_clear EVALUATE(RESAMPLER_DECORATE,_resampler_clear)
#define resampler_get_sample_count EVALUATE(RESAMPLER_DECORATE,_resampler_get_sample_count)
#define resampler_get_sample EVALUATE(RESAMPLER_DECORATE,_resampler_get_sample)
#define resampler_get_sample_float EVALUATE(RESAMPLER_DECORATE,_resampler_get_sample_float)
#define resampler_remove_sample EVALUATE(RESAMPLER_DECORATE,_resampler_remove_sample)
#endif

void resampler_init(void);

void * resampler_create(void);
void resampler_delete(void *);
void * resampler_dup(const void *);
void resampler_dup_inplace(void *, const void *);

enum
{
    RESAMPLER_QUALITY_MIN = 0,
    RESAMPLER_QUALITY_ZOH = 0,
    RESAMPLER_QUALITY_BLEP = 1,
    RESAMPLER_QUALITY_LINEAR = 2,
    RESAMPLER_QUALITY_BLAM = 3,
    RESAMPLER_QUALITY_CUBIC = 4,
    RESAMPLER_QUALITY_SINC = 5,
    RESAMPLER_QUALITY_MAX = 5
};

void resampler_set_quality(void *, int quality);

int resampler_get_free_count(void *);
void resampler_write_sample(void *, short sample);
void resampler_write_sample_fixed(void *, int sample, unsigned char depth);
void resampler_set_rate( void *, double new_factor );
int resampler_ready(void *);
void resampler_clear(void *);
int resampler_get_sample_count(void *);
int resampler_get_sample(void *);
float resampler_get_sample_float(void *);
void resampler_remove_sample(void *, int decay);

enum { SINC_WIDTH = 16 };
enum { resampler_buffer_size = SINC_WIDTH * 4 };

enum { RESAMPLER_SHIFT = 10 };
enum { RESAMPLER_SHIFT_EXTRA = 8 };
enum { RESAMPLER_RESOLUTION = 1 << RESAMPLER_SHIFT };
enum { RESAMPLER_RESOLUTION_EXTRA = 1 << (RESAMPLER_SHIFT + RESAMPLER_SHIFT_EXTRA) };
enum { SINC_SAMPLES = RESAMPLER_RESOLUTION * SINC_WIDTH };
enum { CUBIC_SAMPLES = RESAMPLER_RESOLUTION * 4 };

static const float RESAMPLER_BLEP_CUTOFF = 0.90f;
static const float RESAMPLER_BLAM_CUTOFF = 0.93f;
static const float RESAMPLER_SINC_CUTOFF = 0.999f;

#ifdef _MSC_VER
#define ALIGNED     _declspec(align(16))
#else
#define ALIGNED     __attribute__((aligned(16)))
#endif

extern ALIGNED float cubic_lut[CUBIC_SAMPLES];
extern float sinc_lut[SINC_SAMPLES + 1];
extern float window_lut[SINC_SAMPLES + 1];

typedef struct resampler
{
    int write_pos, write_filled;
    int read_pos, read_filled;
    float phase;
    float phase_inc;
    float inv_phase;
    float inv_phase_inc;
    unsigned char quality;
    signed char delay_added;
    signed char delay_removed;
    float last_amp;
    float accumulator;
    float buffer_in[resampler_buffer_size * 2];
    float buffer_out[resampler_buffer_size + SINC_WIDTH * 2 - 1];
} resampler;

#endif
