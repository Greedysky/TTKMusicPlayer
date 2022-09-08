#include "musicdownloadbackgroundrequest.h"
#include "musickwdownloadimagerequest.h"
#include "musictxdownloadimagerequest.h"

MusicDownloadBackgroundRequest::MusicDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicDownloadImageRequest(name, path, parent)
{

}

void MusicDownloadBackgroundRequest::startToDownload()
{
    MusicDownloadImageRequest *d = new MusicKWDownloadBackgroundRequest(m_artName, m_savePath, this);
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
        MusicDownloadImageRequest *d = new MusicTXDownloadBackgroundRequest(m_artName, m_savePath, this);
        d->startToDownload();
    }
}
