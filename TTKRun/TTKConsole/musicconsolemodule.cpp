#include "musicconsolemodule.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicformats.h"
#include "musictkplconfigmanager.h"
#include "musichotkeymanager.h"
#include "musicfileutils.h"
#include "ttkversion.h"

static QString shortcutMessage()
{
    return QString("    %1 (%2)\n").arg("Ctrl+B", "Switch to play state") +
           QString("    %1 (%2)\n").arg("Ctrl+Left", "Play previous") +
           QString("    %1 (%2)\n").arg("Ctrl+Right", "Play next") +
           QString("    %1 (%2)\n").arg("Ctrl+Up", "Volume up") +
           QString("    %1 (%2)\n").arg("Ctrl+Down", "Volume down") +
           QString("    %1 (%2)\n").arg("Ctrl+1", "Play random") +
           QString("    %1 (%2)\n").arg("Ctrl+2", "Playlist loop") +
           QString("    %1 (%2)\n").arg("Ctrl+3", "Play one loop") +
           QString("    %1 (%2)\n").arg("Ctrl+4", "Play once") +
           QString("    %1 (%2)\n").arg("Alt+1", "Enhanced off") +
           QString("    %1 (%2)\n").arg("Alt+2", "Enhanced 3D") +
           QString("    %1 (%2)\n").arg("Alt+3", "Enhanced NICAM") +
           QString("    %1 (%2)\n").arg("Alt+4", "Enhanced subwoofer") +
           QString("    %1 (%2)\n").arg("Alt+5", "Enhanced cocal") +
           QString("    %1 (%2)\n").arg("Ctrl+Q", "Quit");
}

MusicConsoleModule::MusicConsoleModule(QObject *parent)
    : QObject(parent),
      m_volume(100),
      m_playbackMode("Order"),
      m_enhanced("Off")
{
    m_playlist = new MusicPlaylist(this);
    m_playlist->setPlaybackMode(TTK::PlayMode::Order);
    m_player = new MusicPlayer(this);
    m_player->setPlaylist(m_playlist);

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_playlist, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged(int)));

    G_HOTKEY_PTR->addHotKey(this, "Ctrl+B", SLOT(switchToPlayState()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+Left", SLOT(playPrevious()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+Right", SLOT(playNext()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+Up", SLOT(volumeUp()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+Down", SLOT(volumeDown()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+1", SLOT(playOrder()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+2", SLOT(playRandom()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+3", SLOT(playlistLoop()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+4", SLOT(playOneLoop()));
    G_HOTKEY_PTR->addHotKey(this, "Ctrl+5", SLOT(playOnce()));
    G_HOTKEY_PTR->addHotKey(this, "Alt+1", SLOT(setEnhancedOff()));
    G_HOTKEY_PTR->addHotKey(this, "Alt+2", SLOT(setEnhanced3D()));
    G_HOTKEY_PTR->addHotKey(this, "Alt+3", SLOT(setEnhancedNICAM()));
    G_HOTKEY_PTR->addHotKey(this, "Alt+4", SLOT(setEnhancedSubwoofer()));
    G_HOTKEY_PTR->addHotKey(this, "Alt+5", SLOT(setEnhancedVocal()));
    G_HOTKEY_PTR->addHotKey(qApp, "Ctrl+Q", SLOT(quit()));
}

MusicConsoleModule::~MusicConsoleModule()
{
    delete m_player;
    delete m_playlist;
}

bool MusicConsoleModule::initialize()
{
    QString text = TTK_STR_CAT("\n", TTK_APP_NAME, " Console Module v", TTK_VERSION_STR, "\n");
            text += "Offical web page: https://github.com/Greedysky/TTKMusicPlayer\n";
            text += "Copyright(C) 2015 - 2026 Greedysky All Rights Reserved\n";
            text += "TTKMusicPlayer imitates Kugou UI, the music player uses of qmmp core library based on Qt for windows and linux\n\n";
            text += "Global shortcuts:\n" + shortcutMessage() + "\n";

    TTKCommandLineOption op0("-h", "--help", "Show command line help options");
    TTKCommandLineOption op1("-u", "--url", "Media play url path");
    TTKCommandLineOption op2("-d", "--dir", "Media play dir path");
    TTKCommandLineOption op3("-l", "--playlist", "Media playlist url path");

    TTKCommandLineParser parser;
    parser.addOption(op0);
    parser.addOption(op1);
    parser.addOption(op2);
    parser.addOption(op3);
    parser.setDescription(text);
    parser.process();

    if(parser.isEmpty() || parser.isSet(op0))
    {
        parser.showHelp();
        return false;
    }

    if(parser.isSet(op1))
    {
        const QString &path = parser.value(op1);
        if(path.isEmpty())
        {
            TTK_LOG_STREAM("Media play url path is empty");
            return false;
        }
        else
        {
            TTK_LOG_STREAM("Add play url path: " << path);
            m_playlist->add(0, path);
            m_playlist->setCurrentIndex(0);
        }
    }
    else if(parser.isSet(op2))
    {
        const QString &path = parser.value(op2);
        if(path.isEmpty())
        {
            TTK_LOG_STREAM("Media play dir path is empty");
            return false;
        }
        else
        {
            for(const QFileInfo &fin : TTK::File::fileInfoListByPath(path, MusicFormats::supportMusicInputFilterFormats()))
            {
                TTK_LOG_STREAM("Add play url path: " << fin.absoluteFilePath());
                m_playlist->append(0, fin.absoluteFilePath());
            }

            if(!m_playlist->isEmpty())
            {
                m_playlist->setCurrentIndex(0);
            }
        }
    }
    else if(parser.isSet(op3))
    {
        const QString &path = parser.value(op3);
        if(path.isEmpty())
        {
            TTK_LOG_STREAM("Media playlist path is empty");
            return false;
        }
        else
        {
            if(TTK_FILE_SUFFIX(QFileInfo(path)) != TPL_FILE_SUFFIX)
            {
                TTK_LOG_STREAM("Media playlist format not support");
                return false;
            }

            MusicTKPLConfigManager manager;
            if(!manager.fromFile(path))
            {
                TTK_LOG_STREAM("Media playlist read error");
                return false;
            }

            MusicSongItemList items;
            manager.readBuffer(items);

            for(const MusicSongItem &item : qAsConst(items))
            {
                for(const MusicSong &song : qAsConst(item.m_songs))
                {
                    TTK_LOG_STREAM("Add play url path: " << song.path());
                    m_playlist->append(0, song.path());
                }
            }

            if(!m_playlist->isEmpty())
            {
                m_playlist->setCurrentIndex(0);
            }
        }
    }
    else
    {
        TTK_LOG_STREAM("Options unknown error");
        return false;
    }

    TTK_LOG_STREAM("Media files count: " << m_playlist->count() << TTK_LINEFEED);

    m_player->play();
    m_player->setVolume(m_volume);
    return QCoreApplication::exec();
}

void MusicConsoleModule::positionChanged(qint64 position)
{
    print(position, m_player->duration());
}

void MusicConsoleModule::currentIndexChanged(int index)
{
    TTK_LOG_STREAM("Current play index: " << index);
//    TTK_SIGNLE_SHOT(TTK_DN_S2MS, this, resetVolume, TTK_SLOT);

    if(index == TTK_NORMAL_LEVEL)
    {
        m_player->stop();
        TTK_SIGNLE_SHOT(TTK_DN_S2MS, qApp, quit, TTK_SLOT);
        TTK_LOG_STREAM("Media play end and application quit now");
    }
}

void MusicConsoleModule::switchToPlayState()
{
    if(m_playlist->isEmpty())
    {
        return;
    }

    if(m_player->isPlaying())
    {
        m_player->pause();
    }
    else
    {
        m_player->play();
        m_player->setVolume(m_volume);
    }
}

void MusicConsoleModule::playPrevious()
{
    if(m_playlist->isEmpty())
    {
        return;
    }

    m_playlist->setCurrentIndex(PLAY_PREVIOUS_LEVEL);

    m_player->play();
    m_player->setVolume(m_volume);
}

void MusicConsoleModule::playNext()
{
    if(m_playlist->isEmpty())
    {
        return;
    }

    m_playlist->setCurrentIndex(PLAY_NEXT_LEVEL);

    m_player->play();
    m_player->setVolume(m_volume);
}

//void MusicConsoleModule::resetVolume()
//{
//    m_player->setVolume(m_volume);
//}

void MusicConsoleModule::volumeDown()
{
    m_volume = m_player->volume();
    m_volume -= 15;
    if(m_volume < 0)
    {
        m_volume = 0;
    }

    m_player->setVolume(m_volume);
}

void MusicConsoleModule::volumeUp()
{
    m_volume = m_player->volume();
    m_volume += 15;
    if(m_volume > 100)
    {
        m_volume = 100;
    }

    m_player->setVolume(m_volume);
}

void MusicConsoleModule::playOrder()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::Order);
    m_playbackMode = "Order";
}

void MusicConsoleModule::playRandom()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::Random);
    m_playbackMode = "Random";
}

