#include "musicsongssummarizied.h"
#include "musicsongslistwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicsongtag.h"
#include "musicprogresswidget.h"
#include "musiccryptographichash.h"
#include "musicsongssummariziedfloatwidget.h"
#include "musicsongsearchonlinewidget.h"

MusicSongsSummarizied::MusicSongsSummarizied(QWidget *parent)
    : MusicSongsToolBoxWidget(parent), m_floatWidget(nullptr)
{
    m_supperClass = parent;
    m_currentIndexs = MUSIC_NORMAL_LIST;
    m_searchFileListIndex = -1;

    connect(this, SIGNAL(musicPlayIndex(int,int)), parent, SLOT(musicPlayIndex(int,int)));
    connect(this, SIGNAL(showCurrentSong(int)), parent, SLOT(showCurrentSong(int)));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicSongSearchOnlineTableWidget::getClassName(), getClassName());
}

MusicSongsSummarizied::~MusicSongsSummarizied()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
    clearAllLists();
    delete m_floatWidget;
}

QString MusicSongsSummarizied::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsSummarizied::setMusicLists(const MusicSongItems &names)
{
    if(names.count() < 3)
    {
        //if less than three count, add default items
        addNewRowItem( tr("myDefaultPlayItem") );
        addNewRowItem( tr("myLoveSongItem") );
        addNewRowItem( tr("myNetSongItem") );
    }
    else
    {
        m_songItems = names;
        for(int i=0; i<names.count(); ++i)
        {
            createWidgetItem(&m_songItems[i]);
        }
    }
}

void MusicSongsSummarizied::setMusicSongsSearchedFileName(const MusicObject::MIntList &fileIndexs)
{
    MusicSongs songs;
    foreach(int index, fileIndexs)
    {
        songs << m_songItems[currentIndex()].m_songs[index];
    }
    m_songItems[currentIndex()].m_itemObject->clearAllItems();
    m_songItems[currentIndex()].m_itemObject->updateSongsFileName(songs);
}

void MusicSongsSummarizied::searchFileListCache(int index, const QString &text)
{
    MusicObject::MIntList searchResult;
    QStringList searchedSongs(getMusicSongsFileName(currentIndex()));
    for(int j=0; j<searchedSongs.count(); ++j)
    if(searchedSongs[j].contains(text, Qt::CaseInsensitive))
    {
        searchResult << j;
    }
    m_searchFileListIndex = text.count();
    m_searchfileListCache.insert(index, searchResult);
    setMusicSongsSearchedFileName(searchResult);

    if(index == 0)
    {
        m_searchfileListCache.clear();
    }
}

bool MusicSongsSummarizied::searchFileListEmpty() const
{
    return m_searchfileListCache.isEmpty();
}

int MusicSongsSummarizied::getSearchFileListIndex(int row)
{
    MusicObject::MIntList list = m_searchfileListCache.value(m_searchFileListIndex);
    if(row >= list.count())
    {
        return -1;
    }
    return row >= 0 ? list[row] : -1;
}

int MusicSongsSummarizied::getSearchFileListIndexAndClear(int row)
{
    row = getSearchFileListIndex(row);
    m_searchfileListCache.clear();
    emit clearSearchText();
    return row;
}

