#include <math.h>
#include <limits.h>
#include <QtEndian>
#include "audioconverter.h"

#define SAMPLES_PER_CHUNCK 64

#define INT_TO_FLOAT(TYPE,SWAP,in,out,samples,offset,max) \
{ \
    TYPE *in_ref = (TYPE *) (in); \
    for(size_t i = 0, left = samples; i < samples;) \
    { \
        /* Vectorized path */ \
        if(left >= SAMPLES_PER_CHUNCK) \
        { \
            for(size_t j = 0; j < SAMPLES_PER_CHUNCK; ++j) \
            { \
                size_t k = i + j; \
                out[k] = (float) (SWAP(in_ref[k]) - offset) / max; \
            } \
            i += SAMPLES_PER_CHUNCK; \
            left -= SAMPLES_PER_CHUNCK; \
        } \
        /* Scalar path */ \
        else \
        { \
            out[i] = (float) (SWAP(in_ref[i]) - offset) / max; \
            ++i; \
            --left; \
        } \
    } \
}

#define FLOAT_TO_INT(TYPE,SWAP,in,out,samples,offset,min,max) \
{ \
    TYPE *out_ref = (TYPE *) (out); \
    float tmp; \
    for(size_t i = 0; i < samples; ++i) \
    { \
        tmp = in[i] * max; \
        out_ref[i] = SWAP(offset + (TYPE) lrintf(qBound(-(float)min, tmp, (float)max))); \
    } \
}


void AudioConverter::configure(Qmmp::AudioFormat f)
{
    m_format = f;
}

void AudioConverter::toFloat(const unsigned char *in, float *out, size_t samples)
{
    switch(m_format)
    {
    case Qmmp::PCM_S8:
        INT_TO_FLOAT(qint8,,in, out, samples, 0, 0x80);
        break;
    case Qmmp::PCM_U8:
        INT_TO_FLOAT(quint8,,in, out, samples, 0x80, 0x80);
        break;
    case Qmmp::PCM_S16LE:
        INT_TO_FLOAT(qint16, qFromLittleEndian, in, out, samples, 0, 0x8000);
        break;
    case Qmmp::PCM_S16BE:
        INT_TO_FLOAT(qint16, qFromBigEndian, in, out, samples, 0, 0x8000);
        break;
    case Qmmp::PCM_U16LE:
        INT_TO_FLOAT(quint16, qFromLittleEndian, in, out, samples, 0x8000, 0x8000);
        break;
    case Qmmp::PCM_U16BE:
        INT_TO_FLOAT(quint16, qFromBigEndian, in, out, samples, 0x8000, 0x8000);
        break;
    case Qmmp::PCM_S24LE:
        INT_TO_FLOAT(qint32, qFromLittleEndian, in, out, samples, 0, 0x800000);
        break;
    case Qmmp::PCM_S24BE:
        INT_TO_FLOAT(qint32, qFromBigEndian, in, out, samples, 0, 0x800000);
        break;
    case Qmmp::PCM_U24LE:
        INT_TO_FLOAT(quint32, qFromLittleEndian, in, out, samples, 0x800000, 0x800000);
        break;
    case Qmmp::PCM_U24BE:
        INT_TO_FLOAT(quint32, qFromBigEndian, in, out, samples, 0x800000, 0x800000);
        break;
    case Qmmp::PCM_S32LE:
        INT_TO_FLOAT(qint32, qFromLittleEndian, in, out, samples, 0, 0x80000000);
        break;
    case Qmmp::PCM_S32BE:
        INT_TO_FLOAT(qint32, qFromBigEndian, in, out, samples, 0, 0x80000000);
        break;
    case Qmmp::PCM_U32LE:
        INT_TO_FLOAT(quint32, qFromLittleEndian, in, out, samples, 0x80000000, 0x80000000);
        break;
    case Qmmp::PCM_U32BE:
        INT_TO_FLOAT(quint32, qFromBigEndian, in, out, samples, 0x80000000, 0x80000000);
        break;
    case Qmmp::PCM_FLOAT:
    case Qmmp::PCM_UNKNOWN:
        memcpy(out, in, samples * sizeof(float));
    }
}

void AudioConverter::fromFloat(const float *in, unsigned char *out, size_t samples)
{
    switch(m_format)
    {
    case Qmmp::PCM_S8:
        FLOAT_TO_INT(qint8,, in, out, samples, 0, 0x80, 0x7F);
        break;
    case Qmmp::PCM_U8:
        FLOAT_TO_INT(qint8,, in, out, samples, 0x80, 0x80, 0x7F);
        break;
    case Qmmp::PCM_S16LE:
        FLOAT_TO_INT(qint16, qToLittleEndian, in, out, samples, 0, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_S16BE:
        FLOAT_TO_INT(qint16, qToBigEndian, in, out, samples, 0, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_U16LE:
        FLOAT_TO_INT(quint16, qToLittleEndian, in, out, samples, 0x8000, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_U16BE:
        FLOAT_TO_INT(quint16, qToBigEndian, in, out, samples, 0x8000, 0x8000, 0x7FFF);
        break;
    case Qmmp::PCM_S24LE:
        FLOAT_TO_INT(qint32, qToLittleEndian, in, out, samples, 0, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_S24BE:
        FLOAT_TO_INT(qint32, qToBigEndian, in, out, samples, 0, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_U24LE:
        FLOAT_TO_INT(quint32, qToLittleEndian, in, out, samples, 0x800000, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_U24BE:
        FLOAT_TO_INT(quint32, qToBigEndian, in, out, samples, 0x800000, 0x800000, 0x7FFFFF);
        break;
    case Qmmp::PCM_S32LE:
        FLOAT_TO_INT(qint32, qToLittleEndian, in, out, samples, 0, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_S32BE:
        FLOAT_TO_INT(qint32, qToBigEndian, in, out, samples, 0, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_U32LE:
        FLOAT_TO_INT(quint32, qToLittleEndian, in, out, samples, 0x80000000, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_U32BE:
        FLOAT_TO_INT(quint32, qToBigEndian, in, out, samples, 0x80000000, 0x80000000, 0x7FFFFF80);
        break;
    case Qmmp::PCM_FLOAT:
    case Qmmp::PCM_UNKNOWN:
        memcpy(out, in, samples * sizeof(float));
    }
}
