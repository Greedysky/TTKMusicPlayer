#include "musicsongssummariziedwidget.h"
#include "musicsongslistfunctionwidget.h"
#include "musicsongslisttablewidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicsongtag.h"
#include "musicprogresswidget.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsongchecktoolswidget.h"
#include "musicplayedlistpopwidget.h"
#include "musicapplication.h"
#include "musictoastlabel.h"

#include <QScrollBar>
#include <QScrollArea>

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
    connectMusicToolBoxWidgetItem(m_listMaskWidget);
    connect(m_listMaskWidget, SIGNAL(mousePressAt(int)), SLOT(mousePressAt(int)));
    connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(sliderValueChanaged(int)));

    m_songCheckToolsWidget = nullptr;
    m_floatWidget = nullptr;

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicSongSearchOnlineTableWidget::getClassName(), getClassName());
}

MusicSongsSummariziedWidget::~MusicSongsSummariziedWidget()
{
    delete m_listMaskWidget;
    delete m_songCheckToolsWidget;
    delete m_floatWidget;
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllLists();
}

QString MusicSongsSummariziedWidget::getClassName()
{
    return staticMetaObject.className();
}

bool MusicSongsSummariziedWidget::addMusicLists(const MusicSongItems &names)
{
    MusicObject::MIntSet inDeed;
    inDeed << MUSIC_NORMAL_LIST << MUSIC_LOVEST_LIST << MUSIC_NETWORK_LIST << MUSIC_RECENT_LIST;
    foreach(const MusicSongItem &item, names)
    {
        inDeed.remove(item.m_itemIndex);
    }
    //////////////////////////////////////////////////////////////////////
    if(!inDeed.isEmpty())
    {
        //if less than four count(0, 1, 2, 3), find and add default items
        m_songItems << names;
        foreach(const int item, inDeed)
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
            }
            m_songItems << songItem;
        }
        qSort(m_songItems);
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

        int transparent = m_songItems.first().m_itemObject->getTransparent();
        item->m_itemObject->setTransparent(transparent);
    }
}

void MusicSongsSummariziedWidget::importOtherMusicSongs(QStringList &filelist)
{
    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Import File Mode"));
    progress.setRange(0, filelist.count());

    MusicSongTag tag;
    MusicSongItem *item = &m_songItems[m_currentImportIndex];
    int i=0;

    foreach(const QString &path, filelist)
    {
        if(item->m_songs.contains(MusicSong(path)))
        {
            filelist.removeAll(path);
            continue;
        }

        QString time = tag.readFile(path) ? tag.getLengthString() : "-";
        QString name;
        if(M_SETTING_PTR->value(MusicSettingManager::OtherInfoChoiced).toBool() &&
           !tag.getTitle().isEmpty() && !tag.getArtist().isEmpty())
        {
            name = tag.getArtist() + " - "+ tag.getTitle();
        }
        item->m_songs << MusicSong(path, 0, time, name);
        progress.setValue(++i);
    }
    item->m_itemObject->updateSongsFileName(item->m_songs);
    setItemTitle(item);

    MusicSongsToolBoxWidget::setCurrentIndex(m_currentImportIndex);

    MusicToastLabel *toast = new MusicToastLabel(MusicApplication::instance());
    toast->setFontSize(15);
    toast->setFontMargin(20, 20);
    toast->setText(tr("Import Music Songs Done!"));
    toast->popup(this);
}

QStringList MusicSongsSummariziedWidget::getMusicSongsFileName(int index) const
{
    QStringList list;

    if(index < 0 || index >= m_songItems.count())
    {
        return list;
    }

    foreach(const MusicSong &song, m_songItems[index].m_songs)
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

    foreach(const MusicSong &song, m_songItems[index].m_songs)
    {
        list << song.getMusicPath();
    }
    return list;
}

void MusicSongsSummariziedWidget::searchFileListCache(int index, const QString &text)
{
    MusicObject::MIntList searchResult;
    QStringList searchedSongs(getMusicSongsFileName(m_currentIndex));
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
    songItem->m_itemObject->setMusicSongsSearchedFileName(&songItem->m_songs, searchResult);

    if(index == 0)
    {
        m_searchfileListCache.clear();
    }
}

bool MusicSongsSummariziedWidget::searchFileListEmpty() const
{
    return m_searchfileListCache.isEmpty();
}

