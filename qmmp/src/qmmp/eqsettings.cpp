#include "eqsettings.h"

class EqSettingsPrivate
{
public:
    double gains[31] = {0};
    double preamp = 0;
    bool isEnabled = false;
    int bands;
    bool twoPasses = false;

};


EqSettings::EqSettings(const EqSettings &other)
    : d(new EqSettingsPrivate)
{
    operator=(other);
}

EqSettings::EqSettings(EqSettings &&other) noexcept
    : d(nullptr)
{
    std::swap(d, other.d);
}

EqSettings::EqSettings(Bands bands)
    : d(new EqSettingsPrivate)
{
    d->bands = bands;
}

EqSettings::~EqSettings()
{
    delete d;
}

bool EqSettings::isEnabled() const
{
    return d->isEnabled;
}

double EqSettings::gain(int chan) const
{
    return d->gains[chan];
}

double EqSettings::preamp() const
{
    return d->preamp;
}

int EqSettings::bands() const
{
    return static_cast<int>(d->bands);
}

bool EqSettings::twoPasses() const
{
    return d->twoPasses;
}

void EqSettings::setEnabled(bool enabled)
{
    d->isEnabled = enabled;
}

void EqSettings::setGain(int band, double gain)
{
    d->gains[band] = gain;
}

void EqSettings::setPreamp(double preamp)
{
    d->preamp = preamp;
}

void EqSettings::setTwoPasses(bool enabled)
{
    d->twoPasses = enabled;
}

EqSettings &EqSettings::operator=(const EqSettings &other)
{
    for(int i = 0; i < other.d->bands; ++i)
        d->gains[i] = other.d->gains[i];
    d->preamp = other.d->preamp;
    d->isEnabled = other.d->isEnabled;
    d->bands = other.d->bands;
    d->twoPasses = other.d->twoPasses;
    return *this;
}

EqSettings &EqSettings::operator=(EqSettings &&other) noexcept
{
    std::swap(d, other.d);
    return *this;
}

bool EqSettings::operator==(const EqSettings &other) const
{
    for(int i = 0; i < d->bands; ++i)
    {
        if(d->gains[i] != other.d->gains[i])
            return false;
    }

    return d->preamp == other.d->preamp && d->isEnabled == other.d->isEnabled &&
           d->bands == other.d->bands && d->twoPasses == other.d->twoPasses;
}

bool EqSettings::operator!=(const EqSettings &s) const
{
    return !operator==(s);
}
