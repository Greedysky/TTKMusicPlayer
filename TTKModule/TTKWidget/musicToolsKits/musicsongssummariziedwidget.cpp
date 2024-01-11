#include "musicsongssummariziedwidget.h"
#include "musicsongslistfunctionwidget.h"
#include "musicsongslistplaytablewidget.h"
#include "musicsongsearchdialog.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
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

MusicSongsSummariziedWidget::MusicSongsSummariziedWidget(QWidget *parent)
    : MusicSongsToolBoxWidget(parent),
      MusicItemSearchInterfaceClass(),
      m_playRowIndex(MUSIC_NORMAL_LIST),
      m_lastSearchIndex(MUSIC_NORMAL_LIST),
      m_selectDeleteIndex(MUSIC_NONE_LIST),
      m_listFunctionWidget(nullptr),
      m_songSearchWidget(nullptr)
{
    setAcceptDrops(true);

    m_listMaskWidget = new MusicSongsToolBoxMaskWidget(this);
    setInputModule(m_listMaskWidget);

    connect(m_listMaskWidget, SIGNAL(itemIndexChanged(int)), SLOT(itemIndexChanged(int)));
    connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(sliderValueChanaged(int)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(MusicQueryTableWidget::className(), className());
}

MusicSongsSummariziedWidget::~MusicSongsSummariziedWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    delete m_listMaskWidget;
    delete m_listFunctionWidget;
    delete m_songSearchWidget;

    while(!m_containerItems.isEmpty())
    {
        delete m_containerItems.takeLast().m_itemObject;
    }
}

bool MusicSongsSummariziedWidget::addMusicItemList(const MusicSongItemList &items)
{
    TTKIntSet inDeed;
    inDeed << MUSIC_NORMAL_LIST << MUSIC_LOVEST_LIST << MUSIC_NETWORK_LIST << MUSIC_RECENT_LIST;
    for(const MusicSongItem &item : qAsConst(items))
    {
        inDeed.remove(item.m_itemIndex);
    }
    //
    if(!inDeed.isEmpty())
    {
        //if less than four count(0, 1, 2, 3), find and add default items
        m_containerItems << items;
        for(const int item : qAsConst(inDeed))
        {
            MusicSongItem songItem;
            switch(item)
            {
                case MUSIC_NORMAL_LIST:
                {
                    songItem.m_itemIndex = item;
                    songItem.m_itemName = tr("Default Item");
                    break;
                }
                case MUSIC_LOVEST_LIST:
                {
                    songItem.m_itemIndex = item;
                    songItem.m_itemName = tr("Lovest Item");
                    break;
                }
                case MUSIC_NETWORK_LIST:
                {
                    songItem.m_itemIndex = item;
                    songItem.m_itemName = tr("Net Item");
                    break;
                }
                case MUSIC_RECENT_LIST:
                {
                    songItem.m_itemIndex = item;
                    songItem.m_itemName = tr("Recent Item");
                    break;
                }
                default: break;
            }
            m_containerItems << songItem;
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

    return inDeed.isEmpty();
}

void MusicSongsSummariziedWidget::appendMusicItemList(const MusicSongItemList &items)
{
    for(int i = 0; i < items.count(); ++i)
    {
        m_containerItems << items[i];
        MusicSongItem *item = &m_containerItems.back();
        item->m_itemIndex = ++m_itemIndexRaise;
        checkCurrentNameExist(item->m_itemName);
        createWidgetItem(item);
    }
}

void MusicSongsSummariziedWidget::importMusicSongsByUrl(const QString &path, int playlistRow)
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

        importMusicSongsByPath(files, playlistRow);
    }
    else if(TTK::String::isNetworkUrl(path))
    {
        closeSearchWidgetInNeed();

        MusicSongItem *item = &m_containerItems[MUSIC_NETWORK_LIST];
        const QString &prefix = TTK::String::splitToken(path, TTK_SEPARATOR, "?");
        const QByteArray &md5 = TTK::Algorithm::md5(path.toUtf8());
        const MusicSong song(path + "#" + md5 + "." + TTK::String::splitToken(path), TTK_DEFAULT_STR, TTK::String::pefix(prefix));

        if(item->m_songs.contains(song))
        {
            return;
        }

        item->m_songs << song;
        item->m_itemObject->updateSongsList(item->m_songs);
        setItemTitle(item);
    }
    else
    {
        QStringList files;
        if(MusicFormats::supportMusicFormats().contains(TTK_FILE_SUFFIX(fin)))
        {
           files << fin.absoluteFilePath();
        }

        importMusicSongsByPath(files, playlistRow);
    }
}

