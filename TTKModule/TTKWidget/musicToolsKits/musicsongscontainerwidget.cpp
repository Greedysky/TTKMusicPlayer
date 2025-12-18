#include "musicsongscontainerwidget.h"
#include "musicsongslistfunctionwidget.h"
#include "musicsongslistplaytablewidget.h"
#include "musicsongsearchdialog.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musicsongchecktoolswidget.h"
#include "musicplayedlistpopwidget.h"
#include "musiclrcdownloadbatchwidget.h"
#include "musicapplication.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "musicformats.h"

#include <QMimeData>

static constexpr int ITEM_MIN_COUNT = MIN_ITEM_COUNT;
static constexpr int ITEM_MAX_COUNT = 10;
static constexpr int RECENT_ITEM_MAX_COUNT = 50;

MusicSongsContainerWidget *MusicSongsContainerWidget::m_instance = nullptr;

MusicSongsContainerWidget::MusicSongsContainerWidget(QWidget *parent)
    : MusicSongsToolBoxWidget(parent),
      MusicItemSearchInterfaceClass(),
      m_playRowIndex(MUSIC_NORMAL_LIST),
      m_lastSearchIndex(MUSIC_NORMAL_LIST),
      m_selectDeleteIndex(MUSIC_NONE_LIST),
      m_functionWidget(nullptr),
      m_songSearchWidget(nullptr)
{
    m_instance = this;

    m_topMaskWidget = new MusicSongsToolBoxMaskWidget(this);
    setInputModule(m_topMaskWidget);
    setAcceptDrops(true);

    connect(m_topMaskWidget, SIGNAL(itemIndexChanged(int)), SLOT(itemIndexChanged(int)));
    connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(sliderValueChanaged(int)));
}

MusicSongsContainerWidget::~MusicSongsContainerWidget()
{
    delete m_topMaskWidget;
    delete m_functionWidget;
    delete m_songSearchWidget;

    while(!m_containerItems.isEmpty())
    {
        delete m_containerItems.takeLast().m_itemWidget;
    }
}

MusicSongsContainerWidget *MusicSongsContainerWidget::instance()
{
    return m_instance;
}

void MusicSongsContainerWidget::updateSongItem(const MusicSongItem &item)
{
    int index = -1;
    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        if(item.m_id == m_containerItems[i].m_id)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        appendSongItemList({item});
    }
    else
    {
        MusicSongItem *it = &m_containerItems[index];
        it->m_sort = item.m_sort;
        it->m_songs = item.m_songs;
        it->m_itemName = item.m_itemName;
        it->m_itemWidget->removeItems();
        it->m_itemWidget->updateSongsList(it->m_songs);
        setItemTitle(it);

        if(foundMappedIndex(it->m_itemIndex) == m_playRowIndex)
        {
            MusicPlayedListPopWidget::instance()->clear();
            MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
        }
    }
}

bool MusicSongsContainerWidget::addSongItemList(const MusicSongItemList &items)
{
    TTKIntSet inNeed;
    inNeed << MUSIC_NORMAL_LIST << MUSIC_LOVEST_LIST << MUSIC_NETWORK_LIST << MUSIC_RECENT_LIST;
    for(const MusicSongItem &item : qAsConst(items))
    {
        inNeed.remove(item.m_itemIndex);
    }
    //
    if(!inNeed.isEmpty())
    {
        //if less than four count(0, 1, 2, 3), find and add default items
        m_containerItems << items;
        //
        for(const int item : qAsConst(inNeed))
        {
            MusicSongItem it;
            switch(item)
            {
                case MUSIC_NORMAL_LIST:
                {
                    it.m_itemIndex = item;
                    it.m_itemName = tr("Default Item");
                    break;
                }
                case MUSIC_LOVEST_LIST:
                {
                    it.m_itemIndex = item;
                    it.m_itemName = tr("Lovest Item");
                    break;
                }
                case MUSIC_NETWORK_LIST:
                {
                    it.m_itemIndex = item;
                    it.m_itemName = tr("Net Item");
                    break;
                }
                case MUSIC_RECENT_LIST:
                {
                    it.m_itemIndex = item;
                    it.m_itemName = tr("Recent Item");
                    break;
                }
                default: break;
            }
            m_containerItems << it;
        }
        std::sort(m_containerItems.begin(), m_containerItems.end());
    }
    else
    {
        m_containerItems = items;
    }

    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        createWidgetItem(&m_containerItems[i]);
    }

    return inNeed.isEmpty();
}

void MusicSongsContainerWidget::appendSongItemList(const MusicSongItemList &items)
{
    for(int i = 0; i < items.count(); ++i)
    {
        MusicSongItem item = items[i];
        checkTitleNameValid(item.m_itemName);
        createContainerItem(item);
    }
}

