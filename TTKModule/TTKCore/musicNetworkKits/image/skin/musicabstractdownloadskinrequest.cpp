#include "musicabstractdownloadskinrequest.h"

MusicAbstractDownloadSkinRequest::MusicAbstractDownloadSkinRequest(QObject *parent)
    : QObject(parent)
{

}

void MusicAbstractDownloadSkinRequest::startRequest()
{

}

void MusicAbstractDownloadSkinRequest::startRequest(const QString &id)
{
    Q_UNUSED(id);
}