void MusicSongsSummariziedWidget::importMusicSongsByPath(const QStringList &files, int playlistRow)
{
    if(files.isEmpty())
    {
        return;
    }

    if(playlistRow < 0)
    {
        playlistRow = makeValidIndex();
    }

    closeSearchWidgetInNeed();

    MusicProgressWidget progress;
    progress.setTitle(tr("Import file mode"));
    progress.setRange(0, files.count());
    progress.show();

    MusicSongItem *item = &m_containerItems[playlistRow];

    int i = 0;
    for(const QString &path : qAsConst(files))
    {
        if(item->m_songs.contains(MusicSong(path)))
        {
            continue;
        }

        progress.setValue(++i);
        item->m_songs << TTK::generateSongList(path);
    }

    item->m_itemObject->updateSongsList(item->m_songs);
    setItemTitle(item);
    setCurrentIndex(playlistRow);

    MusicToastLabel::popup(tr("Import music songs done"));
}

QStringList MusicSongsSummariziedWidget::musicSongsFileName(int index) const
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

QStringList MusicSongsSummariziedWidget::musicSongsFilePath(int index) const
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

int MusicSongsSummariziedWidget::mapSongIndexByFilePath(int playlistRow, const QString &path) const
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

QString MusicSongsSummariziedWidget::mapFilePathBySongIndex(int playlistRow, int index) const
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

void MusicSongsSummariziedWidget::removeSearchResult(int &row)
{
    if(!hasSearchResult() || !isSearchPlayIndex())
    {
        return;
    }

    const TTKIntList &result = m_searchResultCache.value(m_searchResultLevel);
    if(row >= result.count() || row < 0)
    {
        row = -1;
        return;
    }

    row = result[row];

    clearSearchResult();
    closeSearchWidget();
}

void MusicSongsSummariziedWidget::setCurrentSongTreeIndex(int index)
{
    const int before = m_playRowIndex;
    m_playRowIndex = index;

    if(before >= 0)
    {
        MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[before].m_itemObject);
        if(widget && !m_containerItems[before].m_songs.isEmpty())
        {
            widget->adjustPlayWidgetRow();
        }
    }
}

void MusicSongsSummariziedWidget::playLocation(int index)
{
    if(!hasSearchResult())
    {
        selectRow(index);
        resizeScrollIndex(index * 30);
    }
}

void MusicSongsSummariziedWidget::selectRow(int index)
{
    setCurrentIndex(m_playRowIndex);
    if(m_playRowIndex < 0)
    {
        return;
    }

    closeSearchWidgetInNeed();
    m_containerItems[m_playRowIndex].m_itemObject->selectRow(index);
}

void MusicSongsSummariziedWidget::updateTimeLabel(const QString &current, const QString &total) const
{
    if(m_playRowIndex < 0)
    {
        return;
    }
    TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[m_playRowIndex].m_itemObject)->updateTimeLabel(current, total);
}

void MusicSongsSummariziedWidget::addNewRowItem()
{
    if(m_containerItems.count() <= ITEM_MAX_COUNT)
    {
        QString name = tr("Default Item");
        checkCurrentNameExist(name);
        addNewRowItem(name);
    }
}

void MusicSongsSummariziedWidget::deleteRowItem(int index)
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

    MusicSongItem item = m_containerItems[id];
    for(const MusicSong &song : qAsConst(item.m_songs))
    {
        MusicPlayedListPopWidget::instance()->remove(item.m_itemIndex, song);
    }

    if(m_playRowIndex == id)
    {
        setCurrentIndex(MUSIC_NORMAL_LIST);
        m_itemList.front().m_widgetItem->setExpand(false);
        MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
    }
    else if(m_playRowIndex > id)
    {
        setCurrentIndex(--m_playRowIndex);
    }

    m_containerItems.takeAt(id);
    removeItem(item.m_itemObject);
    delete item.m_itemObject;

    updatePlayedList(id, -1);
}

void MusicSongsSummariziedWidget::deleteRowItems()
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
        m_itemList.front().m_widgetItem->setExpand(false);
        MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
    }

    for(int i = m_containerItems.count() - 1; i > 3; --i)
    {
        MusicSongItem item = m_containerItems.takeLast();
        removeItem(item.m_itemObject);
        delete item.m_itemObject;
    }
}