void MusicSongsContainerWidget::importSongsByUrl(const QString &path, int playlistRow)
{
    if(path.isEmpty())
    {
        return;
    }

    if(playlistRow < 0)
    {
        playlistRow = makeValidIndex();
    }

    const QFileInfo fin(path);
    if(fin.isDir())
    {
        QStringList files;
        for(const QFileInfo &fin : TTK::File::fileInfoListByPath(path))
        {
            if(MusicFormats::supportMusicFormats().contains(TTK_FILE_SUFFIX(fin)))
            {
               files << fin.absoluteFilePath();
            }
        }

        importSongsByPath(files, playlistRow);
    }
    else if(TTK::String::isNetworkUrl(path))
    {
        closeSearchWidgetInNeed();

        MusicResultDataItem item;
        item.m_name = TTK::String::splitToken(path, "?", TTK_SEPARATOR, false);
        item.m_id = item.m_name;
        item.m_nickName = path;
        item.m_description = TTK::String::splitToken(path);
        item.m_title = MUSIC_PLAY_LATER;

        const qint64 size = TTK::fetchFileSizeByUrl(path);
        item.m_count = QString::number(size);

        if(item.m_description == MP3_FILE_SUFFIX)
        {
            item.m_time = TTKTime::formatDuration(size * 8 / TTK_BN_128);
        }
        else if(item.m_description == APE_FILE_SUFFIX || item.m_description == FLAC_FILE_SUFFIX)
        {
            item.m_time = TTKTime::formatDuration(size * 8 / TTK_BN_1000);
        }

        addSongBufferToPlaylist(item);
    }
    else
    {
        QStringList files;
        if(MusicFormats::supportMusicFormats().contains(TTK_FILE_SUFFIX(fin)))
        {
           files << fin.absoluteFilePath();
        }

        importSongsByPath(files, playlistRow);
    }
}

void MusicSongsContainerWidget::importSongsByPath(const QStringList &files, int playlistRow)
{
    if(files.isEmpty())
    {
        return;
    }

    MusicProgressWidget progress;
    progress.setTitle(tr("Import file mode"));
    progress.setRange(0, files.count());
    progress.show();

    importSongsWithProgress(&progress, 0, files, playlistRow);

    MusicToastLabel::popup(tr("Import music songs done"));
}

QStringList MusicSongsContainerWidget::songsFileName(int index) const
{
    QStringList list;

    if(index < 0 || index >= m_containerItems.count())
    {
        return list;
    }

    for(const MusicSong &song : qAsConst(m_containerItems[index].m_songs))
    {
        list << song.name();
    }
    return list;
}

QStringList MusicSongsContainerWidget::songsFilePath(int index) const
{
    QStringList list;
    if(index < 0 || index >= m_containerItems.count())
    {
        return list;
    }

    for(const MusicSong &song : qAsConst(m_containerItems[index].m_songs))
    {
        list << song.path();
    }
    return list;
}

int MusicSongsContainerWidget::mapSongIndexByFilePath(int playlistRow, const QString &path) const
{
    if(playlistRow < 0 || playlistRow >= m_containerItems.count() || path.isEmpty())
    {
        return -1;
    }

    const MusicSongList *songs = &m_containerItems[playlistRow].m_songs;
    for(int i = 0; i < songs->count(); ++i)
    {
        if(MusicSong(path) == songs->at(i))
        {
            return i;
        }
    }
    return -1;
}

QString MusicSongsContainerWidget::mapFilePathBySongIndex(int playlistRow, int index) const
{
    if(playlistRow < 0 || playlistRow >= m_containerItems.count())
    {
        return {};
    }

    const MusicSongList *songs = &m_containerItems[playlistRow].m_songs;
    if(index < 0 || index >= songs->count())
    {
        return {};
    }

    return songs->at(index).path();
}

void MusicSongsContainerWidget::removeSearchResult(int &row)
{
    if(!hasSearchResult() || !isSearchedPlayIndex())
    {
        return;
    }

    const TTKIntList &result = m_searchResultItems.value(m_searchResultLevel);
    if(row >= result.count() || row < 0)
    {
        row = -1;
        return;
    }

    row = result[row];

    clearSearchResult();
    closeSearchWidget();
}

void MusicSongsContainerWidget::setPlayRowIndex(int index)
{
    const int before = m_playRowIndex;
    m_playRowIndex = index;

    if(before >= 0)
    {
        MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[before].m_itemWidget);
        if(widget && !m_containerItems[before].m_songs.isEmpty())
        {
            widget->adjustPlayWidgetRow();
        }
    }
}

void MusicSongsContainerWidget::playLocation(int index)
{
    if(!hasSearchResult())
    {
        selectRow(index);
        resizeScrollIndex(index * 30);
    }
}

void MusicSongsContainerWidget::selectRow(int index)
{
    setCurrentIndex(m_playRowIndex);
    if(m_playRowIndex < 0)
    {
        return;
    }

    closeSearchWidgetInNeed();
    m_containerItems[m_playRowIndex].m_itemWidget->selectRow(index);
}

