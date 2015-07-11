#include "musiclocalsongsearchedit.h"
#include "musiclocalsongsearchpopwidget.h"
#include "core/musicobject.h"
#include <QMenu>
#include <QDebug>

MusicLocalSongSearchEdit::MusicLocalSongSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setStyleSheet("QLineEdit{border-image: url(:/image/searchline);"
                  "font-size:15px; color:#6495ED;}");
    setText(tr("please input search text"));
}

void MusicLocalSongSearchEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    blockSignals(true);
    if(text() == tr("please input search text"))
        setText(QString());
    blockSignals(false);
}

void MusicLocalSongSearchEdit::contextMenuEvent(QContextMenuEvent *)
{
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicObject::MusicSystemTrayMenu);

    QAction *shearM = rightClickMenu.addAction(tr("Shear"),this,SLOT(cut()));
    QAction *copyM = rightClickMenu.addAction(tr("Copy"),this,SLOT(copy()));
    rightClickMenu.addAction(tr("Paste"),this,SLOT(paste()));
    QAction *deleteM = rightClickMenu.addAction(tr("Delete"),this,SLOT(clear()));
    rightClickMenu.addSeparator();
    QAction *selectM = rightClickMenu.addAction(tr("SelectAll"),this,SLOT(selectAll()));

    bool state = hasSelectedText();
    shearM->setEnabled(state);
    copyM->setEnabled(state);
    deleteM->setEnabled(state);
    selectM->setEnabled(!text().trimmed().isEmpty());

    rightClickMenu.exec(QCursor::pos());
}

MusicLocalSongSearchInlineEdit::MusicLocalSongSearchInlineEdit(QWidget *parent)
    : MusicLocalSongSearchEdit(parent)
{
    m_popWidget = new MusicLocalSongSearchPopWidget(parent);
    connect(m_popWidget,SIGNAL(setText(QString)),SLOT(setText(QString)));
    m_popWidget->hide();
}

MusicLocalSongSearchInlineEdit::~MusicLocalSongSearchInlineEdit()
{

}

void MusicLocalSongSearchInlineEdit::focusOutEvent(QFocusEvent *event)
{
    MusicLocalSongSearchEdit::focusOutEvent(event);
    if(m_popWidget && !m_popWidget->hasFocus())
    {
        m_popWidget->lower();
        m_popWidget->hide();
    }
}

void MusicLocalSongSearchInlineEdit::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(text().trimmed().isEmpty())
    {
        m_popWidget->createItems();
        m_popWidget->raise();
        m_popWidget->show();
    }
}
