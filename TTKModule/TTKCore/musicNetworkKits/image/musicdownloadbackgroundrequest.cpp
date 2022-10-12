#include "musicdownloadbackgroundrequest.h"
#include "musickwdownloadimagerequest.h"
#include "musictxdownloadimagerequest.h"
#include "musicbpdownloadimagerequest.h"

MusicDownloadBackgroundRequest::MusicDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicDownloadImageRequest(name, path, parent),
      m_pluginIndex(-1)
{

}

void MusicDownloadBackgroundRequest::startRequest()
{
    m_pluginIndex = -1;
    findImagePlugin();
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
        findImagePlugin();
    }
}

void MusicDownloadBackgroundRequest::findImagePlugin()
{
    switch(++m_pluginIndex)
    {
        case 0:
        {
            MusicDownloadImageRequest *d = new MusicKWDownloadBackgroundRequest(m_artName, m_savePath, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
            d->startRequest();
            break;
        }
        case 1:
        {
            MusicDownloadImageRequest *d = new MusicTXDownloadBackgroundRequest(m_artName, m_savePath, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
            d->startRequest();
            break;
        }
        case 2:
        {
            MusicDownloadImageRequest *d = new MusicBPDownloadBackgroundRequest(m_artName, m_savePath, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
            d->startRequest();
            break;
        }
        default:
        {
            deleteLater();
            break;
        }
    }
}
