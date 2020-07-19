#include "musicdiscoverlistrequest.h"

MusicDiscoverListRequest::MusicDiscoverListRequest(QObject *parent)
    : MusicPagingRequest(parent)
{
    MusicTime::initRandom();
}
