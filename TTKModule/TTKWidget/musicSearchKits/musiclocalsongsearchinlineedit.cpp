#include "musiclocalsongsearchinlineedit.h"
#include "musiclocalsongsearchpopwidget.h"
#include "musicdownloadsongsuggestthread.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloaddiscoverlistthread.h"

MusicLocalSongSearchInlineEdit::MusicLocalSongSearchInlineEdit(QWidget *parent)
    : MusicLocalSongSearchEdit(parent)
{
    m_popWidget = nullptr;
    m_suggestThread = nullptr;
    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));

    m_discoverThread = M_DOWNLOAD_QUERY_PTR->getDiscoverListThread(this);
    connect(m_discoverThread, SIGNAL(downLoadDataChanged(QString)), SLOT(searchToplistInfoFinished(QString)));
    m_discoverThread->startToSearch();
}

MusicLocalSongSearchInlineEdit::~MusicLocalSongSearchInlineEdit()
{
//    delete m_popWidget;
    delete m_discoverThread;
    delete m_suggestThread;
}

void MusicLocalSongSearchInlineEdit::initWidget(QWidget *parent)
{
    setFocus(Qt::MouseFocusReason);
    setFocusPolicy(Qt::ClickFocus);
    m_popWidget = new MusicLocalSongSearchPopWidget(parent);
    connect(m_popWidget, SIGNAL(setText(QString)), SLOT(setText(QString)));
    m_popWidget->hide();
}

void MusicLocalSongSearchInlineEdit::textChanged(const QString &text)
{
    delete m_suggestThread;
    m_suggestThread = M_DOWNLOAD_QUERY_PTR->getSuggestThread(this);
    connect(m_suggestThread, SIGNAL(downLoadDataChanged(QString)), SLOT(suggestDataChanged()));
    m_suggestThread->startToSearch(text);

    popWidgetChanged(text);
}

void MusicLocalSongSearchInlineEdit::suggestDataChanged()
{
    QStringList names;
    foreach(const MusicResultsItem &item, m_suggestThread->getSearchedItems())
    {
        QString value = item.m_name;
        if(!item.m_nickName.isEmpty() && item.m_nickName != "-")
        {
            value = item.m_nickName + " - " + value;
        }
        names << value;
    }

    if(m_popWidget && !text().trimmed().isEmpty())
    {
        if(names.isEmpty())
        {
            m_popWidget->lower();
            m_popWidget->hide();
        }
        m_popWidget->createSuggestItems(names);
    }
}

void MusicLocalSongSearchInlineEdit::searchToplistInfoFinished(const QString &data)
{
    setPlaceholderText(data);
}

void MusicLocalSongSearchInlineEdit::popWidgetChanged(const QString &text)
{
    if(text.trimmed().isEmpty())
    {
        m_popWidget->createItems();
    }

    if(m_popWidget->height() != 0)
    {
        m_popWidget->raise();
        m_popWidget->show();
    }
}

void MusicLocalSongSearchInlineEdit::focusInEvent(QFocusEvent *event)
{
    MusicLocalSongSearchEdit::focusInEvent(event);
    if(m_popWidget && !m_popWidget->isVisible())
    {
        popWidgetChanged(text());
    }
}

void MusicLocalSongSearchInlineEdit::leaveEvent(QEvent *event)
{
    MusicLocalSongSearchEdit::leaveEvent(event);
    clearFocus();
}
