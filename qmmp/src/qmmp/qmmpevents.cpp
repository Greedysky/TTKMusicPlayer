#include "qmmpevents_p.h"

StateChangedEvent::StateChangedEvent(Qmmp::State currentState, Qmmp::State previousState)
    : QEvent(EVENT_STATE_CHANGED),
      m_state(currentState),
      m_prevState(previousState)
{

}

Qmmp::State StateChangedEvent::currentState() const
{
    return m_state;
}

Qmmp::State StateChangedEvent::previousState() const
{
    return m_prevState;
}

StreamInfoChangedEvent::StreamInfoChangedEvent(const QHash<QString, QString> &info)
    : QEvent(EVENT_STREAM_INFO_CHANGED),
      m_streamInfo(info)
{

}

QHash<QString, QString> StreamInfoChangedEvent::streamInfo() const
{
    return m_streamInfo;
}

TrackInfoEvent::TrackInfoEvent(const TrackInfo &info)
    : QEvent(EVENT_TRACK_INFO_CHANGED),
      m_info(info)
{

}

const TrackInfo &TrackInfoEvent::trackInfo() const
{
    return m_info;
}
