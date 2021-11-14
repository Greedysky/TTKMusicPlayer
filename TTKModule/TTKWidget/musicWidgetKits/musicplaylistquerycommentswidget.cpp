#include "musicplaylistquerycommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicPlaylistQueryCommentsWidget::MusicPlaylistQueryCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicPlaylistQueryCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->generatePlaylistCommentRequest(this);
    connect(m_networkRequest, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}