void MusicSongsContainerWidget::updateDurationLabel(const QString &current, const QString &total) const
{
    if(m_playRowIndex < 0)
    {
        return;
    }
    TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[m_playRowIndex].m_itemWidget)->updateDurationLabel(current, total);
}

void MusicSongsContainerWidget::addNewRowItem()
{
    if(m_containerItems.count() <= ITEM_MAX_COUNT)
    {
        int id = 0;
        for(const MusicSongItem &item : qAsConst(m_containerItems))
        {
            if(id < item.m_id)
            {
               id = item.m_id;
            }
        }

        MusicSongItem item;
        item.m_id = id + 1;
        item.m_itemName = tr("Default Item");

        checkTitleNameValid(item.m_itemName);
        createContainerItem(item);
    }
    else
    {
        MusicToastLabel::popup(tr("Exceeded the maximum number limit"));
    }
}

void MusicSongsContainerWidget::deleteRowItem(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec())
    {
        return;
    }

    const MusicSongItem *item = &m_containerItems[id];
    MusicPlayedListPopWidget::instance()->remove(item->m_itemIndex, item->m_songs);

    if(m_playRowIndex == id)
    {
        setCurrentIndex(MUSIC_NORMAL_LIST);
        m_items.first().m_itemWidget->setExpand(false);
        MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
    }
    else if(m_playRowIndex > id)
    {
        setCurrentIndex(--m_playRowIndex);
    }

    m_containerItems.takeAt(id);
    removeItem(item->m_itemWidget);
    delete item->m_itemWidget;

    updatePlayedRows(id, -1);
}

void MusicSongsContainerWidget::deleteRowItems()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec())
    {
        return;
    }

    if(m_playRowIndex != MUSIC_NORMAL_LIST && TTK::playlistRowValid(m_playRowIndex))
    {
        setCurrentIndex(MUSIC_NORMAL_LIST);
        m_items.first().m_itemWidget->setExpand(false);
        MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
    }

    for(int i = m_containerItems.count() - 1; i > 3; --i)
    {
        MusicSongItem item = m_containerItems.takeLast();
        removeItem(item.m_itemWidget);
        delete item.m_itemWidget;
    }
}

void MusicSongsContainerWidget::deleteAllItems(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    closeSearchWidgetInNeed();

    m_selectDeleteIndex = id;
    //
    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[id].m_itemWidget);
    if(widget->rowCount() > 0)
    {
        widget->setCurrentCell(0, 1);
        widget->clearItems();
    }
    //
    m_selectDeleteIndex = MUSIC_NONE_LIST;

    if(m_containerItems[id].m_songs.isEmpty() && m_playRowIndex == id)
    {
        MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
    }
}

void MusicSongsContainerWidget::changRowItemName(int index, const QString &name)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    bool found = false;
    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        if(i == id)
        {
            continue;
        }

        if(m_containerItems[i].m_itemName == name)
        {
            found = true;
        }
    }

    MusicSongItem *item = &m_containerItems[id];
    if(found)
    {
        MusicToastLabel::popup(tr("Item name is already exists"));
        setItemTitle(item);
        return;
    }

    item->m_itemName = name;
    setItemTitle(item);
}

void MusicSongsContainerWidget::swapDragItemIndex(int start, int end)
{
    start = foundMappedIndex(start);
    end = foundMappedIndex(end);
    if(start == end)
    {
        return;
    }

    //adjust the m_playRowIndex while the item has dragged and dropped
    if(start < end)
    {
        if((start < m_playRowIndex && m_playRowIndex < end) || m_playRowIndex == end)
        {
            m_currentIndex = --m_playRowIndex;
        }
        else if(m_playRowIndex == start)
        {
            m_currentIndex = m_playRowIndex = end;
        }
    }
    else
    {
        if((end < m_playRowIndex && m_playRowIndex < start) || m_playRowIndex == end)
        {
            m_currentIndex = ++m_playRowIndex;
        }
        else if(m_playRowIndex == start)
        {
            m_currentIndex = m_playRowIndex = end;
        }
    }

    swapItem(start, end);
    const MusicSongItem &item = m_containerItems.takeAt(start);
    m_containerItems.insert(end, item);

    updatePlayedRows(start, end);
}

void MusicSongsContainerWidget::addToPlayLater(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    const MusicSongItem *item = &m_containerItems[id];
    MusicPlayedListPopWidget::instance()->insert(item->m_itemIndex, item->m_songs);
}

void MusicSongsContainerWidget::addToPlayedList(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    const MusicSongItem *item = &m_containerItems[id];
    MusicPlayedListPopWidget::instance()->append(item->m_itemIndex, item->m_songs);
}

