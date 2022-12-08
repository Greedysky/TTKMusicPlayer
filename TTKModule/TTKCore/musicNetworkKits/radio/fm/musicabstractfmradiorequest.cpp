#include "musicabstractfmradiorequest.h"

void MusicFMInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(FM_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


MusicAbstractFMRadioRequest::MusicAbstractFMRadioRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}
