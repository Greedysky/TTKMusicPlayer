#include "musicabstractmoveresizewidget.h"
#include "musicbackgroundmanager.h"

MusicAbstractMoveResizeContainWidget::MusicAbstractMoveResizeContainWidget(QWidget *parent)
    : TTKAbstractMoveResizeWidget(parent)
{
    G_BACKGROUND_PTR->addObserver(this);
}

MusicAbstractMoveResizeContainWidget::~MusicAbstractMoveResizeContainWidget()
{
    G_BACKGROUND_PTR->removeObserver(this);
}

void MusicAbstractMoveResizeContainWidget::backgroundChanged()
{
    setBackgroundPixmap(size());
}

void MusicAbstractMoveResizeContainWidget::show()
{
    backgroundChanged();
    QWidget::show();
}

void MusicAbstractMoveResizeContainWidget::resizeEvent(QResizeEvent *event)
{
    TTKAbstractMoveResizeWidget::resizeEvent(event);
    backgroundChanged();
}
