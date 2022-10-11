#include "musicdownloadimagerequest.h"
#include "musicbackgroundmanager.h"
#include "musictopareawidget.h"

MusicDownloadImageRequest::MusicDownloadImageRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractNetwork(parent),
      m_index(0),
      m_counter(0),
      m_artName(name),
      m_savePath(path)
{

}

void MusicDownloadImageRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    deleteLater();
}

void MusicDownloadImageRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    if(++m_index >= m_counter)
    {
        G_BACKGROUND_PTR->setArtistName(m_artName);
        MusicTopAreaWidget::instance()->musicBackgroundThemeDownloadFinished();
        Q_EMIT downLoadDataChanged(TTK_DEFAULT_STR);
        deleteAll();
    }
}
