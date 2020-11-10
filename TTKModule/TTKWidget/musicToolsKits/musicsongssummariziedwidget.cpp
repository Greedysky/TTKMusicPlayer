#include "musicsongssummariziedwidget.h"
#include "musicsongslistfunctionwidget.h"
#include "musicsongslisttablewidget.h"
#include "musiclocalsongsearchdialog.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicsongtag.h"
#include "musicprogresswidget.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsongchecktoolswidget.h"
#include "musicplayedlistpopwidget.h"
#include "musiclrcdownloadbatchwidget.h"
#include "musicapplication.h"
#include "musictoastlabel.h"

#define  ITEM_MIN_COUNT             4
#define  ITEM_MAX_COUNT             10
#define  RECENT_ITEM_MAX_COUNT      50

MusicSongsSummariziedWidget::MusicSongsSummariziedWidget(QWidget *parent)
    : MusicSongsToolBoxWidget(parent)
{
    m_currentPlayToolIndex = MUSIC_NORMAL_LIST;
    m_searchFileListIndex = -1;
    m_currentImportIndex = MUSIC_NORMAL_LIST;
    m_currentDeleteIndex = -1;
    m_toolDeleteChanged = false;

    m_listMaskWidget = new MusicSongsToolBoxMaskWidget(this);
    setInputObject(m_listMaskWidget);
    connect(m_listMaskWidget, SIGNAL(mousePressAt(int)), SLOT(mousePressAt(int)));
    connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(sliderValueChanaged(int)));

    m_songCheckToolsWidget = nullptr;
    m_listFunctionWidget = nullptr;
    m_musicSongSearchWidget = nullptr;
    m_lrcBatchDownloadWidget = nullptr;

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicSongSearchTableWidget::getClassName(), getClassName());
}

MusicSongsSummariziedWidget::~MusicSongsSummariziedWidget()
{
    delete m_listMaskWidget;
    delete m_songCheckToolsWidget;
    delete m_listFunctionWidget;
    delete m_musicSongSearchWidget;
    delete m_lrcBatchDownloadWidget;
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllLists();
}

bool MusicSongsSummariziedWidget::addMusicLists(const MusicSongItems &names)
{
    TTKIntSet inDeed;
    inDeed << MUSIC_NORMAL_LIST << MUSIC_LOVEST_LIST << MUSIC_NETWORK_LIST << MUSIC_RECENT_LIST;
    for(const MusicSongItem &item : qAsConst(names))
    {
        inDeed.remove(item.m_itemIndex);
    }
    //
    if(!inDeed.isEmpty())
    {
        //if less than four count(0, 1, 2, 3), find and add default items
        m_songItems << names;
        for(const int item : qAsConst(inDeed))
        {
            MusicSongItem songItem;
            switch(item)
            {
                case MUSIC_NORMAL_LIST:
                    {
                        songItem.m_itemIndex = item;
                        songItem.m_itemName = tr("myDefaultPlayItem");
                        break;
                    }
                case MUSIC_LOVEST_LIST:
                    {
                        songItem.m_itemIndex = item;
                        songItem.m_itemName = tr("myLoveSongItem");
                        break;
                    }
                case MUSIC_NETWORK_LIST:
                    {
                        songItem.m_itemIndex = item;
                        songItem.m_itemName = tr("myNetSongItem");
                        break;
                    }
                case MUSIC_RECENT_LIST:
                    {
                        songItem.m_itemIndex = item;
                        songItem.m_itemName = tr("myRecentSongItem");
                        break;
                    }
                default: break;
            }
            m_songItems << songItem;
        }
        std::sort(m_songItems.begin(), m_songItems.end());
    }
    else
    {
        m_songItems = names;
    }

    for(int i=0; i<m_songItems.count(); ++i)
    {
        createWidgetItem(&m_songItems[i]);
    }

    return inDeed.isEmpty();
}

void MusicSongsSummariziedWidget::appendMusicLists(const MusicSongItems &names)
{
    for(int i=0; i<names.count(); ++i)
    {
        m_songItems << names[i];
        MusicSongItem *item = &m_songItems.last();
        item->m_itemIndex = ++m_itemIndexRaise;
        checkCurrentNameExist(item->m_itemName);
        createWidgetItem(item);
    }
}

