#include "musicsongssummarizied.h"
#include "musicsongsListwidget.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicsongtag.h"
#include "musicprogresswidget.h"

#include <QScrollBar>
#include <QTableWidgetItem>
#include <QLayout>

MusicSongsSummarizied::MusicSongsSummarizied(QWidget *parent)
    : QToolBox(parent), m_renameLine(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet(MusicUIObject::MToolBoxStyle01);
    for(int i=0; i<3; ++i)
    {
        MusicSongsListWidget* w = new MusicSongsListWidget(this);
        m_mainSongLists.append(w);
    }
    addItem(m_mainSongLists[0], tr("myDefaultPlayItem"));
    addItem(m_mainSongLists[1], tr("myLoveSongItem"));
    addItem(m_mainSongLists[2], tr("myNetSongItem"));
    layout()->setSpacing(0);
    changeItemIcon();
    m_currentIndexs = 0;

    for(int i=0; i<3; ++i)
    {
        connect(m_mainSongLists[i], SIGNAL(cellDoubleClicked(int,int)), parent, SLOT(musicPlayIndex(int,int)));
        connect(m_mainSongLists[i], SIGNAL(musicPlayOrder()), parent, SLOT(musicPlayOrder()));
        connect(m_mainSongLists[i], SIGNAL(musicPlayRandom()), parent, SLOT(musicPlayRandom()));
        connect(m_mainSongLists[i], SIGNAL(musicPlayListLoop()), parent, SLOT(musicPlayListLoop()));
        connect(m_mainSongLists[i], SIGNAL(musicPlayOneLoop()), parent, SLOT(musicPlayOneLoop()));
        connect(m_mainSongLists[i], SIGNAL(musicPlayItemOnce()), parent, SLOT(musicPlayItemOnce()));
        connect(m_mainSongLists[i], SIGNAL(musicAddNewFiles()), parent, SLOT(musicImportSongsOnlyFile()));
        connect(m_mainSongLists[i], SIGNAL(musicAddNewDir()), parent, SLOT(musicImportSongsOnlyDir()));
        connect(m_mainSongLists[i], SIGNAL(isCurrentIndexs(bool&)), SLOT(isCurrentIndexs(bool&)));
        connect(m_mainSongLists[i], SIGNAL(deleteItemAt(MIntList,bool)), SLOT(setDeleteItemAt(MIntList,bool)));
        connect(m_mainSongLists[i], SIGNAL(getMusicIndexSwaped(int,int,int,QStringList&)),
                                    SLOT(setMusicIndexSwaped(int,int,int,QStringList&)));
    }

    connect(this, SIGNAL(showCurrentSong(int)), parent, SLOT(showCurrentSong(int)));
    connect(m_mainSongLists[0], SIGNAL(musicSongToLovestListAt(int)), SLOT(addMusicSongToLovestListAt(int)));
    connect(m_mainSongLists[2], SIGNAL(musicSongToLovestListAt(int)), SLOT(addMusicSongToLovestListAt(int)));

    connect(this, SIGNAL(currentChanged(int)), SLOT(currentIndexChanged(int)));

    M_CONNECTION->setValue("MusicSongsSummarizied", this);
    M_CONNECTION->connect("MusicSongSearchOnlineTableWidget", "MusicSongsSummarizied");
}

MusicSongsSummarizied::~MusicSongsSummarizied()
{
    clearAllLists();
}

void MusicSongsSummarizied::setMusicLists(const MusicSongsList &names)
{
    m_musicFileNames = names;
    for(int i=0; i<m_musicFileNames.count(); ++i)
    {
        m_mainSongLists[i]->setSongsFileName(&m_musicFileNames[i]);
    }
}

void MusicSongsSummarizied::setMusicSongsSearchedFileName(const MIntList &fileIndexs)
{
    MusicSongs songs;
    for(int i=0; i<fileIndexs.count(); ++i)
    {
        songs << m_musicFileNames[currentIndex()][fileIndexs[i]];
    }
    m_mainSongLists[currentIndex()]->clearAllItems();
    m_mainSongLists[currentIndex()]->updateSongsFileName(songs);
}

void MusicSongsSummarizied::importOtherMusicSongs(const QStringList &filelist)
{
    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Import File Mode"));
    progress.setRange(0, filelist.count());

    MusicSongTag tag;
    for(int i=0; i<filelist.count(); ++i)
    {
        tag.readFile(filelist[i]);
        QString time(tag.getLengthString());
        m_musicFileNames[0] << MusicSong(filelist[i], 0, time.left(time.lastIndexOf(':')), QString());
        progress.setValue(i + 1);
    }
    m_mainSongLists[0]->updateSongsFileName(m_musicFileNames[0]);
}

QStringList MusicSongsSummarizied::getMusicSongsFileName(int index) const
{
    QStringList list;
    MusicSongs songs = m_musicFileNames[index];
    for(int i=0; i<songs.count(); ++i)
    {
        list << songs[i].getMusicName();
    }
    return list;
}

QStringList MusicSongsSummarizied::getMusicSongsFilePath(int index) const
{
    QStringList list;
    MusicSongs songs = m_musicFileNames[index];
    for(int i=0; i<songs.count(); ++i)
    {
        list << songs[i].getMusicPath();
    }
    return list;
}

void MusicSongsSummarizied::selectRow(int index)
{
    QToolBox::setCurrentIndex(m_currentIndexs);
    m_mainSongLists[m_currentIndexs]->selectRow(index);
}

void MusicSongsSummarizied::setTimerLabel(const QString &time) const
{
    m_mainSongLists[m_currentIndexs]->setTimerLabel(time);
}

