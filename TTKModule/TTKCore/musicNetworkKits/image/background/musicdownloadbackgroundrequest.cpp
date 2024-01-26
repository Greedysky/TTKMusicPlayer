#include "musicdownloadbackgroundrequest.h"
#include "musicbpdownloadimagerequest.h"
#include "musickgdownloadimagerequest.h"
#include "musickwdownloadimagerequest.h"
#include "musictxdownloadimagerequest.h"

MusicDownloadBackgroundRequest::MusicDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : QObject(parent),
      m_findCount(0),
      m_pluginIndex(-1),
      m_name(name),
      m_path(path)
{

}

void MusicDownloadBackgroundRequest::startToRequest()
{
    m_findCount = 0;
    m_pluginIndex = -1;
    findAllPlugins();
}

void MusicDownloadBackgroundRequest::downLoadFinished(const QString &bytes)
{
    if(bytes.isEmpty())
    {
        findAllPlugins();
        return;
    }

    bool ok = false;
    const int value = bytes.toInt(&ok);
    if(ok)
    {
        m_findCount += value;
        if(m_findCount < MAX_IMAGE_COUNT)
        {
            findAllPlugins();
        }
    }
}

void MusicDownloadBackgroundRequest::findAllPlugins()
{
    MusicAbstractDownloadImageRequest *d = nullptr;
    switch(++m_pluginIndex)
    {
        case 0: d = new MusicKGDownloadBackgroundRequest(m_name, m_path, this); break;
        case 1: d = new MusicKWDownloadBackgroundRequest(m_name, m_path, this); break;
        case 2: d = new MusicTXDownloadBackgroundRequest(m_name, m_path, this); break;
        case 3: d = new MusicBPDownloadBackgroundRequest(m_name, m_path, this); break;
        default: break;
    }

    if(d)
    {
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
        //
        d->setRemainCount(MAX_IMAGE_COUNT - m_findCount);
        d->startToRequest();
    }
}