void MusicConsoleModule::playlistLoop()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::ListLoop);
    m_playbackMode = "ListLoop";
}

void MusicConsoleModule::playOneLoop()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::OneLoop);
    m_playbackMode = "OneLoop";
}

void MusicConsoleModule::playOnce()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::Once);
    m_playbackMode = "Once";
}

void MusicConsoleModule::setEnhancedOff()
{
    m_player->setEnhance(TTK::Enhance::Off);
    m_enhanced = "Off";
}

void MusicConsoleModule::setEnhanced3D()
{
    m_player->setEnhance(TTK::Enhance::M3D);
    m_enhanced = "3D";
}

void MusicConsoleModule::setEnhancedNICAM()
{
    m_player->setEnhance(TTK::Enhance::NICAM);
    m_enhanced = "NICAM";
}

void MusicConsoleModule::setEnhancedSubwoofer()
{
    m_player->setEnhance(TTK::Enhance::Subwoofer);
    m_enhanced = "Subwoofer";
}

void MusicConsoleModule::setEnhancedVocal()
{
    m_player->setEnhance(TTK::Enhance::Vocal);
    m_enhanced = "Vocal";
}

void MusicConsoleModule::print(qint64 position, qint64 duration) const
{
    const MusicPlayItem &item = m_playlist->currentItem();
    TTK_LOG_STREAM(QString("Time:[%1/%2], Volume:%3, PlaybackMode:%4, Enhance:%5, Media Path: %6")
                .arg(TTKTime::formatDuration(position), TTKTime::formatDuration(duration))
                .arg(m_player->volume())
                .arg(m_playbackMode, m_enhanced, item.m_path));
}
