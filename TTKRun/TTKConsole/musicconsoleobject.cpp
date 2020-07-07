#include "musicconsoleobject.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicfileutils.h"
#include "musicformats.h"
#include "musictkplconfigmanager.h"
#include "musichotkeymanager.h"

#if TTK_QT_VERSION_CHECK(5,2,0)
#include <QCommandLineOption>
#include <QCommandLineParser>
#endif

MusicConsoleObject::MusicConsoleObject(QObject *parent)
    : QObject(parent)
{
    m_musicPlayer = new MusicPlayer(this);
    m_musicPlaylist = new MusicPlaylist(this);

    m_volume = 100;
    m_playbackMode = "Order";
    m_enhanced = "Off";

    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOrder);
    m_musicPlayer->setPlaylist(m_musicPlaylist);

    connect(m_musicPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_musicPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_musicPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(currentIndexChanged(int)));

    M_HOTKEY_PTR->addHotKey("Ctrl+B");
    M_HOTKEY_PTR->addHotKey("Ctrl+Left");
    M_HOTKEY_PTR->addHotKey("Ctrl+Right");
    M_HOTKEY_PTR->addHotKey("Ctrl+Up");
    M_HOTKEY_PTR->addHotKey("Ctrl+Down");
    M_HOTKEY_PTR->addHotKey("Ctrl+1");
    M_HOTKEY_PTR->addHotKey("Ctrl+2");
    M_HOTKEY_PTR->addHotKey("Ctrl+3");
    M_HOTKEY_PTR->addHotKey("Ctrl+4");
    M_HOTKEY_PTR->addHotKey("Ctrl+5");
    M_HOTKEY_PTR->addHotKey("Alt+1");
    M_HOTKEY_PTR->addHotKey("Alt+2");
    M_HOTKEY_PTR->addHotKey("Alt+3");
    M_HOTKEY_PTR->addHotKey("Alt+4");
    M_HOTKEY_PTR->addHotKey("Alt+5");
    M_HOTKEY_PTR->addHotKey("Ctrl+Q");

    connect(M_HOTKEY_PTR->getHotKey(0), SIGNAL(activated()), SLOT(musicStatePlay()));
    connect(M_HOTKEY_PTR->getHotKey(1), SIGNAL(activated()), SLOT(musicPlayPrevious()));
    connect(M_HOTKEY_PTR->getHotKey(2), SIGNAL(activated()), SLOT(musicPlayNext()));
    connect(M_HOTKEY_PTR->getHotKey(3), SIGNAL(activated()), SLOT(musicActionVolumePlus()));
    connect(M_HOTKEY_PTR->getHotKey(4), SIGNAL(activated()), SLOT(musicActionVolumeSub()));

    connect(M_HOTKEY_PTR->getHotKey(5), SIGNAL(activated()), SLOT(musicPlayOrder()));
    connect(M_HOTKEY_PTR->getHotKey(6), SIGNAL(activated()), SLOT(musicPlayRandom()));
    connect(M_HOTKEY_PTR->getHotKey(7), SIGNAL(activated()), SLOT(musicPlaylistLoop()));
    connect(M_HOTKEY_PTR->getHotKey(8), SIGNAL(activated()), SLOT(musicPlayOneLoop()));
    connect(M_HOTKEY_PTR->getHotKey(9), SIGNAL(activated()), SLOT(musicPlayItemOnce()));

    connect(M_HOTKEY_PTR->getHotKey(10), SIGNAL(activated()), SLOT(musicEnhancedOff()));
    connect(M_HOTKEY_PTR->getHotKey(11), SIGNAL(activated()), SLOT(musicEnhanced3D()));
    connect(M_HOTKEY_PTR->getHotKey(12), SIGNAL(activated()), SLOT(musicEnhancedNICAM()));
    connect(M_HOTKEY_PTR->getHotKey(13), SIGNAL(activated()), SLOT(musicEnhancedSubwoofer()));
    connect(M_HOTKEY_PTR->getHotKey(14), SIGNAL(activated()), SLOT(musicEnhancedVocal()));

    connect(M_HOTKEY_PTR->getHotKey(15), SIGNAL(activated()), qApp, SLOT(quit()));

}

