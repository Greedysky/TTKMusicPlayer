#include "musicapplication.h"
#include "ui_musicapplication.h"
#include "musicplayedlistpopwidget.h"
#include "musicsongscontainerwidget.h"
#include "musicconfigmanager.h"
#include "musicplayer.h"
#include "musicformats.h"
#include "musicplaylist.h"
#include "musicbackgroundmanager.h"
#include "musicbottomareawidget.h"
#include "musictopareawidget.h"
#include "musicrightareawidget.h"
#include "musicleftareawidget.h"
#include "musicapplicationmodule.h"
#include "musichotkeymanager.h"
#include "musicfunctionuiobject.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "musicplaylistmanager.h"
#include "musictinyuiobject.h"
#include "musicdispatchmanager.h"
#include "musictkplconfigmanager.h"
#include "musicinputdialog.h"
#include "ttkversion.h"

#include "qalgorithm/aeswrapper.h"

MusicApplication *MusicApplication::m_instance = nullptr;

MusicApplication::MusicApplication(QWidget *parent)
    : TTKAbstractMoveResizeWidget(false, parent),
      m_ui(new Ui::MusicApplication),
      m_quitWindowMode(false),
      m_currentPlaylistRow(TTK_NORMAL_LEVEL)
{
    m_instance = this;

    m_applicationModule = new MusicApplicationModule(this);
    m_topAreaWidget = new MusicTopAreaWidget(this);
    m_bottomAreaWidget = new MusicBottomAreaWidget(this);
    m_rightAreaWidget = new MusicRightAreaWidget(this);
    m_leftAreaWidget = new MusicLeftAreaWidget(this);

    m_ui->setupUi(this);
    const QSize &size = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setMinimumSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    setMaximumSize(size.width(), size.height());

    m_player = new MusicPlayer(this);
    m_playlist = new MusicPlaylist(this);
    m_songTreeWidget = new MusicSongsContainerWidget(this);
    m_ui->songsContainer->addWidget(m_songTreeWidget);

    m_bottomAreaWidget->setupUi(m_ui);
    m_topAreaWidget->setupUi(m_ui);
    m_rightAreaWidget->setupUi(m_ui);
    m_leftAreaWidget->setupUi(m_ui);
    m_topAreaWidget->backgroundAnimationChanged(false);

    connect(m_rightAreaWidget, SIGNAL(updateBackgroundTheme()), m_topAreaWidget, SLOT(backgroundTransparentChanged()));
    connect(m_rightAreaWidget, SIGNAL(updateBackgroundThemeDownload()), m_topAreaWidget, SLOT(backgroundThemeDownloadFinished()));

    m_playlist->setPlaybackMode(TTK::PlayMode::Order); //The default is the order of play
    m_ui->musicPlayedList->setPlaylist(m_playlist);
    m_player->setPlaylist(m_playlist);
    m_player->setVolume(100);  //The default Volume is 100

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_player, SIGNAL(stateChanged(TTK::PlayState)), SLOT(playerStateChanged(TTK::PlayState)));
    connect(m_playlist, SIGNAL(currentIndexChanged(int)), SLOT(showCurrentSong()));
    connect(m_ui->musicDesktopLrc, SIGNAL(clicked(bool)), m_rightAreaWidget, SLOT(setDestopLrcVisible(bool)));

    m_ui->musicKey->setFocus();
    m_ui->lrcDisplayAllButton->hide();
    m_ui->functionsContainer->setCurrentIndex(MUSIC_SEARCH_PAGE);

    m_ui->musicTimeWidget->setInputModule(this);

    G_HOTKEY_PTR->addHotKey(this, SLOT(switchToPlayState()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(showSettingWidget()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(playPrevious()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(playNext()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(volumeUp()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(volumeDown()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(volumeMute()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(importSongsPopup()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(addSongToLovestList()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(currentPlayLocation()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(importSongsItemList()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(exportSongsItemList()));
    G_HOTKEY_PTR->addHotKey(m_applicationModule, SLOT(showSpectrumWidget()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(windowConciseChanged()));
    G_HOTKEY_PTR->addHotKey(m_applicationModule, SLOT(showAboutWidget()));
    G_HOTKEY_PTR->addHotKey(this, SLOT(quitWindow()));

    // Objects Mouse tracking
    setObjectsTracking({m_ui->background, m_ui->songsContainer});

    readSystemConfigFromFile();
    TTK_SIGNLE_SHOT(m_rightAreaWidget, showSongMainWidget, TTK_SLOT);
}

MusicApplication::~MusicApplication()
{
    delete m_player;
    delete m_playlist;
    delete m_songTreeWidget;
    delete m_bottomAreaWidget;
    delete m_topAreaWidget;
    delete m_rightAreaWidget;
    delete m_leftAreaWidget;
    delete m_applicationModule;
    delete m_ui;
}

MusicApplication *MusicApplication::instance()
{
    return m_instance;
}

QString MusicApplication::currentFileName() const
{
    if(m_playlist->currentIndex() < 0 || m_currentPlaylistRow < 0)
    {
        return {};
    }

    const MusicPlayItem &item = m_playlist->currentItem();
    const MusicSongItemList &items = m_songTreeWidget->items();

    if(0 <= m_currentPlaylistRow && m_currentPlaylistRow < items.count())
    {
        const MusicSongList &songs = items[m_currentPlaylistRow].m_songs;
        const int index = m_songTreeWidget->mapSongIndexByFilePath(m_currentPlaylistRow, item.m_path);
        return (index != -1) ? songs[index].name() : QString();
    }
    return {};
}

QString MusicApplication::currentFilePath() const
{
    if(m_playlist->currentIndex() < 0 || m_currentPlaylistRow < 0)
    {
        return {};
    }

    const MusicPlayItem &item = m_playlist->currentItem();
    const MusicSongItemList &items = m_songTreeWidget->items();

    if(0 <= m_currentPlaylistRow && m_currentPlaylistRow < items.count())
    {
        const MusicSongList &songs = items[m_currentPlaylistRow].m_songs;
        const int index = m_songTreeWidget->mapSongIndexByFilePath(m_currentPlaylistRow, item.m_path);
        return (index != -1) ? songs[index].path() : QString();
    }
    return {};
}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return m_playlist->currentIndex() == TTK_NORMAL_LEVEL;
}

void MusicApplication::loadCurrentSongLrc()
{
    //Loading the current song lrc
    if(checkMusicListCurrentIndex())
    {
        return;
    }

    const QString &name = currentFileName();
    const QString &prefixPath = TTK::String::lrcDirPrefix() + name;
    const QString &nativePath = QFileInfo(currentFilePath()).absolutePath() + TTK_SEPARATOR + name;
    const QStringList &formats = {LRC_FILE, KRC_FILE, QRC_FILE, TRC_FILE, YRC_FILE};

    QString path;
    for(const QString &format : formats)
    {
        path = prefixPath + format;
        if(QFile::exists(path))
        {
            break;
        }

        // try to load same dir file
        path = nativePath + format;
        if(QFile::exists(path))
        {
            break;
        }

        path.clear();
    }

    m_rightAreaWidget->loadCurrentSongLrc(name, path);
    //reset current song lrc index.
    TTK_SIGNLE_SHOT(resetCurrentSongLrcIndex, TTK_SLOT);
}

void MusicApplication::importSongsByOutside(const QString &path, bool play)
{
    if(path.isEmpty())
    {
        return;
    }

    m_songTreeWidget->importSongsByPath({path}, MUSIC_NORMAL_LIST);

    if(play)
    {
        m_currentPlaylistRow = MUSIC_NONE_LIST;
        m_songTreeWidget->setCurrentIndex(MUSIC_NORMAL_LIST);
        const int index = m_songTreeWidget->mapSongIndexByFilePath(MUSIC_NORMAL_LIST, path);
        playIndexBy(index, TTK_NORMAL_LEVEL);
    }
}

QString MusicApplication::containsDownloadItem(bool &contains) const
{
    contains = false;
    QString path;
    if(m_songTreeWidget->playRowIndex() != TTK_NORMAL_LEVEL)
    {
        const MusicPlayItem &item = m_playlist->currentItem();
        if(item.isValid())
        {
            if(m_songTreeWidget->playRowIndex() != MUSIC_NETWORK_LIST)
            {
                path = item.m_path;
            }
            else
            {
                const MusicSong currentSong(item.m_path);
                path = QString("%1%2.%3").arg(G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString(), currentFileName(), currentSong.format());
            }

            contains = QFile::exists(path);
        }
    }
    return path;
}

bool MusicApplication::containsLovestItem() const
{
    if(m_songTreeWidget->playRowIndex() != TTK_NORMAL_LEVEL)
    {
        const MusicPlayItem &item = m_playlist->currentItem();
        const MusicSongItemList &items = m_songTreeWidget->items();
        if(item.isValid() && item.m_playlistRow < items.count())
        {
            const MusicSongList &currentSongs = items[item.m_playlistRow].m_songs;
            const int index = m_songTreeWidget->mapSongIndexByFilePath(item.m_playlistRow, item.m_path);
            const MusicSongList &loveSongs = items[MUSIC_LOVEST_LIST].m_songs;
            return index != -1 ? loveSongs.contains(currentSongs[index]) : false;
        }
    }
    return false;
}

bool MusicApplication::containsLovestItem(int index) const
{
    if(m_songTreeWidget->currentIndex() != TTK_NORMAL_LEVEL && index > TTK_NORMAL_LEVEL)
    {
        const MusicSongItemList &items = m_songTreeWidget->items();
        if(m_songTreeWidget->currentIndex() < items.count())
        {
            const MusicSongList &currentSongs = items[m_songTreeWidget->currentIndex()].m_songs;
            const MusicSongList &loveSongs = items[MUSIC_LOVEST_LIST].m_songs;
            return loveSongs.contains(currentSongs[index]);
        }
    }
    return false;
}

void MusicApplication::updateCurrentArtist()
{
    m_songTreeWidget->updateCurrentArtist();
}

bool MusicApplication::isPlaying() const
{
    return m_player->isPlaying();
}

qint64 MusicApplication::duration() const
{
    return m_player->duration();
}

TTK::PlayMode MusicApplication::playMode() const noexcept
{
    return m_playlist->playbackMode();
}

void MusicApplication::quitWindow()
{
    TTK::Core::enableBreakPoint(true);
    //
    m_quitWindowMode = true;
    m_applicationModule->cleanup();
    m_applicationModule->windowCloseAnimation();
    //Write configuration files
    writeSystemConfigToFile();
}

void MusicApplication::positionChanged(qint64 position)
{
    m_rightAreaWidget->updateCurrentLrc(position, m_player->duration(), isPlaying());
    m_ui->musicTimeWidget->setValue(position);

    if(m_playlist->isEmpty())
    {
        m_ui->playCurrentTime->setText(TTK_TIME_INIT);
    }
    else
    {
        m_ui->playCurrentTime->setText(TTKTime::formatDuration(position));
    }
    //Show the current play time
    m_songTreeWidget->updateDurationLabel(m_ui->playCurrentTime->text(), m_ui->playTotalTime->text());
}

void MusicApplication::durationChanged(qint64 duration)
{
    //Show the current play total time
    m_ui->musicTimeWidget->setRange(0, duration);
    m_ui->playTotalTime->setText(TTK_SEPARATOR + TTKTime::formatDuration(duration));
    //Loading the current song lrc
    loadCurrentSongLrc();
    //Update song quality label
    m_ui->musicQualityLabel->updateQuality();
}

void MusicApplication::playerStateChanged(TTK::PlayState state)
{
    if(state == TTK::PlayState::Stopped)
    {
        const bool concise = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
        m_ui->musicKey->setStyleSheet(concise ? TTK::UI::TinyBtnPlay : TTK::UI::BtnPlay);
    }
}

void MusicApplication::showCurrentSong()
{
    QString name;
    const MusicPlayItem &item = m_playlist->currentItem();
    const int index = m_songTreeWidget->mapSongIndexByFilePath(item.m_playlistRow, item.m_path);
    m_currentPlaylistRow = item.m_playlistRow;
    m_songTreeWidget->setPlayRowIndex(item.m_playlistRow);

    if(index > TTK_NORMAL_LEVEL) //The list to end
    {
        name = currentFileName();
        ///detecting whether the file has been downloaded
        bool exist = false;
        containsDownloadItem(exist);
        m_ui->musicDownload->setStyleSheet(exist ? TTK::UI::BtnDownload : TTK::UI::BtnUnDownload);
        //
        exist = containsLovestItem();
        m_ui->musicBestLove->setStyleSheet(exist ? TTK::UI::BtnLove : TTK::UI::BtnUnLove);
        //
        m_songTreeWidget->selectRow(index);
    }
    else
    {
        const bool concise = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
        m_ui->musicBestLove->setStyleSheet(TTK::UI::BtnUnLove);
        m_ui->musicDownload->setStyleSheet(TTK::UI::BtnUnDownload);
        m_ui->musicKey->setStyleSheet(concise ? TTK::UI::TinyBtnPlay : TTK::UI::BtnPlay);
        m_player->stop();
        m_rightAreaWidget->stopDrawLrc();

        m_bottomAreaWidget->setCurrentPlayState(isPlaying());
        m_rightAreaWidget->setCurrentPlayState(isPlaying());
        m_topAreaWidget->setCurrentPlayState(isPlaying());
        m_ui->musicTimeWidget->setCurrentPlayState(isPlaying());

        durationChanged(0);
        positionChanged(0);
        m_rightAreaWidget->loadCurrentSongLrc(name, name);
    }

    m_ui->musicPlayedList->selectCurrentMedia();
    m_songTreeWidget->appendRecentSongs(index);
    m_songTreeWidget->setSongPlayCount(index);
    m_ui->musicSongTitle->setText(name);
    m_ui->musicMoreFunction->setCurrentSongName(name);
    //Show the current play song information
    m_bottomAreaWidget->setLabelText(name);
    m_topAreaWidget->setLabelText(name);
    //display current ArtTheme pic
    m_rightAreaWidget->checkMetaDataValid(false);
    G_BACKGROUND_PTR->setArtistName(currentFileName());
    m_topAreaWidget->backgroundThemeDownloadFinished();
}

void MusicApplication::switchToPlayState()
{
    if(m_playlist->isEmpty())
    {
        return; //The playlist is not performing space-time
    }

    const bool concise = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
    if(isPlaying())
    {
        m_ui->musicKey->setStyleSheet(concise ? TTK::UI::TinyBtnPlay : TTK::UI::BtnPlay);
        m_player->pause();
        m_topAreaWidget->setBackgroundAnimation(false);
        m_rightAreaWidget->stopDrawLrc();
    }
    else
    {
        m_ui->musicKey->setStyleSheet(concise ? TTK::UI::TinyBtnPause : TTK::UI::BtnPause);
        m_player->play();
        m_topAreaWidget->backgroundThemeDownloadFinished();
        m_rightAreaWidget->startDrawLrc();
    }

    m_bottomAreaWidget->setCurrentPlayState(isPlaying());
    m_rightAreaWidget->setCurrentPlayState(isPlaying());
    m_topAreaWidget->setCurrentPlayState(isPlaying());
    m_ui->musicTimeWidget->setCurrentPlayState(isPlaying());
}

void MusicApplication::switchToStopState()
{
    if(!isPlaying())
    {
        return;
    }

    m_playlist->setCurrentIndex(TTK_NORMAL_LEVEL);
}

void MusicApplication::playPrevious()
{
    if(m_playlist->isEmpty())
    {
        return; //The playlist is not performing space-time
    }

    m_ui->musicPlayedList->clearQueueState();
    m_playlist->setCurrentIndex(PLAY_PREVIOUS_LEVEL);

    m_player->stop();
    switchToPlayState();
}

void MusicApplication::playNext()
{
    if(m_playlist->isEmpty())
    {
        return; //The playlist is not performing space-time
    }

    m_playlist->setCurrentIndex(PLAY_NEXT_LEVEL);

    m_player->stop();
    switchToPlayState();
}

void MusicApplication::playOrder()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::Order);
    m_ui->musicPlayMode->setPlaybackMode(TTK::PlayMode::Order);
}

void MusicApplication::playRandom()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::Random);
    m_ui->musicPlayMode->setPlaybackMode(TTK::PlayMode::Random);
}

void MusicApplication::playlistLoop()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::ListLoop);
    m_ui->musicPlayMode->setPlaybackMode(TTK::PlayMode::ListLoop);
}

void MusicApplication::playOneLoop()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::OneLoop);
    m_ui->musicPlayMode->setPlaybackMode(TTK::PlayMode::OneLoop);
}

void MusicApplication::playOnce()
{
    m_playlist->setPlaybackMode(TTK::PlayMode::Once);
    m_ui->musicPlayMode->setPlaybackMode(TTK::PlayMode::Once);
}

void MusicApplication::volumeDown()
{
    int currentVol = m_player->volume();
    currentVol -= 15;
    if(currentVol < 0)
    {
        currentVol = 0;   //reset music volume
    }
    volumeChanged(currentVol);
}

void MusicApplication::volumeUp()
{
    int currentVol = m_player->volume();
    currentVol += 15;
    if(currentVol > 100)
    {
        currentVol = 100;   //reset music volume
    }
    volumeChanged(currentVol);
}

void MusicApplication::volumeMute()
{
    m_player->setMuted(!m_player->isMuted());
    const int volume = m_player->volume();

    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_ui->musicSound->setValue(volume);
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, volume);
}

void MusicApplication::volumeChanged(int volume)
{
    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_ui->musicSound->setValue(volume);
    m_player->setVolume(volume);
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, volume);
}

