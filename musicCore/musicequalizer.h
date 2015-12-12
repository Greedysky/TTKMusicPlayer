#ifndef QMUSICEQUALIZER_H
#define QMUSICEQUALIZER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musiclibexportglobal.h"
#include "musiclibzplay.h"

using namespace libZPlay;

class MUSIC_CORE_EXPORT MusicEqualizer
{
public:
    explicit MusicEqualizer(ZPlay *parent);
    ~MusicEqualizer() = default;

    void setEqEffect(const MIntList &hz);
    void setEnaleEffect(bool enale);
    void setEchoEffect();
    void setMixChannelEffect();
    void setReverseEffect();
    void setSideCutEffect();
    void setCenterCutEffect();
    void setRateUpEffect();
    void setRateDownEffect();
    void setPitchUpEffect();
    void setPitchDownEffect();
    void setTempoUpEffect();
    void setTempoDownEffect();
    void setFadeOutEffect();
    void setFadeInEffect();
    void readEqInformation();

protected:
    void init();

    ZPlay *m_parentClass;
    int m_Fcount;
    int m_fSideCut;
    int m_fCenterCut;
    int m_nRate;
    int m_nPitch;
    int m_nTempo;
    bool m_reverse;
    bool m_mixChannelEffect;
    bool m_echoEffect;

};

#endif // QMUSICEQUALIZER_H
