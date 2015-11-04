#include "musiclocalsongsearchedit.h"
#include "musicuiobject.h"
#include <QMenu>

MusicLocalSongSearchEdit::MusicLocalSongSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet(MusicUIObject::MLineEditStyle03);
    setText(tr("please input search text"));
}

void MusicLocalSongSearchEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    blockSignals(true);
    if(text() == tr("please input search text"))
    {
        setText(QString());
    }
    blockSignals(false);
}

void MusicLocalSongSearchEdit::contextMenuEvent(QContextMenuEvent *)
{
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);

    QAction *shearM = rightClickMenu.addAction(tr("Shear"), this, SLOT(cut()));
    QAction *copyM = rightClickMenu.addAction(tr("Copy"), this, SLOT(copy()));
    rightClickMenu.addAction(tr("Paste"), this, SLOT(paste()));
    QAction *deleteM = rightClickMenu.addAction(tr("Delete"), this, SLOT(clear()));
    rightClickMenu.addSeparator();
    QAction *selectM = rightClickMenu.addAction(tr("SelectAll"), this, SLOT(selectAll()));

    bool state = hasSelectedText();
    shearM->setEnabled(state);
    copyM->setEnabled(state);
    deleteM->setEnabled(state);
    selectM->setEnabled(!text().trimmed().isEmpty());

    rightClickMenu.exec(QCursor::pos());
}
