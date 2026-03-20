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

EqSettings::EqSettings(EqSettings &&other)
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

EqSettings &EqSettings::operator=(const EqSettings &s)
{
    for(int i = 0; i < s.d->bands; ++i)
        d->gains[i] = s.d->gains[i];
    d->preamp = s.d->preamp;
    d->isEnabled = s.d->isEnabled;
    d->bands = s.d->bands;
    d->twoPasses = s.d->twoPasses;
    return *this;
}

EqSettings &EqSettings::operator=(EqSettings &&s)
{
    std::swap(d, s.d);
    return *this;
}

bool EqSettings::operator==(const EqSettings &s) const
{
    for(int i = 0; i < d->bands; ++i)
    {
        if(d->gains[i] != s.d->gains[i])
            return false;
    }

    return d->preamp == s.d->preamp && d->isEnabled == s.d->isEnabled &&
           d->bands == s.d->bands && d->twoPasses == s.d->twoPasses;
}

bool EqSettings::operator!=(const EqSettings &s) const
{
    return !operator==(s);
}
