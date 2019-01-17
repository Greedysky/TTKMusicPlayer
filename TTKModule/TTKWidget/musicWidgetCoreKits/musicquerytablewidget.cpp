#include "musicquerytablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicitemdelegate.h"
#include "musicgiflabelwidget.h"

#include <qmath.h>
#include <QActionGroup>

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    m_downLoadManager = nullptr;
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_downLoadManager;
}

void MusicQueryTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    delete m_downLoadManager;
    m_downLoadManager = query;
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(createSearchedItem(MusicSearchedItem)), SLOT(createSearchedItem(MusicSearchedItem)));
}

MusicDownLoadQueryThreadAbstract *MusicQueryTableWidget::getQueryInput()
{
    return m_downLoadManager;
}

void MusicQueryTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}



MusicQueryItemTableWidget::MusicQueryItemTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    m_loadingLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Gif_Cicle_Blue, this);
    m_actionGroup = new QActionGroup(this);
    m_labelDelegate = new MusicLabelDelegate(this);

    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionGroupClick(QAction*)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicQueryItemTableWidget::~MusicQueryItemTableWidget()
{
    delete m_loadingLabel;
    delete m_actionGroup;
    delete m_labelDelegate;
}

void MusicQueryItemTableWidget::startSearchQuery(const QString &text)
{
    Q_UNUSED(text);
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
}

void MusicQueryItemTableWidget::listCellClicked(int row, int column)
{
    MusicQueryTableWidget::listCellClicked(row, column);

    if(rowCount() > 0 && row == rowCount() - 1)
    {
        QTableWidgetItem *it = item(row, 0);
        if(it && it->data(MUSIC_TEXTS_ROLE).toString() == tr("More Data"))
        {
            setItemDelegateForRow(row, nullptr);
            clearSpans();
            removeRow(row);

            m_loadingLabel->run(true);
            m_downLoadManager->startToPage(m_downLoadManager->getPageIndex() + 1);
        }
    }
}

void MusicQueryItemTableWidget::clearAllItems()
{
    if(rowCount() > 0)
    {
        setItemDelegateForRow(rowCount() - 1, nullptr);
    }
    MusicAbstractTableWidget::clear();
}

void MusicQueryItemTableWidget::actionGroupClick(QAction *action)
{
    const int row = currentRow();
    if(row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    const QString &songName = (row != -1 && rowCount() > 0) ? item(row, 1)->toolTip() : QString();
    const QString &artistName = (row != -1 && rowCount() > 0) ? item(row, 2)->toolTip() : QString();

    switch( action->data().toInt() )
    {
        case 0: musicDownloadLocal(row); break;
        case 1: emit restartSearchQuery(songName); break;
        case 2: emit restartSearchQuery(artistName); break;
        case 3: emit restartSearchQuery(artistName + " - " + songName); break;
        default: break;
    }
}

void MusicQueryItemTableWidget::createFinishedItem()
{
    m_loadingLabel->run(false);

    setRowCount(rowCount() + 1);
    const int count = rowCount() - 1;
    for(int i=0; i<columnCount(); ++i)
    {
        setItem(count, i, new QTableWidgetItem);
    }
    setSpan(count, 0, 1, columnCount());

    QTableWidgetItem *it = item(count, 0);
    if(it)
    {
        const int total = ceil(m_downLoadManager->getPageTotal()*1.0/m_downLoadManager->getPageSize());
        const bool more = (total > m_downLoadManager->getPageIndex() + 1);
        it->setData(MUSIC_TEXTS_ROLE, more ? tr("More Data") : tr("No More Data"));
        setItemDelegateForRow(count, m_labelDelegate);
    }
}

void MusicQueryItemTableWidget::createContextMenu(QMenu &menu)
{
    if(!m_downLoadManager)
    {
        return;
    }

    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    m_actionGroup->addAction(menu.addAction(tr("musicDownload")))->setData(0);

    menu.addSeparator();

    const int row = currentRow();
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    const MusicObject::MusicSongInformation &info = musicSongInfos[row];
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(info.m_songName)))->setData(1);
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(info.m_singerName)))->setData(2);
    m_actionGroup->addAction(menu.addAction(tr("search '%1 - %2'").arg(info.m_singerName).arg(info.m_songName)))->setData(3);
}

void MusicQueryItemTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryTableWidget::resizeEvent(event);
    m_loadingLabel->move((width() - m_loadingLabel->width())/2, (height() - m_loadingLabel->height())/2);
}

QString MusicQueryItemTableWidget::randToGetStrength() const
{
    switch(qrand()%5)
    {
        case 0: return QString(":/video/lb_video_1");
        case 1: return QString(":/video/lb_video_2");
        case 2: return QString(":/video/lb_video_3");
        case 3: return QString(":/video/lb_video_4");
        case 4: return QString(":/video/lb_video_5");
        default:return QString(":/video/lb_video_5");
    }
}