void MusicSongsSummariziedWidget::importMusicSongsByPath(QStringList &filelist)
{
    if(!searchFileListEmpty() && m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->close();
    }

    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Import File Mode"));
    progress.setRange(0, filelist.count());

    MusicSongTag tag;
    MusicSongItem *item = &m_songItems[m_currentImportIndex];
    int i=0;

    const QStringList &filelistCopy = filelist;
    for(const QString &path : qAsConst(filelistCopy))
    {
        if(item->m_songs.contains(MusicSong(path)))
        {
            filelist.removeAll(path);
            continue;
        }

        const bool state = tag.read(path);
        const QString &time = state ? tag.getLengthString() : "-";
        QString name;
        if(M_SETTING_PTR->value(MusicSettingManager::OtherUseInfo).toBool() && state && !tag.getTitle().isEmpty() && !tag.getArtist().isEmpty())
        {
            name = tag.getArtist() + " - "+ tag.getTitle();
        }
        item->m_songs << MusicSong(path, 0, time, name);
        progress.setValue(++i);
    }
    item->m_itemObject->updateSongsFileName(item->m_songs);
    setItemTitle(item);

    MusicSongsToolBoxWidget::setCurrentIndex(m_currentImportIndex);

    MusicToastLabel::popup(tr("Import Music Songs Done!"));
}

QStringList MusicSongsSummariziedWidget::getMusicSongsFileName(int index) const
{
    QStringList list;

    if(index < 0 || index >= m_songItems.count())
    {
        return list;
    }

    for(const MusicSong &song : qAsConst(m_songItems[index].m_songs))
    {
        list << song.getMusicName();
    }
    return list;
}

QStringList MusicSongsSummariziedWidget::getMusicSongsFilePath(int index) const
{
    QStringList list;

    if(index < 0 || index >= m_songItems.count())
    {
        return list;
    }

    for(const MusicSong &song : qAsConst(m_songItems[index].m_songs))
    {
        list << song.getMusicPath();
    }
    return list;
}

int MusicSongsSummariziedWidget::mapSongIndexByFilePath(int toolIndex, const QString &path) const
{
    if(toolIndex < 0 || toolIndex >= m_songItems.count() || path.isEmpty())
    {
        return -1;
    }

    const MusicSongs songs(m_songItems[toolIndex].m_songs);
    for(int i=0; i<songs.count(); ++i)
    {
        if(MusicSong(path) == songs[i])
        {
            return i;
        }
    }
    return -1;
}

QString MusicSongsSummariziedWidget::mapFilePathBySongIndex(int toolIndex, int index) const
{
    if(toolIndex < 0 || toolIndex >= m_songItems.count())
    {
        return QString();
    }

    const MusicSongs songs(m_songItems[toolIndex].m_songs);
    if(index < 0 || index >= songs.count())
    {
        return QString();
    }

    return songs[index].getMusicPath();
}

void MusicSongsSummariziedWidget::searchFileListCache(int index)
{
    TTKIntList searchResult;
    const QStringList searchedSongs(getMusicSongsFileName(m_currentIndex));
    QString text;
    if(m_musicSongSearchWidget)
    {
        text = m_musicSongSearchWidget->getSearchedText();
    }

    for(int j=0; j<searchedSongs.count(); ++j)
    {
        if(searchedSongs[j].contains(text, Qt::CaseInsensitive))
        {
            searchResult << j;
        }
    }
    m_searchFileListIndex = text.count();
    m_searchfileListCache.insert(index, searchResult);

    MusicSongItem *songItem = &m_songItems[m_currentIndex];
    TTKStatic_cast(MusicSongsListTableWidget*, songItem->m_itemObject)->setMusicSongsSearchedFileName(&songItem->m_songs, searchResult);

    if(index == 0)
    {
        if(songItem->m_songs.isEmpty())
        {
            songItem->m_itemObject->updateSongsFileName(songItem->m_songs);
        }
        m_searchfileListCache.clear();
    }
}

bool MusicSongsSummariziedWidget::searchFileListEmpty() const
{
    return m_searchfileListCache.isEmpty();
}