int MusicSongsSummariziedWidget::getSearchFileListIndex(int row)
{
    MusicObject::MIntList list = m_searchfileListCache.value(m_searchFileListIndex);
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
    emit clearSearchText();
    return row;
}

void MusicSongsSummariziedWidget::setCurrentMusicSongTreeIndex(int index)
{
    if(m_currentPlayToolIndex < 0)
    {
        return;
    }

    if(!m_songItems[m_currentPlayToolIndex].m_songs.isEmpty())
    {
        m_songItems[m_currentPlayToolIndex].m_itemObject->replacePlayWidgetRow();
    }
    m_currentPlayToolIndex = index;
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
    m_songItems[m_currentPlayToolIndex].m_itemObject->selectRow(index);
}

void MusicSongsSummariziedWidget::setTimerLabel(const QString &time) const
{
    if(m_currentPlayToolIndex < 0)
    {
        return;
    }
    m_songItems[m_currentPlayToolIndex].m_itemObject->setTimerLabel(time);
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
    int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(message.exec())
    {
        return;
    }

    if(m_currentPlayToolIndex == id)
    {
        MusicSongsToolBoxWidget::setCurrentIndex(0);
        m_itemList.first().m_widgetItem->setItemExpand(false);
        MusicApplication::instance()->musicPlayIndex(-1);
    }
    else if(m_currentPlayToolIndex > id)
    {
        MusicSongsToolBoxWidget::setCurrentIndex(--m_currentPlayToolIndex);
    }

    MusicSongItem item = m_songItems.takeAt(id);
    removeItem(item.m_itemObject);
    delete item.m_itemObject;
    foreach(const MusicSong &song, item.m_songs)
    {
        MusicPlayedListPopWidget::instance()->remove(item.m_itemIndex, song);
    }

    QList< std::pair<int, int> > pairs;
    foreach(const MusicSongItem &item, m_songItems)
    {
        int mappedIndex = foundMappingIndex(item.m_itemIndex);
        item.m_itemObject->setParentToolIndex(mappedIndex);
        pairs << std::pair<int, int>(item.m_itemIndex, mappedIndex);
    }
    MusicPlayedListPopWidget::instance()->resetToolIndex(pairs);
}

