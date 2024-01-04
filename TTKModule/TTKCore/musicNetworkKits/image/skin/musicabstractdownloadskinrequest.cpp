#include "musicabstractdownloadskinrequest.h"

MusicAbstractDownloadSkinRequest::MusicAbstractDownloadSkinRequest(QObject *parent)
    : QObject(parent)
{

}

void MusicAbstractDownloadSkinRequest::startToRequest()
{

}

void MusicAbstractDownloadSkinRequest::startToRequest(const QString &id)
{
    Q_UNUSED(id);
}
