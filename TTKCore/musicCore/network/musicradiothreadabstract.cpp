#include "musicradiothreadabstract.h"

MusicRadioThreadAbstract::MusicRadioThreadAbstract(QObject *parent, QNetworkCookieJar *cookie)
    : MusicNetworkAbstract(parent)
{
    m_cookJar = cookie;
}

MusicRadioThreadAbstract::~MusicRadioThreadAbstract()
{
    deleteAll();
}