int MusicSongsSummariziedWidget::getSearchFileListIndex(int row)
{
    const TTKIntList &list = m_searchfileListCache.value(m_searchFileListIndex);
    if(row >= list.count() || row < 0)
    {
        return -1;
    }
    return list[row];
}

int MusicSongsSummariziedWidget::getSearchFileListIndexAndClear(int row)
{
    row = getSearchFileListIndex(row);
    m_searchfileListCache.clear();
    if(m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->close();
    }
    return row;
}

void MusicSongsSummariziedWidget::setCurrentMusicSongTreeIndex(int index)
{
    const int before = m_currentPlayToolIndex;
    m_currentPlayToolIndex = index;

    if(before >= 0 && !m_songItems[before].m_songs.isEmpty())
    {
        TTKStatic_cast(MusicSongsListTableWidget*, m_songItems[before].m_itemObject)->replacePlayWidgetRow();
    }
}

void MusicSongsSummariziedWidget::playLocation(int index)
{
    if(searchFileListEmpty())
    {
        selectRow(index);
        resizeScrollIndex(index*30);
    }
}

void MusicSongsSummariziedWidget::selectRow(int index)
{
    MusicSongsToolBoxWidget::setCurrentIndex(m_currentPlayToolIndex);
    if(m_currentPlayToolIndex < 0)
    {
        return;
    }

    if(!searchFileListEmpty() && m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->close();
    }
    m_songItems[m_currentPlayToolIndex].m_itemObject->selectRow(index);
}

void MusicSongsSummariziedWidget::updateTimeLabel(const QString &current, const QString &total) const
{
    if(m_currentPlayToolIndex < 0)
    {
        return;
    }
    TTKStatic_cast(MusicSongsListTableWidget*, m_songItems[m_currentPlayToolIndex].m_itemObject)->updateTimeLabel(current, total);
}

void MusicSongsSummariziedWidget::addNewRowItem()
{
    if(m_songItems.count() <= ITEM_MAX_COUNT)
    {
        QString name = tr("defaultItem");
        checkCurrentNameExist(name);
        addNewRowItem(name);
    }
}

void MusicSongsSummariziedWidget::deleteRowItem(int index)
{
    const int id = foundMappingIndex(index);
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

    MusicSongItem item = m_songItems[id];
    for(const MusicSong &song : qAsConst(item.m_songs))
    {
        MusicPlayedListPopWidget::instance()->remove(item.m_itemIndex, song);
    }

    if(m_currentPlayToolIndex == id)
    {
        MusicSongsToolBoxWidget::setCurrentIndex(MUSIC_NORMAL_LIST);
        m_itemList.first().m_widgetItem->setItemExpand(false);
        MusicApplication::instance()->musicPlayIndex(-1);
    }
    else if(m_currentPlayToolIndex > id)
    {
        MusicSongsToolBoxWidget::setCurrentIndex(--m_currentPlayToolIndex);
    }

    item = m_songItems.takeAt(id);
    removeItem(item.m_itemObject);
    delete item.m_itemObject;

    resetToolIndex();
}

void MusicSongsSummariziedWidget::deleteRowItems()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec())
    {
        return;
    }

    if(m_currentPlayToolIndex != MUSIC_NORMAL_LIST && m_currentPlayToolIndex != MUSIC_LOVEST_LIST &&
       m_currentPlayToolIndex != MUSIC_NETWORK_LIST && m_currentPlayToolIndex != MUSIC_RECENT_LIST)
    {
        MusicSongsToolBoxWidget::setCurrentIndex(MUSIC_NORMAL_LIST);
        m_itemList.first().m_widgetItem->setItemExpand(false);
        MusicApplication::instance()->musicPlayIndex(-1);
    }

    for(int i = m_songItems.count() - 1; i>3; --i)
    {
        MusicSongItem item = m_songItems.takeLast();
        removeItem(item.m_itemObject);
        delete item.m_itemObject;
    }
}