void MusicApplication::importSongsPopup()
{
    QMenu menu;
    menu.setStyleSheet(TTK::UI::MenuStyle02);
    menu.addAction(tr("Open Files"), this, SLOT(importSongsByFiles()));
    menu.addAction(tr("Open Dir"), this, SLOT(importSongsByDir()));
    menu.addAction(tr("Open Url"), this, SLOT(importSongsByUrl()));
    menu.addSeparator();
    menu.addAction(tr("Files Drag Drop"))->setEnabled(false);

    TTK::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicApplication::importSongsByFiles(int index)
{
    const QStringList &files = TTK::File::getOpenFileNames(this, MusicFormats::supportMusicInputFormats());
    m_songTreeWidget->importSongsByPath(files, index);
}

void MusicApplication::importSongsByDir(int index)
{
    const QString &path = TTK::File::getExistingDirectory(this);
    if(path.isEmpty())
    {
        return;
    }

    m_songTreeWidget->importSongsByUrl(path, index);
}

void MusicApplication::importSongsByUrl()
{
    MusicInputDialog dialog;
    dialog.setPlaceholderText(tr("Please enter the local or network playback address"));
    if(!dialog.exec())
    {
        return;
    }

    const QString &path = dialog.text();
    if(path.isEmpty())
    {
        return;
    }

    m_songTreeWidget->importSongsByUrl(path, TTK_LOW_LEVEL);
}

void MusicApplication::importSongsItemList()
{
    const QStringList &files = TTK::File::getOpenFileNames(this, MusicFormats::supportPlaylistInputFormats());
    if(files.isEmpty())
    {
        return;
    }

    MusicPlaylistManager manager;
    MusicSongItemList items;
    manager.readSongItems(files, items);
    m_songTreeWidget->appendSongItemList(items);
}

void MusicApplication::exportSongsItem(int index)
{
    const QString &path = TTK::File::getSaveFileName(this, MusicFormats::supportPlaylistOutputFormats());
    if(path.isEmpty())
    {
        return;
    }

    const MusicSongItemList &items = m_songTreeWidget->items();
    if(index < 0 || index >= items.count())
    {
        return;
    }

    MusicPlaylistManager manager;
    manager.writeSongItem(path, {items[index]});

    MusicToastLabel::popup(tr("Export current file success"));
}

void MusicApplication::exportSongsItemList()
{
    const QString &path = TTK::File::getSaveFileName(this, MusicFormats::supportPlaylistOutputFormats());
    if(path.isEmpty())
    {
        return;
    }

    MusicPlaylistManager manager;
    manager.writeSongItem(path, m_songTreeWidget->items());

    MusicToastLabel::popup(tr("Export current file success"));
}

void MusicApplication::playSortBy(int row)
{
    const QString &path = m_songTreeWidget->mapFilePathBySongIndex(m_currentPlaylistRow, row);
    m_playlist->setCurrentIndex(m_currentPlaylistRow, path);
}

void MusicApplication::playedIndexBy(int row)
{
    m_playlist->setCurrentIndex(row);
    m_player->stop();
    switchToPlayState();
}

void MusicApplication::playIndexBy(int row)
{
    m_player->stop();

    if(row != -1)
    {
        generatePlaylistItems();
        m_currentPlaylistRow = row;
        m_playlist->setCurrentIndex(m_currentPlaylistRow, m_songTreeWidget->mapFilePathBySongIndex(m_currentPlaylistRow, row));
    }
    else
    {
        m_playlist->clear();
        m_currentPlaylistRow = row;
        m_playlist->setCurrentIndex(row);
    }
}

void MusicApplication::playIndexBy(int row, int)
{
    m_player->stop();

    if(m_currentPlaylistRow != m_songTreeWidget->currentIndex() || m_playlist->isEmpty())
    {
        generatePlaylistItems();
        const MusicSongItemList &items = m_songTreeWidget->items();
        const int index = m_songTreeWidget->currentIndex();
        if(0 <= index && index < items.count())
        {
            m_ui->musicPlayedList->append(items[index].m_songs);
        }
    }

    m_songTreeWidget->removeSearchResult(row);
    m_playlist->setCurrentIndex(m_currentPlaylistRow, m_songTreeWidget->mapFilePathBySongIndex(m_currentPlaylistRow, row));
    switchToPlayState();
}

void MusicApplication::playIndexClicked(int row, int column)
{
    if(m_currentPlaylistRow == m_songTreeWidget->currentIndex())
    {
        generatePlaylistItems();
        const MusicSongItemList &items = m_songTreeWidget->items();
        const int index = m_songTreeWidget->currentIndex();
        if(0 <= index && index < items.count())
        {
            m_ui->musicPlayedList->append(items[index].m_songs);
        }
    }

    playIndexBy(row, column);
}

void MusicApplication::playAnyTimePosition(int time)
{
    //Drag the progress indicator to rewind or fast-forward through the current song
    m_player->setPosition(time);
    //Set lrc corrent to show
    m_rightAreaWidget->findTimePosition(time);
}

void MusicApplication::showSettingWidget()
{
    m_rightAreaWidget->showSettingWidget();
}

void MusicApplication::currentPlayLocation()
{
    if(m_playlist->isEmpty())
    {
        return;
    }

    const MusicPlayItem &item = m_playlist->currentItem();
    const int index = m_songTreeWidget->mapSongIndexByFilePath(item.m_playlistRow, item.m_path);
    m_songTreeWidget->playLocation(index);
}

void MusicApplication::addSongToLovestList(bool state)
{
    const MusicPlayItem &item = m_playlist->currentItem();
    const int index = m_songTreeWidget->mapSongIndexByFilePath(item.m_playlistRow, item.m_path);
    if(m_playlist->isEmpty() || index < 0)
    {
        return;
    }

    bool contains = true;
    if(state)
    {
        contains = containsLovestItem();
        if(contains)
        {
            m_songTreeWidget->songToLovestListAt(false, index);
            m_leftAreaWidget->setSongLoveState(false);
        }
        else
        {
            m_songTreeWidget->songToLovestListAt(true, index);
            m_leftAreaWidget->setSongLoveState(true);
        }
    }
    else
    {
        m_songTreeWidget->songToLovestListAt(false, index);
        m_leftAreaWidget->setSongLoveState(false);
    }

    MusicToastLabel::popup(!contains ? tr("Add music to lovest list done") : tr("Remove music to lovest list done"));
}

void MusicApplication::windowConciseChanged()
{
    m_bottomAreaWidget->setWindowConcise();
    m_topAreaWidget->setBackgroundAnimation(!G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool());
}

void MusicApplication::enhancedSongChanged(int type)
{
    m_player->setEnhanced(TTKStaticCast(MusicPlayer::Enhance, type));
}

void MusicApplication::createRightMenu()
{
    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    QMenu *addNewFilesMenu = menu.addMenu(tr("Add New Files"));
    addNewFilesMenu->addAction(tr("Open Files"), this, SLOT(importSongsByFiles()));
    addNewFilesMenu->addAction(tr("Open Dir"), this, SLOT(importSongsByDir()));
    addNewFilesMenu->addAction(tr("Open Url"), this, SLOT(importSongsByUrl()));
    TTK::Widget::adjustMenuPosition(addNewFilesMenu);

    QMenu *playbackModeMenu = menu.addMenu(tr("Playback Mode"));

    QList<QAction*> actions;
    actions << playbackModeMenu->addAction(tr("Play Once"), this, SLOT(playOnce()));
    actions << playbackModeMenu->addAction(tr("Single Cycle"), this, SLOT(playOneLoop()));
    actions << playbackModeMenu->addAction(tr("Order Play"), this, SLOT(playOrder()));
    actions << playbackModeMenu->addAction(tr("List Cycle"), this, SLOT(playlistLoop()));
    actions << playbackModeMenu->addAction(tr("Random Play"), this, SLOT(playRandom()));

    int index = TTK_NORMAL_LEVEL;
    switch(playMode())
    {
        case TTK::PlayMode::Once: index = 0; break;
        case TTK::PlayMode::OneLoop: index = 1; break;
        case TTK::PlayMode::Order: index = 2; break;
        case TTK::PlayMode::ListLoop: index = 3; break;
        case TTK::PlayMode::Random: index = 4; break;
        default: break;
    }

    if(index > TTK_NORMAL_LEVEL && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }

    menu.addSeparator();

    QMenu *remoteControlMenu = menu.addMenu(tr("Remote Control"));
    remoteControlMenu->addAction(tr("Square Remote"), m_topAreaWidget, SLOT(showSquareRemote()));
    remoteControlMenu->addAction(tr("Rectangle Remote"), m_topAreaWidget, SLOT(showRectangleRemote()));
    remoteControlMenu->addAction(tr("Simple Style Remote"), m_topAreaWidget, SLOT(showSimpleStyleRemote()));
    remoteControlMenu->addAction(tr("Complex Style Remote"), m_topAreaWidget, SLOT(showComplexStyleRemote()));
    remoteControlMenu->addAction(tr("Ripple Remote"), m_topAreaWidget, SLOT(showRippleRemote()));
    remoteControlMenu->addAction(tr("Delete Remote"), m_topAreaWidget, SLOT(deleteCurrentRemote()));
    TTK::Widget::adjustMenuPosition(remoteControlMenu);

    menu.addAction(QIcon(":/contextMenu/btn_equalizer"), tr("Equalizer"), m_applicationModule, SLOT(showEqualizerWidget()));
    menu.addAction(tr("Sound Effect"), m_applicationModule, SLOT(showSoundEffectWidget()));
    menu.addAction(tr("Timing Settings"), m_applicationModule, SLOT(showTimerWidget()));
    menu.addAction(tr("Music Spectrum"), m_applicationModule, SLOT(showSpectrumWidget()));
    menu.addSeparator();

    QAction *window = menu.addAction(tr("Window Top"), m_applicationModule, SLOT(setWindowToTop()));
    window->setIcon(QIcon(m_applicationModule->windowToTop() ? ":/contextMenu/btn_selected" : QString()));
    menu.addAction(tr("Reset Window"), m_applicationModule, SLOT(resetWindowGeometry()));

    QMenu *downloadMenu = menu.addMenu(tr("Download"));
    index = !G_SETTING_PTR->value(MusicSettingManager::DownloadLimitEnable).toInt();
    actions.clear();
    actions << downloadMenu->addAction(tr("Full Download"), MusicRightAreaWidget::instance(), SLOT(changeDownloadFulllyWidget()));
    actions << downloadMenu->addAction(tr("Custom"), MusicRightAreaWidget::instance(), SLOT(changeDownloadCustumWidget()));

    if(index > TTK_NORMAL_LEVEL && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }

    if(!m_applicationModule->isLastedVersion())
    {
        menu.addAction(QIcon(":/contextMenu/btn_update"), tr("New Version"), m_applicationModule, SLOT(showVersionWidget()));
    }
    menu.addAction(QIcon(":/contextMenu/btn_setting"), tr("Settings"), this, SLOT(showSettingWidget()));

    QMenu *informationMenu = menu.addMenu(QIcon(":/contextMenu/btn_about"), tr("About"));
    informationMenu->addAction(QIcon(":/contextMenu/btn_bug_reoprt"), tr("Bug Report"), m_applicationModule, SLOT(showBugReportView()));
    informationMenu->addAction(QIcon(":/contextMenu/btn_about"), tr("Version") + TTK_STR_CAT(TTK_VERSION_STR, TTK_VERSION_TIME_STR), m_applicationModule, SLOT(showAboutWidget()));
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_quit"), tr("Quit"), this, SLOT(quitWindow()));
    menu.exec(QCursor::pos());
}

void MusicApplication::applyParameter()
{
    //This attribute is effective immediately.
    m_playlist->setShuffleMode(G_SETTING_PTR->value(MusicSettingManager::OtherRandomShuffleMode).toBool());
    //
    m_applicationModule->applyParameter();
    m_rightAreaWidget->applyParameter();
    m_bottomAreaWidget->applyParameter();
}

void MusicApplication::removeLoveItemAt(const QString &path, bool current)
{
    removeItemAt({path}, false, current, MUSIC_LOVEST_LIST);
}

void MusicApplication::removeItemAt(const QStringList &path, bool remove, bool current, int playlistRow)
{
    if(path.isEmpty())
    {
        return;
    }

    if(current)
    {
        const MusicPlayItem &item = m_playlist->currentItem();
        TTKIntList index;
        for(const QString &p : qAsConst(path))
        {
            int idx = -1;
            do
            {
                idx = m_playlist->find(item.m_playlistRow, p, idx + 1);
                if(idx != -1)
                {
                    index << idx;
                }
            } while(idx != -1);
        }

        if(index.isEmpty())
        {
            return;
        }
        std::sort(index.begin(), index.end());

        bool contains = false; ///the play one is delete list
        int oldIndex = m_playlist->currentIndex();
        ///check if delete one that the play one
        if(index.count() == 1 && index.front() == oldIndex)
        {
            contains = true;
        }
        ///other ways
        for(int i = index.count() - 1; i >= 0; --i)
        {
            m_ui->musicPlayedList->remove(index[i]);
            if(i != 0 && !contains && oldIndex <= index[i] && oldIndex >= index[i - 1])
            {
                oldIndex -= i;
                contains = true;
            }
        }

        if(!contains && m_playlist->currentIndex() > index[0])
        {
            oldIndex -= index.count();
        }

        if(oldIndex == m_playlist->count()) ///Play index error correction
        {
            --oldIndex;
        }
        m_playlist->setCurrentIndex(oldIndex);

        if(contains)
        {
            //The corresponding item is deleted from the playlist
            m_player->stop();
            switchToPlayState();

            if(remove)
            {
                if(playlistRow != MUSIC_NETWORK_LIST)
                {
                    G_DISPATCH_PTR->dispatch(TTKDispatchManager::Module::FileRemove, item.m_path);
                }
                else
                {
                    G_DISPATCH_PTR->dispatch(TTKDispatchManager::Module::FileRemove, TTK::generateNetworkSongPath(item.m_path));
                    G_DISPATCH_PTR->dispatch(TTKDispatchManager::Module::FileRemove, TTK::generateNetworkSongMetaPath(item.m_path));
                }
            }
        }
    }
    else
    {
        for(const QString &p : qAsConst(path))
        {
            m_ui->musicPlayedList->remove(playlistRow, p);
        }
    }
}

void MusicApplication::currentLrcUpdated()
{
    const QString &fileName = currentFileName();
    QFile::remove(TTK::String::lrcDirPrefix() + fileName + LRC_FILE);
    m_rightAreaWidget->checkMetaDataValid(true);
}

void MusicApplication::resetCurrentSongLrcIndex()
{
    const int pos = m_player->position();
    if(pos != -1)
    {
        m_rightAreaWidget->findTimePosition(pos);
    }
}

void MusicApplication::updateCurrentTime(qint64 pos)
{
    if(isPlaying())
    {
        m_player->setPosition(pos);
    }
}

void MusicApplication::setPlaySongChanged(int index)
{
    if(m_playlist->isEmpty() || index < 0 || index >= m_playlist->count())
    {
        return;
    }

    playIndexBy(index, TTK_NORMAL_LEVEL);
}

void MusicApplication::currentPlaylist(QStringList &list)
{
    list = m_songTreeWidget->songsFileName(m_songTreeWidget->currentIndex());
}

void MusicApplication::resizeEvent(QResizeEvent *event)
{
    if(m_ui->background->isRunning())
    {
        event->ignore();
        return;
    }

    if(!m_quitWindowMode)
    {
        G_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, size());
        m_topAreaWidget->backgroundThemeChangedByResize();
        m_rightAreaWidget->resizeWindow();
        m_bottomAreaWidget->resizeWindow();
        TTKAbstractMoveResizeWidget::resizeEvent(event);
    }
    else
    {
        setMinimumSize(0, 0); ///remove fixed size
        m_ui->background->stop();
        event->ignore();
    }
}

