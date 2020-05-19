#include "musicfmradiothreadabstract.h"

MusicFMRadioThreadAbstract::MusicFMRadioThreadAbstract(QObject *parent, QNetworkCookieJar *cookie)
    : MusicNetworkAbstract(parent)
{
    m_cookJar = cookie;
}

MusicFMRadioThreadAbstract::~MusicFMRadioThreadAbstract()
{
    deleteAll();
}
