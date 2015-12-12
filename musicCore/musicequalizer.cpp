#include "musicequalizer.h"
#include "musicsettingmanager.h"

MusicEqualizer::MusicEqualizer(ZPlay *parent)
    : m_parentClass(parent)
{
    m_Fcount = 10;
    init();
}

void MusicEqualizer::init()
{
    m_fSideCut = 0;
    m_fCenterCut = 0;
    m_nRate = 100;
    m_nPitch = 100;
    m_nTempo = 100;
    m_reverse = false;
    m_mixChannelEffect = false;
    m_echoEffect = false;
}

void MusicEqualizer::readEqInformation()
{
    ///Read the equalizer parameters from a configuration file
    if(M_SETTING->value(MusicSettingManager::EqualizerEnableChoiced).toInt())
    {
        setEnaleEffect(true);
        QStringList eqValue = M_SETTING->value(MusicSettingManager::EqualizerValueChoiced).toString().split(',');
        if(eqValue.count() == 11)
        {
            MIntList hz;
            hz<<eqValue[1].toInt()<<eqValue[2].toInt()<<eqValue[3].toInt()
              <<eqValue[4].toInt()<<eqValue[5].toInt()<<eqValue[6].toInt()
              <<eqValue[7].toInt()<<eqValue[8].toInt()<<eqValue[9].toInt()
              <<eqValue[10].toInt();
            setEqEffect(hz);
        }
    }
}

void MusicEqualizer::setEqEffect(const MIntList &hz)
{
    M_LOGGER << "Equalizer" << "setEqEffect-----" << LOG_END;
    ///set equalizer
    for(int i=0; i<m_Fcount; ++i)
    {
        m_parentClass->SetEqualizerBandGain(i, hz[i]*1000);
    }
}

void MusicEqualizer::setEnaleEffect(bool enale)
{
    ///set effect enale or disable
    m_parentClass->EnableEqualizer(enale);
}

void MusicEqualizer::setEchoEffect()
{
    ///set echo effect
    m_echoEffect = !m_echoEffect;
    TEchoEffect effect[2];

    effect[0].nLeftDelay = 1000;
    effect[0].nLeftEchoVolume = 20;
    effect[0].nLeftSrcVolume = 80;
    effect[0].nRightDelay = 500;
    effect[0].nRightEchoVolume = 20;
    effect[0].nRightSrcVolume = 80;

    effect[1].nLeftDelay = 300;
    effect[1].nLeftEchoVolume = 20;
    effect[1].nLeftSrcVolume = 0;
    effect[1].nRightDelay = 300;
    effect[1].nRightEchoVolume = 20;
    effect[1].nRightSrcVolume = 0;

    m_parentClass->SetEchoParam(effect, 2);
    m_parentClass->EnableEcho(m_echoEffect);
}

void MusicEqualizer::setMixChannelEffect()
{
    ///set mix channel effect
    m_mixChannelEffect = !m_mixChannelEffect;
    m_parentClass->MixChannels(m_mixChannelEffect, 50, 50);
}

void MusicEqualizer::setReverseEffect()
{
    ///set reverse effect
    m_reverse = !m_reverse;
    if(m_parentClass->ReverseMode(m_reverse) == 0)
    {
        M_LOGGER << "Equalizer" << m_parentClass->GetError() << LOG_END;
    }
}

void MusicEqualizer::setSideCutEffect()
{
    ///set side cut effect
    m_fSideCut = !m_fSideCut;
    m_parentClass->StereoCut(m_fSideCut, 1, 0);
    m_fCenterCut = 0;
}

void MusicEqualizer::setCenterCutEffect()
{
    ///set center cut effect
    m_fCenterCut = !m_fCenterCut;
    m_parentClass->StereoCut(m_fCenterCut, 0, 0);
    m_fSideCut = 0;
}

void MusicEqualizer::setRateUpEffect()
{
    ///set pitch up effect
    m_nRate += 5;
    m_parentClass->SetRate(m_nRate);
}

void MusicEqualizer::setRateDownEffect()
{
    ///set rate down effect
    m_nRate -= 5;
    m_parentClass->SetRate(m_nRate);
}

void MusicEqualizer::setPitchUpEffect()
{
    ///set pitch up effect
    m_nPitch += 5;
    m_parentClass->SetPitch(m_nPitch);
}

void MusicEqualizer::setPitchDownEffect()
{
    ///set pitch down effect
    m_nPitch -= 5;
    if(m_nPitch < 0) m_nPitch = 0;
    m_parentClass->SetPitch(m_nPitch);
}

void MusicEqualizer::setTempoUpEffect()
{
    ///set tempo up effect
    m_nTempo += 5;
    m_parentClass->SetTempo(m_nTempo);
}

void MusicEqualizer::setTempoDownEffect()
{
    ///set tempo down effect
    m_nTempo -= 5;
    if(m_nTempo < 0) m_nTempo = 0;
    m_parentClass->SetTempo(m_nTempo);
}

void MusicEqualizer::setFadeOutEffect()
{
    ///set fade out effect
    TStreamTime start;
    TStreamTime end;
    m_parentClass->GetPosition(&start);
    end.sec = start.sec + 5;
    m_parentClass->SlideVolume(tfSecond, &start, 100, 100, tfSecond, &end, 0, 0);
}

void MusicEqualizer::setFadeInEffect()
{
    ///set fade in effect
    TStreamTime start;
    TStreamTime end;
    m_parentClass->GetPosition(&start);
    end.sec = start.sec + 5;
    m_parentClass->SlideVolume(tfSecond, &start, 0, 0, tfSecond, &end, 100, 100);
}