void MusicSongsSummariziedWidget::deleteRowItems()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(message.exec())
    {
        return;
    }

    if(m_currentPlayToolIndex != MUSIC_NORMAL_LIST && m_currentPlayToolIndex != MUSIC_LOVEST_LIST &&
       m_currentPlayToolIndex != MUSIC_NETWORK_LIST && m_currentPlayToolIndex != MUSIC_RECENT_LIST)
    {
        MusicSongsToolBoxWidget::setCurrentIndex(0);
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
    int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    m_currentDeleteIndex = id;
    m_toolDeleteChanged = true;
    MusicSongsListTableWidget *w = m_songItems[id].m_itemObject;
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
    int id = foundMappingIndex(index);
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
    int id = foundMappingIndex(index);
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
    int id = foundMappingIndex(index);
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

void MusicSongsSummariziedWidget::setCurrentIndex()
{
    QStringList keyList = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    if(keyList.count() != 3)
    {
        return;
    }
    m_currentPlayToolIndex = keyList[1].toInt();
    int index = keyList[2].toInt();
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

    MusicSong song = m_songItems[m_currentIndex].m_songs[row];
    MusicSongItem *item = &m_songItems[MUSIC_LOVEST_LIST];

    ///if current play list contains, call main add and remove function
    if(MusicSong(MusicApplication::instance()->getCurrentFilePath()) == song)
    {
        MusicApplication::instance()->musicAddSongToLovestListAt(oper);
        return;
    }

    if(oper)    ///Add to lovest list
    {
        item->m_songs << song;
        item->m_itemObject->updateSongsFileName(item->m_songs);
        if(m_currentIndex == MUSIC_LOVEST_LIST)
        {
            emit updatePlayLists(song.getMusicPath());
        }
        setItemTitle(item);
    }
    else        ///Remove to lovest list
    {
        if(item->m_songs.removeOne(song))
        {
            item->m_itemObject->clearAllItems();
            item->m_itemObject->updateSongsFileName(item->m_songs);
            setItemTitle(item);
        }
    }
}

void MusicSongsSummariziedWidget::musicSongToLovestListAt(bool oper, int row)
{
    if(m_currentPlayToolIndex < 0 || m_currentPlayToolIndex >= m_songItems.count())
    {
        return;
    }

    MusicSong song = m_songItems[m_currentPlayToolIndex].m_songs[row];
    MusicSongItem *item = &m_songItems[MUSIC_LOVEST_LIST];
    if(oper)    ///Add to lovest list
    {
        item->m_songs << song;
        item->m_itemObject->updateSongsFileName(item->m_songs);
        if(m_currentPlayToolIndex == MUSIC_LOVEST_LIST)
        {
            emit updatePlayLists(song.getMusicPath());
        }
        setItemTitle(item);
    }
    else        ///Remove to lovest list
    {
        if(item->m_songs.removeOne(song))
        {
            item->m_itemObject->clearAllItems();
            item->m_itemObject->updateSongsFileName(item->m_songs);
            setItemTitle(item);
        }
    }
}

void MusicSongsSummariziedWidget::addNetMusicSongToList(const QString &name, const QString &time,
                                                        const QString &format, bool play)
{
    QString musicSong = MusicUtils::Algorithm::mdII(name, ALG_DOWNLOAD_KEY, false);
    const QString path = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(name).arg(format);
    MusicSongItem *item = &m_songItems[MUSIC_NETWORK_LIST];
    item->m_songs << MusicSong(path, 0, time, musicSong);
    item->m_itemObject->updateSongsFileName(item->m_songs);
    if(m_currentPlayToolIndex == MUSIC_NETWORK_LIST)
    {
        emit updatePlayLists(path);
    }
    setItemTitle(item);

    if(play)
    {
        ///when download finished just play it at once
        MusicSongsToolBoxWidget::setCurrentIndex(MUSIC_NETWORK_LIST);
        MusicApplication::instance()->musicPlayIndex(item->m_songs.count() - 1, 0);
    }
}

void MusicSongsSummariziedWidget::addSongToPlayList(const QStringList &items)
{
    if(items.isEmpty())
    {
        return;
    }

    QStringList files(items);
    importOtherMusicSongs(files);
    if(m_currentPlayToolIndex == MUSIC_NORMAL_LIST)
    {
        foreach(const QString &var, files)
        {
            emit updatePlayLists(var);
        }
    }

    MusicSongItem *songItem = &m_songItems[MUSIC_NORMAL_LIST];
    MusicSongs *musicSongs = &songItem->m_songs;
    MusicSong song = MusicSong(items.last());

    int index = musicSongs->count() - 1;
    if(musicSongs->contains(song))
    {
        index = musicSongs->indexOf(song);
    }

    /// just play it at once
    MusicSongsToolBoxWidget::setCurrentIndex(0);
    MusicApplication::instance()->musicPlayIndex(index, 0);
}

void MusicSongsSummariziedWidget::setDeleteItemAt(const MusicObject::MIntList &index, bool fileRemove)
{
    if(index.count() == 0 || !searchFileListEmpty())
    {
        return;
    }

    int cIndex = m_toolDeleteChanged ? m_currentDeleteIndex : m_currentIndex;
    MusicSongItem *item = &m_songItems[cIndex];
    for(int i=index.count()-1; i>=0; --i)
    {
        MusicSong song = item->m_songs.takeAt(index[i]);
        if( cIndex != m_currentPlayToolIndex && cIndex == MUSIC_LOVEST_LIST )
        {
            int playIndex = m_songItems[m_currentPlayToolIndex].m_itemObject->getPlayRowIndex();
            MusicSongs songs = m_songItems[m_currentPlayToolIndex].m_songs;
            if(playIndex > -1 && playIndex < songs.count())
            {
                if(songs[playIndex].getMusicPath() == song.getMusicPath())
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
    if(cIndex == m_currentPlayToolIndex)
    {
        MusicApplication::instance()->setDeleteItemAt(index, fileRemove);
    }
    setItemTitle(item);

    //create upload file widget if current items is all been deleted
    item->m_itemObject->createUploadFileWidget();
}

void MusicSongsSummariziedWidget::setMusicIndexSwaped(int before, int after, int play, MusicSongs &songs)
{
    MusicSongs *names = &m_songItems[m_currentIndex].m_songs;
    if(before > after)
    {
        for(int i=before; i>after; --i)
        {
            names->swap(i, i - 1);
        }
    }
    else
    {
        for(int i=before; i<after; ++i)
        {
            names->swap(i, i + 1);
        }
    }
    songs = *names;

    if(m_currentIndex == m_currentPlayToolIndex)
    {
        emit updateMediaLists(getMusicSongsFilePath(m_currentPlayToolIndex), play);
    }
}

void MusicSongsSummariziedWidget::isCurrentIndexs(bool &state)
{
    state = ( m_currentIndex == m_currentPlayToolIndex );
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
        song->setMusicPlayCount( song->getMusicPlayCount() + 1);
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
    MusicSong music( (*songs)[index] );
    MusicSongs *musics = &item->m_songs;
    if(!musics->contains(music))
    {
        if(musics->count() >= RECENT_ITEM_MAX_COUNT)
        {
            musics->takeFirst();
            item->m_itemObject->clearAllItems();
        }

        music.setMusicPlayCount(music.getMusicPlayCount() + 1);
        musics->append(music);
        item->m_itemObject->updateSongsFileName(*musics);
        QString title(QString("%1[%2]").arg(item->m_itemName).arg(musics->count()));
        setTitle(item->m_itemObject, title);
    }
    else
    {
        for(int i=0; i<musics->count(); ++i)
        {
            MusicSong *m = &(*musics)[i];
            if(music == *m)
            {
                m->setMusicPlayCount(m->getMusicPlayCount() + 1);
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
    m_songItems[m_currentPlayToolIndex].m_itemObject->updateCurrentArtist();
}

void MusicSongsSummariziedWidget::showFloatWidget()
{
    if(m_floatWidget == nullptr)
    {
        m_floatWidget = new MusicSongsListFunctionWidget(this);
        connect(m_floatWidget, SIGNAL(deleteObject()), SLOT(deleteFloatWidget()));
        m_floatWidget->setGeometry();
        m_floatWidget->show();
    }
    else
    {
        m_floatWidget->setGeometry();
        m_floatWidget->active();
    }
}

void MusicSongsSummariziedWidget::musicListSongSortBy(int index)
{
    int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    MusicSongsListTableWidget *w = m_songItems[id].m_itemObject;
    MusicSong::Sort sort = MusicSong::SortByFileName;
    index = m_songItems[id].m_sort.m_index;
    if(index != -1)
    {
        sort = MStatic_cast(MusicSong::Sort, index);
    }

    MusicSongs *songs = &m_songItems[id].m_songs;
    MusicSong oMusicSong(MusicApplication::instance()->getCurrentFilePath());

    for(int i=0; i<songs->count(); ++i)
    {
        (*songs)[i].setMusicSort(sort);
    }
    if(m_songItems[id].m_sort.m_sortType == Qt::DescendingOrder)
    {
        qSort(*songs);
    }
    else
    {
        qSort(songs->begin(), songs->end(), qGreater<MusicSong>());
    }

    w->clearAllItems();
    w->setSongsFileName(songs);

    index = songs->indexOf(oMusicSong);
    if(m_currentIndex == m_currentPlayToolIndex)
    {
        MusicApplication::instance()->musicPlaySort(index);
    }
}

void MusicSongsSummariziedWidget::sliderValueChanaged(int value)
{
    if(value >= 35*(m_currentIndex + 1) && m_currentIndex > -1 && m_currentIndex < m_songItems.count())
    {
        MusicSongItem *songItem = &m_songItems[m_currentIndex];
        m_listMaskWidget->setItemIndex(songItem->m_itemIndex);
        m_listMaskWidget->setMusicSort(&songItem->m_sort);
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
    delete m_floatWidget;
    m_floatWidget = nullptr;
}

void MusicSongsSummariziedWidget::checkCurrentNameExist(QString &name)
{
    for(int i=1; i<=ITEM_MAX_COUNT; ++i)
    {
        bool hasName = false;
        QString check = name + QString::number(i);
        foreach(const MusicSongItem &songItem, m_songItems)
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

    int transparent = m_songItems.first().m_itemObject->getTransparent();
    m_songItems.last().m_itemObject->setTransparent(transparent);
}

void MusicSongsSummariziedWidget::createWidgetItem(MusicSongItem *item)
{
    MusicSongsListTableWidget *w = new MusicSongsListTableWidget(-1, this);
    w->setMovedScrollBar(m_scrollArea->verticalScrollBar());
    w->setMusicSort(&item->m_sort);

    item->m_itemObject = w;
    item->m_itemIndex = m_itemIndexRaise;
    addItem(w, item->m_itemName);
    setMusicSort(w, &item->m_sort);
    w->setParentToolIndex(foundMappingIndex(item->m_itemIndex));

    connect(w, SIGNAL(cellDoubleClicked(int,int)), MusicApplication::instance(), SLOT(musicPlayIndexClicked(int,int)));
    connect(w, SIGNAL(musicAddNewFiles()), SLOT(musicImportSongsOnlyFile()));
    connect(w, SIGNAL(musicAddNewDir()), SLOT(musicImportSongsOnlyDir()));
    connect(w, SIGNAL(isCurrentIndexs(bool&)), SLOT(isCurrentIndexs(bool&)));
    connect(w, SIGNAL(isSearchFileListEmpty(bool&)), SLOT(isSearchFileListEmpty(bool&)));
    connect(w, SIGNAL(deleteItemAt(MusicObject::MIntList,bool)), SLOT(setDeleteItemAt(MusicObject::MIntList,bool)));
    connect(w, SIGNAL(getMusicIndexSwaped(int,int,int,MusicSongs&)), SLOT(setMusicIndexSwaped(int,int,int,MusicSongs&)));
    connect(w, SIGNAL(musicListSongToLovestListAt(bool,int)), SLOT(musicListSongToLovestListAt(bool,int)));
    connect(w, SIGNAL(showFloatWidget()), SLOT(showFloatWidget()));
    connect(w, SIGNAL(musicListSongSortBy(int)), SLOT(musicListSongSortBy(int)));

    ///connect to items
    connectMusicToolBoxWidgetItem(m_itemList.last().m_widgetItem);

    w->setSongsFileName(&item->m_songs);
    setTitle(w, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicSongsSummariziedWidget::clearAllLists()
{
    while(!m_songItems.isEmpty())
    {
        MusicSongsListTableWidget *w = m_songItems.takeLast().m_itemObject;
        delete w;
        w = nullptr;
    }
}

void MusicSongsSummariziedWidget::setItemTitle(MusicSongItem *item)
{
    QString title(QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    setTitle(item->m_itemObject, title);
    if(m_listMaskWidget->isVisible() && m_listMaskWidget->getItemIndex() == item->m_itemIndex)
    {
        m_listMaskWidget->setTitle(title);
    }
}

void MusicSongsSummariziedWidget::connectMusicToolBoxWidgetItem(QObject *object)
{
    connect(object, SIGNAL(addNewRowItem()), SLOT(addNewRowItem()));
    connect(object, SIGNAL(deleteRowItemAll(int)), SLOT(deleteRowItemAll(int)));
    connect(object, SIGNAL(deleteRowItem(int)), SLOT(deleteRowItem(int)));
    connect(object, SIGNAL(changRowItemName(int,QString)), SLOT(changRowItemName(int,QString)));
    connect(object, SIGNAL(addNewFiles(int)), SLOT(addNewFiles(int)));
    connect(object, SIGNAL(addNewDir(int)), SLOT(addNewDir(int)));
    connect(object, SIGNAL(musicListSongSortBy(int)), SLOT(musicListSongSortBy(int)));
    connect(object, SIGNAL(swapDragItemIndex(int,int)), SLOT(swapDragItemIndex(int,int)));
}

void MusicSongsSummariziedWidget::resizeEvent(QResizeEvent *event)
{
    MusicSongsToolBoxWidget::resizeEvent(event);
    if(m_floatWidget)
    {
        m_floatWidget->setGeometry();
    }
}

void MusicSongsSummariziedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsToolBoxWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("addNewItem"), this, SLOT(addNewRowItem()));
    menu.addAction(tr("importItem"), MusicApplication::instance(), SLOT(musicImportSongsItemList()));
    menu.addAction(tr("musicTest"), this, SLOT(musicSongsCheckTestTools()));
    menu.addAction(tr("deleteAllItem"), this, SLOT(deleteRowItems()))->setEnabled(m_songItems.count() > ITEM_MIN_COUNT);
    menu.exec(QCursor::pos());
}