MusicConsoleObject::~MusicConsoleObject()
{
    TTK_LOGGER_INFO("\nRelease all");
    delete m_musicPlayer;
    delete m_musicPlaylist;
}

bool MusicConsoleObject::init(const QCoreApplication &app)
{
#if TTK_QT_VERSION_CHECK(5,2,0)
    QCommandLineOption op1("u", "", ".");
    QCommandLineOption op2("d", "", ".");
    QCommandLineOption op3("l", "", ".");

    QCommandLineParser parser;
    parser.addOption(op1);
    parser.addOption(op2);
    parser.addOption(op3);
    parser.process(app);

    if(app.arguments().count() == 1)
    {
        TTK_LOGGER_INFO("\nOptions:");
        TTK_LOGGER_INFO("-u //Music play url path");
        TTK_LOGGER_INFO("-d //Music play dir path");
        TTK_LOGGER_INFO("-l //Music playlist url path\n");
        return false;
    }

    if(parser.isSet(op1))
    {
        const QString &url = parser.value(op1);
        if(url.isEmpty())
        {
            TTK_LOGGER_ERROR("Music play url path is empty");
            return false;
        }
        else
        {
            TTK_LOGGER_INFO("Add play url path: " << url);
            m_musicPlaylist->addMedia(0, url);
            m_musicPlaylist->setCurrentIndex(0);
        }
    }
    else if(parser.isSet(op2))
    {
        const QString &url = parser.value(op2);
        if(url.isEmpty())
        {
            TTK_LOGGER_ERROR("Music play dir path is empty");
            return false;
        }
        else
        {
            foreach(const QFileInfo &file, MusicUtils::File::getFileListByDir(url, MusicFormats::supportFormatsFilterString(), true))
            {
                TTK_LOGGER_INFO("Add play url path: " << file.absoluteFilePath());
                m_musicPlaylist->appendMedia(0, file.absoluteFilePath());
            }

            if(!m_musicPlaylist->isEmpty())
            {
                m_musicPlaylist->setCurrentIndex(0);
            }
        }
    }
    else if(parser.isSet(op3))
    {
        const QString &url = parser.value(op3);
        if(url.isEmpty())
        {
            TTK_LOGGER_ERROR("Music playlist path is empty");
            return false;
        }
        else
        {
            if(QFileInfo(url).suffix().toLower() != LST_FILE_PREFIX)
            {
                TTK_LOGGER_INFO("Music playlist format not support");
                return false;
            }

            MusicSongItems items;
            MusicTKPLConfigManager manager;
            if(!manager.readConfig(url))
            {
                TTK_LOGGER_ERROR("Music playlist read error");
                return false;
            }
            manager.readPlaylistData(items);

            foreach(const MusicSongItem &item, items)
            {
                foreach(const MusicSong &song, item.m_songs)
                {
                    TTK_LOGGER_INFO("Add play url path: " << song.getMusicPath());
                    m_musicPlaylist->appendMedia(0, song.getMusicPath());
                }
            }

            if(!m_musicPlaylist->isEmpty())
            {
                m_musicPlaylist->setCurrentIndex(0);
            }
        }
    }
    else
    {
        TTK_LOGGER_ERROR("Options error");
        return false;
    }

    TTK_LOGGER_INFO("\nMusic Files count: " << m_musicPlaylist->mediaCount() << "\n");

    m_musicPlayer->play();
    m_musicPlayer->setVolume(m_volume);
#else
    TTK_LOGGER_ERROR("Qt version less than 5.2 not support commend line");
#endif

    return app.exec();
}

void MusicConsoleObject::durationChanged(qint64 duration)
{
    print(0, duration);
}

void MusicConsoleObject::positionChanged(qint64 position)
{
    print(position, m_musicPlayer->duration());
}

