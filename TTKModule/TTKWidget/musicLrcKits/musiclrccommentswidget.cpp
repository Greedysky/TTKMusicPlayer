#include "musiclrccommentswidget.h"
#include "musicdownloadqueryfactory.h"

MusicLrcCommentsWidget::MusicLrcCommentsWidget(QWidget *parent)
    : MusicCommentsWidget(parent)
{

}

QString MusicLrcCommentsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcCommentsWidget::initWidget(bool isPain)
{
    MusicCommentsWidget::initWidget(isPain);

    m_commentsThread = M_DOWNLOAD_QUERY_PTR->getSongCommentThread(this);
    connect(m_commentsThread, SIGNAL(createSearchedItems(MusicSongCommentItem)), SLOT(createSearchedItems(MusicSongCommentItem)));
}

void MusicLrcCommentsWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