void MusicApplication::closeEvent(QCloseEvent *event)
{
    TTKAbstractMoveResizeWidget::closeEvent(event);
    event->ignore();

    if(!m_bottomAreaWidget->systemCloseConfig() && m_bottomAreaWidget->systemTrayIsVisible())
    {
        hide();
        m_bottomAreaWidget->showMessage(tr("Prompt"), tr("TTKMusicPlayer will run in the background"));
    }
    else
    {
        quitWindow();
    }
}

void MusicApplication::contextMenuEvent(QContextMenuEvent *event)
{
    const int h = event->y();
    if(h < m_ui->topWidget->height() || h > height() - m_ui->bottomWidget->height())
    {
        TTKAbstractMoveResizeWidget::contextMenuEvent(event);
        createRightMenu();
    }
    else
    {
        event->accept();
    }
}

void MusicApplication::enterEvent(QtEnterEvent *event)
{
    TTKAbstractMoveResizeWidget::enterEvent(event);
    m_applicationModule->sideAnimationByOff();
}

void MusicApplication::leaveEvent(QEvent *event)
{
    TTKAbstractMoveResizeWidget::leaveEvent(event);
    m_applicationModule->sideAnimationByOn();
}

void MusicApplication::mouseMoveEvent(QMouseEvent *event)
{
    TTKAbstractMoveResizeWidget::mouseMoveEvent(event);

    // ignore if the window mode is concise
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        return;
    }

    if(m_ui->background->isRunning())
    {
        setFixedSize(G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize());
    }
    else
    {
        const QSize &size = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
        setMinimumSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
        setMaximumSize(size.width(), size.height());
    }
}

