#include "musicsongstoolitemrenamedwidget.h"
#include "musicuiobject.h"

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(
            const QString &originText, QWidget *parent)
    : QLineEdit(parent)
{
    setGeometry(1, 0, 330, 20);
    setText(originText);
    setStyleSheet(MusicUIObject::MLineEditStyle01);
    setFocus(Qt::MouseFocusReason);
    connect(this, SIGNAL(editingFinished()), SLOT(renameFinished()));
}

QString MusicSongsToolItemRenamedWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolItemRenamedWidget::focusOutEvent(QFocusEvent *event)
{
    emit renameFinished(text());
    QLineEdit::focusOutEvent(event);
}

void MusicSongsToolItemRenamedWidget::contextMenuEvent(QContextMenuEvent *)
{
    //do nothing just cover the origin
}
