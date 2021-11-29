#include "musicdownloadimagerequest.h"
#include "musicbackgroundmanager.h"
#include "musictopareawidget.h"

MusicDownloadImageRequest::MusicDownloadImageRequest(const QString &name, const QString &save, QObject *parent)
    : MusicAbstractNetwork(parent),
      m_index(0),
      m_counter(0),
      m_artName(name),
      m_savePath(save)
{

}

void MusicDownloadImageRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    if(++m_index >= m_counter)
    {
        G_BACKGROUND_PTR->setArtistName(m_artName);
        MusicTopAreaWidget::instance()->musicBackgroundThemeDownloadFinished();
        emit downLoadDataChanged(TTK_DEFAULT_STR);
        deleteAll();
        deleteLater();
    }
}