void MusicSongsSummariziedWidget::deleteRowItemAll(int index)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    if(!searchFileListEmpty() && m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->close();
    }

    m_currentDeleteIndex = id;
    m_toolDeleteChanged = true;
    MusicSongsListTableWidget *w = TTKStatic_cast(MusicSongsListTableWidget*, m_songItems[id].m_itemObject);
    if(w->rowCount() > 0)
    {
        w->setCurrentCell(0, 1);
        w->setDeleteItemAll();
    }
    m_toolDeleteChanged = false;

    if(m_songItems[id].m_songs.isEmpty() && m_currentPlayToolIndex == id)
    {
        MusicApplication::instance()->musicPlayIndex(-1);
    }
}

void MusicSongsSummariziedWidget::changRowItemName(int index, const QString &name)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    MusicSongItem *item = &m_songItems[id];
    item->m_itemName = name;
    setItemTitle(item);
}

void MusicSongsSummariziedWidget::addNewFiles(int index)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    m_currentImportIndex = id;
    MusicApplication::instance()->musicImportSongsOnlyFile();
    m_currentImportIndex = MUSIC_NORMAL_LIST;
}

void MusicSongsSummariziedWidget::addNewDir(int index)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    m_currentImportIndex = id;
    MusicApplication::instance()->musicImportSongsOnlyDir();
    m_currentImportIndex = MUSIC_NORMAL_LIST;
}

void MusicSongsSummariziedWidget::swapDragItemIndex(int before, int after)
{
    before = foundMappingIndex(before);
    after = foundMappingIndex(after);
    if(before == after)
    {
        return;
    }

    //adjust the m_currentPlayToolIndex while the item has dragged and dropped
    if(before < after)
    {
        if((before < m_currentPlayToolIndex && m_currentPlayToolIndex < after) || m_currentPlayToolIndex == after)
        {
            m_currentIndex = --m_currentPlayToolIndex;
        }
        else if(m_currentPlayToolIndex == before)
        {
            m_currentIndex = m_currentPlayToolIndex = after;
        }
    }
    else
    {
        if((after < m_currentPlayToolIndex && m_currentPlayToolIndex < before) || m_currentPlayToolIndex == after)
        {
            m_currentIndex = ++m_currentPlayToolIndex;
        }
        else if(m_currentPlayToolIndex == before)
        {
            m_currentIndex = m_currentPlayToolIndex = after;
        }
    }

    swapItem(before, after);
    MusicSongItem item = m_songItems.takeAt(before);
    m_songItems.insert(after, item);

    resetToolIndex();
}

void MusicSongsSummariziedWidget::addToPlayLater(int index)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    const MusicSongItem *item = &m_songItems[id];
    const MusicSongs *songs = &item->m_songs;
    for(int i=songs->count()-1; i>=0; --i)
    {
        MusicPlayedListPopWidget::instance()->insert(item->m_itemIndex, (*songs)[i]);
    }
}

void MusicSongsSummariziedWidget::addToPlayedList(int index)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    const MusicSongItem *item = &m_songItems[id];
    for(const MusicSong &song : qAsConst(item->m_songs))
    {
        MusicPlayedListPopWidget::instance()->append(item->m_itemIndex, song);
    }
}

void MusicSongsSummariziedWidget::musicImportSongsOnlyFile()
{
    m_currentImportIndex = m_currentIndex;
    MusicApplication::instance()->musicImportSongsOnlyFile();
    m_currentImportIndex = MUSIC_NORMAL_LIST;
}

void MusicSongsSummariziedWidget::musicImportSongsOnlyDir()
{
    m_currentImportIndex = m_currentIndex;
    MusicApplication::instance()->musicImportSongsOnlyDir();
    m_currentImportIndex = MUSIC_NORMAL_LIST;
}

void MusicSongsSummariziedWidget::musicSongsCheckTestTools()
{
    delete m_songCheckToolsWidget;
    m_songCheckToolsWidget = new MusicSongCheckToolsWidget(this);
    m_songCheckToolsWidget->show();
}

void MusicSongsSummariziedWidget::musicLrcBatchDownload()
{
    delete m_lrcBatchDownloadWidget;
    m_lrcBatchDownloadWidget = new MusicLrcDownloadBatchWidget(this);
    m_lrcBatchDownloadWidget->show();
}

void MusicSongsSummariziedWidget::setCurrentIndex()
{
    const QStringList &keyList = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    if(keyList.count() != 3)
    {
        return;
    }
    m_currentPlayToolIndex = keyList[1].toInt();
    const int index = keyList[2].toInt();
    MusicSongsToolBoxWidget::setCurrentIndex(index);
    setMusicPlayCount(index);
    MusicApplication::instance()->showCurrentSong(index);
}

