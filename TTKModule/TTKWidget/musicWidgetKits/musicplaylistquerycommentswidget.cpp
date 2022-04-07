#include "musicplaylistquerycommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicPlaylistQueryCommentsWidget::MusicPlaylistQueryCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicPlaylistQueryCommentsWidget::initialize(bool isPain)
{
    MusicCommentsWidget::initialize(isPain);

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makePlaylistCommentRequest(this);
    connect(m_networkRequest, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}
