#include "musicabstractfmradiorequest.h"

MusicAbstractFMRadioRequest::MusicAbstractFMRadioRequest(QObject *parent, QNetworkCookieJar *cookie)
    : MusicAbstractNetwork(parent)
{
    m_cookJar = cookie;
}

MusicAbstractFMRadioRequest::~MusicAbstractFMRadioRequest()
{
    deleteAll();
}