void MusicSongsSummariziedWidget::musicListSongToLovestListAt(bool oper, int row)
{
    if(m_currentIndex < 0 || m_currentIndex >= m_songItems.count() || !searchFileListEmpty())
    {
        return;
    }

    const MusicSong &song = m_songItems[m_currentIndex].m_songs[row];
    MusicSongItem *item = &m_songItems[MUSIC_LOVEST_LIST];

    ///if current play list contains, call main add and remove function
    if(MusicSong(MusicApplication::instance()->getCurrentFilePath()) == song)
    {
        MusicApplication::instance()->musicAddSongToLovestListAt(oper);
        return;
    }

    MusicSongsListTableWidget *w = TTKStatic_cast(MusicSongsListTableWidget*, item->m_itemObject);
    if(oper)    ///Add to lovest list
    {
        item->m_songs << song;
        w->updateSongsFileName(item->m_songs);
        setItemTitle(item);
    }
    else        ///Remove to lovest list
    {
        if(item->m_songs.removeOne(song))
        {
            w->clearAllItems();
            w->updateSongsFileName(item->m_songs);
            setItemTitle(item);
            MusicApplication::instance()->setLoveDeleteItemAt(song.getMusicPath(), m_currentPlayToolIndex == MUSIC_LOVEST_LIST);
        }
    }
}

void MusicSongsSummariziedWidget::musicSongToLovestListAt(bool oper, int row)
{
    if(m_currentPlayToolIndex < 0 || m_currentPlayToolIndex >= m_songItems.count())
    {
        return;
    }

    const MusicSong &song = m_songItems[m_currentPlayToolIndex].m_songs[row];
    MusicSongItem *item = &m_songItems[MUSIC_LOVEST_LIST];
    MusicSongsListTableWidget *w = TTKStatic_cast(MusicSongsListTableWidget*, item->m_itemObject);
    if(oper)    ///Add to lovest list
    {
        item->m_songs << song;
        w->updateSongsFileName(item->m_songs);
        setItemTitle(item);
    }
    else        ///Remove to lovest list
    {
        if(item->m_songs.removeOne(song))
        {
            w->clearAllItems();
            w->updateSongsFileName(item->m_songs);
            setItemTitle(item);
            MusicApplication::instance()->setLoveDeleteItemAt(song.getMusicPath(), m_currentPlayToolIndex == MUSIC_LOVEST_LIST);
        }
    }
}

void MusicSongsSummariziedWidget::addNetMusicSongToList(const QString &name, const QString &time, const QString &format, bool play)
{
    const QString &musicSong = MusicUtils::Algorithm::mdII(name, ALG_ARC_KEY, false);
    const QString &path = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(name).arg(format);
    MusicSongItem *item = &m_songItems[MUSIC_NETWORK_LIST];
    item->m_songs << MusicSong(path, 0, time, musicSong);
    item->m_itemObject->updateSongsFileName(item->m_songs);
    setItemTitle(item);

    if(play)
    {
        ///when download finished just play it at once
        MusicSongsToolBoxWidget::setCurrentIndex(MUSIC_NETWORK_LIST);
        MusicApplication::instance()->musicPlayIndexClicked(item->m_songs.count() - 1, 0);
    }
}

void MusicSongsSummariziedWidget::addSongToPlaylist(const QStringList &items)
{
    if(items.isEmpty())
    {
        return;
    }

    QStringList files(items);
    importMusicSongsByPath(files);

    const MusicSongItem *songItem = &m_songItems[MUSIC_NORMAL_LIST];
    const MusicSongs *musicSongs = &songItem->m_songs;
    const MusicSong &song = MusicSong(items.last());

    int index = musicSongs->count() - 1;
    if(musicSongs->contains(song))
    {
        index = musicSongs->indexOf(song);
    }

    /// just play it at once
    MusicSongsToolBoxWidget::setCurrentIndex(MUSIC_NORMAL_LIST);
    MusicApplication::instance()->musicPlayIndexClicked(index, 0);
}

