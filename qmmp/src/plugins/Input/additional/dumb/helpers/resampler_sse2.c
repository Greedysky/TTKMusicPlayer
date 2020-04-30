#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_SSE2
#include <math.h>
#include <xmmintrin.h>
#include "internal/resampler.h"

int resampler_run_blep_sse(resampler * r, float ** out_, float * out_end)
{
    int in_size = r->write_filled;
    float const* in_ = r->buffer_in + resampler_buffer_size + r->write_pos - r->write_filled;
    int used = 0;
    in_size -= 1;
    if ( in_size > 0 )
    {
        float* out = *out_;
        float const* in = in_;
        float const* const in_end = in + in_size;
        float last_amp = r->last_amp;
        float inv_phase = r->inv_phase;
        float inv_phase_inc = r->inv_phase_inc;
        
        const int step = RESAMPLER_BLEP_CUTOFF * RESAMPLER_RESOLUTION;
        const int window_step = RESAMPLER_RESOLUTION;
        
        do
        {
            float sample;
            
            if ( out + SINC_WIDTH * 2 > out_end )
                break;
            
            sample = *in++ - last_amp;
            
            if (sample)
            {
                float kernel_sum = 0.0f;
                __m128 kernel[SINC_WIDTH / 2];
                __m128 temp1, temp2;
                __m128 samplex;
                float *kernelf = (float*)(&kernel);
                int phase_reduced = (int)(inv_phase * RESAMPLER_RESOLUTION);
                int phase_adj = phase_reduced * step / RESAMPLER_RESOLUTION;
                int i = SINC_WIDTH;

                for (; i >= -SINC_WIDTH + 1; --i)
                {
                    int pos = i * step;
                    int window_pos = i * window_step;
                    kernel_sum += kernelf[i + SINC_WIDTH - 1] = sinc_lut[abs(phase_adj - pos)] * window_lut[abs(phase_reduced - window_pos)];
                }
                last_amp += sample;
                sample /= kernel_sum;
                samplex = _mm_set1_ps( sample );
                for (i = 0; i < SINC_WIDTH / 2; ++i)
                {
                    temp1 = _mm_load_ps( (const float *)( kernel + i ) );
                    temp1 = _mm_mul_ps( temp1, samplex );
                    temp2 = _mm_loadu_ps( (const float *) out + i * 4 );
                    temp1 = _mm_add_ps( temp1, temp2 );
                    _mm_storeu_ps( (float *) out + i * 4, temp1 );
                }
            }
            
            inv_phase += inv_phase_inc;
            
            out += (int)inv_phase;
            
            inv_phase = fmod(inv_phase, 1.0f);
        }
        while ( in < in_end );
        
        r->inv_phase = inv_phase;
        r->last_amp = last_amp;
        *out_ = out;
        
        used = (int)(in - in_);
        
        r->write_filled -= used;
    }
    
    return used;
}

int resampler_run_blam_sse(resampler * r, float ** out_, float * out_end)
{
    int in_size = r->write_filled;
    float const* in_ = r->buffer_in + resampler_buffer_size + r->write_pos - r->write_filled;
    int used = 0;
    in_size -= 2;
    if ( in_size > 0 )
    {
        float* out = *out_;
        float const* in = in_;
        float const* const in_end = in + in_size;
        float last_amp = r->last_amp;
        float phase = r->phase;
        float phase_inc = r->phase_inc;
        float inv_phase = r->inv_phase;
        float inv_phase_inc = r->inv_phase_inc;
        
        const int step = RESAMPLER_BLAM_CUTOFF * RESAMPLER_RESOLUTION;
        const int window_step = RESAMPLER_RESOLUTION;

        do
        {
            float sample;
            
            if ( out + SINC_WIDTH * 2 > out_end )
                break;

            sample = in[0];
            if (phase_inc < 1.0f)
            {
                sample += (in[1] - in[0]) * phase;
            }
            sample -= last_amp;
            
            if (sample)
            {
                float kernel_sum = 0.0f;
                __m128 kernel[SINC_WIDTH / 2];
                __m128 temp1, temp2;
                __m128 samplex;
                float *kernelf = (float*)(&kernel);
                int phase_reduced = (int)(inv_phase * RESAMPLER_RESOLUTION);
                int phase_adj = phase_reduced * step / RESAMPLER_RESOLUTION;
                int i = SINC_WIDTH;

                for (; i >= -SINC_WIDTH + 1; --i)
                {
                    int pos = i * step;
                    int window_pos = i * window_step;
                    kernel_sum += kernelf[i + SINC_WIDTH - 1] = sinc_lut[abs(phase_adj - pos)] * window_lut[abs(phase_reduced - window_pos)];
                }
                last_amp += sample;
                sample /= kernel_sum;
                samplex = _mm_set1_ps( sample );
                for (i = 0; i < SINC_WIDTH / 2; ++i)
                {
                    temp1 = _mm_load_ps( (const float *)( kernel + i ) );
                    temp1 = _mm_mul_ps( temp1, samplex );
                    temp2 = _mm_loadu_ps( (const float *) out + i * 4 );
                    temp1 = _mm_add_ps( temp1, temp2 );
                    _mm_storeu_ps( (float *) out + i * 4, temp1 );
                }
            }
            
            if (inv_phase_inc < 1.0f)
            {
                ++in;
                inv_phase += inv_phase_inc;
                out += (int)inv_phase;
                inv_phase = fmod(inv_phase, 1.0f);
            }
            else
            {
                phase += phase_inc;
                ++out;
                
                if (phase >= 1.0f)
                {
                    ++in;
                    phase = fmod(phase, 1.0f);
                }
            }
        }
        while ( in < in_end );

        r->phase = phase;
        r->inv_phase = inv_phase;
        r->last_amp = last_amp;
        *out_ = out;
        
        used = (int)(in - in_);
        
        r->write_filled -= used;
    }
    
    return used;
}

