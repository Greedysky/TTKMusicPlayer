#include "musiclrccommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicLrcCommentsWidget::MusicLrcCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicLrcCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_downloadRequest = M_DOWNLOAD_QUERY_PTR->getSongCommentRequest(this);
    connect(m_downloadRequest, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}

void MusicLrcCommentsWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
