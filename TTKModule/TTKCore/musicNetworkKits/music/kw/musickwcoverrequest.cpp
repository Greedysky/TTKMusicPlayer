#include "musickwcoverrequest.h"
#include "musicdatasourcerequest.h"
#include "musickwqueryinterface.h"

MusicKWCoverSourceRequest::MusicKWCoverSourceRequest(QObject *parent)
    : MusicCoverRequest(parent),
      m_decode(false)
{

}

void MusicKWCoverSourceRequest::startRequest(const QString &url)
{
    if(url.startsWith(TTK::Algorithm::mdII(KW_ALBUM_COVER_URL, false)))
    {
        m_decode = false;
    }
    else if(!url.isEmpty())
    {
        m_decode = true;
    }
    else
    {
        deleteLater();
        return;
    }

    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startRequest(url);
}

void MusicKWCoverSourceRequest::downLoadFinished()
{
    MusicCoverRequest::downLoadFinished();
}

void MusicKWCoverSourceRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicCoverRequest::downLoadFinished();

    if(!m_decode)
    {
        Q_EMIT downLoadRawDataChanged(bytes);
        deleteLater();
    }
    else
    {
        if(!bytes.isEmpty() && bytes != "NO_PIC")
        {
            MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
            connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            d->startRequest(bytes);
        }
        else
        {
            deleteLater();
        }
    }
}

