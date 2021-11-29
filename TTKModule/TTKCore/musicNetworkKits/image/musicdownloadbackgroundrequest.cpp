#include "musicdownloadbackgroundrequest.h"

#include "musickwdownloadimagerequest.h"
#include "musicqqdownloadimagerequest.h"

MusicDownloadBackgroundRequest::MusicDownloadBackgroundRequest(const QString &name, const QString &save, QObject *parent)
    : MusicDownloadImageRequest(name, save, parent)
{

}

void MusicDownloadBackgroundRequest::startToDownload()
{
    MusicDownloadImageRequest *d = new MusicKWDownloadImageRequest(m_artName, m_savePath, this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
    d->startToDownload();
}

void MusicDownloadBackgroundRequest::downLoadFinished(const QString &bytes)
{
    if(bytes == TTK_DEFAULT_STR)
    {
        deleteLater();
        return;
    }
    else if(bytes.toInt() == 0)
    {
        MusicDownloadImageRequest *d = new MusicQQDownloadImageRequest(m_artName, m_savePath, this);
        d->startToDownload();
    }
}
