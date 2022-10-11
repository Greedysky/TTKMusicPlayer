#include "musicbdqueryinterface.h"
#include "musicabstractnetwork.h"

void MusicBDInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}