void MusicSongsSummariziedWidget::deleteRowItemAll(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    closeSearchWidgetInNeed();

    m_selectDeleteIndex = id;
    //
    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[id].m_itemObject);
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

void MusicSongsSummariziedWidget::changRowItemName(int index, const QString &name)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    MusicSongItem *item = &m_containerItems[id];
    item->m_itemName = name;
    setItemTitle(item);
}

void MusicSongsSummariziedWidget::swapDragItemIndex(int start, int end)
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

    updatePlayedList(start, end);
}

void MusicSongsSummariziedWidget::addToPlayLater(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    const MusicSongItem *item = &m_containerItems[id];
    const MusicSongList *songs = &item->m_songs;
    for(int i = songs->count() - 1; i >= 0; --i)
    {
        MusicPlayedListPopWidget::instance()->insert(item->m_itemIndex, songs->at(i));
    }
}

void MusicSongsSummariziedWidget::addToPlayedList(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    const MusicSongItem *item = &m_containerItems[id];
    for(const MusicSong &song : qAsConst(item->m_songs))
    {
        MusicPlayedListPopWidget::instance()->append(item->m_itemIndex, song);
    }
}

void MusicSongsSummariziedWidget::importSongsByFiles(int index)
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

    MusicApplication::instance()->importSongsByFiles(index);
}

void MusicSongsSummariziedWidget::importSongsByDir(int index)
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

    MusicApplication::instance()->importSongsByDir(index);
}

void MusicSongsSummariziedWidget::showSongCheckToolsWidget()
{
    TTKGenerateSingleWidget(MusicSongCheckToolsWidget);
}

void MusicSongsSummariziedWidget::showLrcDownloadBatchWidget()
{
    TTKGenerateSingleWidget(MusicLrcDownloadBatchWidget);
}

void MusicSongsSummariziedWidget::searchResultChanged(int, int column)
{
    if(m_currentIndex == -1)
    {
        return;
    }

    if(!isSearchPlayIndex())
    {
        const QStringList searchedSongs(musicSongsFileName(m_lastSearchIndex));
        TTKIntList result;
        for(int i = 0; i < searchedSongs.count(); ++i)
        {
            result << i;
        }

        MusicSongItem *item = &m_containerItems[m_lastSearchIndex];
        TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemObject)->updateSearchFileName(&item->m_songs, result);

        if(item->m_songs.isEmpty())
        {
            item->m_itemObject->updateSongsList(item->m_songs);
        }

        clearSearchResult();
        m_lastSearchIndex = m_currentIndex;
    }

    const QString &text = m_songSearchWidget->text();
    const QStringList searchedSongs(musicSongsFileName(m_currentIndex));
    TTKIntList result;
    for(int i = 0; i < searchedSongs.count(); ++i)
    {
        if(searchedSongs[i].contains(text, Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    m_searchResultLevel = column;
    m_searchResultCache.insert(column, result);

    MusicSongItem *item = &m_containerItems[m_currentIndex];
    TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemObject)->updateSearchFileName(&item->m_songs, result);

    if(column == 0)
    {
        if(item->m_songs.isEmpty())
        {
            item->m_itemObject->updateSongsList(item->m_songs);
        }

        clearSearchResult();
    }
}

void MusicSongsSummariziedWidget::updateCurrentIndex()
{
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    if(lastPlayIndex.count() != 3)
    {
        return;
    }

    m_playRowIndex = lastPlayIndex[1].toInt();
    const int index = lastPlayIndex[2].toInt();
    setCurrentIndex(index);
    setMusicPlayCount(index);

    MusicApplication::instance()->showCurrentSong();
}

void MusicSongsSummariziedWidget::songToLovestListAt(bool state, int row)
{
    if(m_playRowIndex < 0 || m_playRowIndex >= m_containerItems.count())
    {
        return;
    }

    const MusicSong &song = m_containerItems[m_playRowIndex].m_songs[row];
    MusicSongItem *item = &m_containerItems[MUSIC_LOVEST_LIST];

    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemObject);
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

void MusicSongsSummariziedWidget::addSongToLovestList(bool state, int row)
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

    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemObject);
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

