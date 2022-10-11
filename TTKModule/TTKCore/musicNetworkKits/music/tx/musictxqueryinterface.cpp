#include "musictxqueryinterface.h"
#include "musicabstractnetwork.h"

void MusicTXInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(TX_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}
