#include "musicapplication.h"
#include "musicsettingmanager.h"
#include <QQmlContext>

#include "core/ttkmusicsongssummarizied.h"
#include "core/ttkmusicplaylist.h"
#include "core/ttkmusicplayer.h"
#include "core/ttkmusicutils.h"
#include "core/ttkmusicconfigmanager.h"

MusicApplication::MusicApplication(QQmlContext *parent)
    : QObject(parent)
{
    m_ttkUtils =  new TTKMusicUtils(this);
    m_ttkPlaylist = new TTKMusicPlaylist(this);
    m_ttkPlayer = new TTKMusicPlayer(this);
    m_ttkPlayer->setPlaylist(m_ttkPlaylist);

    m_songsSummarizied = new TTKMusicSongsSummarizied(this);

    parent->setContextProperty("TTK_APP", this);
    parent->setContextProperty("TTK_UTILS", m_ttkUtils);
    parent->setContextProperty("TTK_PLAYER", m_ttkPlayer);

    connect(m_ttkPlaylist, SIGNAL(currentIndexChanged(int)), SIGNAL(currentIndexChanged(int)));
    readXMLConfigFromText();
}

MusicApplication::~MusicApplication()
{
    writeXMLConfigToText();
    delete m_ttkUtils;
    delete m_ttkPlaylist;
    delete m_ttkPlayer;
    delete m_songsSummarizied;
}

void MusicApplication::importOtherMusicSongs(const QStringList &filelist)
{
    m_songsSummarizied->importOtherMusicSongs(filelist);
    if(m_songsSummarizied->getCurrentIndex() == 0)
    {
        m_ttkPlaylist->appendMedia(filelist);
        emit importSongFinished();
    }
}

bool MusicApplication::empty() const
{
    return m_ttkPlaylist->currentIndex() < 0;
}

int MusicApplication::mediaCount(int index) const
{
    MusicSongItems items(m_songsSummarizied->getMusicLists());
    if(index < 0 || index >= items.count())
    {
        return -1;
    }
    return items[index].m_songs.count();
}

QString MusicApplication::mediaName() const
{
    int index = m_songsSummarizied->getCurrentIndex();
    MusicSongItem item(m_songsSummarizied->getMusicList(index));
    if(item.m_songs.isEmpty() || m_ttkPlaylist->currentIndex() < 0)
    {
        return QString();
    }
    return item.m_songs[m_ttkPlaylist->currentIndex()].getMusicArtistBack();
}

QString MusicApplication::mediaArtist() const
{
    int index = m_songsSummarizied->getCurrentIndex();
    MusicSongItem item(m_songsSummarizied->getMusicList(index));
    if(item.m_songs.isEmpty() || m_ttkPlaylist->currentIndex() < 0)
    {
        return QString();
    }
    return item.m_songs[m_ttkPlaylist->currentIndex()].getMusicArtistFront();
}

QStringList MusicApplication::mediaNames(int index) const
{
    MusicSongItems items(m_songsSummarizied->getMusicLists());
    QStringList list;
    foreach(const MusicSong &song, items[index].m_songs)
    {
        list.append(song.getMusicArtistBack());
    }
    return list;
}

QStringList MusicApplication::mediaArtists(int index) const
{
    MusicSongItems items(m_songsSummarizied->getMusicLists());
    QStringList list;
    foreach(const MusicSong &song, items[index].m_songs)
    {
        list.append(song.getMusicArtistFront());
    }
    return list;
}

int MusicApplication::MusicApplication::playbackMode() const
{
    return m_ttkPlaylist->playbackMode();
}

void MusicApplication::setPlaybackMode(int mode)
{
    m_ttkPlaylist->setPlaybackMode(mode);
}

void MusicApplication::playNext()
{
    m_ttkPlaylist->playNext();
}

void MusicApplication::playPrevious()
{
    m_ttkPlaylist->playPrevious();
}

void MusicApplication::setToolBoxIndex(int index)
{
    m_songsSummarizied->setToolBoxIndex(index);
}

void MusicApplication::setCurrentIndex(int index)
{
    if(m_songsSummarizied->getToolBoxIndex() != m_songsSummarizied->getCurrentIndex())
    {
        m_songsSummarizied->setCurrentIndex(m_songsSummarizied->getToolBoxIndex());
    }
    if(index < 0 || index >= m_ttkPlaylist->mediaCount())
    {
        return;
    }
    m_ttkPlaylist->setCurrentIndex(index);
}

void MusicApplication::readXMLConfigFromText()
{
    TTKMusicConfigManager xml;
//    int value = -1;

    MusicSongItems songs;
    if(xml.readMusicXMLConfig(MUSICPATH_FULL))
    {
        xml.readMusicSongsConfig(songs);
    }
    m_songsSummarizied->addMusicLists(songs);
    //////////////////////////////////////////////////////////////
    if(!xml.readXMLConfig(COFIGPATH_FULL))
    {
        return;
    }

    setPlaybackMode(xml.readMusicPlayModeConfig());
    //Configuration from next time also stopped at the last record.
    QStringList keyList;
    xml.readSystemLastPlayIndexConfig(keyList);
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, keyList);
    //add new music file to playlist
    m_ttkPlaylist->addMedia(m_songsSummarizied->getMusicSongsFilePath(keyList[1].toInt()));
    if(keyList[0] == "1")
    {
        m_songsSummarizied->setToolBoxIndex(keyList[1].toInt());
        m_songsSummarizied->setCurrentIndex(keyList[1].toInt());
        m_ttkPlaylist->setCurrentIndex(keyList[2].toInt());
    }
}

void MusicApplication::writeXMLConfigToText()
{
    TTKMusicConfigManager xml;
    M_SETTING_PTR->setValue(MusicSettingManager::PlayModeChoiced, playbackMode());
    QStringList lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    if(lastPlayIndexChoiced.isEmpty())
    {
        lastPlayIndexChoiced << "0" << "0" << "-1";
    }
    else
    {
        lastPlayIndexChoiced[1] = QString::number(m_songsSummarizied->getCurrentIndex());
        lastPlayIndexChoiced[2] = QString::number(m_ttkPlaylist->currentIndex());
    }
    xml.writeXMLConfig();
    xml.writeMusicSongsConfig(m_songsSummarizied->getMusicLists(), MUSICPATH_FULL);
}
