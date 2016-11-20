#include "musicapplication.h"
#include "musicsettingmanager.h"
#include "musicdownloadstatuslabel.h"
#include "musiccoreutils.h"
#include "musicnetworkthread.h"
#include "musicbackgroundmanager.h"

#include <QTimer>
#include <QQuickWindow>
#include <QQmlContext>

#include "ttkmusicsongssummarizied.h"
#include "ttkmusicplaylist.h"
#include "ttkmusicplayer.h"
#include "ttkmusicutils.h"
#include "ttkmusicconfigmanager.h"
#include "ttknetworkhelper.h"
#include "ttkfilesearchcore.h"
#include "ttkmusiclyricmodel.h"
#include "ttkradiohelper.h"
#if defined (Q_OS_ANDROID)
#include "QZXing.h"
#endif

MusicApplication::MusicApplication(QQmlContext *parent)
    : QObject(parent)
{
    M_NETWORK_PTR->start();
    ///////////////////////////////////////////////////////////////////////////////////
    qmlRegisterType<TTKRadioHelper>("TTKRadioHelper", 1, 0, "TTKRadioHelper");
    qmlRegisterType<TTKFileSearchCore>("TTKFileSearchCore", 1, 0, "TTKFileSearchCore");
#if defined (Q_OS_ANDROID)
    QZXing::registerQMLTypes();
#endif
    ///////////////////////////////////////////////////////////////////////////////////
    m_ttkUtils =  new TTKMusicUtils(this);
    m_ttkPlaylist = new TTKMusicPlaylist(this);
    m_ttkPlayer = new TTKMusicPlayer(this);
    m_networkHelper = new TTKNetworkHelper(this);
    m_ttkLrcModel = new TTKMusicLyricModel(this);
    m_ttkPlayer->setPlaylist(m_ttkPlaylist);

    m_songsSummarizied = new TTKMusicSongsSummarizied(this);
    m_downloadStatus = new MusicDownloadStatusLabel(this);

    m_timeToQuitTimer = new QTimer(this);
    m_timeToQuitTimer->setInterval(-1);
    connect(m_timeToQuitTimer, SIGNAL(timeout()), qApp, SLOT(quit()));

    parent->setContextProperty("TTK_APP", this);
    parent->setContextProperty("TTK_UTILS", m_ttkUtils);
    parent->setContextProperty("TTK_PLAYER", m_ttkPlayer);
    parent->setContextProperty("TTK_NETWORK", m_networkHelper);
    parent->setContextProperty("TTK_LRC", m_ttkLrcModel);

    connect(M_BACKGROUND_PTR, SIGNAL(userSelectIndexChanged()), SIGNAL(updateCurrentBgArtist()));
    connect(m_ttkPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(currentMusicSongChanged(int)));
    readXMLConfigFromText();
}

MusicApplication::~MusicApplication()
{
    writeXMLConfigToText();
    delete m_networkHelper;
    delete m_ttkUtils;
    delete m_ttkPlaylist;
    delete m_ttkPlayer;
    delete m_ttkLrcModel;
    delete m_songsSummarizied;
    delete m_timeToQuitTimer;
}

void MusicApplication::importOtherMusicSongs(const QStringList &filelist)
{
    m_songsSummarizied->importOtherMusicSongs(MUSIC_NORMAL_LIST, filelist);
    if(m_songsSummarizied->getCurrentIndex() == MUSIC_NORMAL_LIST)
    {
        m_ttkPlaylist->appendMedia(filelist);
    }
    emit importSongFinished(MUSIC_NORMAL_LIST);
}

void MusicApplication::importLovestMusicSongs()
{
    QString filePath = m_ttkPlaylist->currentMediaString();
    if(!checkLovestMusicSong())
    {
        m_songsSummarizied->importOtherMusicSongs(MUSIC_LOVEST_LIST, QStringList(filePath));
        if(m_songsSummarizied->getCurrentIndex() == MUSIC_LOVEST_LIST)
        {
            m_ttkPlaylist->appendMedia(filePath);
        }
    }
    else
    {
        if(m_songsSummarizied->getCurrentIndex() == MUSIC_LOVEST_LIST)
        {
            m_ttkPlaylist->removeMedia(filePath);
        }
        m_songsSummarizied->removeMusicSongs(MUSIC_LOVEST_LIST, filePath);
    }
    emit importSongFinished(MUSIC_LOVEST_LIST);
}

