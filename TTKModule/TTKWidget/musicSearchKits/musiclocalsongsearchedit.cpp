#include "musiclocalsongsearchedit.h"
#include "musicuiobject.h"

#include <QKeyEvent>

MusicLocalSongSearchEdit::MusicLocalSongSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet(MusicUIObject::MLineEditStyle03);
    setPlaceholderText(tr("please input search text"));
}

void MusicLocalSongSearchEdit::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);

    QAction *shearM = rightClickMenu.addAction(tr("Shear"), this, SLOT(cut()));
    QAction *copyM = rightClickMenu.addAction(tr("Copy"), this, SLOT(copy()));
    rightClickMenu.addAction(tr("Paste"), this, SLOT(paste()));
    QAction *deleteM = rightClickMenu.addAction(tr("Delete"), this, SLOT(clear()));
    rightClickMenu.addSeparator();
    QAction *selectM = rightClickMenu.addAction(tr("SelectAll"), this, SLOT(selectAll()));

    const bool state = hasSelectedText();
    shearM->setEnabled(state);
    copyM->setEnabled(state);
    deleteM->setEnabled(state);
    selectM->setEnabled(!text().trimmed().isEmpty());

    rightClickMenu.exec(QCursor::pos());
}

void MusicLocalSongSearchEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
}

void MusicLocalSongSearchEdit::keyReleaseEvent(QKeyEvent *event)
{
    QLineEdit::keyReleaseEvent(event);
    switch( event->key() )
    {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if(!text().isEmpty())
            {
                emit enterFinished(text());
            }
            break;
        default: break;
    }
}
