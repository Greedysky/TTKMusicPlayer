#include "musicmprisplayer.h"
#include "musicapplication.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "ttktime.h"

#include <QBuffer>
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
    m_player = new MusicMPRISPlayerCore(this);
    //
    connection.registerService("org.mpris.MediaPlayer2." TTK_APP_NAME);
    connection.registerObject("/org/mpris/MediaPlayer2", this);
}

MusicMPRISPlayer::~MusicMPRISPlayer()
{
    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2." TTK_APP_NAME);
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
    return TTK_APP_NAME;
}

bool MusicMPRISPlayerRoot::hasTrackList() const
{
    return false;
}

QString MusicMPRISPlayerRoot::identity() const
{
    return TTK_APP_NAME;
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
    MusicApplication::instance()->quitWindow();
}

void MusicMPRISPlayerRoot::Raise()
{
    MusicApplication::instance()->show();
}


MusicMPRISPlayerCore::MusicMPRISPlayerCore(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    TTK::initRandom();

    m_prevTrack = 0;
    m_prevPos = 0;
    m_core = SoundCore::instance();
    m_application = MusicApplication::instance();

    connect(m_core, SIGNAL(trackInfoChanged()), SLOT(trackInfoChanged()));
    connect(m_core, SIGNAL(stateChanged(Qmmp::State)), SLOT(stateChanged()));
    connect(m_core, SIGNAL(volumeChanged(int)), SLOT(volumeChanged()));
    connect(m_core, SIGNAL(elapsedChanged(qint64)), SLOT(elapsedChanged(qint64)));
    connect(m_application->m_playlist, SIGNAL(playbackModeChanged(TTK::PlayMode)), SLOT(playbackModeChanged()));

    updateTrackID();
    syncProperties();
}

bool MusicMPRISPlayerCore::canControl() const noexcept
{
    return true;
}

bool MusicMPRISPlayerCore::canGoNext() const noexcept
{
    return !m_application->m_playlist->isEmpty();
}

bool MusicMPRISPlayerCore::canGoPrevious() const noexcept
{
    return !m_application->m_playlist->isEmpty();
}

bool MusicMPRISPlayerCore::canPause() const noexcept
{
    return !m_application->m_playlist->isEmpty();
}

bool MusicMPRISPlayerCore::canPlay() const noexcept
{
    return !m_application->m_playlist->isEmpty();
}

bool MusicMPRISPlayerCore::canSeek() const noexcept
{
    return m_core->duration() > 0;
}

QString MusicMPRISPlayerCore::loopStatus() const noexcept
{
    switch(m_application->playMode())
    {
        case TTK::PlayMode::OneLoop: return "Track";
        case TTK::PlayMode::ListLoop: return "Playlist";
        default: return "None";
    }
}

void MusicMPRISPlayerCore::setLoopStatus(const QString &value) noexcept
{
    if(value == "Track")
    {
        m_application->playOneLoop();
    }
    else if(value == "Playlist")
    {
        m_application->playlistLoop();
    }
    else
    {
        m_application->playOrder();
    }
}

double MusicMPRISPlayerCore::maximumRate() const noexcept
{
    return 1.0;
}