void MusicSongsContainerWidget::importSongsByFiles(int index)
{
    if(index == TTK_LOW_LEVEL)
    {
        index = m_currentIndex;
    }
    else
    {
        const int id = foundMappedIndex(index);
        if(id == -1)
        {
            return;
        }

        index = id;
    }

    importSongsByPath(TTK::File::getOpenFileNames(this, MusicFormats::supportMusicInputFormats()), index);
}

void MusicSongsContainerWidget::importSongsByDir(int index)
{
    if(index == TTK_LOW_LEVEL)
    {
        index = m_currentIndex;
    }
    else
    {
        const int id = foundMappedIndex(index);
        if(id == -1)
        {
            return;
        }

        index = id;
    }

    const QString &path = TTK::File::getExistingDirectory(this);
    if(path.isEmpty())
    {
        return;
    }

    importSongsByUrl(path, index);
}

void MusicSongsContainerWidget::showSongCheckToolsWidget()
{
    TTK_GENERATE_SINGLE_WIDGET(MusicSongCheckToolsWidget);
}

void MusicSongsContainerWidget::showLrcDownloadBatchWidget()
{
    TTK_GENERATE_SINGLE_WIDGET(MusicLrcDownloadBatchWidget);
}

void MusicSongsContainerWidget::searchResultChanged(int, int column)
{
    if(m_currentIndex == -1)
    {
        return;
    }

    if(!isSearchedPlayIndex())
    {
        const QStringList searchedSongs(songsFileName(m_lastSearchIndex));
        TTKIntList result;
        for(int i = 0; i < searchedSongs.count(); ++i)
        {
            result << i;
        }

        MusicSongItem *item = &m_containerItems[m_lastSearchIndex];
        TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemWidget)->updateSearchFileName(&item->m_songs, result);

        if(item->m_songs.isEmpty())
        {
            item->m_itemWidget->updateSongsList(item->m_songs);
        }

        clearSearchResult();
        m_lastSearchIndex = m_currentIndex;
    }

    const QString &text = m_songSearchWidget->text();
    const QStringList searchedSongs(songsFileName(m_currentIndex));

    TTKIntList result;
    for(int i = 0; i < searchedSongs.count(); ++i)
    {
        if(searchedSongs[i].contains(text, Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    m_searchResultLevel = column;
    m_searchResultItems.insert(column, result);

    MusicSongItem *item = &m_containerItems[m_currentIndex];
    TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemWidget)->updateSearchFileName(&item->m_songs, result);

    if(column == 0)
    {
        if(item->m_songs.isEmpty())
        {
            item->m_itemWidget->updateSongsList(item->m_songs);
        }

        clearSearchResult();
    }
}

void MusicSongsContainerWidget::updateCurrentIndex()
{
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    if(lastPlayIndex.count() != 3)
    {
        return;
    }

    m_playRowIndex = lastPlayIndex[1].toInt();
    const int index = lastPlayIndex[2].toInt();
    setCurrentIndex(index);
    setSongPlayCount(index);

    MusicApplication::instance()->showCurrentSong();
}

void MusicSongsContainerWidget::songToLovestListAt(bool state, int row)
{
    if(m_playRowIndex < 0 || m_playRowIndex >= m_containerItems.count())
    {
        return;
    }

    const MusicSong &song = m_containerItems[m_playRowIndex].m_songs[row];
    MusicSongItem *item = &m_containerItems[MUSIC_LOVEST_LIST];

    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemWidget);
    if(state)    ///Add to lovest list
    {
        item->m_songs << song;
        widget->updateSongsList(item->m_songs);
        setItemTitle(item);
    }
    else        ///Remove to lovest list
    {
        if(item->m_songs.removeOne(song))
        {
            widget->removeItems();
            widget->updateSongsList(item->m_songs);
            setItemTitle(item);
            MusicApplication::instance()->removeLoveItemAt(song.path(), m_playRowIndex == MUSIC_LOVEST_LIST);
        }
    }
}

void MusicSongsContainerWidget::addSongToLovestList(bool state, int row)
{
    if(m_currentIndex < 0 || m_currentIndex >= m_containerItems.count() || hasSearchResult())
    {
        return;
    }

    const MusicSong &song = m_containerItems[m_currentIndex].m_songs[row];
    MusicSongItem *item = &m_containerItems[MUSIC_LOVEST_LIST];

    ///if current play list contains, call main add and remove function
    if(MusicSong(MusicApplication::instance()->currentFilePath()) == song)
    {
        MusicApplication::instance()->addSongToLovestList(state);
        return;
    }

    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemWidget);
    if(state)    ///Add to lovest list
    {
        item->m_songs << song;
        widget->updateSongsList(item->m_songs);
        setItemTitle(item);
    }
    else        ///Remove to lovest list
    {
        if(item->m_songs.removeOne(song))
        {
            widget->removeItems();
            widget->updateSongsList(item->m_songs);
            setItemTitle(item);
            MusicApplication::instance()->removeLoveItemAt(song.path(), m_playRowIndex == MUSIC_LOVEST_LIST);
        }
    }
}

