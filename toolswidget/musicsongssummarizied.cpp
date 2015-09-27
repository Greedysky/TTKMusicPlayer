#include "musicsongssummarizied.h"
#include "musicsongsListwidget.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"

#include <QTableWidgetItem>
#include <QFileInfo>
#include <QLayout>
#include <QDesktopServices>

MusicSongsSummarizied::MusicSongsSummarizied(QWidget *parent) :
    QToolBox(parent),m_renameLine(NULL)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet(MusicUIObject::MToolBoxStyle01);
    for(int i=0; i<3; ++i)
    {
        MusicSongsListWidget* w = new MusicSongsListWidget(this);
        m_mainSongLists.append(w);
    }
    addItem(m_mainSongLists[0],tr("myDefaultPlayItem"));
    addItem(m_mainSongLists[1],tr("myLoveSongItem"));
    addItem(m_mainSongLists[2],tr("myNetSongItem"));
    layout()->setSpacing(0);
    changeItemIcon();
    m_currentIndexs = 0;

    for(int i=0; i<3; ++i)
    {
        connect(m_mainSongLists[i],SIGNAL(cellDoubleClicked(int,int)),parent,SLOT(musicPlayIndex(int,int)));
        connect(m_mainSongLists[i],SIGNAL(musicPlay(int)),this,SLOT(musicPlay(int)));
        connect(m_mainSongLists[i],SIGNAL(musicPlayOrder()),parent,SLOT(musicPlayOrder()));
        connect(m_mainSongLists[i],SIGNAL(musicPlayRandom()),parent,SLOT(musicPlayRandom()));
        connect(m_mainSongLists[i],SIGNAL(musicPlayListLoop()),parent,SLOT(musicPlayListLoop()));
        connect(m_mainSongLists[i],SIGNAL(musicPlayOneLoop()),parent,SLOT(musicPlayOneLoop()));
        connect(m_mainSongLists[i],SIGNAL(musicPlayItemOnce()),parent,SLOT(musicPlayItemOnce()));
        connect(m_mainSongLists[i],SIGNAL(musicAddNewFiles()),parent,SLOT(musicImportSongsOnlyFile()));
        connect(m_mainSongLists[i],SIGNAL(musicAddNewDir()),parent,SLOT(musicImportSongsOnlyDir()));
        connect(m_mainSongLists[i],SIGNAL(deleteItemAt(MIntList,bool)),SLOT(setDeleteItemAt(MIntList,bool)));
        connect(m_mainSongLists[i],SIGNAL(currentTextChanged(int,QString)),SLOT(currentTextChanged(int,QString)));
        connect(m_mainSongLists[i],SIGNAL(musicOpenFileDir(int)),SLOT(musicOpenFileDir(int)));
        connect(m_mainSongLists[i],SIGNAL(getMusicSongInformation(int,MusicSong&)),
                                     SLOT(setMusicSongInformation(int,MusicSong&)));
        connect(m_mainSongLists[i],SIGNAL(getMusicSongFileInformation(int,QString&,QString&,bool)),
                                     SLOT(setMusicSongFileInformation(int,QString&,QString&,bool)));
        connect(m_mainSongLists[i],SIGNAL(getMusicIndexSwaped(int,int,int,QStringList&)),
                                     SLOT(setMusicIndexSwaped(int,int,int,QStringList&)));
        connect(this,SIGNAL(showCurrentSong(int)),parent,SLOT(showCurrentSong(int)));
    }
    connect(m_mainSongLists[0], SIGNAL(musicSongToLovestListAt(int)), SLOT(addMusicSongToLovestListAt(int)));
    connect(m_mainSongLists[2], SIGNAL(musicSongToLovestListAt(int)), SLOT(addMusicSongToLovestListAt(int)));

    connect(this,SIGNAL(currentChanged(int)),SLOT(currentIndexChanged(int)));
    connect(this,SIGNAL(musicPlay(int,int)),parent,SLOT(musicPlayIndex(int,int)));

    M_Connection->setValue("MusicSongsSummarizied", this);
    M_Connection->connect("MusicSongSearchOnlineTableWidget", "MusicSongsSummarizied");
}

