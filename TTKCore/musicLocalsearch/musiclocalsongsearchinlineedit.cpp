#include "musiclocalsongsearchinlineedit.h"
#include "musiclocalsongsearchpopwidget.h"
#include "musicsettingmanager.h"

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
    m_popWidget = new MusicLocalSongSearchPopWidget(parent);
    connect(m_popWidget, SIGNAL(setText(QString)), SLOT(setText(QString)));
    m_popWidget->hide();
}

void MusicLocalSongSearchInlineEdit::resizeWindow()
{
    if(!m_popWidget)
    {
        return;
    }
    int w = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    m_popWidget->move(545 + w - WINDOW_WIDTH_MIN, 45);
}

void MusicLocalSongSearchInlineEdit::focusInEvent(QFocusEvent *event)
{
    MusicLocalSongSearchEdit::focusInEvent(event);
    if(m_popWidget && !m_popWidget->isVisible() && text().trimmed().isEmpty())
    {
        m_popWidget->createItems();
        m_popWidget->raise();
        m_popWidget->show();
    }
}

void MusicLocalSongSearchInlineEdit::leaveEvent(QEvent *event)
{
    MusicLocalSongSearchEdit::leaveEvent(event);
    clearFocus();
}
