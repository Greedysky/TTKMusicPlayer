#include "musicdiscoverlistrequest.h"

MusicDiscoverListRequest::MusicDiscoverListRequest(QObject *parent)
    : MusicPageQueryRequest(parent)
{
    MusicTime::initRandom();
}
