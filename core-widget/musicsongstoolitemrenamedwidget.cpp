#include "musicsongstoolitemrenamedwidget.h"
#include "musicuiobject.h"

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(
    int offset, const QString &originText, QWidget *parent) :QLineEdit(parent)
{
    setGeometry(1,offset,265,20);
    setText(originText);
    setStyleSheet(MusicUIObject::MCustomStyle04);
    setFocus(Qt::MouseFocusReason);
    connect(this, SIGNAL(editingFinished()), SLOT(renameFinished()));
}

void MusicSongsToolItemRenamedWidget::focusOutEvent(QFocusEvent *)
{
    emit renameFinished(text());
}

void MusicSongsToolItemRenamedWidget::contextMenuEvent(QContextMenuEvent *)
{
    //do nothing just cover the origin
}
