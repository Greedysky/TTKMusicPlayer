#include "musicabstractmoveresizewidget.h"
#include "musicbackgroundmanager.h"

MusicAbstractMoveResizePlainWidget::MusicAbstractMoveResizePlainWidget(QWidget *parent)
    : TTKAbstractMoveResizeWidget(false, parent)
{
    G_BACKGROUND_PTR->addObserver(this);
}

MusicAbstractMoveResizePlainWidget::~MusicAbstractMoveResizePlainWidget()
{
    G_BACKGROUND_PTR->removeObserver(this);
}

void MusicAbstractMoveResizePlainWidget::backgroundChanged()
{
    setBackgroundPixmap(size());
}

void MusicAbstractMoveResizePlainWidget::show()
{
    backgroundChanged();
    QWidget::show();
}

void MusicAbstractMoveResizePlainWidget::resizeEvent(QResizeEvent *event)
{
    TTKAbstractMoveResizeWidget::resizeEvent(event);
    backgroundChanged();
}
