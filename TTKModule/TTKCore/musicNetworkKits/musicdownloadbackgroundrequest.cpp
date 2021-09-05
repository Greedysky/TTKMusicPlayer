#include "musicdownloadbackgroundrequest.h"
#include "musicbackgroundmanager.h"
#include "musictopareawidget.h"

MusicDownloadBackgroundRequest::MusicDownloadBackgroundRequest(const QString &name, const QString &save, QObject *parent)
    : MusicAbstractNetwork(parent),
      m_index(0),
      m_counter(0),
      m_artName(name),
      m_savePath(save)
{

}

void MusicDownloadBackgroundRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    if(++m_index >= m_counter)
    {
        G_BACKGROUND_PTR->setArtistName(m_artName);
        MusicTopAreaWidget::instance()->musicBackgroundThemeDownloadFinished();
        emit downLoadDataChanged(STRING_NULL);
        deleteAll();
        deleteLater();
    }
}