void MusicConsoleObject::currentIndexChanged(int index)
{
    TTK_LOGGER_INFO("\nCurrent Play Indedx: " << index);
    QTimer::singleShot(MT_S2MS, this, SLOT(resetVolume()));
}

void MusicConsoleObject::musicStatePlay()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;
    }

    if(m_musicPlayer->isPlaying())
    {
        m_musicPlayer->pause();
    }
    else
    {
        m_musicPlayer->play();
        m_musicPlayer->setVolume(m_volume);
    }
}

void MusicConsoleObject::musicPlayPrevious()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;
    }

    if(m_musicPlaylist->playbackMode() == MusicObject::PM_PlayRandom)
    {
        m_musicPlaylist->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playPrevious();
    }

    m_musicPlayer->play();
    m_musicPlayer->setVolume(m_volume);
}

void MusicConsoleObject::musicPlayNext()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;
    }

    if(m_musicPlaylist->playbackMode() == MusicObject::PM_PlayRandom)
    {
        m_musicPlaylist->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playNext();
    }

    m_musicPlayer->play();
    m_musicPlayer->setVolume(m_volume);
}

void MusicConsoleObject::resetVolume()
{
    m_musicPlayer->setVolume(m_volume);
}

void MusicConsoleObject::musicActionVolumeSub()
{
    m_volume = m_musicPlayer->volume();
    m_volume -= 15;
    if(m_volume < 0)
    {
        m_volume = 0;
    }

    m_musicPlayer->setVolume(m_volume);
}

void MusicConsoleObject::musicActionVolumePlus()
{
    m_volume = m_musicPlayer->volume();
    m_volume += 15;
    if(m_volume > 100)
    {
        m_volume = 100;
    }

    m_musicPlayer->setVolume(m_volume);
}

void MusicConsoleObject::musicPlayOrder()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOrder);
    m_playbackMode = "Order";
}

void MusicConsoleObject::musicPlayRandom()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayRandom);
    m_playbackMode = "Random";
}

void MusicConsoleObject::musicPlaylistLoop()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlaylistLoop);
    m_playbackMode = "ListLoop";
}

void MusicConsoleObject::musicPlayOneLoop()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOneLoop);
    m_playbackMode = "OneLoop";
}

void MusicConsoleObject::musicPlayItemOnce()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOnce);
    m_playbackMode = "Once";
}

void MusicConsoleObject::musicEnhancedOff()
{
    m_musicPlayer->setMusicEnhanced(MusicPlayer::EnhancedOff);
    m_enhanced = "Off";
}

void MusicConsoleObject::musicEnhanced3D()
{
    m_musicPlayer->setMusicEnhanced(MusicPlayer::Enhanced3D);
    m_enhanced = "3D";
}

void MusicConsoleObject::musicEnhancedNICAM()
{
    m_musicPlayer->setMusicEnhanced(MusicPlayer::EnhancedNICAM);
    m_enhanced = "NICAM";
}

void MusicConsoleObject::musicEnhancedSubwoofer()
{
    m_musicPlayer->setMusicEnhanced(MusicPlayer::EnhancedSubwoofer);
    m_enhanced = "Subwoofer";
}

void MusicConsoleObject::musicEnhancedVocal()
{
    m_musicPlayer->setMusicEnhanced(MusicPlayer::EnhancedVocal);
    m_enhanced = "Vocal";
}

void MusicConsoleObject::print(qint64 position, qint64 duration)
{
    TTK_LOGGER_INFO(QString("Music Name: %1, Time:[%2/%3], Volume:%4, PlaybackMode:%5, Enhance:%6"))
                .arg(m_musicPlaylist->currentMediaPath())
                .arg(MusicTime::msecTime2LabelJustified(position))
                .arg(MusicTime::msecTime2LabelJustified(duration))
                .arg(m_musicPlayer->volume())
                .arg(m_playbackMode)
                .arg(m_enhanced);
}
