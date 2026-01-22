#include "musicabstractdownloadimagerequest.h"
#include "musicbackgroundmanager.h"
#include "musictopareawidget.h"

MusicAbstractDownloadImageRequest::MusicAbstractDownloadImageRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractNetwork(parent),
      m_remainCount(0),
      m_index(0),
      m_counter(0),
      m_name(name),
      m_path(path)
{

}

void MusicAbstractDownloadImageRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    deleteLater();
}

void MusicAbstractDownloadImageRequest::downloadDataFinished()
{
    MusicAbstractNetwork::downloadFinished();
    if(++m_index >= m_counter)
    {
        G_BACKGROUND_PTR->setArtistName(m_name);
        MusicTopAreaWidget::instance()->backgroundThemeDownloadFinished();
        deleteAll();
    }
}
