#include "musiclrcsearchtablewidget.h"
#include "core/musiclrcdownloadthread.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,175);
    headerview->resizeSection(2,143);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,24);
    setTransparent(255);

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
    m_downLoadManager->startSearchSong(Music, text);
}

void MusicLrcSearchTableWidget::clearAllItems()
{
    clearContents();
    setColumnCount(5);
    setRowCount(0);
}

void MusicLrcSearchTableWidget::creatSearchedItems(const QString &songname,
                        const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    setStyleSheet("QTableWidget{selection-background-color: rgba(20,20,20,100);}" + \
                   MusicObject::MusicScrollBarStyle);
    QTableWidgetItem *item = new QTableWidgetItem(QString::number( count ));
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem(songname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem(artistname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/radio/collect")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);
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

