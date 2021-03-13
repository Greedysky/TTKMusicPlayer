#include "musicabstractdjradiorequest.h"

MusicAbstractDJRadioRequest::MusicAbstractDJRadioRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicAbstractDJRadioRequest::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