void MusicApplication::importDownloadMusicSongs(int type, const QString &path)
{
    m_songsSummarizied->importOtherMusicSongs(type, QStringList(path));
    if(m_songsSummarizied->getCurrentIndex() == type)
    {
        m_ttkPlaylist->appendMedia(path);
    }
    emit importSongFinished(type);
}

void MusicApplication::importNetworkMusicSongs(const QString &key, const QString &path)
{
    m_songsSummarizied->importNetworkMusicSongs(key, path);
    m_songsSummarizied->setCurrentIndex(MUSIC_RECENT_LIST);
    m_songsSummarizied->setToolBoxIndex(MUSIC_RECENT_LIST);
    m_ttkPlaylist->addMedia( m_songsSummarizied->getMusicSongsFilePath(MUSIC_RECENT_LIST) );
    m_ttkPlaylist->setCurrentIndex( m_ttkPlaylist->mediaCount() - 1);
}

void MusicApplication::removeMusicSongs()
{
    if(m_ttkPlaylist->isEmpty())
    {
        return;
    }

    int index = m_ttkPlaylist->currentIndex();
    removeMusicSongs(m_songsSummarizied->getCurrentIndex(), m_songsSummarizied->getCurrentIndex(), index);

    emit removeItemFromPlayerCenter(index);
    if(m_ttkPlaylist->isEmpty())
    {
        emit emptyPlayerCenter(true);
    }
}

void MusicApplication::removeMusicSongs(int index)
{
    if(m_ttkPlaylist->isEmpty())
    {
        return;
    }

    removeMusicSongs(m_songsSummarizied->getToolBoxIndex(), m_songsSummarizied->getCurrentIndex(), index);
    if(m_ttkPlaylist->isEmpty())
    {
        emit emptyPlayerCenter(false);
    }
}

bool MusicApplication::checkLovestMusicSong() const
{
    return m_songsSummarizied->getMusicSongsFilePath(MUSIC_LOVEST_LIST).contains(m_ttkPlaylist->currentMediaString());
}

bool MusicApplication::empty() const
{
    return m_ttkPlaylist->currentIndex() < MUSIC_NORMAL_LIST;
}

int MusicApplication::mediaCount(int index) const
{
    MusicSongItems items(m_songsSummarizied->getMusicLists());
    if(index < 0 || index >= items.count())
    {
        return 0;
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
    if(index < 0 || index >= items.count())
    {
        return QStringList();
    }

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
    if(index < 0 || index >= items.count())
    {
        return QStringList();
    }

    QStringList list;
    foreach(const MusicSong &song, items[index].m_songs)
    {
        list.append(song.getMusicArtistFront());
    }
    return list;
}

QStringList MusicApplication::mediaPaths(int index) const
{
    MusicSongItems items(m_songsSummarizied->getMusicLists());
    if(index < 0 || index >= items.count())
    {
        return QStringList();
    }

    QStringList list;
    foreach(const MusicSong &song, items[index].m_songs)
    {
        list.append(song.getMusicPath());
    }
    return list;
}

QString MusicApplication::mediaPath(int tool, int index) const
{
    QStringList paths( mediaPaths(tool) );
    return (index < 0 || index >= paths.count()) ? QString() : paths[index];
}

int MusicApplication::mediaPlayCount(int index) const
{
    MusicSongItems items(m_songsSummarizied->getMusicLists());
    MusicSongs songs(items[MUSIC_RECENT_LIST].m_songs);
    if(index < 0 || index >= songs.count())
    {
        return 1;
    }

    return songs[index].getMusicPlayCount();
}

QString MusicApplication::artistImagePath() const
{
    QString name = MusicUtils::Core::artistName( getCurrentFileName() );
    name = ART_DIR_FULL + name + SKN_FILE;
    return QFile::exists(name) ? "file:///" + name : QString();
}

QString MusicApplication::artistBgImagePath() const
{
    return "file:///" + M_BACKGROUND_PTR->getMBackground();
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

void MusicApplication::setCurrentIndex(int id, int index)
{
    int preId = m_songsSummarizied->getCurrentIndex();
    if(preId != id)
    {
        m_ttkPlaylist->addMedia(m_songsSummarizied->getMusicSongsFilePath(id));
    }

    m_songsSummarizied->setCurrentIndex(id);
    if(index < MUSIC_NORMAL_LIST || index >= m_ttkPlaylist->mediaCount())
    {
        return;
    }
    m_ttkPlaylist->setCurrentIndex(index);
}

int MusicApplication::getCurrentIndex()
{
    return m_songsSummarizied->getCurrentIndex() == m_songsSummarizied->getToolBoxIndex() ?
           m_ttkPlaylist->currentIndex() : -1;
}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return (m_ttkPlaylist->currentIndex() == -1);
}

QString MusicApplication::getCurrentFileName() const
{
    if(m_ttkPlaylist->currentIndex() < 0)
    {
        return QString();
    }

    QStringList nanmes = m_songsSummarizied->getMusicSongsFileName(m_songsSummarizied->getCurrentIndex());
    return (nanmes.count() > m_ttkPlaylist->currentIndex()) ? nanmes[m_ttkPlaylist->currentIndex()].trimmed() : QString();
}

void MusicApplication::musicLoadCurrentSongLrc()
{
    //Loading the current song lrc
    if(m_ttkPlaylist->currentIndex() == -1)
    {
        return;
    }

    QString filename = getCurrentFileName();
    QString path = MusicUtils::Core::lrcPrefix() + filename + LRC_FILE;
    m_ttkLrcModel->loadCurrentSongLrc(path);
}

bool MusicApplication::timeToQuitAppIsSet() const
{
    return m_timeToQuitTimer->interval() != -1;
}

void MusicApplication::setTimeToQuitApp(int time) const
{
    m_timeToQuitTimer->stop();
    if(time > -1)
    {
        m_timeToQuitTimer->setInterval(time);
        m_timeToQuitTimer->start();
    }
    else
    {
        m_timeToQuitTimer->setInterval(-1);
    }
}

void MusicApplication::currentMusicSongChanged(int index)
{
    m_ttkLrcModel->clear();
    m_downloadStatus->musicCheckHasLrcAlready();
    QString path = QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL).arg(mediaArtist()).arg(0).arg(SKN_FILE);
    M_BACKGROUND_PTR->setMBackground(path);
    m_songsSummarizied->setRecentMusicSongs(index);
    emit currentIndexChanged(index);
}