void MusicSongsSummariziedWidget::addSongBufferToPlaylist(const MusicResultDataItem &songItem)
{
    MusicSongItem *item = &m_containerItems[MUSIC_NETWORK_LIST];
    const QByteArray &md5 = TTK::Algorithm::md5(songItem.m_id.toUtf8());
    MusicSong song(songItem.m_nickName + "#" + md5 + "." + songItem.m_description, songItem.m_updateTime, songItem.m_name);
    song.setFormat(songItem.m_description);
    song.setSizeStr(songItem.m_count);

    int index = item->m_songs.indexOf(song);
    if(index == -1)
    {
        item->m_songs << song;
        item->m_itemObject->updateSongsList(item->m_songs);
        setItemTitle(item);
        index = item->m_songs.count() - 1;
    }

    if(songItem.m_category == MUSIC_PLAY_NOW)
    {
        ///when download finished just play it at once
        setCurrentIndex(MUSIC_NETWORK_LIST);
        MusicApplication::instance()->playIndexClicked(index, 0);
    }
}

void MusicSongsSummariziedWidget::addSongToPlaylist(const QStringList &items)
{
    if(items.isEmpty())
    {
        return;
    }

    QStringList files(items);
    const int row = makeValidIndex();
    importMusicSongsByPath(files, row);

    const MusicSongItem *item = &m_containerItems[row];
    const MusicSongList *musicSongs = &item->m_songs;
    const MusicSong &song = MusicSong(items.back());

    int index = musicSongs->count() - 1;
    if(musicSongs->contains(song))
    {
        index = musicSongs->indexOf(song);
    }

    /// just play it at once
    setCurrentIndex(row);
    MusicApplication::instance()->playIndexClicked(index, 0);
}

void MusicSongsSummariziedWidget::removeItemAt(const TTKIntList &index, bool fileRemove)
{
    if(index.isEmpty() || hasSearchResult())
    {
        return;
    }

    const int currentIndex = m_selectDeleteIndex != MUSIC_NONE_LIST ? m_selectDeleteIndex : m_currentIndex;
    MusicSongItem *item = &m_containerItems[currentIndex];
    QStringList deleteFiles;
    for(int i = index.count() - 1; i >= 0; --i)
    {
        const MusicSong &song = item->m_songs.takeAt(index[i]);
        deleteFiles << song.path();
        if(currentIndex != m_playRowIndex && currentIndex == MUSIC_LOVEST_LIST)
        {
            const int playIndex = m_containerItems[m_playRowIndex].m_itemObject->playRowIndex();
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
            QFile::remove(currentIndex == MUSIC_NETWORK_LIST ? TTK::generateNetworkSongPath(song.path()) : song.path());
        }
    }

    MusicApplication::instance()->removeItemAt(deleteFiles, fileRemove, currentIndex == m_playRowIndex, currentIndex);

    setItemTitle(item);
    //create upload file widget if current items is all been deleted
    TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemObject)->createUploadFileModule();
}

void MusicSongsSummariziedWidget::itemIndexSwaped(int start, int end, int play, MusicSongList &songs)
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
        MusicPlayedListPopWidget::instance()->setCurrentIndex(m_currentIndex, songs[play]);
    }
}

void MusicSongsSummariziedWidget::isCurrentPlaylistRow(bool &state)
{
    const int currentIndex = m_selectDeleteIndex != MUSIC_NONE_LIST ? m_selectDeleteIndex : m_currentIndex;
    state = (currentIndex == m_playRowIndex);
}

void MusicSongsSummariziedWidget::isSearchedResultEmpty(bool &empty)
{
    empty = !hasSearchResult();
}

void MusicSongsSummariziedWidget::setMusicPlayCount(int index)
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

void MusicSongsSummariziedWidget::setRecentMusicSongs(int index)
{
    if(index < 0 || m_playRowIndex < 0 || m_playRowIndex == MUSIC_NETWORK_LIST || m_playRowIndex == MUSIC_RECENT_LIST)
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
    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, item->m_itemObject);
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

void MusicSongsSummariziedWidget::queryMusicItemList(MusicSongItemList &songs)
{
    songs = m_containerItems;
}

void MusicSongsSummariziedWidget::updateCurrentArtist()
{
    if(m_playRowIndex < 0)
    {
        return;
    }

    TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[m_playRowIndex].m_itemObject)->updateCurrentArtist();
}

void MusicSongsSummariziedWidget::showFloatWidget()
{
    if(m_listFunctionWidget == nullptr)
    {
        m_listFunctionWidget = new MusicSongsListFunctionWidget(this);
        connect(m_listFunctionWidget, SIGNAL(deleteObject()), SLOT(deleteFloatWidget()));
        resizeWindow();
        m_listFunctionWidget->show();
    }
    else
    {
        resizeWindow();
        m_listFunctionWidget->active();
    }
}

