#include "musiccoversourcerequest.h"
#include "musicdatasourcerequest.h"

MusicCoverSourceRequest::MusicCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent)
{

}

void MusicCoverSourceRequest::startToRequest(const QString &url)
{
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadRawDataFinished(QByteArray)));
    req->startToRequest(url);
}

void MusicCoverSourceRequest::downLoadRawDataFinished(const QByteArray &bytes)
{
    MusicCoverRequest::downLoadFinished();

    Q_EMIT downLoadRawDataChanged(bytes);
    deleteAll();
}

