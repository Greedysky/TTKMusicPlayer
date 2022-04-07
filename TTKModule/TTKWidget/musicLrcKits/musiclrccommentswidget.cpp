#include "musiclrccommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicLrcCommentsWidget::MusicLrcCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicLrcCommentsWidget::initialize(bool isPain)
{
    MusicCommentsWidget::initialize(isPain);

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeSongCommentRequest(this);
    connect(m_networkRequest, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}

void MusicLrcCommentsWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
