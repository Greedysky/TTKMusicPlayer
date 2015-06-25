#include "musicsongsearchonlinewidget.h"
#include "core/musiclrcdownloadthread.h"
#include "core/musicsongdownloadthread.h"
#include "toolswidget/musicmydownloadrecordobject.h"

MusicSongSearchOnlineWidget::MusicSongSearchOnlineWidget(QWidget *parent) :
    MusicTableWidgetAbstract(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,310);
    headerview->resizeSection(1,46);
    headerview->resizeSection(2,26);
    headerview->resizeSection(3,26);
    setStyleSheet("QTableWidget{selection-background-color:pink;}" + \
                   MusicObject::MusicScrollBarStyle);
    setTransparent(255);
    m_songItemIndex = 0;

    m_downLoadManager = new MusicDownLoadManagerThread(this);
    connect(m_downLoadManager,SIGNAL(clearAllItems()),this,SLOT(clearAllItems()));
    connect(m_downLoadManager,SIGNAL(showDownLoadInfoFor(DownLoadType)),
            this,SIGNAL(showDownLoadInfoFor(DownLoadType)));
    connect(m_downLoadManager,SIGNAL(showDownLoadInfoFinished(QString)),
            this,SIGNAL(showDownLoadInfoFinished(QString)));
    connect(m_downLoadManager,SIGNAL(creatSearchedItems(QString,QString,double)),
            this,SLOT(creatSearchedItems(QString,QString,double)));

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
    m_downLoadManager->startSearchSong(text);
}

void MusicSongSearchOnlineWidget::clearAllItems()
{
    m_songItemIndex = 0;
    //Remove all the original item
    clearContents();
    setColumnCount(4);
    setRowCount(0);
}

void MusicSongSearchOnlineWidget::creatSearchedItems(const QString &songname,
            const QString &artistname, double time)
{
    setRowCount(m_downLoadManager->getSongIdIndex());

    QTableWidgetItem *item0 = new QTableWidgetItem(songname + " - " + artistname);
    item0->setTextColor(QColor(50,50,50));
    item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->setItem(m_songItemIndex,0,item0);

    int minute = static_cast<int>(time)/60;
    int second = static_cast<int>(time)%60;

    QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1:%2")
                              .arg(QString::number(minute).rightJustified(2,'0'))
                              .arg(QString::number(second).rightJustified(2,'0')));

    item1->setTextColor(QColor(50,50,50));
    item1->setTextAlignment(Qt::AlignCenter);
    this->setItem(m_songItemIndex,1,item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/addtoplaylist")),"");
    item1->setTextAlignment(Qt::AlignCenter);
    this->setItem(m_songItemIndex,2,item2);

    QTableWidgetItem *item3 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/musicdownload")),"");
    item1->setTextAlignment(Qt::AlignCenter);
    this->setItem(m_songItemIndex,3,item3);

    ++m_songItemIndex;
}

void MusicSongSearchOnlineWidget::listCellClicked(int row,int col)
{
    switch(col)
    {
      case 2:
        addSearchMusicToPlayList(row);break;
      case 3:
        musicDownloadLocal(row);break;
      default:break;
    }
}

void MusicSongSearchOnlineWidget::addSearchMusicToPlayList(int row)
{
    emit showDownLoadInfoFor(Buffing);
    musicDownloadLocal(row);
    emit MuiscSongToPlayListChanged(this->item(row,0)->text());
}

void MusicSongSearchOnlineWidget::musicDownloadLocal(int row)
{
    emit showDownLoadInfoFor(DownLoading);
    QList< QStringList > musicSongInfo(m_downLoadManager->getMusicSongInfo());
    QString musicSong = this->item(row,0)->text();
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
    new MusicLrcDownLoadThread(musicSongInfo[row][1],LRC_DOWNLOAD +
                               musicSong + LRC_FILE,this);
    new MusicSongDownloadThread(musicSongInfo[row][2],
          ART_DOWNLOAD + musicSongInfo[row][3] + JPG_FILE,this);

}

void MusicSongSearchOnlineWidget::itemDoubleClicked(int row, int)
{
    addSearchMusicToPlayList(row);
}

