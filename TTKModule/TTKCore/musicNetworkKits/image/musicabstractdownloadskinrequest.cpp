#include "musicabstractdownloadskinrequest.h"

MusicAbstractDownloadSkinRequest::MusicAbstractDownloadSkinRequest(QObject *parent)
    : QObject(parent)
{

}

void MusicAbstractDownloadSkinRequest::startToDownload()
{

}

void MusicAbstractDownloadSkinRequest::startToDownload(const QString &id)
{
    Q_UNUSED(id);
}
