#include "musiclocalsongsearchinlineedit.h"
#include "musiclocalsongsearchpopwidget.h"

MusicLocalSongSearchInlineEdit::MusicLocalSongSearchInlineEdit(QWidget *parent)
    : MusicLocalSongSearchEdit(parent)
{
    m_popWidget = new MusicLocalSongSearchPopWidget(parent);
    connect(m_popWidget, SIGNAL(setText(QString)), SLOT(setText(QString)));
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