void MusicSongsSummarizied::setMusicLists(const MMusicList &names)
{
    m_musicFileNames = names;
    for(int i=0; i<m_musicFileNames.count(); ++i)
    {
        m_mainSongLists[i]->musicSongsFileName(m_musicFileNames[i].m_names);
    }
}

void MusicSongsSummarizied::setMusicSongsSearchedFileName(const MIntList &fileIndexs)
{
    QStringList t;
    for(int i=0; i<fileIndexs.count(); ++i)
    {
        t.append(m_musicFileNames[currentIndex()].m_names[fileIndexs[i]]);
    }
    m_mainSongLists[currentIndex()]->clearAllItems();
    m_mainSongLists[currentIndex()]->musicSongsFileName(t);
}

void MusicSongsSummarizied::importOtherMusicSongs(const QStringList &filelist)
{
    for(int i=0; i<filelist.count(); ++i)
    {
        QString splitString = filelist[i].split("/").last();
        m_musicFileNames[0].m_names.append(splitString.left(splitString.lastIndexOf('.')));
    }
    m_musicFileNames[0].m_paths.append(filelist);
    m_musicFileNames[0].m_playCount.append(0);
    m_mainSongLists[0]->musicSongsFileName(m_musicFileNames[0].m_names);
}

void MusicSongsSummarizied::selectRow(int index)
{
    QToolBox::setCurrentIndex(m_currentIndexs);
    m_mainSongLists[m_currentIndexs]->selectRow(index);
}

void MusicSongsSummarizied::setTimerLabel(const QString& time) const
{
    m_mainSongLists[m_currentIndexs]->setTimerLabel(time);
}

void MusicSongsSummarizied::updateArtPicture()
{
    m_mainSongLists[m_currentIndexs]->updateArtPicture();
}

void MusicSongsSummarizied::setTransparent(int alpha)
{
    alpha = alpha*2.55;
    setStyleSheet(MusicUIObject::MToolBoxStyle01 + \
          QString("QToolBox::tab{background-color:rgba(255,255,255,%1);}").arg(alpha) );
}

void MusicSongsSummarizied::setMusicPlayCount(int index)
{
    int countNumber = m_musicFileNames[m_currentIndexs].m_playCount[index];
    m_musicFileNames[m_currentIndexs].m_playCount[index] = ++countNumber;
}

QString MusicSongsSummarizied::itemText(int row, int col) const
{
    return m_mainSongLists[m_currentIndexs]->item(row,col)->text();
}

MusicSongsSummarizied::~MusicSongsSummarizied()
{
    clearAllLists();
}

void MusicSongsSummarizied::clearAllLists()
{
    while(!m_mainSongLists.isEmpty())
    {
        MusicSongsListWidget *w = m_mainSongLists.takeLast();
        delete w;
        w = NULL;
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
        m_musicFileNames[currentIndex()].m_names.removeAt(index[i]);
        if(fileRemove)
        {
            QFile::remove(m_musicFileNames[currentIndex()].m_paths[index[i]]);
        }
        m_musicFileNames[currentIndex()].m_paths.removeAt(index[i]);
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
    addItem(w,tr("newSongItem"));
    changeItemIcon();
}

void MusicSongsSummarizied::addMusicSongToLovestListAt(int row)
{
    const QString path = m_musicFileNames[currentIndex()].m_paths[row];
    m_musicFileNames[1].m_names.append(m_musicFileNames[currentIndex()].m_names[row]);
    m_musicFileNames[1].m_paths.append(path);
    m_mainSongLists[1]->musicSongsFileName(m_musicFileNames[1].m_names);
    if(m_currentIndexs == 1)
    {
        emit updatePlayLists(path);
    }
    MusicMessageBox message;
    message.setText(tr("add music to lovest list done!"));
    message.exec();
}

void MusicSongsSummarizied::addNetMusicSongToList(const QString &name)
{
    const QString path = MUSIC_DOWNLOAD + name + MUSIC_FILE;
    m_musicFileNames[2].m_names.append(name);
    m_musicFileNames[2].m_paths.append(path);
    m_mainSongLists[2]->musicSongsFileName(m_musicFileNames[2].m_names);
    if(m_currentIndexs == 2)
    {
        emit updatePlayLists(path);
    }
}

void MusicSongsSummarizied::deleteItem()
{
    int index = this->currentIndex();
    if(index == 0 || index == 1 || index == 2)
    {
        MusicMessageBox message;
        message.setText(tr("The origin one can't delete!"));
        message.exec();
        return;//Not allow to delete the origin three item
    }
    this->removeItem(index);
}

void MusicSongsSummarizied::changItemName()
{
    int index = this->currentIndex();
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
                                  QToolBox::itemText(m_renameIndex),this);
        connect(m_renameLine,SIGNAL(renameFinished(QString)),this,SLOT(setChangItemName(QString)));
        m_renameLine->show();
    }
}

