#include "musictxqueryinterface.h"
#include "musicabstractnetwork.h"

#define REFER_URL   "M25YVkpIeHVOaVFRY0k3dmFWOFJsOE1tU013ZWV0Sy8="

void MusicTXInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(TX_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}
