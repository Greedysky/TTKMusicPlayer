#include "musiclocalsongsearchedit.h"
#include "musicuiobject.h"

#include <QMenu>
#include <QKeyEvent>

MusicLocalSongSearchEdit::MusicLocalSongSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet(MusicUIObject::MLineEditStyle03);
    addFilterText(tr("please input search text"));
}

QString MusicLocalSongSearchEdit::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongSearchEdit::addFilterText(const QString &text)
{
    m_filterText = text;
    setText(text);
}

void MusicLocalSongSearchEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    blockSignals(true);
    if(text() == m_filterText)
    {
        setText(QString());
    }
    blockSignals(false);
}

void MusicLocalSongSearchEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    blockSignals(true);
    if(text().isEmpty())
    {
        setText(m_filterText);
    }
    blockSignals(false);
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

    bool state = hasSelectedText();
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
    }
}