void MusicSongsSummariziedWidget::songListSortBy(int index)
{
    const int id = foundMappedIndex(index);
    if(id == -1)
    {
        return;
    }

    closeSearchWidgetInNeed();

    MusicSongsListPlayTableWidget *widget = TTKObjectCast(MusicSongsListPlayTableWidget*, m_containerItems[id].m_itemObject);
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

void MusicSongsSummariziedWidget::showSearchWidget()
{
    if(m_songSearchWidget == nullptr)
    {
        m_songSearchWidget = new MusicSongSearchDialog(this);
        resizeWindow();
    }

    m_songSearchWidget->setVisible(!m_songSearchWidget->isVisible());
}

void MusicSongsSummariziedWidget::sliderValueChanaged(int value)
{
    if(value >= 40 * (m_currentIndex + 1) && m_currentIndex > -1 && m_currentIndex < m_containerItems.count())
    {
        MusicSongItem *item = &m_containerItems[m_currentIndex];
        m_listMaskWidget->setIndex(item->m_itemIndex);
        m_listMaskWidget->setSongSort(&item->m_sort);
        m_listMaskWidget->setTitle(QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
        m_listMaskWidget->setExpand(true);
        m_listMaskWidget->raise();
        m_listMaskWidget->show();
    }
    else
    {
        m_listMaskWidget->hide();
    }
}

void MusicSongsSummariziedWidget::deleteFloatWidget()
{
    delete m_listFunctionWidget;
    m_listFunctionWidget = nullptr;
}

void MusicSongsSummariziedWidget::resizeEvent(QResizeEvent *event)
{
    MusicSongsToolBoxWidget::resizeEvent(event);
    resizeWindow();
}

void MusicSongsSummariziedWidget::dragEnterEvent(QDragEnterEvent *event)
{
    MusicSongsToolBoxWidget::dragEnterEvent(event);
    event->setDropAction(Qt::IgnoreAction);
    event->accept();
}

void MusicSongsSummariziedWidget::dragMoveEvent(QDragMoveEvent *event)
{
    MusicSongsToolBoxWidget::dragMoveEvent(event);

    bool contains = false;
    for(const MusicToolBoxWidgetItem &item : qAsConst(m_itemList))
    {
        if(!TTK::playlistRowValid(item.m_itemIndex))
        {
            continue;
        }

        QWidget *container = item.m_widgetItem->item();
        if(item.m_widgetItem->isActive() || (container && container->isVisible()))
        {
            contains = true;
        }
    }

    event->setDropAction(contains ? Qt::CopyAction : Qt::IgnoreAction);
    event->accept();
}

void MusicSongsSummariziedWidget::dropEvent(QDropEvent *event)
{
    MusicSongsToolBoxWidget::dropEvent(event);

    for(const MusicToolBoxWidgetItem &item : qAsConst(m_itemList))
    {
        if(!TTK::playlistRowValid(item.m_itemIndex))
        {
            continue;
        }

        QWidget *container = item.m_widgetItem->item();
        if(item.m_widgetItem->isActive() || (container && container->isVisible()))
        {
            QStringList files;
            const QMimeData *data = event->mimeData();

            for(const QUrl &url : data->urls())
            {
                files << url.toLocalFile();
            }

            importMusicSongsByPath(files, foundMappedIndex(item.m_itemIndex));
            break;
        }
    }
}

void MusicSongsSummariziedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsToolBoxWidget::contextMenuEvent(event);

    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);
    menu.addAction(tr("Create Item"), this, SLOT(addNewRowItem()));
    menu.addAction(tr("Import Item"), MusicApplication::instance(), SLOT(importSongsItemList()));
    menu.addAction(tr("Music Test Tools"), this, SLOT(showSongCheckToolsWidget()));
    menu.addAction(tr("Lrc Batch Download"), this, SLOT(showLrcDownloadBatchWidget()));
    menu.addAction(tr("Delete All"), this, SLOT(deleteRowItems()))->setEnabled(m_containerItems.count() > ITEM_MIN_COUNT);

    TTK::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicSongsSummariziedWidget::closeSearchWidget()
{
    if(m_songSearchWidget)
    {
        m_songSearchWidget->close();
    }
}

void MusicSongsSummariziedWidget::closeSearchWidgetInNeed()
{
    if(hasSearchResult())
    {
        closeSearchWidget();
    }
}

void MusicSongsSummariziedWidget::checkCurrentNameExist(QString &name)
{
    QString check = name;
    for(int i = 1; i <= ITEM_MAX_COUNT; ++i)
    {
        bool hasName = false;
        for(const MusicSongItem &songItem : qAsConst(m_containerItems))
        {
            if(check == songItem.m_itemName)
            {
                hasName = true;
                check = name + QString::number(i);
                break;
            }
        }

        if(!hasName)
        {
            name = check;
            break;
        }
    }
}

void MusicSongsSummariziedWidget::addNewRowItem(const QString &name)
{
    MusicSongItem item;
    item.m_itemName = name;
    m_containerItems << item;
    createWidgetItem(&m_containerItems.back());
}

void MusicSongsSummariziedWidget::createWidgetItem(MusicSongItem *item)
{
    MusicSongsListPlayTableWidget *widget = new MusicSongsListPlayTableWidget(TTK_NORMAL_LEVEL, this);
    widget->setMovedScrollBar(m_scrollArea->verticalScrollBar());
    widget->setSongSort(&item->m_sort);

    item->m_itemObject = widget;
    item->m_itemIndex = m_itemIndexRaise;

    addCellItem(widget, item->m_itemName);
    setSongSort(widget, &item->m_sort);
    widget->setPlaylistRow(foundMappedIndex(item->m_itemIndex));

    connect(widget, SIGNAL(isCurrentPlaylistRow(bool&)), SLOT(isCurrentPlaylistRow(bool&)));
    connect(widget, SIGNAL(isSearchedResultEmpty(bool&)), SLOT(isSearchedResultEmpty(bool&)));
    connect(widget, SIGNAL(deleteItemAt(TTKIntList,bool)), SLOT(removeItemAt(TTKIntList,bool)));
    connect(widget, SIGNAL(itemIndexSwaped(int,int,int,MusicSongList&)), SLOT(itemIndexSwaped(int,int,int,MusicSongList&)));
    connect(widget, SIGNAL(addSongToLovestList(bool,int)), SLOT(addSongToLovestList(bool,int)));
    connect(widget, SIGNAL(showFloatWidget()), SLOT(showFloatWidget()));
    connect(widget, SIGNAL(songListSortBy(int)), SLOT(songListSortBy(int)));

    ///connect to items
    setInputModule(m_itemList.back().m_widgetItem);

    widget->setSongsList(&item->m_songs);
    setTitle(widget, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicSongsSummariziedWidget::setItemTitle(MusicSongItem *item)
{
    const QString title(QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    setTitle(item->m_itemObject, title);

    if(m_listMaskWidget->isVisible() && m_listMaskWidget->index() == item->m_itemIndex)
    {
        m_listMaskWidget->setTitle(title);
    }
}

void MusicSongsSummariziedWidget::setInputModule(QObject *object) const
{
    connect(object, SIGNAL(addNewRowItem()), SLOT(addNewRowItem()));
    connect(object, SIGNAL(deleteRowItemAll(int)), SLOT(deleteRowItemAll(int)));
    connect(object, SIGNAL(deleteRowItem(int)), SLOT(deleteRowItem(int)));
    connect(object, SIGNAL(changRowItemName(int,QString)), SLOT(changRowItemName(int,QString)));
    connect(object, SIGNAL(addNewFiles(int)), SLOT(importSongsByFiles(int)));
    connect(object, SIGNAL(addNewDir(int)), SLOT(importSongsByDir(int)));
    connect(object, SIGNAL(songListSortBy(int)), SLOT(songListSortBy(int)));
    connect(object, SIGNAL(swapDragItemIndex(int,int)), SLOT(swapDragItemIndex(int,int)));
    connect(object, SIGNAL(addToPlayLater(int)), SLOT(addToPlayLater(int)));
    connect(object, SIGNAL(addToPlayedList(int)), SLOT(addToPlayedList(int)));
}

void MusicSongsSummariziedWidget::resizeWindow()
{
    if(m_listFunctionWidget)
    {
        m_listFunctionWidget->move(width() - m_listFunctionWidget->width() - 15, height() - 40 - m_listFunctionWidget->height());
    }

    if(m_songSearchWidget)
    {
        m_songSearchWidget->move(0, height() - m_songSearchWidget->height());
    }
}

void MusicSongsSummariziedWidget::updatePlayedList(int begin, int end)
{
    for(const MusicSongItem &item : qAsConst(m_containerItems))
    {
        const int index = foundMappedIndex(item.m_itemIndex);
        item.m_itemObject->setPlaylistRow(index);
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

    MusicPlayedListPopWidget::instance()->updatePlayedList(items);
}

