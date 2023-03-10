#include "musicabstractfmradiorequest.h"

void MusicFMInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(FM_UA_URL, ALG_UA_KEY, false).toUtf8());
    TTK::setSslConfiguration(request);
}


MusicAbstractFMRadioRequest::MusicAbstractFMRadioRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}
