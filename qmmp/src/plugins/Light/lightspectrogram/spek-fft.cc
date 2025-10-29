#include <cmath>

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavcodec/avfft.h>
}

#include "spek-fft.h"

class FFTPlanImpl : public FFTPlan
{
public:
    FFTPlanImpl(int nbits);
    ~FFTPlanImpl();

    virtual void execute() override final;

private:
    struct RDFTContext *m_cx;

};

std::unique_ptr<FFTPlan> FFT::create(int nbits)
{
    return std::unique_ptr<FFTPlan>(new FFTPlanImpl(nbits));
}

FFTPlanImpl::FFTPlanImpl(int nbits)
    : FFTPlan(nbits),
      m_cx(av_rdft_init(nbits, DFT_R2C))
{
}

FFTPlanImpl::~FFTPlanImpl()
{
    av_rdft_end(m_cx);
}

void FFTPlanImpl::execute()
{
    av_rdft_calc(m_cx, get_input());

    // Calculate magnitudes.
    const int n = get_input_size();
    const float n2 = n * n;
    set_output(0, 10.0f * log10f(get_input(0) * get_input(0) / n2));
    set_output(n / 2, 10.0f * log10f(get_input(1) * get_input(1) / n2));
    for (int i = 1; i < n / 2; ++i) {
        const float re = get_input(i * 2);
        const float im = get_input(i * 2 + 1);
        set_output(i, 10.0f * log10f((re * re + im * im) / n2));
    }
}