void MusicApplication::mouseReleaseEvent(QMouseEvent *event)
{
    TTKAbstractMoveResizeWidget::mouseReleaseEvent(event);
    m_applicationModule->sideAnimationByOn();
}

void MusicApplication::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!m_ui->background->isRunning() && QtMouseY(event) <= m_ui->topWidget->height() && !G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        TTKAbstractMoveResizeWidget::mouseDoubleClickEvent(event);
        if(event->buttons() == Qt::LeftButton)
        {
            isMaximized() ? showNormal() : showMaximized();
        }
    }
    else
    {
        event->ignore();
    }
}

bool MusicApplication::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_ui->centerWidget && (QEvent::ContextMenu == event->type() || QEvent::MouseButtonPress == event->type()))
    {
        return true;
    }
    return TTKAbstractMoveResizeWidget::eventFilter(object, event);
}

void MusicApplication::generatePlaylistItems()
{
    m_currentPlaylistRow = m_songTreeWidget->currentIndex();
    m_playlist->add(m_currentPlaylistRow, m_songTreeWidget->songsFilePath(m_currentPlaylistRow));
    m_songTreeWidget->setPlayRowIndex(m_currentPlaylistRow);
}

void MusicApplication::readSystemConfigFromFile()
{
    int value = TTK_NORMAL_LEVEL;
    //Path configuration song
    MusicSongItemList items;
    {
        MusicTKPLConfigManager manager;
        if(manager.fromFile(PLAYLIST_PATH_FULL))
        {
            manager.readBuffer(items);
        }
    }

    MusicConfigManager manager;
    if(!manager.fromFile(COFIG_PATH_FULL))
    {
        TTK_ERROR_STREAM("Load config file error:" << COFIG_PATH_FULL);
        return;
    }

    manager.readBuffer();

    m_applicationModule->loadNetWorkSetting();
    const bool success = m_songTreeWidget->addSongItemList(items);

    switch(TTKStaticCast(TTK::PlayMode, G_SETTING_PTR->value(MusicSettingManager::PlayMode).toInt()))
    {
        case TTK::PlayMode::Order: playOrder();break;
        case TTK::PlayMode::Random: playRandom();break;
        case TTK::PlayMode::ListLoop: playlistLoop();break;
        case TTK::PlayMode::OneLoop: playOneLoop();break;
        case TTK::PlayMode::Once: playOnce();break;
        default:break;
    }

    value = G_SETTING_PTR->value(MusicSettingManager::RemoteWidgetMode).toInt();
    if(value != 0)
    {
        m_topAreaWidget->remoteTypeChanged(value);
    }

    //The size of the volume of the allocation of songs
    volumeChanged(G_SETTING_PTR->value(MusicSettingManager::Volume).toInt());

    //Configure playback mode
    m_ui->musicEnhancedButton->setEnhancedSongConfig(G_SETTING_PTR->value(MusicSettingManager::EnhancedMusicIndex).toInt());
    m_applicationModule->updateSoundEffectConfig(true);
    if(G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt() == 1)
    {
        m_player->setEqualizerConfig();
    }

    //global hotkey
    if(G_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toBool())
    {
        QStringList hotkeys = G_SETTING_PTR->value(MusicSettingManager::HotkeyValue).toString().split(TTK_SPLITER);
        if(hotkeys.count() != G_HOTKEY_PTR->count())
        {
            hotkeys = G_HOTKEY_PTR->defaultKeys();
        }

        for(int i = 0; i < hotkeys.count(); ++i)
        {
            G_HOTKEY_PTR->setHotKey(i, hotkeys[i]);
        }
        //
        G_HOTKEY_PTR->setEnabled(true);
    }

    //musicSetting
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
    //Just always set fade false, because it is not finished yet.
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable, false);
#ifdef Q_OS_UNIX
    //Disable window quit mode on unix
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode, false);
#endif
    //Set interior lrc mode always on
    m_rightAreaWidget->setInteriorLrcVisible(true);

    //Set the desktop lrc should be shown
    m_rightAreaWidget->setWindowLrcTypeChanged();
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, manager.readShowDesktopLrcGeometry());

    //Set the current background color and alpha value
    m_topAreaWidget->setBackgroundParameter();

    //Configuration from next time also stopped at the last record.
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    //add new music file to playlist
    value = lastPlayIndex[1].toInt();
    m_playlist->add(value, m_songTreeWidget->songsFilePath(value));
    if(TTK_NORMAL_LEVEL < value && value < items.count())
    {
        m_ui->musicPlayedList->append(items[value].m_songs);
    }

    applyParameter();

    if(success && lastPlayIndex[0] == "1")
    {
        TTK_SIGNLE_SHOT(m_songTreeWidget, updateCurrentIndex, TTK_SLOT);
        const int index = lastPlayIndex[2].toInt();
        m_currentPlaylistRow = (index == TTK_NORMAL_LEVEL) ? TTK_NORMAL_LEVEL : value;
        m_playlist->blockSignals(true);
        m_playlist->setCurrentIndex(m_currentPlaylistRow, m_songTreeWidget->mapFilePathBySongIndex(m_currentPlaylistRow, index));
        m_playlist->blockSignals(false);
        m_ui->musicPlayedList->selectCurrentMedia();
    }

    //Configure automatic playback
    if(G_SETTING_PTR->value(MusicSettingManager::StartUpPlayMode).toInt() == 1)
    {
        switchToPlayState();
    }

    m_bottomAreaWidget->setCurrentPlayState(isPlaying());
    m_rightAreaWidget->setCurrentPlayState(isPlaying());
    m_topAreaWidget->setCurrentPlayState(isPlaying());

    //Set the lrc color the user set
    m_bottomAreaWidget->lockDesktopLrc(G_SETTING_PTR->value(MusicSettingManager::DLrcLockedMode).toInt());

    //init or reset the window
    value = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt();
    m_bottomAreaWidget->setDestopLrcVisible(value);
    m_rightAreaWidget->setDestopLrcVisible(value);

    //Reset geometry
    setGeometry(manager.readWindowGeometry());

    //Reset window concise
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        windowConciseChanged();
    }

    //Update check on
    if(G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateEnable).toBool())
    {
        m_applicationModule->soureUpdateCheck();
    }
}

