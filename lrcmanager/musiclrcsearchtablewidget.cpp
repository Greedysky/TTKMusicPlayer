#include "musiclrcsearchtablewidget.h"
#include "musictextdownloadthread.h"
#include "musicuiobject.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicTableQueryWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,175);
    headerview->resizeSection(2,143);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,24);
    setTransparent(255);
}

MusicLrcSearchTableWidget::~MusicLrcSearchTableWidget()
{
    clearAllItems();
}

void MusicLrcSearchTableWidget::startSearchQuery(const QString &text)
{
    m_downLoadManager->startSearchSong(Music, text);
}

void MusicLrcSearchTableWidget::clearAllItems()
{
    MusicTableWidgetAbstract::clearAllItems();
    setColumnCount(5);
}

void MusicLrcSearchTableWidget::creatSearchedItems(const QString &songname,
                        const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    setStyleSheet("QTableWidget{selection-background-color: rgba(20,20,20,100);}" + \
                   MusicUIObject::MusicScrollBarStyle);
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
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());

    MusicTextDownLoadThread* lrcDownload = new MusicTextDownLoadThread(musicSongInfo[row][1],
                             LRC_DOWNLOAD + m_currentSongName + LRC_FILE,this);
    connect(lrcDownload,SIGNAL(musicDownLoadFinished(QString)),
                        SIGNAL(lrcDownloadStateChanged(QString)));
    lrcDownload->startToDownload();
}

void MusicLrcSearchTableWidget::itemDoubleClicked(int row, int)
{
    musicDownloadLocal(row);
}

