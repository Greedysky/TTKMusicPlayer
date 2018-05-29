#include "musiclrccommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicLrcCommentsWidget::MusicLrcCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

void MusicLrcCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_commentsThread = M_DOWNLOAD_QUERY_PTR->getSongCommentThread(this);
    connect(m_commentsThread, SIGNAL(createSearchedItem(MusicResultsItem)), SLOT(createSearchedItem(MusicResultsItem)));
}

void MusicLrcCommentsWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