void MusicSongsContainerWidget::addSongBufferToPlaylist(const MusicResultDataItem &item)
{
    MusicSongItem *it = &m_containerItems[MUSIC_NETWORK_LIST];
    const QByteArray &md5 = TTK::Algorithm::md5(item.m_id.toUtf8());
    MusicSong song(item.m_nickName + "#" + md5 + "." + item.m_description, item.m_time, item.m_name);
    song.setFormat(item.m_description);
    song.setSizeStr(item.m_count);

    int index = it->m_songs.indexOf(song);
    if(index == -1)
    {
        it->m_songs << song;
        it->m_itemWidget->updateSongsList(it->m_songs);
        setItemTitle(it);
        index = it->m_songs.count() - 1;

        // write song meta info to file
        TTK::generateNetworkSongMeta(md5, item);
    }

    if(item.m_title == MUSIC_PLAY_NOW)
    {
        ///when download finished just play it at once
        setCurrentIndex(MUSIC_NETWORK_LIST);
        MusicApplication::instance()->playIndexClicked(index, 0);
    }
}

void MusicSongsContainerWidget::addSongToPlaylist(const QStringList &items)
{
    if(items.isEmpty())
    {
        MusicToastLabel::popup(tr("Import music songs is empty"));
        return;
    }

    QStringList files(items);
    const int row = makeValidIndex();
    importSongsByPath(files, row);

    const MusicSongItem *item = &m_containerItems[row];
    const MusicSongList *musicSongs = &item->m_songs;
    const MusicSong &song = MusicSong(items.last());

    int index = musicSongs->count() - 1;
    if(musicSongs->contains(song))
    {
        index = musicSongs->indexOf(song);
    }

    /// just play it at once
    setCurrentIndex(row);
    MusicApplication::instance()->playIndexClicked(index, 0);
}

void MusicSongsContainerWidget::removeItemAt(const TTKIntList &rows, bool fileRemove)
{
    if(rows.isEmpty() || hasSearchResult())
    {
        return;
    }

    const int currentIndex = m_selectDeleteIndex != MUSIC_NONE_LIST ? m_selectDeleteIndex : m_currentIndex;
    MusicSongItem *item = &m_containerItems[currentIndex];
    QStringList deleteFiles;
    for(int i = rows.count() - 1; i >= 0; --i)
    {
        const MusicSong &song = item->m_songs.takeAt(rows[i]);
        deleteFiles << song.path();
        if(currentIndex != m_playRowIndex && currentIndex == MUSIC_LOVEST_LIST)
        {
            const int playIndex = m_containerItems[m_playRowIndex].m_itemWidget->playRowIndex();
            const MusicSongList &songs = m_containerItems[m_playRowIndex].m_songs;
            if(playIndex > -1 && playIndex < songs.count())
            {
                if(songs[playIndex] == song)
                {
                    MusicApplication::instance()->addSongToLovestList(false);
                }
            }
        }

        if(fileRemove)
        {
            if(currentIndex != MUSIC_NETWORK_LIST)
            {
                QFile::remove(song.path());
            }
            else
            {
                QFile::remove(TTK::generateNetworkSongPath(song.path()));
                QFile::remove(TTK::generateNetworkSongMetaPath(song.path()));
            }
        }
    }

    MusicApplication::instance()->removeItemAt(deleteFiles, fileRemove, currentIndex == m_playRowIndex, currentIndex);

    setItemTitle(item);
    //create upload file widget if current items is all been deleted
    TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemWidget)->createUploadFileModule();
}

void MusicSongsContainerWidget::itemRowSwaped(int start, int end, int play, MusicSongList &songs)
{
    MusicSongList *names = &m_containerItems[m_currentIndex].m_songs;
    if(start > end)
    {
        for(int i = start; i > end; --i)
        {
            QtContainerSwap(names, i, i - 1);
        }
    }
    else
    {
        for(int i = start; i < end; ++i)
        {
            QtContainerSwap(names, i, i + 1);
        }
    }

    songs = *names;

    if(m_currentIndex == m_playRowIndex)
    {
        MusicPlayedListPopWidget::instance()->selectCurrentMedia(m_currentIndex, songs[play]);
    }
}

void MusicSongsContainerWidget::isCurrentPlaylistRow(bool &state)
{
    const int currentIndex = m_selectDeleteIndex != MUSIC_NONE_LIST ? m_selectDeleteIndex : m_currentIndex;
    state = (currentIndex == m_playRowIndex);
}

void MusicSongsContainerWidget::isSearchedResultEmpty(bool &empty)
{
    empty = !hasSearchResult();
}

