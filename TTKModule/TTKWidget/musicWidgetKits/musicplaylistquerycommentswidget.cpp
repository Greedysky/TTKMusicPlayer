#include "musicplaylistquerycommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicPlaylistQueryCommentsWidget::MusicPlaylistQueryCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicPlaylistQueryCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_downloadRequest = M_DOWNLOAD_QUERY_PTR->getPlaylistCommentRequest(this);
    connect(m_downloadRequest, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}
