#include "musicquerytablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicitemdelegate.h"
#include "musicgiflabelwidget.h"

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

QString MusicQueryTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    delete m_downLoadManager;
    m_downLoadManager = query;
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(createSearchedItems(MusicSearchedItem)), SLOT(createSearchedItems(MusicSearchedItem)));
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

QString MusicQueryItemTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryItemTableWidget::startSearchQuery(const QString &text)
{
    Q_UNUSED(text);
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
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
    int row = currentRow();
    if( row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    QString songName = (row != -1 && rowCount() > 0) ? item(row, 1)->toolTip() : QString();
    QString artistName = (row != -1 && rowCount() > 0) ? item(row, 2)->toolTip() : QString();

    switch( action->data().toInt() )
    {
        case 0: musicDownloadLocal(row); break;
        case 1: emit restartSearchQuery(songName); break;
        case 2: emit restartSearchQuery(artistName); break;
        case 3: emit restartSearchQuery(songName + "-" + artistName); break;
    }
}

void MusicQueryItemTableWidget::createFinishedItem()
{
    m_loadingLabel->hide();
    m_loadingLabel->stop();

    setRowCount(rowCount() + 1);
    int count = rowCount() - 1;
    for(int i=0; i<columnCount(); ++i)
    {
        setItem(count, i, new QTableWidgetItem);
    }
    setSpan(count, 0, 1, columnCount());

    QTableWidgetItem *it = item(count, 0);
    if(it)
    {
        it->setData(MUSIC_TEXTS_ROLE, tr("No More Data"));
        setItemDelegateForRow(count, m_labelDelegate);
    }
}

void MusicQueryItemTableWidget::createContextMenu(QMenu &menu)
{
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    m_actionGroup->addAction(menu.addAction(tr("musicDownload")))->setData(0);

    menu.addSeparator();

    QString songName = currentRow() != -1 && rowCount() > 0 ?
                item(currentRow(), 1)->toolTip() : QString();
    QString artistName = currentRow() != -1 && rowCount() > 0 ?
                item(currentRow(), 2)->toolTip() : QString();
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(songName)))->setData(1);
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(artistName)))->setData(2);
    m_actionGroup->addAction(menu.addAction(tr("search '%1 - %2'").arg(songName).arg(artistName)))->setData(3);
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
        case 0: return QString::fromUtf8(":/video/lb_video_1");
        case 1: return QString::fromUtf8(":/video/lb_video_2");
        case 2: return QString::fromUtf8(":/video/lb_video_3");
        case 3: return QString::fromUtf8(":/video/lb_video_4");
        case 4: return QString::fromUtf8(":/video/lb_video_5");
        default:return QString::fromUtf8(":/video/lb_video_5");
    }
}
