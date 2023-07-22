#include "musicsongsearchinterioredit.h"
#include "musicsongsearchpopwidget.h"
#include "musicsongsuggestrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musicdiscoverlistrequest.h"

MusicSongSearchInteriorEdit::MusicSongSearchInteriorEdit(QWidget *parent)
    : MusicSearchEdit(parent),
      m_popWidget(nullptr),
      m_suggestRequest(nullptr)
{
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
    setPopWidgetVisible(false);
}

void MusicSongSearchInteriorEdit::setText(const QString &text)
{
    blockSignals(true);
    MusicSearchEdit::setText(text);
    blockSignals(false);

    setPopWidgetVisible(false);
}

void MusicSongSearchInteriorEdit::textChanged(const QString &text)
{
    delete m_suggestRequest;
    m_suggestRequest = G_DOWNLOAD_QUERY_PTR->makeSuggestRequest(this);
    connect(m_suggestRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(suggestDataChanged()));
    m_suggestRequest->startToSearch(text);

    if(text.trimmed().isEmpty())
    {
        m_popWidget->initialize(this);
    }

    if(m_popWidget->height() != 0)
    {
        setPopWidgetVisible(true);
    }
}

void MusicSongSearchInteriorEdit::selectedTextChanged(const QString &text, bool update)
{
    if(update)
    {
        setText(text);
    }
    else
    {
        blockSignals(true);
        MusicSearchEdit::setText(text);
        blockSignals(false);
    }
}

void MusicSongSearchInteriorEdit::suggestDataChanged()
{
    QStringList names;
    for(const MusicResultDataItem &item : m_suggestRequest->searchedItems())
    {
        names << item.m_name;
    }

    if(m_popWidget && !text().trimmed().isEmpty())
    {
        if(names.isEmpty())
        {
            setPopWidgetVisible(false);
        }
        else if(!m_popWidget->isVisible())
        {
            setPopWidgetVisible(true);
        }

        m_popWidget->createSuggestItems(names);
    }
}

void MusicSongSearchInteriorEdit::searchToplistInfoFinished(const QString &bytes)
{
    setPlaceholderText(bytes);
}

void MusicSongSearchInteriorEdit::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            clearFocus();
            break;
        }
        case Qt::Key_Up:
        {
            m_popWidget->selectRow(true);
            break;
        }
        case Qt::Key_Down:
        {
            m_popWidget->selectRow(false);
            break;
        }
        default: break;
    }

    MusicSearchEdit::keyReleaseEvent(event);
}

void MusicSongSearchInteriorEdit::mousePressEvent(QMouseEvent *event)
{
    MusicSearchEdit::mousePressEvent(event);
    if(m_popWidget && !m_popWidget->isVisible())
    {
        textChanged(text());
    }
}

void MusicSongSearchInteriorEdit::focusOutEvent(QFocusEvent *event)
{
    MusicSearchEdit::focusOutEvent(event);
    if(m_popWidget && m_popWidget->isVisible())
    {
        m_popWidget->close();
    }
}

void MusicSongSearchInteriorEdit::setPopWidgetVisible(bool show)
{
    if(show)
    {
        m_popWidget->raise();
        m_popWidget->show();
    }
    else
    {
        m_popWidget->lower();
        m_popWidget->hide();
    }
}
