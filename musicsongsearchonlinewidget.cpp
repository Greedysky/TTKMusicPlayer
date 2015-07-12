#include "musicsongsearchonlinewidget.h"
#include "core/musiclrcdownloadthread.h"
#include "core/musicsongdownloadthread.h"
#include "core/musicdatadownloadthread.h"
#include "toolswidget/musicmydownloadrecordobject.h"
#include "localsearch/musiclocalsongsearchrecordobject.h"

#include <QDateTime>

MusicSongSearchOnlineWidget::MusicSongSearchOnlineWidget(QWidget *parent) :
    MusicTableWidgetAbstract(parent)
{
    setColumnCount(6);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,215);
    headerview->resizeSection(2,163);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,26);
    headerview->resizeSection(5,26);
    setTransparent(255);
    m_songItemIndex = 0;

    m_downLoadManager = new MusicDownLoadManagerThread(this);
    connect(m_downLoadManager,SIGNAL(clearAllItems()),this,SLOT(clearAllItems()));
    connect(m_downLoadManager,SIGNAL(showDownLoadInfoFor(DownLoadType)),
            this,SIGNAL(showDownLoadInfoFor(DownLoadType)));
    connect(m_downLoadManager,SIGNAL(showDownLoadInfoFinished(QString)),
            this,SIGNAL(showDownLoadInfoFinished(QString)));
    connect(m_downLoadManager,SIGNAL(creatSearchedItems(QString,QString,QString)),
            this,SLOT(creatSearchedItems(QString,QString,QString)));

    connect(this,SIGNAL(cellEntered(int,int)),SLOT(listCellEntered(int,int)));
    connect(this,SIGNAL(cellClicked(int,int)),SLOT(listCellClicked(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(itemDoubleClicked(int,int)));
}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    clearAllItems();
}

void MusicSongSearchOnlineWidget::contextMenuEvent(QContextMenuEvent *)
{
//    QTableWidget::contextMenuEvent(event);
}

void MusicSongSearchOnlineWidget::startSearchSong(const QString &text)
{
    ////////////////////////////////////////////////
    QStringList names, times;
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig()) return;
    search.readSearchConfig(names, times);
    names.insert(0, text);
    times.insert(0, QString::number(QDateTime::currentMSecsSinceEpoch()));
    search.writeSearchConfig(names, times);
    ////////////////////////////////////////////////
    m_downLoadManager->startSearchSong(text);
}

void MusicSongSearchOnlineWidget::clearAllItems()
{
    m_songItemIndex = 0;
    //Remove all the original item
    clearContents();
    setColumnCount(6);
    setRowCount(0);
}

void MusicSongSearchOnlineWidget::creatSearchedItems(const QString &songname,
                          const QString &artistname, const QString &time)
{
    setRowCount(m_downLoadManager->getSongIdIndex());

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

    QTableWidgetItem *item4 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/addtoplaylist")),"");
    item4->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 4, item4);

    QTableWidgetItem *item5 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/musicdownload")),"");
    item5->setTextAlignment(Qt::AlignCenter);
    setItem(m_songItemIndex, 5, item5);

    ++m_songItemIndex;
}

void MusicSongSearchOnlineWidget::listCellClicked(int row,int col)
{
    switch(col)
    {
      case 4:
        addSearchMusicToPlayList(row);break;
      case 5:
        musicDownloadLocal(row);break;
      default:break;
    }
}

void MusicSongSearchOnlineWidget::addSearchMusicToPlayList(int row)
{
    emit showDownLoadInfoFor(Buffing);
    musicDownloadLocal(row);
    emit MuiscSongToPlayListChanged( item(row, 2)->text() + " - " + item(row, 1)->text() );
}

void MusicSongSearchOnlineWidget::musicDownloadLocal(int row)
{
    emit showDownLoadInfoFor(DownLoading);
    QList< QStringList > musicSongInfo(m_downLoadManager->getMusicSongInfo());
    QString musicSong =  item(row, 2)->text() + " - " + item(row, 1)->text() ;
    QString downloadName = MUSIC_DOWNLOAD + musicSong + MUSIC_FILE;

    ////////////////////////////////////////////////
    QStringList name, path;
    MusicMyDownloadRecordObject down(this);
    if(!down.readDownloadXMLConfig())
        return;
    down.readDownloadConfig(name,path);
    name<<musicSong;
    path<<QFileInfo(downloadName).absoluteFilePath();
    down.writeDownloadConfig(name,path);
    ////////////////////////////////////////////////

    MusicSongDownloadThread *downSong = new MusicSongDownloadThread(
                                            musicSongInfo[row][0], downloadName,this);
    connect(downSong,SIGNAL(musicDownLoadFinished(QString)),this,
                     SIGNAL(showDownLoadInfoFinished(QString)));
    downSong->startToDownload();

    (new MusicLrcDownLoadThread(musicSongInfo[row][1],LRC_DOWNLOAD +
                               musicSong + LRC_FILE,this))->startToDownload();
    (new MusicDataDownloadThread(musicSongInfo[row][2],
          ART_DOWNLOAD + musicSongInfo[row][3] + JPG_FILE,this))->startToDownload();

}

void MusicSongSearchOnlineWidget::itemDoubleClicked(int row, int)
{
    addSearchMusicToPlayList(row);
}

