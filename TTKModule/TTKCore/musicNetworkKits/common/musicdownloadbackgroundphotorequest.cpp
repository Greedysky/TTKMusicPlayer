#include "musicdownloadbackgroundphotorequest.h"

#include "musickwdownloadbackgroundrequest.h"
#include "musicqqdownloadbackgroundrequest.h"

MusicDownloadBackgroundPhotoRequest::MusicDownloadBackgroundPhotoRequest(const QString &name, const QString &save, QObject *parent)
    : MusicDownloadBackgroundRequest(name, save, parent)
{

}

void MusicDownloadBackgroundPhotoRequest::startToDownload()
{
    MusicDownloadBackgroundRequest *d = new MusicKWDownloadBackgroundRequest(m_artName, m_savePath, this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
    d->startToDownload();
}

void MusicDownloadBackgroundPhotoRequest::downLoadFinished(const QString &bytes)
{
    if(bytes == STRING_NULL)
    {
        deleteLater();
        return;
    }
    else if(bytes.toInt() == 0)
    {
        MusicDownloadBackgroundRequest *d = new MusicQQDownloadBackgroundRequest(m_artName, m_savePath, this);
        d->startToDownload();
    }
}
