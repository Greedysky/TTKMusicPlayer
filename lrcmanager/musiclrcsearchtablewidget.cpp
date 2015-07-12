#include "musiclrcsearchtablewidget.h"
#include "core/musiclrcdownloadthread.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,175);
    headerview->resizeSection(2,145);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,24);
    setTransparent(255);
    m_songItemIndex = 0;

    m_downLoadManager = new MusicDownLoadManagerThread(this);
    connect(m_downLoadManager,SIGNAL(clearAllItems()),this,SLOT(clearAllItems()));
    connect(m_downLoadManager,SIGNAL(creatSearchedItems(QString,QString,QString)),
            this,SLOT(creatSearchedItems(QString,QString,QString)));

    connect(this,SIGNAL(cellEntered(int,int)),SLOT(listCellEntered(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(itemDoubleClicked(int,int)));
}

MusicLrcSearchTableWidget::~MusicLrcSearchTableWidget()
{
    clearAllItems();
}

void MusicLrcSearchTableWidget::contextMenuEvent(QContextMenuEvent *)
{
//    QTableWidget::contextMenuEvent(event);
}

void MusicLrcSearchTableWidget::startSearchLrc(const QString &text)
{
    m_downLoadManager->startSearchSong(text);
}

void MusicLrcSearchTableWidget::clearAllItems()
{
    m_songItemIndex = 0;
    //Remove all the original item
    clearContents();
    setColumnCount(5);
    setRowCount(0);
}

void MusicLrcSearchTableWidget::creatSearchedItems(const QString &songname,
                        const QString &artistname, const QString &time)
{
    setRowCount(m_downLoadManager->getSongIdIndex());
    setStyleSheet("QTableWidget{selection-background-color: rgba(20,20,20,100);}" + \
                   MusicObject::MusicScrollBarStyle);
    QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(m_songItemIndex + 1));
    item0->setTextColor(QColor(50,50,50));
    item0->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(songname);
    item1->setTextColor(QColor(50,50,50));
    item1->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(artistname);
    item2->setTextColor(QColor(50,50,50));
    item2->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(time);
    item3->setTextColor(QColor(50,50,50));
    item3->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 3, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/radio/collect")),"");
    item4->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 4, item4);

    ++m_songItemIndex;
}

void MusicLrcSearchTableWidget::musicDownloadLocal(int row)
{
    QList< QStringList > musicSongInfo(m_downLoadManager->getMusicSongInfo());

    MusicLrcDownLoadThread* lrcDownload = new MusicLrcDownLoadThread(musicSongInfo[row][1],
                            LRC_DOWNLOAD + m_currentSongName + LRC_FILE,this);
    connect(lrcDownload,SIGNAL(musicDownLoadFinished(QString)),
                        SIGNAL(lrcDownloadStateChanged(QString)));
    lrcDownload->startToDownload();
}

void MusicLrcSearchTableWidget::itemDoubleClicked(int row, int)
{
    musicDownloadLocal(row);
}