void MusicSongsSummarizied::importOtherMusicSongs(const QStringList &filelist)
{
    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Import File Mode"));
    progress.setRange(0, filelist.count());

    MusicSongTag tag;
    MusicSongItem *item = &m_songItems[MUSIC_NORMAL_LIST];
    for(int i=0; i<filelist.count(); ++i)
    {
        QString time = tag.readFile(filelist[i]) ? tag.getLengthString() : "-";
        item->m_songs << MusicSong(filelist[i], 0, time, QString());
        progress.setValue(i + 1);
    }
    item->m_itemObject->updateSongsFileName(item->m_songs);
    setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

QStringList MusicSongsSummarizied::getMusicSongsFileName(int index) const
{
    QStringList list;
    foreach(MusicSong song, m_songItems[index].m_songs)
    {
        list << song.getMusicName();
    }
    return list;
}

QStringList MusicSongsSummarizied::getMusicSongsFilePath(int index) const
{
    QStringList list;
    foreach(MusicSong song, m_songItems[index].m_songs)
    {
        list << song.getMusicPath();
    }
    return list;
}

void MusicSongsSummarizied::playLocation(int index)
{
    selectRow(index);
    resizeScrollIndex(index*30);
}

void MusicSongsSummarizied::selectRow(int index)
{
    MusicSongsToolBoxWidget::setCurrentIndex(m_currentIndexs);
    m_songItems[m_currentIndexs].m_itemObject->selectRow(index);
}

void MusicSongsSummarizied::setTimerLabel(const QString &time) const
{
    m_songItems[m_currentIndexs].m_itemObject->setTimerLabel(time);
}

void MusicSongsSummarizied::setTransparent(int alpha)
{
    foreach(MusicSongItem item, m_songItems)
    {
        item.m_itemObject->setTransparent(alpha*2.55);
        item.m_itemObject->update();
    }
}

void MusicSongsSummarizied::setMusicPlayCount(int index)
{
    if(index < 0)
    {
        return;
    }
    MusicSongs *songs = &m_songItems[m_currentIndexs].m_songs;
    if(!songs->isEmpty() && index < songs->count())
    {
        MusicSong *song = &(*songs)[index];
        song->setMusicPlayCount( song->getMusicPlayCount() + 1);
    }
}

void MusicSongsSummarizied::deleteFloatWidget()
{
    delete m_floatWidget;
    m_floatWidget = nullptr;
}

void MusicSongsSummarizied::getMusicLists(MusicSongItems &songs)
{
    songs = m_songItems;
}

void MusicSongsSummarizied::updateCurrentArtist()
{
    m_songItems[m_currentIndexs].m_itemObject->updateCurrentArtist();
}

int MusicSongsSummarizied::foundMappingIndex(int index)
{
    int id = -1;
    for(int i=0; i<m_songItems.count(); ++i)
    {
        if(m_songItems[i].m_itemIndex == index)
        {
            id = i;
            break;
        }
    }
    return id;
}

void MusicSongsSummarizied::addNewRowItem(const QString &name)
{
    MusicSongItem item;
    item.m_itemName = name;
    item.m_itemIndex = m_songItems.count();
    m_songItems << item;
    createWidgetItem(&m_songItems.last());
}

void MusicSongsSummarizied::createWidgetItem(MusicSongItem *item)
{
    MusicSongsListWidget *w = new MusicSongsListWidget(this);
    item->m_itemObject = w;
    addItem(w, item->m_itemName);

    connect(w, SIGNAL(cellDoubleClicked(int,int)), m_supperClass, SLOT(musicPlayIndex(int,int)));
    connect(w, SIGNAL(musicPlayOrder()), m_supperClass, SLOT(musicPlayOrder()));
    connect(w, SIGNAL(musicPlayRandom()), m_supperClass, SLOT(musicPlayRandom()));
    connect(w, SIGNAL(musicPlayListLoop()), m_supperClass, SLOT(musicPlayListLoop()));
    connect(w, SIGNAL(musicPlayOneLoop()), m_supperClass, SLOT(musicPlayOneLoop()));
    connect(w, SIGNAL(musicPlayItemOnce()), m_supperClass, SLOT(musicPlayItemOnce()));
    connect(w, SIGNAL(musicAddNewFiles()), m_supperClass, SLOT(musicImportSongsOnlyFile()));
    connect(w, SIGNAL(musicAddNewDir()), m_supperClass, SLOT(musicImportSongsOnlyDir()));
    connect(w, SIGNAL(isCurrentIndexs(bool&)), SLOT(isCurrentIndexs(bool&)));
    connect(w, SIGNAL(isSearchFileListEmpty(bool&)), SLOT(isSearchFileListEmpty(bool&)));
    connect(w, SIGNAL(deleteItemAt(MusicObject::MIntList,bool)), SLOT(setDeleteItemAt(MusicObject::MIntList,bool)));
    connect(w, SIGNAL(getMusicIndexSwaped(int,int,int,QStringList&)),
               SLOT(setMusicIndexSwaped(int,int,int,QStringList&)));
    ///connect to items
    connect(m_itemList.last(), SIGNAL(addNewRowItem()), SLOT(addNewRowItem()));
    connect(m_itemList.last(), SIGNAL(deleteRowItem(int)), SLOT(deleteRowItem(int)));
    connect(m_itemList.last(), SIGNAL(changRowItemName(int,QString)), SLOT(changRowItemName(int,QString)));

    w->setSongsFileName(&item->m_songs);
    setTitle(w, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicSongsSummarizied::clearAllLists()
{
    while(!m_songItems.isEmpty())
    {
        MusicSongsListWidget *w = m_songItems.takeLast().m_itemObject;
        delete w;
        w = nullptr;
    }
}

void MusicSongsSummarizied::setDeleteItemAt(const MusicObject::MIntList &index, bool fileRemove)
{
    if(index.count() == 0)
    {
        return;
    }

    MusicSongItem *item = &m_songItems[currentIndex()];
    for(int i=index.count()-1; i>=0; --i)
    {
        MusicSong song = item->m_songs.takeAt(index[i]);
        if(fileRemove)
        {
            QFile::remove(song.getMusicPath());
        }
    }
    if(currentIndex() == m_currentIndexs)
    {
        emit deleteItemAt(index, fileRemove);
    }
    setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicSongsSummarizied::addMusicSongToLovestListAt(int row)
{
    MusicSong song = m_songItems[m_currentIndexs].m_songs[row];
    MusicSongItem *item = &m_songItems[MUSIC_LOVEST_LIST];
    item->m_songs << song;
    item->m_itemObject->updateSongsFileName(item->m_songs);
    if(m_currentIndexs == MUSIC_LOVEST_LIST)
    {
        emit updatePlayLists(song.getMusicPath());
    }
    setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicSongsSummarizied::removeMusicSongToLovestListAt(int row)
{
    MusicSong song = m_songItems[m_currentIndexs].m_songs[row];
    MusicSongItem *item = &m_songItems[MUSIC_LOVEST_LIST];
    if(item->m_songs.removeOne(song))
    {
        item->m_itemObject->clearAllItems();
        item->m_itemObject->updateSongsFileName(item->m_songs);
        setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    }
}

void MusicSongsSummarizied::addNetMusicSongToList(const QString &name, const QString &time,
                                                  const QString &format, bool play)
{
    QString musicSong = MusicCryptographicHash().decrypt(name, DOWNLOAD_KEY);
    const QString path = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(name).arg(format);
    MusicSongItem *item = &m_songItems[MUSIC_NETWORK_LIST];
    item->m_songs << MusicSong(path, 0, time, musicSong);
    item->m_itemObject->updateSongsFileName(item->m_songs);
    if(m_currentIndexs == MUSIC_NETWORK_LIST)
    {
        emit updatePlayLists(path);
    }
    setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));

    if(play)
    {
        ///when download finished just play it at once
        MusicSongsToolBoxWidget::setCurrentIndex(MUSIC_NETWORK_LIST);
        emit musicPlayIndex(item->m_songs.count() - 1, 0);
    }
}

void MusicSongsSummarizied::addSongToPlayList(const QStringList &items)
{
    if(items.isEmpty())
    {
        return;
    }

    importOtherMusicSongs(items);
    if(m_currentIndexs == MUSIC_NORMAL_LIST)
    {
        foreach(QString var, items)
        {
            emit updatePlayLists(var);
        }
    }
    /// just play it at once
    MusicSongsToolBoxWidget::setCurrentIndex(0);
    emit musicPlayIndex(m_songItems[MUSIC_NORMAL_LIST].m_songs.count() - 1, 0);
}

void MusicSongsSummarizied::addNewRowItem()
{
    addNewRowItem( tr("defaultItem") );
}

void MusicSongsSummarizied::deleteRowItem(int index)
{
    int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    MusicSongItem item = m_songItems.takeAt(id);
    removeItem(item.m_itemObject);
    delete item.m_itemObject;
}

void MusicSongsSummarizied::changRowItemName(int index, const QString &name)
{
    int id = foundMappingIndex(index);
    if(id == -1)
    {
        return;
    }

    m_songItems[id].m_itemName = name;
}

void MusicSongsSummarizied::wheelEvent(QWheelEvent *event)
{
    MusicSongsToolBoxWidget::wheelEvent(event);
    if(m_floatWidget == nullptr)
    {
        m_floatWidget = new MusicSongsSummariziedFloatWidget(this);
        connect(m_floatWidget, SIGNAL(deleteObject()), SLOT(deleteFloatWidget()));
        m_floatWidget->setGeometry(this);
        m_floatWidget->show();
    }
}

void MusicSongsSummarizied::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsToolBoxWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("addNewItem"), this, SLOT(addNewRowItem()));
    menu.addAction(tr("importItem"));
    menu.addAction(tr("musicTest"));
    menu.addAction(tr("deleteAllItem"));
    menu.exec(QCursor::pos());
}

void MusicSongsSummarizied::setPlaybackMode(MusicObject::SongPlayType mode) const
{
    foreach(MusicSongItem item, m_songItems)
    {
        item.m_itemObject->setPlaybackMode(mode);
    }
}

void MusicSongsSummarizied::setMusicIndexSwaped(int before, int after, int play, QStringList &list)
{
    MusicSongs *names = &m_songItems[currentIndex()].m_songs;
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

    list = getMusicSongsFileName(currentIndex());
    if(currentIndex() == m_currentIndexs)
    {
        emit updateMediaLists(getMusicSongsFilePath(m_currentIndexs), play);
    }
}

void MusicSongsSummarizied::isCurrentIndexs(bool &state)
{
    state = ( currentIndex() == m_currentIndexs );
}

void MusicSongsSummarizied::isSearchFileListEmpty(bool &empty)
{
    empty = searchFileListEmpty();
}

void MusicSongsSummarizied::setCurrentMusicSongTreeIndex(int index)
{
    if(!m_songItems[m_currentIndexs].m_songs.isEmpty())
    {
        m_songItems[m_currentIndexs].m_itemObject->replacePlayWidgetRow();
    }
    m_currentIndexs = index;
}

void MusicSongsSummarizied::setCurrentIndex()
{
    QStringList keyList = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    if(keyList.count() != 3)
    {
        return;
    }
    m_currentIndexs = keyList[1].toInt();
    int index = keyList[2].toInt();
    MusicSongsToolBoxWidget::setCurrentIndex(index);
    setMusicPlayCount(index);
    emit showCurrentSong(index);
}