void MusicSongsSummariziedWidget::setDeleteItemAt(const TTKIntList &index, bool fileRemove)
{
    if(index.count() == 0 || !searchFileListEmpty())
    {
        return;
    }

    const int cIndex = m_toolDeleteChanged ? m_currentDeleteIndex : m_currentIndex;
    MusicSongItem *item = &m_songItems[cIndex];
    QStringList deleteFiles;
    for(int i=index.count()-1; i>=0; --i)
    {
        const MusicSong &song = item->m_songs.takeAt(index[i]);
        deleteFiles << song.getMusicPath();
        if(cIndex != m_currentPlayToolIndex && cIndex == MUSIC_LOVEST_LIST)
        {
            const int playIndex = m_songItems[m_currentPlayToolIndex].m_itemObject->getPlayRowIndex();
            const MusicSongs &songs = m_songItems[m_currentPlayToolIndex].m_songs;
            if(playIndex > -1 && playIndex < songs.count())
            {
                if(songs[playIndex] == song)
                {
                    MusicApplication::instance()->musicAddSongToLovestListAt(false);
                }
            }
        }
        if(fileRemove)
        {
            QFile::remove(song.getMusicPath());
        }
    }

    MusicApplication::instance()->setDeleteItemAt(deleteFiles, fileRemove, cIndex == m_currentPlayToolIndex, cIndex);

    setItemTitle(item);

    //create upload file widget if current items is all been deleted
    TTKStatic_cast(MusicSongsListTableWidget*, item->m_itemObject)->createUploadFileWidget();
}

void MusicSongsSummariziedWidget::setMusicIndexSwaped(int before, int after, int play, MusicSongs &songs)
{
    MusicSongs *names = &m_songItems[m_currentIndex].m_songs;
    if(before > after)
    {
        for(int i=before; i>after; --i)
        {
#if TTK_QT_VERSION_CHECK(5,13,0)
            names->swapItemsAt(i, i - 1);
#else
            names->swap(i, i - 1);
#endif
        }
    }
    else
    {
        for(int i=before; i<after; ++i)
        {
#if TTK_QT_VERSION_CHECK(5,13,0)
            names->swapItemsAt(i, i + 1);
#else
            names->swap(i, i + 1);
#endif
        }
    }
    songs = *names;

    if(m_currentIndex == m_currentPlayToolIndex)
    {
        MusicPlayedListPopWidget::instance()->setCurrentIndex(m_currentIndex, songs[play]);
    }
}

void MusicSongsSummariziedWidget::isCurrentIndex(bool &state)
{
    const int cIndex = m_toolDeleteChanged ? m_currentDeleteIndex : m_currentIndex;
    state = (cIndex == m_currentPlayToolIndex);
}

void MusicSongsSummariziedWidget::isSearchFileListEmpty(bool &empty)
{
    empty = searchFileListEmpty();
}

void MusicSongsSummariziedWidget::setMusicPlayCount(int index)
{
    if(index < 0 || m_currentPlayToolIndex < 0)
    {
        return;
    }

    MusicSongs *songs = &m_songItems[m_currentPlayToolIndex].m_songs;
    if(!songs->isEmpty() && index < songs->count())
    {
        MusicSong *song = &(*songs)[index];
        song->setMusicPlayCount(song->getMusicPlayCount() + 1);
    }
}

void MusicSongsSummariziedWidget::setRecentMusicSongs(int index)
{
    if(index < 0 || m_currentPlayToolIndex < 0 || m_currentPlayToolIndex == MUSIC_RECENT_LIST)
    {
        return;
    }

    MusicSongs *songs = &m_songItems[m_currentPlayToolIndex].m_songs;
    if(songs->isEmpty() || index >= songs->count())
    {
        return;
    }

    MusicSongItem *item = &m_songItems[MUSIC_RECENT_LIST];
    MusicSong music((*songs)[index]);
    MusicSongs *musics = &item->m_songs;
    MusicSongsListTableWidget *w = TTKStatic_cast(MusicSongsListTableWidget*, item->m_itemObject);
    if(!musics->contains(music))
    {
        if(musics->count() >= RECENT_ITEM_MAX_COUNT)
        {
            musics->takeFirst();
            w->clearAllItems();
        }

        music.setMusicPlayCount(music.getMusicPlayCount() + 1);
        musics->append(music);
        w->updateSongsFileName(*musics);

        const QString title(QString("%1[%2]").arg(item->m_itemName).arg(musics->count()));
        setTitle(w, title);
    }
    else
    {
        for(int i=0; i<musics->count(); ++i)
        {
            MusicSong *song = &(*musics)[i];
            if(music == *song)
            {
                song->setMusicPlayCount(song->getMusicPlayCount() + 1);
                break;
            }
        }
    }
}

