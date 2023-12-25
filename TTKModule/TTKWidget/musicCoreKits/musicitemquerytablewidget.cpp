#include "musicitemquerytablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicdownloadwidget.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musicrightareawidget.h"
#include "musicdownloadbatchwidget.h"

MusicItemQueryTableWidget::MusicItemQueryTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(8);

    m_labelDelegate = new TTKLabelItemDelegate(this);
    m_labelDelegate->setStyleSheet(TTK::UI::BackgroundStyle13);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 342);
    headerview->resizeSection(2, 110);
    headerview->resizeSection(3, 47);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);
    headerview->resizeSection(6, 26);
    headerview->resizeSection(7, 26);

    G_CONNECTION_PTR->setValue(MusicQueryTableWidget::className(), this);
    G_CONNECTION_PTR->connect(MusicQueryTableWidget::className(), MusicSongsSummariziedWidget::className());
}

MusicItemQueryTableWidget::~MusicItemQueryTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    delete m_labelDelegate;
    removeItems();
}

void MusicItemQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicQueryTableWidget::setQueryInput(query);
    connect(query, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
}

void MusicItemQueryTableWidget::startSearchQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())
    {
        removeItems();
        return;
    }

    m_networkRequest->setQueryType(MusicAbstractQueryRequest::QueryType::Music);
    m_networkRequest->startToSearch(text);
}

void MusicItemQueryTableWidget::downloadQueryResult(int row)
{
    if(!isValid(row))
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(m_networkRequest, row);
    download->show();
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
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    const TTKIntList &list = checkedIndexList();
    if(list.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    for(int i = 0; i < list.count(); ++i)
    {
        if(downloadDataFrom(songInfos[list[i]], play && (i == 0)))
        {
            continue;
        }
    }
}

void MusicItemQueryTableWidget::downloadBatchData()
{
    const TTKIntList &list = checkedIndexList();
    if(list.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    MusicDownloadBatchWidget w;
    w.setSongName(m_networkRequest, list);
    w.exec();
}

void MusicItemQueryTableWidget::resizeSection()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, 342 + (width - WINDOW_WIDTH_MIN) / 2.0);
    headerview->resizeSection(2, 110 + (width - WINDOW_WIDTH_MIN) / 2.0);

    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));

        it = item(i, 2);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    }
}

void MusicItemQueryTableWidget::menuActionChanged(QAction *action)
{
    const int row = currentRow();
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
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
        case 3: MusicRightAreaWidget::instance()->showArtistFound(info.m_singerName, info.m_artistId); break;
        case 4: MusicRightAreaWidget::instance()->showSongSearchedFound(info.m_songName); break;
        case 5: MusicRightAreaWidget::instance()->showAlbumFound(info.m_albumName, info.m_albumId); break;
        case 6: MusicRightAreaWidget::instance()->showSongSearchedFound(item(row, 1)->toolTip()); break;
        default: break;
    }
}

void MusicItemQueryTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryTableWidget::resizeEvent(event);
    resizeSection();
}

void MusicItemQueryTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);

    QMenu menu;
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    const int row = currentRow();
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(!isValid(row) || row >= songInfos.count())
    {
        return;
    }

    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"))->setData(0);
    menu.addAction(tr("Add to list"))->setData(1);
    menu.addAction(tr("Download More..."))->setData(2);

    menu.addSeparator();

    const TTK::MusicSongInformation &info = songInfos[row];
    menu.addAction(tr("Search '%1'").arg(info.m_singerName))->setData(3);
    menu.addAction(tr("Search '%1'").arg(info.m_songName))->setData(4);
    menu.addAction(tr("Search '%1'").arg(info.m_albumName))->setData(5);
    menu.addAction(tr("Search '%1 - %2'").arg(info.m_singerName, info.m_songName))->setData(6);
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
    if(rowCount() > 0)
    {
        setItemDelegateForRow(rowCount() - 1, nullptr);
    }

    MusicQueryTableWidget::removeItems();
    setColumnCount(8);
}

void MusicItemQueryTableWidget::createSearchedItem(const MusicResultInfoItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName + " - " + songItem.m_songName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_albumName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 2, item);

                      item = new QTableWidgetItem(songItem.m_duration);
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
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

    setFixedHeight(rowHeight(0) * rowCount());
}

void MusicItemQueryTableWidget::createFinishedItem()
{
    setRowCount(rowCount() + 1);
    const int count = rowCount() - 1;
    for(int i = 0; i < columnCount(); ++i)
    {
        setItem(count, i, new QTableWidgetItem);
    }
    setSpan(count, 0, 1, columnCount());

    QTableWidgetItem *it = item(count, 0);
    if(it)
    {
        it->setData(TTK_DISPLAY_ROLE, tr("No more data"));
        setItemDelegateForRow(count, m_labelDelegate);

        setFixedHeight(rowHeight(0) * rowCount());
    }
}

void MusicItemQueryTableWidget::addSearchMusicToPlaylist(int row, bool play)
{
    if(!isValid(row))
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(row >= songInfos.count())
    {
        return;
    }

    downloadDataFrom(songInfos[row], play);
}

bool MusicItemQueryTableWidget::downloadDataFrom(const TTK::MusicSongInformation &info, bool play)
{
    if(!G_NETWORK_PTR->isOnline())
    {
        return false;
    }

    TTK::MusicSongInformation songInfo;
    songInfo.m_songId = info.m_songId;
    m_networkRequest->startToQueryResult(&songInfo, TTK_BN_128);

    if(!songInfo.m_songProps.isEmpty())
    {
        const TTK::MusicSongProperty &prop = songInfo.m_songProps.front();

        MusicResultDataItem result;
        result.m_name = info.m_singerName + " - " + info.m_songName;
        result.m_updateTime = info.m_duration;
        result.m_id = info.m_songId;
        result.m_nickName = prop.m_url;
        result.m_description = prop.m_format;
        result.m_playCount = prop.m_size;
        result.m_tags = play ? MUSIC_PLAY_NOW : MUSIC_PLAY_LATER;

        if(m_networkRequest)
        {
            result.m_id = m_networkRequest->queryServer() + result.m_id;
        }
        Q_EMIT songBufferToPlaylist(result);
    }

    return true;
}
