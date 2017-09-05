#include "musiclocalsongsearchinlineedit.h"
#include "musiclocalsongsearchpopwidget.h"

MusicLocalSongSearchInlineEdit::MusicLocalSongSearchInlineEdit(QWidget *parent)
    : MusicLocalSongSearchEdit(parent)
{
    m_popWidget = nullptr;
}

MusicLocalSongSearchInlineEdit::~MusicLocalSongSearchInlineEdit()
{
//    delete m_popWidget;
}

QString MusicLocalSongSearchInlineEdit::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongSearchInlineEdit::initWidget(QWidget *parent)
{
    setFocus(Qt::MouseFocusReason);
    setFocusPolicy(Qt::ClickFocus);
    m_popWidget = new MusicLocalSongSearchPopWidget(parent);
    connect(m_popWidget, SIGNAL(setText(QString)), SLOT(setText(QString)));
    m_popWidget->hide();
}

void MusicLocalSongSearchInlineEdit::focusInEvent(QFocusEvent *event)
{
    MusicLocalSongSearchEdit::focusInEvent(event);
    if(m_popWidget && !m_popWidget->isVisible() && text().trimmed().isEmpty())
    {
        m_popWidget->createItems();
        if(m_popWidget->height() != 0)
        {
            m_popWidget->raise();
            m_popWidget->show();
        }
    }
}

void MusicLocalSongSearchInlineEdit::leaveEvent(QEvent *event)
{
    MusicLocalSongSearchEdit::leaveEvent(event);
    clearFocus();
}