void MusicSongsSummarizied::setTransparent(int alpha)
{
    alpha = alpha*2.55;
    setStyleSheet(MusicUIObject::MToolBoxStyle01 + \
                  QString("QToolBox::tab{background-color:rgba(255,255,255,%1);}").arg(alpha) );
}

void MusicSongsSummarizied::setMusicPlayCount(int index)
{
    MusicSongs *songs = &m_musicFileNames[m_currentIndexs];
    if(!songs->isEmpty())
    {
        int countNumber = (*songs)[index].getMusicPlayCount();
        (*songs)[index].setMusicPlayCount(++countNumber);
    }
}

void MusicSongsSummarizied::clearAllLists()
{
    while(!m_mainSongLists.isEmpty())
    {
        MusicSongsListWidget *w = m_mainSongLists.takeLast();
        delete w;
        w = nullptr;
    }
}

void MusicSongsSummarizied::setDeleteItemAt(const MIntList &index, bool fileRemove)
{
    if(index.count() == 0)
    {
        return;
    }
    for(int i=index.count() - 1; i>=0; --i)
    {
        MusicSong song = m_musicFileNames[currentIndex()].takeAt(index[i]);
        if(fileRemove)
        {
            QFile::remove(song.getMusicPath());
        }
    }
    if(currentIndex() == m_currentIndexs)
    {
        emit deleteItemAt(index);
    }
}

void MusicSongsSummarizied::addNewItem()
{
    if(m_mainSongLists.count() > 8)
    {
        return;
    }

    MusicSongsListWidget *w = new MusicSongsListWidget(this);
    m_mainSongLists.append(w);
    addItem(w, tr("newSongItem"));
    changeItemIcon();
}

void MusicSongsSummarizied::addMusicSongToLovestListAt(int row)
{
    MusicSong song = m_musicFileNames[currentIndex()][row];
    m_musicFileNames[1] << song;
    m_mainSongLists[1]->updateSongsFileName(m_musicFileNames[1]);
    if(m_currentIndexs == 1)
    {
        emit updatePlayLists(song.getMusicPath());
    }
    MusicMessageBox message;
    message.setText(tr("add music to lovest list done!"));
    message.exec();
}

void MusicSongsSummarizied::addNetMusicSongToList(const QString &name, const QString &time,
                                                  const QString &format)
{
    const QString path = QString("%1%2.%3").arg(MUSIC_DOWNLOAD_AL).arg(name).arg(format);
    m_musicFileNames[2] << MusicSong(path, 0, time, name);
    m_mainSongLists[2]->updateSongsFileName(m_musicFileNames[2]);
    if(m_currentIndexs == 2)
    {
        emit updatePlayLists(path);
    }
}

void MusicSongsSummarizied::deleteItem()
{
    int index = currentIndex();
    if(index == 0 || index == 1 || index == 2)
    {
        MusicMessageBox message;
        message.setText(tr("The origin one can't delete!"));
        message.exec();
        return;//Not allow to delete the origin three item
    }
    removeItem(index);
}

void MusicSongsSummarizied::changItemName()
{
    int index = currentIndex();
    if(index == 0 || index == 1 || index == 2)
    {
        MusicMessageBox message;
        message.setText(tr("The origin one can't rename!"));
        message.exec();
        return;//Not allow to change name for the origin three item
    }
    if(!m_renameLine)
    {
        m_renameIndex = currentIndex();
        m_renameLine =new MusicSongsToolItemRenamedWidget(m_renameIndex*26,
                                  QToolBox::itemText(m_renameIndex), this);
        connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
        m_renameLine->show();
    }
}

void MusicSongsSummarizied::setChangItemName(const QString &name)
{
    setItemText(m_renameIndex, name);
    delete m_renameLine;
    m_renameLine = nullptr;
}

void MusicSongsSummarizied::currentIndexChanged(int)
{
    if(currentIndex() == -1)
    {
        return;
    }
    changeItemIcon();
    if(m_renameLine)
    {
        m_renameLine->renameFinished();
    }
}

void MusicSongsSummarizied::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(QIcon(":/contextMenu/delete"), tr("deleteItem"), this, SLOT(deleteItem()));
    menu.addAction(QIcon(":/contextMenu/add"), tr("addNewItem"), this, SLOT(addNewItem()));
    menu.addAction(tr("changItemName"), this, SLOT(changItemName()));
    menu.exec(event->globalPos());
}

void MusicSongsSummarizied::changeItemIcon()
{
    for(int i=0; i<count(); ++i)
    {
        setItemIcon(i, QIcon(":/image/arrowup"));
    }
    setItemIcon(currentIndex(), QIcon(":/image/arrowdown"));
}

void MusicSongsSummarizied::setPlaybackMode(MusicObject::SongPlayType mode) const
{
    foreach(MusicSongsListWidget *m, m_mainSongLists)
    {
        m->setPlaybackMode(mode);
    }
}

void MusicSongsSummarizied::setMusicIndexSwaped(int before, int after, int play, QStringList &list)
{
    MusicSongs *names = &m_musicFileNames[currentIndex()];
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

void MusicSongsSummarizied::currentMusicSongTreeIndexChanged(int index)
{
    m_mainSongLists[m_currentIndexs]->replacePlayWidgetRow();
    m_currentIndexs = index;
}

void MusicSongsSummarizied::setCurrentIndex()
{
    QStringList keyList = M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    m_currentIndexs = keyList[1].toInt();
    int index = keyList[2].toInt();
    QToolBox::setCurrentIndex(index);
    setMusicPlayCount(index);
    emit showCurrentSong(index);
}