void MusicApplication::removeMusicSongs(int tool, int current, int index)
{
    m_songsSummarizied->removeMusicSongs(tool, index);
    if(tool == current)
    {
        m_ttkPlaylist->removeMedia(index);
        if(m_ttkPlaylist->currentIndex() == index)
        {
            m_ttkPlaylist->playNext();
        }
        else if(index < m_ttkPlaylist->currentIndex())
        {
            index = m_ttkPlaylist->currentIndex() - 1;
            m_ttkPlaylist->blockSignals(true);
            m_ttkPlaylist->setCurrentIndex(index < 0 ? 0 : index);
            m_ttkPlaylist->blockSignals(false);
        }

        if(m_ttkPlaylist->isEmpty())
        {
            m_ttkPlaylist->setCurrentIndex(-1);
            m_ttkPlayer->pause();
        }
    }
    emit updateItemShowCount();
}

void MusicApplication::readXMLConfigFromText()
{
    TTKMusicConfigManager xml;
    int value = -1;

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

    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorChoiced, xml.readShowLrcColor());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFgColorChoiced, xml.readShowLrcFgColor());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcTypeChoiced, xml.readShowLrcType());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, xml.readShowLrcSize());

    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced, 0);
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerMultipleChoiced, 1);
    M_SETTING_PTR->setValue(MusicSettingManager::ShowInlineLrcChoiced, 1);
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, 1);

    //////////////////////////////////////////////////////////////
    //The size of the volume of the allocation of songs
    value = xml.readMusicPlayVolumeConfig();
    M_SETTING_PTR->setValue(MusicSettingManager::VolumeChoiced, value);
    m_ttkPlayer->setMusicEnhanced(xml.readEnhancedMusicConfig());
    value = xml.readEqualizerEnable();
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnableChoiced, value);
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerValueChoiced, xml.readEqualizerValue());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndexChoiced, xml.readEqualizerIndex());
    if(value == 1)
    {
        m_ttkPlayer->setEqInformation();
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

    m_ttkPlayer->getEqEffectSettings();
    M_SETTING_PTR->setValue(MusicSettingManager::VolumeChoiced, m_ttkPlayer->volume());
    M_SETTING_PTR->setValue(MusicSettingManager::PlayModeChoiced, playbackMode());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicChoiced, m_ttkPlayer->getMusicEnhanced());
    QStringList keyList = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    if(keyList.isEmpty())
    {
        keyList << "0" << "0" << "-1";
    }
    else
    {
        keyList[1] = QString::number(m_songsSummarizied->getCurrentIndex());
        keyList[2] = QString::number(m_ttkPlaylist->currentIndex());
    }
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, keyList);
    xml.writeXMLConfig();
    xml.writeMusicSongsConfig(m_songsSummarizied->getMusicLists(), MUSICPATH_FULL);
}
