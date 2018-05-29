#include "musicplaylistfoundcommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicPlaylistFoundCommentsWidget::MusicPlaylistFoundCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicPlaylistFoundCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_commentsThread = M_DOWNLOAD_QUERY_PTR->getPlaylistCommentThread(this);
    connect(m_commentsThread, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}
