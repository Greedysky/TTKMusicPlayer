#include "musiccoversourcerequest.h"
#include "musicdatasourcerequest.h"

MusicCoverSourceRequest::MusicCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent)
{

}

void MusicCoverSourceRequest::startRequest(const QString &url)
{
    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startRequest(url);
}

void MusicCoverSourceRequest::downLoadFinished()
{
    MusicCoverRequest::downLoadFinished();
}

void MusicCoverSourceRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicCoverRequest::downLoadFinished();

    Q_EMIT downLoadRawDataChanged(bytes);
    deleteLater();
}