QVariantMap MusicMPRISPlayerCore::metadata() const
{
    if(m_core->path().isEmpty())
    {
        return QVariantMap();
    }

    QVariantMap map;
    TrackInfo info = m_core->trackInfo();
    map["mpris:length"] = qMax(m_core->duration() * TTK_DN_S2MS, qint64(0));

    const QString &coverPath = MetaDataManager::instance()->getCoverPath(info.path());
    if(!coverPath.isEmpty())
    {
        map["mpris:artUrl"] = QUrl::fromLocalFile(coverPath).toString();
    }
    else
    {
        const QImage &coverImage = MetaDataManager::instance()->getCover(info.path());
        if(!coverImage.isNull())
        {
            QByteArray tmp;
            QBuffer buffer(&tmp);
            buffer.open(QIODevice::WriteOnly);
            coverImage.save(&buffer, "JPEG");
            map["mpris:artUrl"] = QString("data:image/jpeg;base64,%1").arg(QString::fromLatin1(tmp.toBase64()));
        }
    }


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
        map["xesam:artist"] = QStringList(info.value(Qmmp::ARTIST));
    }

    if(!info.value(Qmmp::ALBUMARTIST).isEmpty())
    {
        map["xesam:albumArtist"] = QStringList(info.value(Qmmp::ALBUMARTIST));
    }

    if(!info.value(Qmmp::COMMENT).isEmpty())
    {
        map["xesam:comment"] = QStringList(info.value(Qmmp::COMMENT));
    }

    if(!info.value(Qmmp::COMPOSER).isEmpty())
    {
        map["xesam:composer"] = QStringList(info.value(Qmmp::COMPOSER));
    }

    if(!info.value(Qmmp::DISCNUMBER).isEmpty())
    {
        map["xesam:discNumber"] = info.value(Qmmp::DISCNUMBER).toInt();
    }

    if(!info.value(Qmmp::GENRE).isEmpty())
    {
        map["xesam:genre"] = QStringList(info.value(Qmmp::GENRE));
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

double MusicMPRISPlayerCore::minimumRate() const noexcept
{
    return 1.0;
}

QString MusicMPRISPlayerCore::playbackStatus() const noexcept
{
    if(m_core->state() == Qmmp::Playing)
    {
        return "Playing";
    }
    else if(m_core->state() == Qmmp::Paused)
    {
        return "Paused";
    }
    return "Stopped";
}

qlonglong MusicMPRISPlayerCore::position() const
{
    return qMax(m_core->elapsed() * TTK_DN_S2MS, qint64(0));
}

double MusicMPRISPlayerCore::rate() const noexcept
{
    return 1.0;
}

void MusicMPRISPlayerCore::setRate(double value) noexcept
{
    Q_UNUSED(value)
}

bool MusicMPRISPlayerCore::shuffle() const noexcept
{
    return m_application->playMode() == TTK::PlayMode::Random;
}

void MusicMPRISPlayerCore::setShuffle(bool value) noexcept
{
    m_application->m_playlist->setPlaybackMode(value ? TTK::PlayMode::Random : TTK::PlayMode::Order);
}

double MusicMPRISPlayerCore::volume() const
{
    return m_application->m_player->volume();
}

void MusicMPRISPlayerCore::setVolume(double value)
{
    m_application->m_player->setVolume(value);
}

void MusicMPRISPlayerCore::Previous()
{
    m_application->playPrevious();
}

void MusicMPRISPlayerCore::Next()
{
    m_application->playNext();
}

void MusicMPRISPlayerCore::OpenUri(const QString &uri)
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

    m_application->importSongsByOutside(path, true);
}

void MusicMPRISPlayerCore::Pause()
{
    m_application->switchToPlayState();
}

void MusicMPRISPlayerCore::Play()
{
    m_application->switchToPlayState();
}

void MusicMPRISPlayerCore::PlayPause()
{
    m_application->switchToPlayState();
}

void MusicMPRISPlayerCore::Stop()
{
    m_application->switchToStopState();
}

void MusicMPRISPlayerCore::Seek(qlonglong offset)
{
    m_core->seek(qMax(qint64(0), m_core->elapsed() +  offset / 1000));
}

void MusicMPRISPlayerCore::SetPosition(const QDBusObjectPath &trackId, qlonglong position)
{
    if(m_trackID == trackId)
    {
        m_core->seek(position / 1000);
    }
    else
    {
        TTK_WARN_STREAM("SetPosition called with a invalid trackId");
    }
}

void MusicMPRISPlayerCore::trackInfoChanged()
{
    updateTrackID();
    sendProperties();
}

void MusicMPRISPlayerCore::stateChanged()
{
    if(m_core->state() == Qmmp::Playing)
    {
        updateTrackID();
        m_prevPos = 0;
    }
    sendProperties();
}

void MusicMPRISPlayerCore::volumeChanged()
{
    sendProperties();
}

void MusicMPRISPlayerCore::elapsedChanged(qint64 elapsed)
{
    if(abs(elapsed - m_prevPos) > 2 * TTK_DN_S2MS)
    {
        Q_EMIT Seeked(elapsed * TTK_DN_S2MS);
    }
    m_prevPos = elapsed;
}

void MusicMPRISPlayerCore::playbackModeChanged()
{
    sendProperties();
}

void MusicMPRISPlayerCore::updateTrackID()
{
    if(m_prevTrack != m_application->m_playlist->currentIndex())
    {
        m_trackID = QDBusObjectPath(QString("%1/Track/%2").arg("/org/qmmp/MediaPlayer2").arg(TTK::random()));
        m_prevTrack = m_application->m_playlist->currentIndex();
    }
}

void MusicMPRISPlayerCore::syncProperties()
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

void MusicMPRISPlayerCore::sendProperties()
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

    QDBusMessage msg = QDBusMessage::createSignal("/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged");
    msg << "org.mpris.MediaPlayer2.Player";
    msg << map;
    msg << QStringList();
    QDBusConnection::sessionBus().send(msg);
}
