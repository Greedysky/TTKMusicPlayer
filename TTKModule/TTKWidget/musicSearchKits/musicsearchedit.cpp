#include "musicsearchedit.h"
#include "musicwidgetutils.h"

#include <QKeyEvent>

MusicSearchEdit::MusicSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet(MusicUIObject::LineEditStyle02);
    setPlaceholderText(tr("Please input search words!"));
}

void MusicSearchEdit::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MenuStyle02);

    QAction *shearAction = menu.addAction(tr("Shear"), this, SLOT(cut()));
    QAction *copyAction = menu.addAction(tr("Copy"), this, SLOT(copy()));
    menu.addAction(tr("Paste"), this, SLOT(paste()));
    QAction *deleteAction = menu.addAction(tr("Delete"), this, SLOT(clear()));
    menu.addSeparator();
    QAction *selectAction = menu.addAction(tr("Select All"), this, SLOT(selectAll()));

    const bool state = hasSelectedText();
    shearAction->setEnabled(state);
    copyAction->setEnabled(state);
    deleteAction->setEnabled(state);
    selectAction->setEnabled(!text().trimmed().isEmpty());

    MusicUtils::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicSearchEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
}

void MusicSearchEdit::keyReleaseEvent(QKeyEvent *event)
{
    QLineEdit::keyReleaseEvent(event);
    switch(event->key())
    {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            if(!text().isEmpty())
            {
                Q_EMIT enterFinished(text());
            }
            break;
        }
        default: break;
    }
}
