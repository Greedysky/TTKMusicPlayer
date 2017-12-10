#include "musiclocalsongsearchinlineedit.h"
#include "musiclocalsongsearchpopwidget.h"
#include "musicdownloadquerythreadabstract.h"
#include "musicdownloadqueryfactory.h"

MusicLocalSongSearchInlineEdit::MusicLocalSongSearchInlineEdit(QWidget *parent)
    : MusicLocalSongSearchEdit(parent)
{
    m_popWidget = nullptr;
    m_suggestThread = nullptr;
    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
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

void MusicLocalSongSearchInlineEdit::textChanged(const QString &text)
{
    delete m_suggestThread;
    m_suggestThread = M_DOWNLOAD_QUERY_PTR->getSuggestThread(this);
    connect(m_suggestThread, SIGNAL(downLoadDataChanged(QString)), SLOT(suggestDataChanged()));
    m_suggestThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, text);

    popWidgetChanged(text);
}

void MusicLocalSongSearchInlineEdit::suggestDataChanged()
{
    QStringList names;
    foreach(const MusicObject::MusicSongInformation &info, m_suggestThread->getMusicSongInfos())
    {
        QString value = info.m_songName;
        if(!info.m_singerName.isEmpty())
        {
            value = info.m_singerName + " - " + value;
        }
        names << value;
    }

    if(m_popWidget && !text().trimmed().isEmpty())
    {
        m_popWidget->createSuggestItems(names);
    }
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