void MusicSongsSummariziedWidget::getMusicLists(MusicSongItems &songs)
{
    songs = m_songItems;
}

void MusicSongsSummariziedWidget::updateCurrentArtist()
{
    if(m_currentPlayToolIndex < 0)
    {
        return;
    }
    TTKStatic_cast(MusicSongsListTableWidget*, m_songItems[m_currentPlayToolIndex].m_itemObject)->updateCurrentArtist();
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

void MusicSongsSummariziedWidget::musicListSongSortBy(int index)
{
    const int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    if(!searchFileListEmpty() && m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->close();
    }

    MusicSongsListTableWidget *w = TTKStatic_cast(MusicSongsListTableWidget*, m_songItems[id].m_itemObject);
    MusicSong::Sort sort = MusicSong::SortByFileName;
    index = m_songItems[id].m_sort.m_index;
    if(index != -1)
    {
        sort = TTKStatic_cast(MusicSong::Sort, index);
    }

    MusicSongs *songs = &m_songItems[id].m_songs;
    const MusicSong oMusicSong(MusicApplication::instance()->getCurrentFilePath());

    for(int i=0; i<songs->count(); ++i)
    {
        (*songs)[i].setMusicSort(sort);
    }

    if(m_songItems[id].m_sort.m_sortType == Qt::DescendingOrder)
    {
        std::sort(songs->begin(), songs->end());
    }
    else
    {
        std::sort(songs->begin(), songs->end(), std::greater<MusicSong>());
    }

    w->clearAllItems();
    w->setSongsFileName(songs);

    index = songs->indexOf(oMusicSong);
    if(m_currentIndex == m_currentPlayToolIndex)
    {
        MusicApplication::instance()->musicPlaySort(index);
    }
}

void MusicSongsSummariziedWidget::musicSearchWidget()
{
    if(m_musicSongSearchWidget == nullptr)
    {
        m_musicSongSearchWidget = new MusicLocalSongSearchDialog(this);
        resizeWindow();
    }
    m_musicSongSearchWidget->setVisible(!m_musicSongSearchWidget->isVisible());
}

void MusicSongsSummariziedWidget::sliderValueChanaged(int value)
{
    if(value >= 40*(m_currentIndex + 1) && m_currentIndex > -1 && m_currentIndex < m_songItems.count())
    {
        MusicSongItem *songItem = &m_songItems[m_currentIndex];
        m_listMaskWidget->setItemIndex(songItem->m_itemIndex);
        m_listMaskWidget->setMusicSongSort(&songItem->m_sort);
        m_listMaskWidget->setTitle(QString("%1[%2]").arg(songItem->m_itemName).arg(songItem->m_songs.count()));
        m_listMaskWidget->setItemExpand(true);
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

void MusicSongsSummariziedWidget::checkCurrentNameExist(QString &name)
{
    for(int i=1; i<=ITEM_MAX_COUNT; ++i)
    {
        bool hasName = false;
        const QString &check = name + QString::number(i);
        for(const MusicSongItem &songItem : qAsConst(m_songItems))
        {
            if(check == songItem.m_itemName)
            {
                hasName = true;
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
    m_songItems << item;
    createWidgetItem(&m_songItems.last());
}

void MusicSongsSummariziedWidget::createWidgetItem(MusicSongItem *item)
{
    MusicSongsListTableWidget *w = new MusicSongsListTableWidget(-1, this);
    w->setMovedScrollBar(m_scrollArea->verticalScrollBar());
    w->setMusicSongSort(&item->m_sort);

    item->m_itemObject = w;
    item->m_itemIndex = m_itemIndexRaise;
    addItem(w, item->m_itemName);
    setMusicSongSort(w, &item->m_sort);
    w->setParentToolIndex(foundMappingIndex(item->m_itemIndex));

    connect(w, SIGNAL(musicAddNewFiles()), SLOT(musicImportSongsOnlyFile()));
    connect(w, SIGNAL(musicAddNewDir()), SLOT(musicImportSongsOnlyDir()));
    connect(w, SIGNAL(isCurrentIndex(bool&)), SLOT(isCurrentIndex(bool&)));
    connect(w, SIGNAL(isSearchFileListEmpty(bool&)), SLOT(isSearchFileListEmpty(bool&)));
    connect(w, SIGNAL(deleteItemAt(TTKIntList,bool)), SLOT(setDeleteItemAt(TTKIntList,bool)));
    connect(w, SIGNAL(getMusicIndexSwaped(int,int,int,MusicSongs&)), SLOT(setMusicIndexSwaped(int,int,int,MusicSongs&)));
    connect(w, SIGNAL(musicListSongToLovestListAt(bool,int)), SLOT(musicListSongToLovestListAt(bool,int)));
    connect(w, SIGNAL(showFloatWidget()), SLOT(showFloatWidget()));
    connect(w, SIGNAL(musicListSongSortBy(int)), SLOT(musicListSongSortBy(int)));

    ///connect to items
    setInputObject(m_itemList.last().m_widgetItem);

    w->setSongsFileName(&item->m_songs);
    setTitle(w, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicSongsSummariziedWidget::clearAllLists()
{
    while(!m_songItems.isEmpty())
    {
        delete m_songItems.takeLast().m_itemObject;
    }
}

void MusicSongsSummariziedWidget::setItemTitle(MusicSongItem *item)
{
    const QString title(QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    setTitle(item->m_itemObject, title);

    if(m_listMaskWidget->isVisible() && m_listMaskWidget->getItemIndex() == item->m_itemIndex)
    {
        m_listMaskWidget->setTitle(title);
    }
}

void MusicSongsSummariziedWidget::setInputObject(QObject *object) const
{
    connect(object, SIGNAL(addNewRowItem()), SLOT(addNewRowItem()));
    connect(object, SIGNAL(deleteRowItemAll(int)), SLOT(deleteRowItemAll(int)));
    connect(object, SIGNAL(deleteRowItem(int)), SLOT(deleteRowItem(int)));
    connect(object, SIGNAL(changRowItemName(int,QString)), SLOT(changRowItemName(int,QString)));
    connect(object, SIGNAL(addNewFiles(int)), SLOT(addNewFiles(int)));
    connect(object, SIGNAL(addNewDir(int)), SLOT(addNewDir(int)));
    connect(object, SIGNAL(musicListSongSortBy(int)), SLOT(musicListSongSortBy(int)));
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

    if(m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->move(0, height() - m_musicSongSearchWidget->height());
    }
}

void MusicSongsSummariziedWidget::resetToolIndex()
{
    PlayedPairList pairs;
    for(const MusicSongItem &item : qAsConst(m_songItems))
    {
        const int mappedIndex = foundMappingIndex(item.m_itemIndex);
        item.m_itemObject->setParentToolIndex(mappedIndex);
        if(item.m_itemIndex != mappedIndex)
        {
            pairs << PlayedPairItem(item.m_itemIndex, mappedIndex);
        }
    }
    MusicPlayedListPopWidget::instance()->resetToolIndex(pairs);
}

void MusicSongsSummariziedWidget::resizeEvent(QResizeEvent *event)
{
    MusicSongsToolBoxWidget::resizeEvent(event);
    resizeWindow();
}

void MusicSongsSummariziedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsToolBoxWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(tr("addNewItem"), this, SLOT(addNewRowItem()));
    menu.addAction(tr("importItem"), MusicApplication::instance(), SLOT(musicImportSongsItemList()));
    menu.addAction(tr("musicTest"), this, SLOT(musicSongsCheckTestTools()));
    menu.addAction(tr("lrcBatch"), this, SLOT(musicLrcBatchDownload()));
    menu.addAction(tr("deleteAllItem"), this, SLOT(deleteRowItems()))->setEnabled(m_songItems.count() > ITEM_MIN_COUNT);
    menu.exec(QCursor::pos());
}
