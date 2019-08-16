#include "musicdownloadbackgroundthread.h"
#include "musicbackgroundmanager.h"
#ifndef MUSIC_MOBILE
#include "musictopareawidget.h"
#endif

MusicDownloadBackgroundThread::MusicDownloadBackgroundThread(const QString &name, const QString &save, QObject *parent)
    : MusicNetworkAbstract(parent), m_index(0), m_counter(0), m_artName(name), m_savePath(save)
{

}

void MusicDownloadBackgroundThread::downLoadFinished()
{
    if(++m_index >= m_counter)
    {
        M_BACKGROUND_PTR->setArtistName( m_artName );
#ifndef MUSIC_MOBILE
        MusicTopAreaWidget::instance()->musicBackgroundThemeDownloadFinished();
#else
        const QString &path = QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL).arg(m_savePath).arg(0).arg(SKN_FILE);
        M_BACKGROUND_PTR->setMBackground(path);
        emit M_BACKGROUND_PTR->setUserSelectArtistIndex(0);
#endif
        deleteLater();
    }
}
