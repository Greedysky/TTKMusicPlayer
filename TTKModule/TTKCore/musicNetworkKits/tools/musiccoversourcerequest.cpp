#include "musiccoversourcerequest.h"
#include "musicdatasourcerequest.h"

MusicCoverSourceRequest::MusicCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent)
{

}

void MusicCoverSourceRequest::startToRequest(const QString &url)
{
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadRawDataFinished(QByteArray)));
    req->startToRequest(url);
}

void MusicCoverSourceRequest::downloadRawDataFinished(const QByteArray &bytes)
{
    MusicCoverRequest::downloadFinished();

    Q_EMIT downloadRawDataChanged(bytes);
    deleteAll();
}

