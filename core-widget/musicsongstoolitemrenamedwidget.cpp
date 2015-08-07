#include "musicsongstoolitemrenamedwidget.h"

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(
    int offset, const QString &originText, QWidget *parent) :QLineEdit(parent)
{
    setGeometry(1,offset,265,20);
    setText(originText);
    setStyleSheet("background:white;border:3px solid rgb(173,216,230)");
    setFocus(Qt::MouseFocusReason);
    connect(this,SIGNAL(editingFinished()),this,SLOT(renameFinished()));
}

void MusicSongsToolItemRenamedWidget::focusOutEvent(QFocusEvent *)
{
    emit renameFinished(text());
}

void MusicSongsToolItemRenamedWidget::contextMenuEvent(QContextMenuEvent *)
{
    //do nothing just cover the origin
}