void MusicSongsContainerWidget::setSongPlayCount(int index)
{
    if(index < 0 || m_playRowIndex < 0)
    {
        return;
    }

    MusicSongList &songs = m_containerItems[m_playRowIndex].m_songs;
    if(!songs.isEmpty() && index < songs.count())
    {
        MusicSong &song = songs[index];
        song.setPlayCount(song.playCount() + 1);
    }
}

void MusicSongsContainerWidget::appendRecentSongs(int index)
{
    if(index < 0 || m_playRowIndex < MUSIC_NORMAL_LIST || m_playRowIndex == MUSIC_NETWORK_LIST || m_playRowIndex == MUSIC_RECENT_LIST)
    {
        return;
    }

    MusicSongList *songs = &m_containerItems[m_playRowIndex].m_songs;
    if(songs->isEmpty() || index >= songs->count())
    {
        return;
    }

    MusicSongItem *item = &m_containerItems[MUSIC_RECENT_LIST];
    MusicSong recentSong(songs->at(index));
    MusicSongList *recentSongs = &item->m_songs;
    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemWidget);
    if(!recentSongs->contains(recentSong))
    {
        if(recentSongs->count() >= RECENT_ITEM_MAX_COUNT)
        {
            recentSongs->takeFirst();
            widget->removeItems();
        }

        recentSong.setPlayCount(recentSong.playCount() + 1);
        recentSongs->append(recentSong);
        widget->updateSongsList(*recentSongs);

        const QString title(QString("%1[%2]").arg(item->m_itemName).arg(recentSongs->count()));
        setTitle(widget, title);
    }
    else
    {
        for(MusicSong &song : *recentSongs)
        {
            if(recentSong == song)
            {
                song.setPlayCount(song.playCount() + 1);
                break;
            }
        }
    }
}

void MusicSongsContainerWidget::querySongItemList(MusicSongItemList &songs)
{
    songs = m_containerItems;
}

void MusicSongsContainerWidget::updateCurrentArtist()
{
    if(m_playRowIndex < 0)
    {
        return;
    }

    TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[m_playRowIndex].m_itemWidget)->updateCurrentArtist();
}

void MusicSongsContainerWidget::showFloatWidget()
{
    if(m_functionWidget == nullptr)
    {
        m_functionWidget = new MusicSongsListFunctionWidget(this);
        connect(m_functionWidget, SIGNAL(deleteSelfObject()), SLOT(deleteFunctionWidget()));
        resizeWindow();
        m_functionWidget->show();
    }
    else
    {
        resizeWindow();
        m_functionWidget->active();
    }
}

void MusicSongsContainerWidget::songListSortBy(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    closeSearchWidgetInNeed();

    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[id].m_itemWidget);
    MusicSong::Sort sort = MusicSong::Sort::ByFileName;
    index = m_containerItems[id].m_sort.m_type;
    if(index != -1)
    {
        sort = TTKStaticCast(MusicSong::Sort, index);
    }

    MusicSongList *songs = &m_containerItems[id].m_songs;
    const MusicSong song(MusicApplication::instance()->currentFilePath());

    for(int i = 0; i < songs->count(); ++i)
    {
        (*songs)[i].setSort(sort);
    }

    if(m_containerItems[id].m_sort.m_order == Qt::DescendingOrder)
    {
        std::sort(songs->begin(), songs->end());
    }
    else
    {
        std::sort(songs->begin(), songs->end(), std::greater<MusicSong>());
    }

    widget->removeItems();
    widget->setSongsList(songs);

    index = songs->indexOf(song);
    if(m_currentIndex == m_playRowIndex)
    {
        MusicApplication::instance()->playSortBy(index);
    }
}

void MusicSongsContainerWidget::showSearchWidget()
{
    if(m_songSearchWidget == nullptr)
    {
        m_songSearchWidget = new MusicSongSearchDialog(this);
        resizeWindow();
    }

    m_songSearchWidget->setVisible(!m_songSearchWidget->isVisible());
}

void MusicSongsContainerWidget::sliderValueChanaged(int value)
{
    if(value >= 40 * (m_currentIndex + 1) && m_currentIndex > -1 && m_currentIndex < m_containerItems.count())
    {
        MusicSongItem *item = &m_containerItems[m_currentIndex];
        m_topMaskWidget->setIndex(item->m_itemIndex);
        m_topMaskWidget->setSongSort(&item->m_sort);
        m_topMaskWidget->setTitle(QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
        m_topMaskWidget->setExpand(true);
        m_topMaskWidget->raise();
        m_topMaskWidget->show();
    }
    else
    {
        m_topMaskWidget->hide();
    }
}

void MusicSongsContainerWidget::deleteFunctionWidget()
{
    delete m_functionWidget;
    m_functionWidget = nullptr;
}

void MusicSongsContainerWidget::resizeEvent(QResizeEvent *event)
{
    MusicSongsToolBoxWidget::resizeEvent(event);
    resizeWindow();
}

void MusicSongsContainerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    MusicSongsToolBoxWidget::dragEnterEvent(event);
    event->setDropAction(Qt::IgnoreAction);
    event->accept();
}

