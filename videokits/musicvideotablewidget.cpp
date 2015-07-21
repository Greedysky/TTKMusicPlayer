#include "musicvideotablewidget.h"
#include "../core/musicdatadownloadthread.h"

#include <time.h>

MusicVideoTableWidget::MusicVideoTableWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    setColumnCount(8);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,175);
    headerview->resizeSection(2,145);
    headerview->resizeSection(3,30);
    headerview->resizeSection(4,55);
    headerview->resizeSection(5,24);
    headerview->resizeSection(6,24);
    headerview->resizeSection(7,24);
    setTransparent(255);
    qsrand(time(NULL));

    m_downLoadManager = new MusicDownLoadQueryThread(this);
    connect(m_downLoadManager,SIGNAL(clearAllItems()),this,SLOT(clearAllItems()));
    connect(m_downLoadManager,SIGNAL(creatSearchedItems(QString,QString,QString)),
            this,SLOT(creatSearchedItems(QString,QString,QString)));

    connect(this,SIGNAL(cellClicked(int,int)),SLOT(listCellClicked(int,int)));
    connect(this,SIGNAL(cellEntered(int,int)),SLOT(listCellEntered(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(itemDoubleClicked(int,int)));
}

MusicVideoTableWidget::~MusicVideoTableWidget()
{
    clearAllItems();
}

void MusicVideoTableWidget::contextMenuEvent(QContextMenuEvent *)
{
//    QTableWidget::contextMenuEvent(event);
}

void MusicVideoTableWidget::startSearchMV(const QString &text)
{
    m_downLoadManager->startSearchSong(MV, text);
}

void MusicVideoTableWidget::clearAllItems()
{
    clearContents();
    setColumnCount(8);
    setRowCount(0);
}

QString MusicVideoTableWidget::randToGetStrength()
{
    switch(qrand()%5)
    {
        case 0: return QString::fromUtf8(":/video/video_1");
        case 1: return QString::fromUtf8(":/video/video_2");
        case 2: return QString::fromUtf8(":/video/video_3");
        case 3: return QString::fromUtf8(":/video/video_4");
        case 4: return QString::fromUtf8(":/video/video_5");
    }
    return QString::fromUtf8(":/video/video_5");
}

void MusicVideoTableWidget::creatSearchedItems(const QString &songname,
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

                      item = new QTableWidgetItem(QIcon( randToGetStrength() ),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/showMV")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 5, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/autionplay")),"");
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 6, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/musicdownload")),"");
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 7, item);
}

void MusicVideoTableWidget::listCellClicked(int row,int col)
{
    switch(col)
    {
      case 5:
      case 6:
            itemDoubleClicked(row, -1);break;
      case 7:
            musicDownloadLocal(row);break;
      default:break;
    }
}

void MusicVideoTableWidget::musicDownloadLocal(int row)
{
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());

    MusicDataDownloadThread* download = new MusicDataDownloadThread(musicSongInfo[row][2],QString("%1 - %2.%3")
                       .arg(musicSongInfo[row][0]).arg(musicSongInfo[row][1]).arg(musicSongInfo[row][3]), this);
    download->startToDownload();
}

void MusicVideoTableWidget::itemDoubleClicked(int row, int)
{
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());
    emit mvURLChanged(musicSongInfo[row][2]);
}