int resampler_run_cubic_sse(resampler * r, float ** out_, float * out_end)
{
    int in_size = r->write_filled;
    float const* in_ = r->buffer_in + resampler_buffer_size + r->write_pos - r->write_filled;
    int used = 0;
    in_size -= 4;
    if ( in_size > 0 )
    {
        float* out = *out_;
        float const* in = in_;
        float const* const in_end = in + in_size;
        float phase = r->phase;
        float phase_inc = r->phase_inc;
        
        do
        {
            __m128 temp1, temp2;
            __m128 samplex = _mm_setzero_ps();
            
            if ( out >= out_end )
                break;
            
            temp1 = _mm_loadu_ps( (const float *)( in ) );
            temp2 = _mm_load_ps( (const float *)( cubic_lut + (int)(phase * RESAMPLER_RESOLUTION) * 4 ) );
            temp1 = _mm_mul_ps( temp1, temp2 );
            samplex = _mm_add_ps( samplex, temp1 );
            temp1 = _mm_movehl_ps( temp1, samplex );
            samplex = _mm_add_ps( samplex, temp1 );
            temp1 = samplex;
            temp1 = _mm_shuffle_ps( temp1, samplex, _MM_SHUFFLE(0, 0, 0, 1) );
            samplex = _mm_add_ps( samplex, temp1 );
            _mm_store_ss( out, samplex );
            ++out;
            
            phase += phase_inc;
            
            in += (int)phase;
            
            phase = fmod(phase, 1.0f);
        }
        while ( in < in_end );
        
        r->phase = phase;
        *out_ = out;
        
        used = (int)(in - in_);
        
        r->write_filled -= used;
    }
    
    return used;
}

int resampler_run_sinc_sse(resampler * r, float ** out_, float * out_end)
{
    int in_size = r->write_filled;
    float const* in_ = r->buffer_in + resampler_buffer_size + r->write_pos - r->write_filled;
    int used = 0;
    in_size -= SINC_WIDTH * 2;
    if ( in_size > 0 )
    {
        float* out = *out_;
        float const* in = in_;
        float const* const in_end = in + in_size;
        float phase = r->phase;
        float phase_inc = r->phase_inc;
        
        int step = phase_inc > 1.0f ? (int)(RESAMPLER_RESOLUTION / phase_inc * RESAMPLER_SINC_CUTOFF) : (int)(RESAMPLER_RESOLUTION * RESAMPLER_SINC_CUTOFF);
        int window_step = RESAMPLER_RESOLUTION;
        
        do
        {
            // accumulate in extended precision
            float kernel_sum = 0.0;
            __m128 kernel[SINC_WIDTH / 2];
            __m128 temp1, temp2;
            __m128 samplex = _mm_setzero_ps();
            float *kernelf = (float*)(&kernel);
            int i = SINC_WIDTH;
            int phase_reduced = (int)(phase * RESAMPLER_RESOLUTION);
            int phase_adj = phase_reduced * step / RESAMPLER_RESOLUTION;
            
            if ( out >= out_end )
                break;
            
            for (; i >= -SINC_WIDTH + 1; --i)
            {
                int pos = i * step;
                int window_pos = i * window_step;
                kernel_sum += kernelf[i + SINC_WIDTH - 1] = sinc_lut[abs(phase_adj - pos)] * window_lut[abs(phase_reduced - window_pos)];
            }
            for (i = 0; i < SINC_WIDTH / 2; ++i)
            {
                temp1 = _mm_loadu_ps( (const float *)( in + i * 4 ) );
                temp2 = _mm_load_ps( (const float *)( kernel + i ) );
                temp1 = _mm_mul_ps( temp1, temp2 );
                samplex = _mm_add_ps( samplex, temp1 );
            }
            kernel_sum = 1.0 / kernel_sum;
            temp1 = _mm_movehl_ps( temp1, samplex );
            samplex = _mm_add_ps( samplex, temp1 );
            temp1 = samplex;
            temp1 = _mm_shuffle_ps( temp1, samplex, _MM_SHUFFLE(0, 0, 0, 1) );
            samplex = _mm_add_ps( samplex, temp1 );
            temp1 = _mm_set_ss( kernel_sum );
            samplex = _mm_mul_ps( samplex, temp1 );
            _mm_store_ss( out, samplex );
            ++out;
            
            phase += phase_inc;
            
            in += (int)phase;
            
            phase = fmod(phase, 1.0f);
        }
        while ( in < in_end );
        
        r->phase = phase;
        *out_ = out;
        
        used = (int)(in - in_);
        
        r->write_filled -= used;
    }
    
    return used;
}
#endif

