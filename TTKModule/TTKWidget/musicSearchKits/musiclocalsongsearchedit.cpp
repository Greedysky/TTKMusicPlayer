#include "musiclocalsongsearchedit.h"
#include "musicwidgetutils.h"

#include <QKeyEvent>

MusicLocalSongSearchEdit::MusicLocalSongSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet(MusicUIObject::MQSSLineEditStyle03);
    setPlaceholderText(tr("Please input search words!"));
}

void MusicLocalSongSearchEdit::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    QAction *shearM = menu.addAction(tr("Shear"), this, SLOT(cut()));
    QAction *copyM = menu.addAction(tr("Copy"), this, SLOT(copy()));
    menu.addAction(tr("Paste"), this, SLOT(paste()));
    QAction *deleteM = menu.addAction(tr("Delete"), this, SLOT(clear()));
    menu.addSeparator();
    QAction *selectM = menu.addAction(tr("Select All"), this, SLOT(selectAll()));

    const bool state = hasSelectedText();
    shearM->setEnabled(state);
    copyM->setEnabled(state);
    deleteM->setEnabled(state);
    selectM->setEnabled(!text().trimmed().isEmpty());

    MusicUtils::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicLocalSongSearchEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
}

void MusicLocalSongSearchEdit::keyReleaseEvent(QKeyEvent *event)
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