void MusicSongsContainerWidget::dragMoveEvent(QDragMoveEvent *event)
{
    MusicSongsToolBoxWidget::dragMoveEvent(event);

    for(const MusicToolBoxWidgetItem &item : qAsConst(m_items))
    {
        if(!TTK::playlistRowValid(item.m_itemIndex))
        {
            continue;
        }

        item.m_itemWidget->isActive();
    }

    event->setDropAction(Qt::CopyAction);
    event->accept();
}

void MusicSongsContainerWidget::dropEvent(QDropEvent *event)
{
    MusicSongsToolBoxWidget::dropEvent(event);

    QStringList dirs, files;
    const QMimeData *data = event->mimeData();

    for(const QUrl &url : data->urls())
    {
        const QString &path = url.toLocalFile();
        if(QFileInfo(path).isDir())
        {
            dirs << path;
        }
        else
        {
            files << path;
        }
    }

    bool contains = false;
    for(const MusicToolBoxWidgetItem &item : qAsConst(m_items))
    {
        if(!TTK::playlistRowValid(item.m_itemIndex))
        {
            continue;
        }

        if(item.m_itemWidget->isExpand() || item.m_itemWidget->isActive())
        {
            contains = true;
            importSongsByPath(files, foundMappedIndex(item.m_itemIndex));
            break;
        }
    }

    if(!contains)
    {
        importSongsByPath(files, m_playRowIndex);
    }

    if(!dirs.isEmpty())
    {
        int count = 0;
        QList<QStringList> files;

        for(const QString &dir : qAsConst(dirs))
        {
            files << TTK::File::fileListByPath(dir, MusicFormats::supportMusicInputFilterFormats());
            count += files.last().count();
        }

        MusicProgressWidget progress;
        progress.setTitle(tr("Import dir mode"));
        progress.setRange(0, count);
        progress.show();

        count = 0;
        for(int i = 0; i < dirs.count(); ++i)
        {
            MusicSongItem item;
            item.m_itemName = QFileInfo(dirs[i]).baseName();

            checkTitleNameValid(item.m_itemName);
            item = *createContainerItem(item);

            importSongsWithProgress(&progress, count, files[i], foundMappedIndex(item.m_itemIndex));

            count += files[i].count();
        }

        MusicToastLabel::popup(tr("Import music songs done"));
    }
}

void MusicSongsContainerWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsToolBoxWidget::contextMenuEvent(event);

    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);
    menu.addAction(tr("Create Item"), this, SLOT(addNewRowItem()));
    menu.addAction(tr("Import Item"), MusicApplication::instance(), SLOT(importSongsItemList()));
    menu.addAction(tr("Export Items"), MusicApplication::instance(), SLOT(exportSongsItemList()));
    menu.addAction(tr("Delete All"), this, SLOT(deleteRowItems()))->setEnabled(m_containerItems.count() > ITEM_MIN_COUNT);
    menu.addAction(tr("Music Test Tools"), this, SLOT(showSongCheckToolsWidget()));
    menu.addAction(tr("Lrc Batch Download"), this, SLOT(showLrcDownloadBatchWidget()));

    TTK::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicSongsContainerWidget::closeSearchWidget()
{
    if(m_songSearchWidget)
    {
        m_songSearchWidget->close();
    }
}

void MusicSongsContainerWidget::closeSearchWidgetInNeed()
{
    if(hasSearchResult())
    {
        closeSearchWidget();
    }
}

void MusicSongsContainerWidget::checkTitleNameValid(QString &name)
{
    QString check = name;
    for(int i = 1; i <= ITEM_MAX_COUNT; ++i)
    {
        bool found = false;
        for(const MusicSongItem &item : qAsConst(m_containerItems))
        {
            if(check == item.m_itemName)
            {
                found = true;
                check = name + QString::number(i);
                break;
            }
        }

        if(!found)
        {
            name = check;
            break;
        }
    }
}

