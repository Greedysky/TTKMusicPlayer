#include "musicvideotablewidget.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"

#include <time.h>

MusicVideoTableWidget::MusicVideoTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setColumnCount(8);
    resizeWindow(1.0f);
    setTransparent(255);
    qsrand(time(NULL));
    M_CONNECTION->setValue("MusicVideoTableWidget", this);
}

MusicVideoTableWidget::~MusicVideoTableWidget()
{
    M_CONNECTION->disConnect("MusicVideoTableWidget");
    clearAllItems();
}

void MusicVideoTableWidget::startSearchQuery(const QString &text)
{
    m_downLoadManager->startSearchSong(MVQuery, text);
}

void MusicVideoTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(8);
}

QString MusicVideoTableWidget::randToGetStrength() const
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
                                               const QString &artistname,
                                               const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());
    setStyleSheet(MusicUIObject::MTableWidgetStyle01 + \
                  MusicUIObject::MScrollBarStyle01);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem(songname);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem(artistname);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(QIcon( randToGetStrength() ), "");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/showMV")), "");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 5, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/autionplay")), "");
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 6, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/musicdownload")), "");
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 7, item);
}

void MusicVideoTableWidget::listCellClicked(int row,int col)
{
    MusicQueryTableWidget::listCellClicked(row, col);
    switch(col)
    {
        case 5:
        case 6:
            itemDoubleClicked(row, 999);break;
        case 7:
            musicDownloadLocal(row);break;
        default:break;
    }
}

void MusicVideoTableWidget::musicDownloadLocal(int row)
{
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    DownloadSongInfos musicSongInfo(m_downLoadManager->getMusicSongInfo());

    MusicDataDownloadThread* download = new MusicDataDownloadThread(musicSongInfo[row].m_songUrl.first().m_url, QString("%1 - %2.%3")
        .arg(musicSongInfo[row].m_singerName).arg(musicSongInfo[row].m_songName).arg(musicSongInfo[row].m_format), Download_Video, this);
    download->startToDownload();
}

void MusicVideoTableWidget::resizeWindow(float delta)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30*delta);
    headerview->resizeSection(1, 175*delta);
    headerview->resizeSection(2, 151*delta);
    headerview->resizeSection(3, 30*delta);
    headerview->resizeSection(4, 55*delta);
    headerview->resizeSection(5, 24*delta);
    headerview->resizeSection(6, 24*delta);
    headerview->resizeSection(7, 24*delta);
}

void MusicVideoTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0)
    {
        return;
    }
    DownloadSongInfos musicSongInfo(m_downLoadManager->getMusicSongInfo());
    emit mvURLChanged(musicSongInfo[row].m_songUrl.first().m_url);
}

void MusicVideoTableWidget::getMusicMvInfo(SongUrlFormats &data)
{
    DownloadSongInfos musicSongInfo(m_downLoadManager->getMusicSongInfo());
    data = !musicSongInfo.isEmpty() && m_previousClickRow != -1
           ? musicSongInfo[m_previousClickRow].m_songUrl : SongUrlFormats();
}

void MusicVideoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}
