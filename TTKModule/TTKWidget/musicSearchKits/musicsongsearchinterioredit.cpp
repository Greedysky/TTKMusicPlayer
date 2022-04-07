#include "musicsongsearchinterioredit.h"
#include "musicsongsearchpopwidget.h"
#include "musicsongsuggestrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musicdiscoverlistrequest.h"

MusicSongSearchInteriorEdit::MusicSongSearchInteriorEdit(QWidget *parent)
    : MusicSearchEdit(parent)
{
    m_popWidget = nullptr;
    m_suggestRequest = nullptr;
    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));

    m_discoverRequest = G_DOWNLOAD_QUERY_PTR->makeDiscoverListRequest(this);
    connect(m_discoverRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(searchToplistInfoFinished(QString)));
    m_discoverRequest->startToSearch();
}

MusicSongSearchInteriorEdit::~MusicSongSearchInteriorEdit()
{
    delete m_discoverRequest;
    delete m_suggestRequest;
}

void MusicSongSearchInteriorEdit::initialize(QWidget *parent)
{
    setFocus(Qt::MouseFocusReason);
    setFocusPolicy(Qt::ClickFocus);

    m_popWidget = new MusicSongSearchPopWidget(parent);
    connect(m_popWidget, SIGNAL(setText(QString)), SLOT(setText(QString)));
    m_popWidget->hide();
}

void MusicSongSearchInteriorEdit::closePopWidget()
{
    if(m_popWidget && m_popWidget->isVisible())
    {
        m_popWidget->close();
    }
}

void MusicSongSearchInteriorEdit::textChanged(const QString &text)
{
    delete m_suggestRequest;
    m_suggestRequest = G_DOWNLOAD_QUERY_PTR->makeSuggestRequest(this);
    connect(m_suggestRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(suggestDataChanged()));
    m_suggestRequest->startToSearch(text);

    popWidgetChanged(text);
}

void MusicSongSearchInteriorEdit::suggestDataChanged()
{
    QStringList names;
    for(const MusicResultsItem &item : m_suggestRequest->getSearchedItems())
    {
        QString value = item.m_name;
        if(!item.m_nickName.isEmpty() && item.m_nickName != TTK_DEFAULT_STR)
        {
            value = QString("%1 - %2").arg(item.m_nickName).arg(value);
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

void MusicSongSearchInteriorEdit::searchToplistInfoFinished(const QString &bytes)
{
    setPlaceholderText(bytes);
}

void MusicSongSearchInteriorEdit::popWidgetChanged(const QString &text)
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

void MusicSongSearchInteriorEdit::focusInEvent(QFocusEvent *event)
{
    MusicSearchEdit::focusInEvent(event);
    if(m_popWidget && !m_popWidget->isVisible())
    {
        popWidgetChanged(text());
    }
}

void MusicSongSearchInteriorEdit::leaveEvent(QEvent *event)
{
    MusicSearchEdit::leaveEvent(event);
    clearFocus();
}
