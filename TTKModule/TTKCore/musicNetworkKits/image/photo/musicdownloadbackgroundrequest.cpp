#include "musicdownloadbackgroundrequest.h"
#include "musicdownloadbackgroundmodule.h"

MusicDownloadBackgroundRequest::MusicDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent),
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
            MusicAbstractDownloadImageRequest *d = new MusicKWDownloadBackgroundRequest(m_artName, m_savePath, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
            d->startRequest();
            break;
        }
        case 1:
        {
            MusicAbstractDownloadImageRequest *d = new MusicTXDownloadBackgroundRequest(m_artName, m_savePath, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
            d->startRequest();
            break;
        }
        case 2:
        {
            MusicAbstractDownloadImageRequest *d = new MusicBPDownloadBackgroundRequest(m_artName, m_savePath, this);
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
