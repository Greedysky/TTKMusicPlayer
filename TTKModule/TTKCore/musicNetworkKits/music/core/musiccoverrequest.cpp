#include "musiccoverrequest.h"

MusicCoverRequest::MusicCoverRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicCoverRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    deleteLater();
}
