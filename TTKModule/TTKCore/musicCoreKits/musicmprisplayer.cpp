#include "musicmprisplayer.h"
#include "musicapplication.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "musictime.h"

#include <QDBusMessage>
#include <qmmp/soundcore.h>
#include <qmmp/metadatamanager.h>
#include <qmmp/abstractengine.h>

MusicMPRISPlayer::MusicMPRISPlayer(QObject *parent)
    : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    //MPRISv2.0
    m_root = new MusicMPRISPlayerRoot(this);
    m_player = new MusicMPRISPlayerBase(this);
    //
    connection.registerService("org.mpris.MediaPlayer2." APP_NAME);
    connection.registerObject("/org/mpris/MediaPlayer2", this);
}

MusicMPRISPlayer::~MusicMPRISPlayer()
{
    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2." APP_NAME);
    delete m_root;
    delete m_player;
}

void MusicMPRISPlayer::run()
{
    ///do nothing
}



MusicMPRISPlayerRoot::MusicMPRISPlayerRoot(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{

}

bool MusicMPRISPlayerRoot::canQuit() const
{
    return true;
}

bool MusicMPRISPlayerRoot::canRaise() const
{
    return true;
}

QString MusicMPRISPlayerRoot::desktopEntry() const
{
    return APP_NAME;
}

bool MusicMPRISPlayerRoot::hasTrackList() const
{
    return false;
}

QString MusicMPRISPlayerRoot::identity() const
{
    return APP_NAME;
}

QStringList MusicMPRISPlayerRoot::supportedMimeTypes() const
{
    QStringList mimeTypes;
    mimeTypes << Decoder::contentTypes();
    mimeTypes << AbstractEngine::contentTypes();
    mimeTypes.removeDuplicates();
    return mimeTypes;
}

QStringList MusicMPRISPlayerRoot::supportedUriSchemes() const
{
    QStringList protocols = MetaDataManager::instance()->protocols();
    if(!protocols.contains("file")) //append file if needed
    {
        protocols.append("file");
    }
    return protocols;
}

void MusicMPRISPlayerRoot::Quit()
{
    MusicApplication::instance()->quitWindowClose();
}

void MusicMPRISPlayerRoot::Raise()
{
    MusicApplication::instance()->show();
}



MusicMPRISPlayerBase::MusicMPRISPlayerBase(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    MusicTime::initRandom();

    m_prev_track = 0;
    m_prev_pos = 0;
    m_music = SoundCore::instance();
    m_player = MusicApplication::instance();

    connect(m_music, SIGNAL(trackInfoChanged()), SLOT(trackInfoChanged()));
    connect(m_music, SIGNAL(stateChanged(Qmmp::State)), SLOT(stateChanged()));
    connect(m_music, SIGNAL(volumeChanged(int)), SLOT(volumeChanged()));
    connect(m_music, SIGNAL(elapsedChanged(qint64)), SLOT(elapsedChanged(qint64)));
    connect(m_player->m_musicPlaylist, SIGNAL(playbackModeChanged(MusicObject::PlayMode)), SLOT(playbackModeChanged()));

    updateTrackID();
    syncProperties();
}

bool MusicMPRISPlayerBase::canControl() const
{
    return true;
}

bool MusicMPRISPlayerBase::canGoNext() const
{
    return !m_player->m_musicPlaylist->isEmpty();
}

bool MusicMPRISPlayerBase::canGoPrevious() const
{
    return !m_player->m_musicPlaylist->isEmpty();
}

bool MusicMPRISPlayerBase::canPause() const
{
    return !m_player->m_musicPlaylist->isEmpty();
}
bool MusicMPRISPlayerBase::canPlay() const
{
    return !m_player->m_musicPlaylist->isEmpty();
}

bool MusicMPRISPlayerBase::canSeek() const
{
    return m_music->duration() > 0;
}

QString MusicMPRISPlayerBase::loopStatus() const
{
    switch(m_player->getPlayMode())
    {
        case MusicObject::PlayOneLoop: return "Track";
        case MusicObject::PlaylistLoop: return "Playlist";
        default: return "None";
    }
}

void MusicMPRISPlayerBase::setLoopStatus(const QString &value)
{
    if(value == "Track")
    {
        m_player->musicPlayOneLoop();
    }
    else if(value == "Playlist")
    {
        m_player->musicPlaylistLoop();
    }
    else
    {
        m_player->musicPlayOrder();
    }
}

double MusicMPRISPlayerBase::maximumRate() const
{
    return 1.0;
}

QVariantMap MusicMPRISPlayerBase::metadata() const
{
    if(m_music->path().isEmpty())
    {
        return QVariantMap();
    }

    QVariantMap map;
    TrackInfo info = m_music->trackInfo();
    map["mpris:length"] = qMax(m_music->duration() * 1000 , qint64(0));
    if(!MetaDataManager::instance()->getCoverPath(info.path()).isEmpty())
    {
        map["mpris:artUrl"] = QUrl::fromLocalFile(MetaDataManager::instance()->getCoverPath(info.path())).toString();
    }

    if(!info.value(Qmmp::ALBUM).isEmpty())
    {
        map["xesam:album"] = info.value(Qmmp::ALBUM);
    }

    if(!info.value(Qmmp::ARTIST).isEmpty())
    {
        map["xesam:artist"] = QStringList() << info.value(Qmmp::ARTIST);
    }

    if(!info.value(Qmmp::ALBUMARTIST).isEmpty())
    {
        map["xesam:albumArtist"] = QStringList() << info.value(Qmmp::ALBUMARTIST);
    }

    if(!info.value(Qmmp::COMMENT).isEmpty())
    {
        map["xesam:comment"] = QStringList() << info.value(Qmmp::COMMENT);
    }

    if(!info.value(Qmmp::COMPOSER).isEmpty())
    {
        map["xesam:composer"] = QStringList() << info.value(Qmmp::COMPOSER);
    }

    if(!info.value(Qmmp::DISCNUMBER).isEmpty())
    {
        map["xesam:discNumber"] = info.value(Qmmp::DISCNUMBER).toInt();
    }

    if(!info.value(Qmmp::GENRE).isEmpty())
    {
        map["xesam:genre"] = QStringList() << info.value(Qmmp::GENRE);
    }

    if(!info.value(Qmmp::TITLE).isEmpty())
    {
        map["xesam:title"] = info.value(Qmmp::TITLE);
    }

    if(!info.value(Qmmp::TRACK).isEmpty())
    {
        map["xesam:trackNumber"] = info.value(Qmmp::TRACK).toInt();
    }

    if(!info.value(Qmmp::YEAR).isEmpty())
    {
        map["xesam:contentCreated"] = info.value(Qmmp::YEAR);
    }

    map["mpris:trackid"] = QVariant::fromValue<QDBusObjectPath>(m_trackID);
    map["xesam:url"] = info.path().startsWith(TTK_SEPARATOR) ? QUrl::fromLocalFile(info.path()).toString() : info.path();
    return map;
}

double MusicMPRISPlayerBase::minimumRate() const
{
    return 1.0;
}

QString MusicMPRISPlayerBase::playbackStatus() const
{
    if(m_music->state() == Qmmp::Playing)
    {
        return "Playing";
    }
    else if(m_music->state() == Qmmp::Paused)
    {
        return "Paused";
    }
    return "Stopped";
}

qlonglong MusicMPRISPlayerBase::position() const
{
    return qMax(m_music->elapsed() * 1000, qint64(0));
}

double MusicMPRISPlayerBase::rate() const
{
    return 1.0;
}

void MusicMPRISPlayerBase::setRate(double value)
{
    Q_UNUSED(value)
}

bool MusicMPRISPlayerBase::shuffle() const
{
    return m_player->getPlayMode() == MusicObject::PlayRandom;
}

void MusicMPRISPlayerBase::setShuffle(bool value)
{
    m_player->m_musicPlaylist->setPlaybackMode(value ? MusicObject::PlayRandom : MusicObject::PlayOrder);
}

double MusicMPRISPlayerBase::volume() const
{
    return m_player->m_musicPlayer->volume();
}

void MusicMPRISPlayerBase::setVolume(double value)
{
    m_player->m_musicPlayer->setVolume(value);
}

void MusicMPRISPlayerBase::Previous()
{
    m_player->musicPlayPrevious();
}

void MusicMPRISPlayerBase::Next()
{
    m_player->musicPlayNext();
}

void MusicMPRISPlayerBase::OpenUri(const QString &uri)
{
    QString path = uri;
    if(uri.startsWith("file://"))
    {
        path = QUrl(uri).toLocalFile();
        if(!QFile::exists(path))
        {
            return; //error
        }
    }

    m_player->musicImportSongsPathOutside({path}, true);
}

void MusicMPRISPlayerBase::Pause()
{
    m_player->musicStatePlay();
}

void MusicMPRISPlayerBase::Play()
{
    m_player->musicStatePlay();
}

void MusicMPRISPlayerBase::PlayPause()
{
    m_player->musicStatePlay();
}

void MusicMPRISPlayerBase::Stop()
{
    m_player->musicStateStop();
}

void MusicMPRISPlayerBase::Seek(qlonglong offset)
{
    m_music->seek(qMax(qint64(0), m_music->elapsed() +  offset / 1000));
}

void MusicMPRISPlayerBase::SetPosition(const QDBusObjectPath &trackId, qlonglong position)
{
    if(m_trackID == trackId)
    {
        m_music->seek(position / 1000);
    }
    else
    {
        TTK_LOGGER_WARN("SetPosition called with a invalid trackId");
    }
}

void MusicMPRISPlayerBase::trackInfoChanged()
{
    updateTrackID();
    sendProperties();
}

void MusicMPRISPlayerBase::stateChanged()
{
    if(m_music->state() == Qmmp::Playing)
    {
        updateTrackID();
        m_prev_pos = 0;
    }
    sendProperties();
}

void MusicMPRISPlayerBase::volumeChanged()
{
    sendProperties();
}

void MusicMPRISPlayerBase::elapsedChanged(qint64 elapsed)
{
    if(std::abs(elapsed - m_prev_pos) > 2000)
    {
        emit Seeked(elapsed * 1000);
    }
    m_prev_pos = elapsed;
}

void MusicMPRISPlayerBase::playbackModeChanged()
{
    sendProperties();
}

void MusicMPRISPlayerBase::updateTrackID()
{
    if(m_prev_track != m_player->m_musicPlaylist->currentIndex())
    {
        m_trackID = QDBusObjectPath(QString("%1/Track/%2").arg("/org/qmmp/MediaPlayer2").arg(MusicTime::random()));
        m_prev_track = m_player->m_musicPlaylist->currentIndex();
    }
}

void MusicMPRISPlayerBase::syncProperties()
{
    m_properties["CanGoNext"] = canGoNext();
    m_properties["CanGoPrevious"] = canGoPrevious();
    m_properties["CanPause"] = canPause();
    m_properties["CanPlay"] = canPlay();
    m_properties["CanSeek"] = canSeek();
    m_properties["LoopStatus"] = loopStatus();
    m_properties["MaximumRate"] = maximumRate();
    m_properties["MinimumRate"] = minimumRate();
    m_properties["PlaybackStatus"] = playbackStatus();
    m_properties["Rate"] = rate();
    m_properties["Shuffle"] = shuffle();
    m_properties["Volume"] = volume();
    m_properties["Metadata"] = metadata();
}

void MusicMPRISPlayerBase::sendProperties()
{
    TTKVariantMap prevProps = m_properties;
    syncProperties();

    QVariantMap map;
    for(auto it = m_properties.constBegin(); it != m_properties.constEnd(); ++it)
    {
        if(it.value() != prevProps.value(it.key()))
        {
            map.insert(it.key(), it.value());
        }
    }

    if(map.isEmpty())
    {
        return;
    }

    QDBusMessage msg = QDBusMessage::createSignal("/org/mpris/MediaPlayer2",
                                                  "org.freedesktop.DBus.Properties", "PropertiesChanged");
    msg << "org.mpris.MediaPlayer2.Player";
    msg << map;
    msg << QStringList();
    QDBusConnection::sessionBus().send(msg);
}