void MusicApplication::writeSystemConfigToFile()
{
    MusicConfigManager manager;
    if(!manager.load(COFIG_PATH_FULL))
    {
        TTK_ERROR_STREAM("Load config file error:" << COFIG_PATH_FULL);
        return;
    }

    m_applicationModule->updateSoundEffectConfig(false);

    G_SETTING_PTR->setValue(MusicSettingManager::WidgetPosition, pos());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicIndex, TTKStaticCast(int, m_player->enhanced()));
    G_SETTING_PTR->setValue(MusicSettingManager::PlayMode, TTKStaticCast(int, m_playlist->playbackMode()));
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, m_ui->musicSound->value());

    QStringList lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    if(lastPlayIndex.isEmpty())
    {
        lastPlayIndex << "1" << "-1" << "-1";
    }
    else
    {
        const MusicPlayItem &item = m_playlist->currentItem();
        lastPlayIndex[1] = QString::number(item.m_playlistRow);
        lastPlayIndex[2] = QString::number(m_songTreeWidget->mapSongIndexByFilePath(item.m_playlistRow, item.m_path));
    }

    G_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndex, lastPlayIndex);
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundThemeValue, m_topAreaWidget->backgroundPath());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparent, m_topAreaWidget->backgroundTransparent());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent, m_topAreaWidget->backgroundListTransparent());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable, m_topAreaWidget->backgroundTransparentEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, m_rightAreaWidget->destopLrcVisible());
    manager.writeBuffer();

    {
        MusicTKPLConfigManager manager;
        if(!manager.load(PLAYLIST_PATH_FULL))
        {
            return;
        }

        manager.writeBuffer(m_songTreeWidget->items());
    }
}
