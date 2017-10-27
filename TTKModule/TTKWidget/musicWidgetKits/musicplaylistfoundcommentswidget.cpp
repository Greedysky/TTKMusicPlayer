#include "musicplaylistfoundcommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicPlaylistFoundCommentsWidget::MusicPlaylistFoundCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

QString MusicPlaylistFoundCommentsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlaylistFoundCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_commentsThread = M_DOWNLOAD_QUERY_PTR->getPlaylistCommentThread(this);
    connect(m_commentsThread, SIGNAL(createSearchedItems(MusicSongCommentItem)), SLOT(createSearchedItems(MusicSongCommentItem)));
}
