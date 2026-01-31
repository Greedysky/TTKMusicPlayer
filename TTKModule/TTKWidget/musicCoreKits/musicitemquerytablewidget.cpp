#include "musicitemquerytablewidget.h"
#include "musicsongscontainerwidget.h"
#include "musicdownloadbatchwidget.h"
#include "musictoastlabel.h"
#include "musicrightareawidget.h"

MusicItemQueryTableWidget::MusicItemQueryTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(8);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 342);
    headerView->resizeSection(2, 110);
    headerView->resizeSection(3, 47);
    headerView->resizeSection(4, 26);
    headerView->resizeSection(5, 26);
    headerView->resizeSection(6, 26);
    headerView->resizeSection(7, 26);
}

MusicItemQueryTableWidget::~MusicItemQueryTableWidget()
{
    removeItems();
}

void MusicItemQueryTableWidget::startToSearchByValue(const QString &value)
{
    if(!G_NETWORK_PTR->isOnline())
    {
        removeItems();
        return;
    }

    m_networkRequest->startToSearch(value);
}

void MusicItemQueryTableWidget::downloadQueryResult(int row)
{
    if(!isValid(row))
    {
        return;
    }

    MusicDownloadWidget *widget = new MusicDownloadWidget(this);
    widget->initialize(m_networkRequest, row);
    widget->show();
}

void MusicItemQueryTableWidget::resizeGeometry()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(1, 342 + (width - WINDOW_WIDTH_MIN) / 2.0);
    headerView->resizeSection(2, 110 + (width - WINDOW_WIDTH_MIN) / 2.0);

    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 31));

        it = item(i, 2);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 31));
    }
}

void MusicItemQueryTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || row >= rowCount() - 1)
    {
        return;
    }

    addSearchMusicToPlaylist(row, true);
}

void MusicItemQueryTableWidget::downloadDataFrom(bool play)
{
    TTK::MusicSongInformationList songInfos(m_networkRequest->items());
    const TTKIntList &rows = checkedItemRows();
    if(rows.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    for(int i = 0; i < rows.count(); ++i)
    {
        const int row = rows[i];
        if(row >= songInfos.count())
        {
            continue;
        }

        downloadDataFrom(&songInfos[row], play && (i == 0 /* first item row */));
    }
}

void MusicItemQueryTableWidget::downloadBatchData()
{
    const TTKIntList &rows = checkedItemRows();
    if(rows.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    MusicDownloadBatchWidget widget;
    widget.initialize(m_networkRequest, rows);
    widget.exec();
}

void MusicItemQueryTableWidget::menuActionChanged(QAction *action)
{
    const int row = currentRow();
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    if(!isValid(row) || row >= songInfos.count())
    {
        return;
    }

    const TTK::MusicSongInformation &info = songInfos[row];
    switch(action->data().toInt())
    {
        case 0: addSearchMusicToPlaylist(row, true); break;
        case 1: addSearchMusicToPlaylist(row, false); break;
        case 2: downloadQueryResult(row); break;
        case 3: MusicRightAreaWidget::instance()->showArtistFound(info.m_artistName, info.m_artistId); break;
        case 4: MusicRightAreaWidget::instance()->showSongSearchedFound(info.m_songName); break;
        case 5: MusicRightAreaWidget::instance()->showAlbumFound(info.m_albumName, info.m_albumId); break;
        case 6: MusicRightAreaWidget::instance()->showSongSearchedFound(item(row, 1)->toolTip()); break;
        default: break;
    }
}

void MusicItemQueryTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);

    QMenu menu;
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    const int row = currentRow();
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    if(!isValid(row) || row >= songInfos.count())
    {
        return;
    }

    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"))->setData(0);
    menu.addAction(tr("Add to list"))->setData(1);
    menu.addAction(tr("Download More..."))->setData(2);
    menu.addSeparator();

    const TTK::MusicSongInformation &info = songInfos[row];
    menu.addAction(tr("Search '%1'").arg(info.m_artistName))->setData(3);
    menu.addAction(tr("Search '%1'").arg(info.m_songName))->setData(4);
    menu.addAction(tr("Search '%1'").arg(info.m_albumName))->setData(5);
    menu.addAction(tr("Search '%1 - %2'").arg(info.m_artistName, info.m_songName))->setData(6);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(menuActionChanged(QAction*)));
    menu.exec(QCursor::pos());
}

void MusicItemQueryTableWidget::itemCellEntered(int row, int column)
{
    MusicQueryTableWidget::itemCellEntered(row, column);

    if(column == 5 || column == 6 || column == 7)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
}

void MusicItemQueryTableWidget::itemCellClicked(int row, int column)
{
    MusicQueryTableWidget::itemCellClicked(row, column);

    switch(column)
    {
        case 5: addSearchMusicToPlaylist(row, true); break;
        case 6: addSearchMusicToPlaylist(row, false); break;
        case 7: downloadQueryResult(row); break;
        default: break;
    }
}

void MusicItemQueryTableWidget::removeItems()
{
    MusicQueryTableWidget::removeItems();
    setColumnCount(8);
}

void MusicItemQueryTableWidget::createResultItem(const MusicResultInfoItem &result)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(TTK::generateSongName(result.m_songName, result.m_artistName));
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 31));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(result.m_albumName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 31));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 2, item);

                      item = new QTableWidgetItem(result.m_duration);
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(result.m_type);
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

    setFixedHeight(rowHeight(0) * (rowCount() + 1));
}

void MusicItemQueryTableWidget::addSearchMusicToPlaylist(int row, bool play)
{
    if(!isValid(row))
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    TTK::MusicSongInformationList songInfos(m_networkRequest->items());
    if(row >= songInfos.count())
    {
        return;
    }

    downloadDataFrom(&songInfos[row], play);
}

bool MusicItemQueryTableWidget::downloadDataFrom(TTK::MusicSongInformation *info, bool play)
{
    if(!G_NETWORK_PTR->isOnline())
    {
        MusicToastLabel::popup(tr("No resource found"));
        return false;
    }

    m_networkRequest->startToQueryResult(info, TTK_BN_128);

    if(info->m_songProps.isEmpty())
    {
        MusicToastLabel::popup(tr("No resource found"));
        return false;
    }

    const TTK::MusicSongProperty &prop = info->m_songProps.first();

    MusicResultDataItem item;
    item.m_name = TTK::generateSongName(info->m_songName, info->m_artistName);
    item.m_time = info->m_duration;
    item.m_id = info->m_songId;
    item.m_nickName = prop.m_url;
    item.m_description = prop.m_format;
    item.m_count = prop.m_size;
    item.m_title = play ? MUSIC_PLAY_NOW : MUSIC_PLAY_LATER;

    if(m_networkRequest)
    {
        item.m_id = m_networkRequest->queryServer() + item.m_id;
    }

    MusicSongsContainerWidget::instance()->addSongBufferToPlaylist(item);
    return true;
}
