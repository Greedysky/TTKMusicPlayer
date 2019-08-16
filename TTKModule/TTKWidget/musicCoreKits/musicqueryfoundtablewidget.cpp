#include "musicqueryfoundtablewidget.h"
#include "musicdatadownloadthread.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicsemaphoreloop.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicsongssummariziedwidget.h"
#include "musicrightareawidget.h"
#include "musicdownloadbatchwidget.h"

MusicQueryFoundTableWidget::MusicQueryFoundTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(8);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 342);
    headerview->resizeSection(2, 110);
    headerview->resizeSection(3, 47);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);
    headerview->resizeSection(6, 26);
    headerview->resizeSection(7, 26);

    m_labelDelegate = new MusicLabelDelegate(this);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicQueryFoundTableWidget::~MusicQueryFoundTableWidget()
{
    delete m_labelDelegate;
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllItems();
}

void MusicQueryFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryTableWidget::setQueryInput(query);
    connect(query, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
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
    m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void MusicQueryFoundTableWidget::musicDownloadLocal(int row)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicQueryFoundTableWidget::downloadDataFrom(bool play)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    const MIntList &list = getSelectedItems();
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    for(int i=0; i<list.count(); ++i)
    {
        if(downloadDataFrom(musicSongInfos[ list[i] ], play && (i == 0)))
        {
            continue;
        }
    }
}

void MusicQueryFoundTableWidget::downloadBatchData(bool music)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    const MIntList &list = getSelectedItems();
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    MusicObject::MusicSongInformations selectedItems;
    foreach(int index, list)
    {
        if(index < 0 || index >= musicSongInfos.count())
        {
            continue;
        }

        selectedItems << musicSongInfos[index];
    }

    MusicDownloadBatchWidget *w = new MusicDownloadBatchWidget(this);
    w->setSongName(selectedItems, music ? MusicDownLoadQueryThreadAbstract::MusicQuery : MusicDownLoadQueryThreadAbstract::MovieQuery);
    w->show();
}

void MusicQueryFoundTableWidget::resizeWindow()
{
    const int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.5 + 342);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.5 + 110);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));

        it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    }
}

void MusicQueryFoundTableWidget::searchChanged(QAction *action)
{
    const int row = currentRow();
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    const MusicObject::MusicSongInformation &info = musicSongInfos[row];
    switch( action->data().toInt() )
    {
        case 0: addSearchMusicToPlaylist(row, true); break;
        case 1: addSearchMusicToPlaylist(row, false); break;
        case 2: musicDownloadLocal(row); break;
        case 3: MusicRightAreaWidget::instance()->musicArtistFound(info.m_singerName, info.m_artistId); break;
        case 4: MusicRightAreaWidget::instance()->musicSongSearchedFound(info.m_songName); break;
        case 5: MusicRightAreaWidget::instance()->musicAlbumFound(info.m_albumName, info.m_albumId); break;
        case 6: MusicRightAreaWidget::instance()->musicSongSearchedFound(item(row, 1)->toolTip()); break;
        default: break;
    }
}

void MusicQueryFoundTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryTableWidget::resizeEvent(event);
    resizeWindow();
}

void MusicQueryFoundTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);

    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);

    const int row = currentRow();
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"))->setData(0);
    menu.addAction(tr("musicAdd"))->setData(1);
    menu.addAction(tr("downloadMore..."))->setData(2);

    menu.addSeparator();

    const MusicObject::MusicSongInformation &info = musicSongInfos[row];
    menu.addAction(tr("search '%1'").arg(info.m_singerName))->setData(3);
    menu.addAction(tr("search '%1'").arg(info.m_songName))->setData(4);
    menu.addAction(tr("search '%1'").arg(info.m_albumName))->setData(5);
    menu.addAction(tr("search '%1 - %2'").arg(info.m_singerName).arg(info.m_songName))->setData(6);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(searchChanged(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicQueryFoundTableWidget::listCellEntered(int row, int column)
{
    MusicQueryTableWidget::listCellEntered(row, column);
    if(column == 5 || column == 6 || column == 7)
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
        case 5:
            addSearchMusicToPlaylist(row, true);
            break;
        case 6:
            addSearchMusicToPlaylist(row, false);
            break;
        case 7:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }
}

void MusicQueryFoundTableWidget::clearAllItems()
{
    if(rowCount() > 0)
    {
        setItemDelegateForRow(rowCount() - 1, nullptr);
    }
    MusicQueryTableWidget::clear();
    setColumnCount(8);
}

void MusicQueryFoundTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(songItem.m_singerName + " - " + songItem.m_songName);
    item->setTextColor(QColor(100, 100, 100));
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(songItem.m_albumName);
    item->setTextColor(QColor(100, 100, 100));
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    setItem(count, 2, item);

                      item = new QTableWidgetItem(songItem.m_time);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setTextColor(QColor(100, 100, 100));
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(songItem.m_type);
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_play"));
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_add"));
    setItem(count, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_download"));
    setItem(count, 7, item);

    setFixedHeight(rowHeight(0)*rowCount());
}

void MusicQueryFoundTableWidget::createFinishedItem()
{
    setRowCount(rowCount() + 1);
    const int count = rowCount() - 1;
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

        setFixedHeight(rowHeight(0)*rowCount());
    }
}

void MusicQueryFoundTableWidget::addSearchMusicToPlaylist(int row, bool play)
{
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row >= musicSongInfos.count())
    {
        return;
    }

    downloadDataFrom(musicSongInfos[row], play);
}

bool MusicQueryFoundTableWidget::downloadDataFrom(const MusicObject::MusicSongInformation &downloadInfo, bool play)
{
    if(!M_NETWORK_PTR->isOnline())
    {
        return false;
    }

    MusicObject::MusicSongAttributes attrs(downloadInfo.m_songAttrs);
    qSort(attrs);
    //to find out the min bitrate

    if(!attrs.isEmpty())
    {
        const MusicObject::MusicSongAttribute &attr = attrs.first();
        const QString &musicEnSong = MusicUtils::Algorithm::mdII(downloadInfo.m_singerName + " - " + downloadInfo.m_songName, ALG_DOWNLOAD_KEY, true);
        const QString &downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(attr.m_format);

        MusicSemaphoreLoop loop(this);
        MusicDataDownloadThread *download = new MusicDataDownloadThread(attr.m_url, downloadName, MusicObject::DownloadMusic, this);
        connect(download, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        download->startToDownload();
        loop.exec();

        emit musicSongToPlaylistChanged(musicEnSong, downloadInfo.m_timeLength, attr.m_format, play);
    }

    return true;
}
