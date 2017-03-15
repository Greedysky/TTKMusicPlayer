#include "musicqueryfoundtablewidget.h"
#include "musicdatadownloadthread.h"
#include "musicdownloadwidget.h"
#include "musiccryptographichash.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicsemaphoreloop.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicsongssummariziedwidget.h"

MusicQueryFoundTableWidget::MusicQueryFoundTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(7);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 436);
    headerview->resizeSection(2, 47);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);
    headerview->resizeSection(6, 26);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicQueryFoundTableWidget::~MusicQueryFoundTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllItems();
}

QString MusicQueryFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryFoundTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    m_downLoadManager->setQueryAllRecords(true);
    m_downLoadManager->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void MusicQueryFoundTableWidget::musicDownloadLocal(int row)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

const MusicObject::MusicSongInfomations& MusicQueryFoundTableWidget::getMusicSongInfos() const
{
    Q_ASSERT(m_downLoadManager);
    return m_downLoadManager->getMusicSongInfos();
}

void MusicQueryFoundTableWidget::downloadDataFrom(bool play)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicObject::MIntList list = getSelectedItems();
    for(int i=0; i<list.count(); ++i)
    {
        if(downloadDataFrom(musicSongInfos[ list[i] ], play && (i == 0)))
        {
            continue;
        }
    }
}

void MusicQueryFoundTableWidget::resizeWindow()
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.9 + 436);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.1 + 47);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));
    }
}

void MusicQueryFoundTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryTableWidget::resizeEvent(event);
    resizeWindow();
}

void MusicQueryFoundTableWidget::listCellEntered(int row, int column)
{
    MusicQueryTableWidget::listCellEntered(row, column);
    if(column == 4 || column == 5 || column == 6)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
}

void MusicQueryFoundTableWidget::listCellClicked(int row, int column)
{
    MusicQueryTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 4:
            addSearchMusicToPlayList(row, true);
            break;
        case 5:
            addSearchMusicToPlayList(row, false);
            break;
        case 6:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }
}

void MusicQueryFoundTableWidget::clearAllItems()
{
    MusicQueryTableWidget::clear();
    setColumnCount(7);
}

void MusicQueryFoundTableWidget::createSearchedItems(const MusicSearchedItem &songItem)
{
    int count = rowCount();
    setRowCount(count + 1);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(songItem.m_artistname + " - " + songItem.m_songname);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 440));
    setItem(count, 1, item);

                      item = new QTableWidgetItem(songItem.m_time);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/tiny/lb_server_type")));
    item->setToolTip(songItem.m_type);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_play")));
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_add")));
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_download")));
    setItem(count, 6, item);

    setFixedHeight(rowHeight(0)*rowCount());
}

void MusicQueryFoundTableWidget::addSearchMusicToPlayList(int row, bool play)
{
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row >= musicSongInfos.count())
    {
        return;
    }

    downloadDataFrom(musicSongInfos[row], play);
}

bool MusicQueryFoundTableWidget::downloadDataFrom(const MusicObject::MusicSongInfomation &downloadInfo, bool play)
{
    if(!M_NETWORK_PTR->isOnline())
    {
        return false;
    }

    QList<int> findMinTag;
    foreach(const MusicObject::MusicSongAttribute &attr, downloadInfo.m_songAttrs)
    {
        findMinTag << attr.m_bitrate;
    }
    qSort(findMinTag);
    //to find out the min bitrate

    foreach(const MusicObject::MusicSongAttribute &attr, downloadInfo.m_songAttrs)
    {
        if(attr.m_bitrate == findMinTag.first())
        {
            QString musicEnSong = MusicCryptographicHash::encryptData(downloadInfo.m_singerName + " - " + downloadInfo.m_songName, DOWNLOAD_KEY);
            QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(attr.m_format);

            MusicSemaphoreLoop loop(this);
            MusicDataDownloadThread *downSong = new MusicDataDownloadThread( attr.m_url, downloadName,
                                                                             MusicDownLoadThreadAbstract::Download_Music, this);
            connect(downSong, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
            downSong->startToDownload();
            loop.exec();

            emit muiscSongToPlayListChanged(musicEnSong, downloadInfo.m_timeLength, attr.m_format, play);
            return true;
        }
    }
    return false;
}