void MusicSongsContainerWidget::createWidgetItem(MusicSongItem *item)
{
    MusicSongsListPlayTableWidget *widget = new MusicSongsListPlayTableWidget(this);
    widget->setMovedScrollBar(m_scrollArea->verticalScrollBar());
    widget->setSongSort(&item->m_sort);

    item->m_itemWidget = widget;
    item->m_itemIndex = m_itemIndexRaise;

    addCellItem(widget, item->m_itemName);
    setSongSort(widget, &item->m_sort);
    widget->setPlaylistRow(foundMappedIndex(item->m_itemIndex));

    connect(widget, SIGNAL(isCurrentPlaylistRow(bool&)), SLOT(isCurrentPlaylistRow(bool&)));
    connect(widget, SIGNAL(isSearchedResultEmpty(bool&)), SLOT(isSearchedResultEmpty(bool&)));
    connect(widget, SIGNAL(deleteItemAt(TTKIntList,bool)), SLOT(removeItemAt(TTKIntList,bool)));
    connect(widget, SIGNAL(itemRowSwaped(int,int,int,MusicSongList&)), SLOT(itemRowSwaped(int,int,int,MusicSongList&)));
    connect(widget, SIGNAL(addSongToLovestList(bool,int)), SLOT(addSongToLovestList(bool,int)));
    connect(widget, SIGNAL(showFloatWidget()), SLOT(showFloatWidget()));
    connect(widget, SIGNAL(songListSortBy(int)), SLOT(songListSortBy(int)));

    ///connect to items
    setInputModule(m_items.last().m_itemWidget);

    widget->setSongsList(&item->m_songs);
    setTitle(widget, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

MusicSongItem* MusicSongsContainerWidget::createContainerItem(const MusicSongItem &item)
{
    m_containerItems << item;
    MusicSongItem *it = &m_containerItems.last();
    createWidgetItem(it);
    return it;
}

void MusicSongsContainerWidget::setItemTitle(MusicSongItem *item)
{
    const QString title(QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    setTitle(item->m_itemWidget, title);

    if(m_topMaskWidget->isVisible() && m_topMaskWidget->index() == item->m_itemIndex)
    {
        m_topMaskWidget->setTitle(title);
    }
}

void MusicSongsContainerWidget::setInputModule(QObject *object) const
{
    connect(object, SIGNAL(addNewRowItem()), SLOT(addNewRowItem()));
    connect(object, SIGNAL(deleteRowItem(int)), SLOT(deleteRowItem(int)));
    connect(object, SIGNAL(deleteAllItems(int)), SLOT(deleteAllItems(int)));
    connect(object, SIGNAL(changRowItemName(int,QString)), SLOT(changRowItemName(int,QString)));
    connect(object, SIGNAL(addNewFiles(int)), SLOT(importSongsByFiles(int)));
    connect(object, SIGNAL(addNewDir(int)), SLOT(importSongsByDir(int)));
    connect(object, SIGNAL(songListSortBy(int)), SLOT(songListSortBy(int)));
    connect(object, SIGNAL(swapDragItemIndex(int,int)), SLOT(swapDragItemIndex(int,int)));
    connect(object, SIGNAL(addToPlayLater(int)), SLOT(addToPlayLater(int)));
    connect(object, SIGNAL(addToPlayedList(int)), SLOT(addToPlayedList(int)));
}

void MusicSongsContainerWidget::resizeWindow()
{
    if(m_functionWidget)
    {
        m_functionWidget->move(width() - m_functionWidget->width() - 15, height() - 40 - m_functionWidget->height());
    }

    if(m_songSearchWidget)
    {
        m_songSearchWidget->move(0, height() - m_songSearchWidget->height());
    }
}

void MusicSongsContainerWidget::updatePlayedRows(int begin, int end)
{
    for(const MusicSongItem &item : qAsConst(m_containerItems))
    {
        const int index = foundMappedIndex(item.m_itemIndex);
        item.m_itemWidget->setPlaylistRow(index);
    }

    TTK::IndexPropertyList items;
    if(end == -1)
    {
        for(int i = begin + 1; i < m_containerItems.count(); ++i)
        {
            items << TTK::IndexProperty(i + 1, i);
        }
    }
    else
    {
        items << TTK::IndexProperty(begin, end);

        if(begin > end)
        {
            for(int i = begin; i < end; ++i)
            {
                items << TTK::IndexProperty(i + 1, i);
            }
        }
        else
        {
            for(int i = end; i > begin; --i)
            {
                items << TTK::IndexProperty(i, i - 1);
            }
        }
    }

    MusicPlayedListPopWidget::instance()->updatePlayedRows(items);
}

void MusicSongsContainerWidget::importSongsWithProgress(QWidget *progress, int offset, const QStringList &files, int playlistRow)
{
    if(!progress)
    {
        return;
    }

    if(playlistRow < 0)
    {
        playlistRow = makeValidIndex();
    }

    closeSearchWidgetInNeed();

    MusicSongItem *item = &m_containerItems[playlistRow];
    MusicProgressWidget *widget = TTKStaticCast(MusicProgressWidget*, progress);

    int i = 0;
    for(const QString &path : qAsConst(files))
    {
        if(item->m_songs.contains(MusicSong(path)))
        {
            continue;
        }

        widget->setValue(++i + offset);
        item->m_songs << TTK::generateSongList(path);
    }

    if(i != 0)
    {
        item->m_itemWidget->updateSongsList(item->m_songs);
        setItemTitle(item);
        setCurrentIndex(playlistRow);
    }
}