void MusicSongsSummarizied::setChangItemName(const QString &name)
{
    this->setItemText(m_renameIndex, name);
    delete m_renameLine;
    m_renameLine = NULL;
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
    menu.addAction(QIcon(":/contextMenu/delete"),tr("deleteItem"), this, SLOT(deleteItem()));
    menu.addAction(QIcon(":/contextMenu/add"),tr("addNewItem"), this, SLOT(addNewItem()));
    menu.addAction(tr("changItemName"), this, SLOT(changItemName()));
    menu.exec(event->globalPos());
}

void MusicSongsSummarizied::changeItemIcon()
{
    for(int i=0; i<count(); ++i)
    {
        setItemIcon(i,QIcon(":/image/arrowup"));
    }
    setItemIcon(currentIndex(),QIcon(":/image/arrowdown"));
}

void MusicSongsSummarizied::currentTextChanged(int index, const QString &text)
{
    m_musicFileNames[currentIndex()].m_names[index] = text;
}

void MusicSongsSummarizied::musicPlay(int index)
{
    emit musicPlay(index,index);
}

void MusicSongsSummarizied::musicOpenFileDir(int index)
{
    if(!QDesktopServices::openUrl(QUrl(QFileInfo(m_musicFileNames[currentIndex()].m_paths[index]).absolutePath()
                                , QUrl::TolerantMode)))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exsit!"));
        message.exec();
    }
}

void MusicSongsSummarizied::setPlaybackMode(MusicObject::SongPlayType mode) const
{
    foreach(MusicSongsListWidget* m, m_mainSongLists)
    {
        m->setPlaybackMode(mode);
    }
}

void MusicSongsSummarizied::setMusicSongInformation(int row, MusicSong &song)
{
    QStringList l = m_musicFileNames[currentIndex()].m_names;
    if(row >= l.count())
    {
        return;
    }

    song.setMusicName(l[row]);
    QFileInfo f(m_musicFileNames[currentIndex()].m_paths[row]);
    song.setMusicSize(f.size());
    song.setMusicPlayCount(m_musicFileNames[currentIndex()].m_playCount[row]);
    song.setMusicType(f.suffix());
}

void MusicSongsSummarizied::setMusicSongFileInformation(int row, QString &name, QString &path, bool st)
{
    int index;
    st ? index = m_currentIndexs : index = currentIndex();
    QStringList l = m_musicFileNames[index].m_names;
    if(row >= l.count())
    {
        return;
    }

    l.isEmpty() ? name = "" : name = l[row];
    l = m_musicFileNames[index].m_paths;
    l.isEmpty() ? path = "" : path = l[row];
}

void MusicSongsSummarizied::setMusicIndexSwaped(int before, int after, int play, QStringList &list)
{
    QStringList *names = &m_musicFileNames[currentIndex()].m_names;
    QStringList *paths = &m_musicFileNames[currentIndex()].m_paths;

    if(before > after)
    {
        for(int i=before; i>after; --i)
        {
            names->swap(i, i-1);
            paths->swap(i, i-1);
        }
    }
    else
    {
        for(int i=before; i<after; ++i)
        {
            names->swap(i, i+1);
            paths->swap(i, i+1);
        }
    }

    list = *names;
    if(currentIndex() == m_currentIndexs)
    {
        emit updateMediaLists(*paths, play);
    }
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
    QToolBox::setCurrentIndex(keyList[2].toInt());
    emit showCurrentSong(keyList[2].toInt());
}
