#include "musicsongsearchonlinewidget.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbgthemedownload.h"
#include "musicmydownloadrecordobject.h"
#include "musiclocalsongsearchrecordobject.h"

#include <QDateTime>
#include <QVBoxLayout>

MusicSongSearchOnlineTableWidget::MusicSongSearchOnlineTableWidget(QWidget *parent)
    : MusicTableQueryWidget(parent)
{
    setColumnCount(6);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,215);
    headerview->resizeSection(2,166);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,26);
    headerview->resizeSection(5,26);
    setTransparent(255);
}

MusicSongSearchOnlineTableWidget::~MusicSongSearchOnlineTableWidget()
{
    clearAllItems();
}

void MusicSongSearchOnlineTableWidget::startSearchQuery(const QString &text)
{
    ////////////////////////////////////////////////
    QStringList names, times;
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig())
    {
        return;
    }
    search.readSearchConfig(names, times);
    names.insert(0, text);
    times.insert(0, QString::number(QDateTime::currentMSecsSinceEpoch()));
    search.writeSearchConfig(names, times);
    ////////////////////////////////////////////////
    m_downLoadManager->startSearchSong(Music, text);
}

void MusicSongSearchOnlineTableWidget::clearAllItems()
{
    MusicTableWidgetAbstract::clearAllItems();
    setColumnCount(6);
}

void MusicSongSearchOnlineTableWidget::creatSearchedItems(const QString &songname,
                               const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem(songname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem(artistname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/addtoplaylist")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/musicdownload")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 5, item);
}

void MusicSongSearchOnlineTableWidget::listCellClicked(int row, int col)
{
    MusicTableQueryWidget::listCellClicked(row, col);
    switch(col)
    {
        case 4:
            addSearchMusicToPlayList(row);
            break;
        case 5:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }
}

void MusicSongSearchOnlineTableWidget::addSearchMusicToPlayList(int row)
{
    emit showDownLoadInfoFor(Buffing);
    musicDownloadLocal(row);
    emit muiscSongToPlayListChanged( item(row, 2)->text() + " - " + item(row, 1)->text() );
}

void MusicSongSearchOnlineTableWidget::musicDownloadLocal(int row)
{
    emit showDownLoadInfoFor(DownLoading);
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());
    QString musicSong =  item(row, 2)->text() + " - " + item(row, 1)->text() ;
    QString downloadName = MUSIC_DOWNLOAD + musicSong + MUSIC_FILE;

    ////////////////////////////////////////////////
    QStringList name, path;
    MusicMyDownloadRecordObject down(this);
    if(!down.readDownloadXMLConfig())
    {
        return;
    }
    down.readDownloadConfig(name,path);
    name<<musicSong;
    path<<QFileInfo(downloadName).absoluteFilePath();
    down.writeDownloadConfig(name,path);
    ////////////////////////////////////////////////

    MusicDataDownloadThread *downSong = new MusicDataDownloadThread(
                                            musicSongInfo[row][0], downloadName,this);
    connect(downSong, SIGNAL(musicDownLoadFinished(QString)),
                      SIGNAL(showDownLoadInfoFinished(QString)));
    downSong->startToDownload();

    (new MusicTextDownLoadThread(musicSongInfo[row][1],LRC_DOWNLOAD +
                                 musicSong + LRC_FILE,this))->startToDownload();
    (new MusicData2DownloadThread(musicSongInfo[row][2],
          ART_DOWNLOAD + musicSongInfo[row][3] + SKN_FILE,this))->startToDownload();

    ///download big picture
    new MusicBgThemeDownload(musicSongInfo[row][3], musicSongInfo[row][3], this);
}

void MusicSongSearchOnlineTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0)
    {
        return;
    }
    addSearchMusicToPlayList(row);
}



MusicSongSearchOnlineWidget::MusicSongSearchOnlineWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setMargin(0);
    boxLayout->setSpacing(0);

    QWidget *toolWidget = new QWidget(this);
    toolWidget->setFixedHeight(65);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    toolWidget->setAutoFillBackground(true);
    toolWidget->setPalette(pal);

    m_searchTableWidget = new  MusicSongSearchOnlineTableWidget(this);
    boxLayout->addWidget(toolWidget);
    boxLayout->addWidget(m_searchTableWidget);
    setLayout(boxLayout);

    connect(m_searchTableWidget, SIGNAL(musicBgDownloadFinished()),
                                 SIGNAL(musicBgDownloadFinished()));
    connect(m_searchTableWidget, SIGNAL(showDownLoadInfoFinished(QString)),
                                 SIGNAL(showDownLoadInfoFinished(QString)));
    connect(m_searchTableWidget, SIGNAL(muiscSongToPlayListChanged(QString)),
                                 SIGNAL(muiscSongToPlayListChanged(QString)));
    connect(m_searchTableWidget, SIGNAL(showDownLoadInfoFor(DownLoadType)),
                                 SIGNAL(showDownLoadInfoFor(DownLoadType)));

}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    delete m_searchTableWidget;
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name) const
{
    m_searchTableWidget->startSearchQuery(name);
}

void MusicSongSearchOnlineWidget::createToolWidget()
{

}
