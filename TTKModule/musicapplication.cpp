#include "musicapplication.h"
#include "ui_musicapplication.h"
#include "musicsongssummariziedwidget.h"
#include "musicconfigmanager.h"
#include "musicplayer.h"
#include "musicformats.h"
#include "musicplaylist.h"
#include "musicbackgroundmanager.h"
#include "musicsettingmanager.h"
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

#include <QMimeData>

MusicApplication *MusicApplication::m_instance = nullptr;

MusicApplication::MusicApplication(QWidget *parent)
    : MusicAbstractMoveResizeWidget(parent),
      m_ui(new Ui::MusicApplication)
{
    m_instance = this;
#if defined Q_OS_WIN && !TTK_QT_VERSION_CHECK(5,2,0)
    setAttribute(Qt::WA_OpaquePaintEvent);
#else
    setAttribute(Qt::WA_TranslucentBackground, false);
#endif
    m_applicationObject = new MusicApplicationModule(this);
    m_topAreaWidget = new MusicTopAreaWidget(this);
    m_bottomAreaWidget = new MusicBottomAreaWidget(this);
    m_rightAreaWidget = new MusicRightAreaWidget(this);
    m_leftAreaWidget = new MusicLeftAreaWidget(this);
    //
    m_ui->setupUi(this);
    const QSize &size = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setMinimumSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    setMaximumSize(size.width(), size.height());
    //
    m_musicPlayer = new MusicPlayer(this);
    m_musicPlaylist = new MusicPlaylist(this);
    m_musicSongTreeWidget = new MusicSongsSummariziedWidget(this);
    m_ui->songsContainer->addWidget(m_musicSongTreeWidget);

    m_bottomAreaWidget->setupUi(m_ui);
    m_topAreaWidget->setupUi(m_ui);
    m_rightAreaWidget->setupUi(m_ui);
    m_leftAreaWidget->setupUi(m_ui);
    m_topAreaWidget->musicBackgroundAnimationChanged(false);

    connect(m_rightAreaWidget, SIGNAL(updateBackgroundTheme()), m_topAreaWidget, SLOT(musicBackgroundTransparentChanged()));
    connect(m_rightAreaWidget, SIGNAL(updateBackgroundThemeDownload()), m_topAreaWidget, SLOT(musicBackgroundThemeDownloadFinished()));

    setAcceptDrops(true);

    m_musicPlaylist->setPlaybackMode(MusicObject::PlayOrder); //The default is the order of play
    m_ui->musicPlayedList->setPlaylist(m_musicPlaylist);
    m_musicPlayer->setPlaylist(m_musicPlaylist);
    m_musicPlayer->setVolume(100);  //The default Volume is 100

    m_quitWindowClose = false;
    m_currentMusicSongTreeIndex = DEFAULT_NORMAL_LEVEL;

    connect(m_musicPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_musicPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_musicPlayer, SIGNAL(stateChanged(MusicObject::PlayState)), SLOT(stateChanged()));
    connect(m_musicPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(showCurrentSong()));

    connect(m_ui->musicDesktopLrc, SIGNAL(clicked(bool)), m_rightAreaWidget, SLOT(setDestopLrcVisible(bool)));

    m_ui->lrcDisplayAllButton->hide();
    m_ui->musicKey->setFocus();
    m_ui->functionsContainer->setCurrentIndex(APP_WINDOW_INDEX_0);

    m_ui->musicTimeWidget->setInputModule(this);
    G_HOTKEY_PTR->setInputModule(this);

    /////////// Mouse tracking
    m_ui->background->installEventFilter(this);
    m_ui->background->setMouseTracking(true);
    m_ui->centerWidget->installEventFilter(this);

    readSystemConfigFromFile();
    QTimer::singleShot(MT_MS, m_rightAreaWidget, SLOT(musicLoadSongIndexWidget()));
}

MusicApplication::~MusicApplication()
{
    delete m_musicPlayer;
    delete m_musicPlaylist;
    delete m_musicSongTreeWidget;
    delete m_bottomAreaWidget;
    delete m_topAreaWidget;
    delete m_rightAreaWidget;
    delete m_leftAreaWidget;
    delete m_applicationObject;
    delete m_ui;
}

MusicApplication *MusicApplication::instance()
{
    return m_instance;
}

QString MusicApplication::currentFileName() const
{
    if(m_musicPlaylist->currentIndex() < 0 || m_currentMusicSongTreeIndex < 0)
    {
        return QString();
    }

    const MusicPlayItem &item = m_musicPlaylist->currentItem();
    const MusicSongItemList items(m_musicSongTreeWidget->musicItemList());
    if(0 <= m_currentMusicSongTreeIndex && m_currentMusicSongTreeIndex < items.count())
    {
        const MusicSongList &songs = items[m_currentMusicSongTreeIndex].m_songs;
        const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(m_currentMusicSongTreeIndex, item.m_path);
        return (index != -1) ? songs[index].musicName() : QString();
    }
    return QString();
}

QString MusicApplication::currentFilePath() const
{
    if(m_musicPlaylist->currentIndex() < 0 || m_currentMusicSongTreeIndex < 0)
    {
        return QString();
    }

    const MusicPlayItem &item = m_musicPlaylist->currentItem();
    const MusicSongItemList items(m_musicSongTreeWidget->musicItemList());
    if(0 <= m_currentMusicSongTreeIndex && m_currentMusicSongTreeIndex < items.count())
    {
        const MusicSongList &songs = items[m_currentMusicSongTreeIndex].m_songs;
        const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(m_currentMusicSongTreeIndex, item.m_path);
        return (index != -1) ? songs[index].musicPath() : QString();
    }
    return QString();
}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return (m_musicPlaylist->currentIndex() == DEFAULT_NORMAL_LEVEL);
}

void MusicApplication::musicLoadCurrentSongLrc()
{
    //display current ArtTheme pic
    m_topAreaWidget->musicBackgroundThemeDownloadFinished();
    //Loading the current song lrc
    if(checkMusicListCurrentIndex())
    {
        return;
    }

    const QString &fileName = currentFileName();
    const QString &path = MusicUtils::String::lrcPrefix() + fileName + LRC_FILE;
    m_rightAreaWidget->loadCurrentSongLrc(fileName, path);
    //reset current song lrc index.
    QTimer::singleShot(MT_S2MS, this, SLOT(resetCurrentSongLrcIndex()));
}

void MusicApplication::radioExecuteOutside(const QString &path)
{
    m_leftAreaWidget->radioExecuteOutside(path);
}

void MusicApplication::musicImportSongsPathOutside(const QStringList &files, bool play)
{
    if(files.isEmpty())
    {
        return;
    }

    m_musicSongTreeWidget->importMusicSongsByPath(files);
    if(play)
    {
        musicPlayIndex(m_musicPlaylist->count() - 1, 0);
    }
}

QString MusicApplication::musicDownloadContains(bool &contains) const
{
    contains = false;
    QString path;
    if(m_musicSongTreeWidget->playToolIndex() != DEFAULT_NORMAL_LEVEL)
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        if(item.isValid())
        {
            const MusicSong currentSong(item.m_path);
            path = QString("%1%2.%3").arg(G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString(),
                                          currentSong.musicName(), currentSong.musicType());
            contains = QFile::exists(path);
        }
    }
    return path;
}

bool MusicApplication::musicLovestContains() const
{
    if(m_musicSongTreeWidget->playToolIndex() != DEFAULT_NORMAL_LEVEL)
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        const MusicSongItemList items(m_musicSongTreeWidget->musicItemList());
        if(item.isValid() && item.m_toolIndex < items.count())
        {
            const MusicSongList &currentSongs = items[item.m_toolIndex].m_songs;
            const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path);
            const MusicSongList &loveSongs = items[MUSIC_LOVEST_LIST].m_songs;
            return index != -1 ? loveSongs.contains(currentSongs[index]) : false;
        }
    }
    return false;
}

bool MusicApplication::musicLovestContains(int index) const
{
    if(m_musicSongTreeWidget->currentIndex() != DEFAULT_NORMAL_LEVEL && index > DEFAULT_NORMAL_LEVEL)
    {
        const MusicSongItemList items(m_musicSongTreeWidget->musicItemList());
        if(m_musicSongTreeWidget->currentIndex() < items.count())
        {
            const MusicSongList &currentSongs = items[m_musicSongTreeWidget->currentIndex()].m_songs;
            const MusicSongList &loveSongs = items[MUSIC_LOVEST_LIST].m_songs;
            return loveSongs.contains(currentSongs[index]);
        }
    }
    return false;
}

void MusicApplication::updateCurrentArtist()
{
    m_musicSongTreeWidget->updateCurrentArtist();
}

bool MusicApplication::isPlaying() const
{
    return m_musicPlayer->isPlaying();
}

qint64 MusicApplication::duration() const
{
    return m_musicPlayer->duration();
}

MusicObject::PlayMode MusicApplication::playMode() const
{
    return m_musicPlaylist->playbackMode();
}

void MusicApplication::quitWindowClose()
{
    //Write configuration files
    writeSystemConfigToFile();
    m_quitWindowClose = true;
    m_applicationObject->windowCloseAnimation();
}

void MusicApplication::positionChanged(qint64 position)
{
    m_rightAreaWidget->updateCurrentLrc(position, m_musicPlayer->duration(), isPlaying());
    m_ui->musicTimeWidget->setValue(position);

    if(m_musicPlaylist->isEmpty())
    {
        m_ui->playCurrentTime->setText(MUSIC_TIME_INIT);
    }
    else
    {
        m_ui->playCurrentTime->setText(MusicTime::msecTime2LabelJustified(position));
    }
    //Show the current play time
    m_musicSongTreeWidget->updateTimeLabel(m_ui->playCurrentTime->text(), m_ui->playTotalTime->text());
}

void MusicApplication::durationChanged(qint64 duration)
{
    //Show the current play total time
    m_ui->musicTimeWidget->setRange(0, duration);
    m_ui->playTotalTime->setText(TTK_SEPARATOR + MusicTime::msecTime2LabelJustified(duration));
    //Loading the current song lrc
    musicLoadCurrentSongLrc();
}

void MusicApplication::stateChanged()
{
    const bool concise = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
    m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MQSSTinyBtnPlay : MusicUIObject::MQSSBtnPlay);
}

void MusicApplication::showCurrentSong()
{
    QString name;
    const MusicPlayItem &item = m_musicPlaylist->currentItem();
    const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path);
    m_currentMusicSongTreeIndex = item.m_toolIndex;
    m_musicSongTreeWidget->setCurrentMusicSongTreeIndex(item.m_toolIndex);

    if(index > DEFAULT_NORMAL_LEVEL) //The list to end
    {
        name = currentFileName();
        ///detecting whether the file has been downloaded
        bool exist = false;
        musicDownloadContains(exist);
        m_ui->musicDownload->setStyleSheet(exist ? MusicUIObject::MQSSBtnDownload : MusicUIObject::MQSSBtnUnDownload);
        //
        exist = musicLovestContains();
        m_ui->musicBestLove->setStyleSheet(exist ? MusicUIObject::MQSSBtnLove : MusicUIObject::MQSSBtnUnLove);
        //
        m_musicSongTreeWidget->selectRow(index);
    }
    else
    {
        const bool concise = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
        m_ui->musicBestLove->setStyleSheet(MusicUIObject::MQSSBtnUnLove);
        m_ui->musicDownload->setStyleSheet(MusicUIObject::MQSSBtnUnDownload);
        m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MQSSTinyBtnPlay : MusicUIObject::MQSSBtnPlay);
        m_musicPlayer->stop();
        m_rightAreaWidget->stopDrawLrc();

        m_bottomAreaWidget->setCurrentPlayStatus(isPlaying());
        m_rightAreaWidget->setCurrentPlayStatus(isPlaying());
        m_topAreaWidget->setCurrentPlayStatus(isPlaying());
        m_ui->musicTimeWidget->setPlayState(isPlaying());

        durationChanged(0);
        positionChanged(0);
        m_rightAreaWidget->loadCurrentSongLrc(name, name);
    }

    m_ui->musicPlayedList->setCurrentIndex();
    m_musicSongTreeWidget->setRecentMusicSongs(index);
    m_musicSongTreeWidget->setMusicPlayCount(index);
    m_ui->showCurrentSong->setText(name);
    m_ui->musicMoreFunction->setCurrentSongName(name);
    //Show the current play song information
    G_BACKGROUND_PTR->clearArtistName();
    m_rightAreaWidget->checkLrcValid();
    m_bottomAreaWidget->setLabelText(name);
    m_topAreaWidget->setLabelText(name);
    //display current ArtTheme pic
    G_BACKGROUND_PTR->setArtistName(currentFileName());
    m_topAreaWidget->musicBackgroundThemeDownloadFinished();
}

void MusicApplication::musicStatePlay()
{
    if(m_musicPlaylist->isEmpty())
    {
        return; //The playlist is not performing space-time
    }

    const bool concise = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
    if(isPlaying())
    {
        m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MQSSTinyBtnPlay : MusicUIObject::MQSSBtnPlay);
        m_musicPlayer->pause();
        m_topAreaWidget->setBackgroundAnimation(false);
        m_rightAreaWidget->stopDrawLrc();
    }
    else
    {
        m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MQSSTinyBtnPause : MusicUIObject::MQSSBtnPause);
        m_musicPlayer->play();
        m_topAreaWidget->musicBackgroundThemeDownloadFinished();
        m_rightAreaWidget->startDrawLrc();
    }

    m_bottomAreaWidget->setCurrentPlayStatus(isPlaying());
    m_rightAreaWidget->setCurrentPlayStatus(isPlaying());
    m_topAreaWidget->setCurrentPlayStatus(isPlaying());
    m_ui->musicTimeWidget->setPlayState(isPlaying());
}

void MusicApplication::musicStateStop()
{
    if(!isPlaying())
    {
        return;
    }

    m_musicPlaylist->setCurrentIndex(DEFAULT_NORMAL_LEVEL);
}

void MusicApplication::musicPlayPrevious()
{
    if(m_musicPlaylist->isEmpty())
    {
        return; //The playlist is not performing space-time
    }

    if(m_musicPlaylist->playbackMode() == MusicObject::PlayRandom)
    {
        m_musicPlaylist->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playPrevious();
    }

    m_musicPlayer->stop();
    musicStatePlay();
}

void MusicApplication::musicPlayNext()
{
    if(m_musicPlaylist->isEmpty())
    {
        return; //The playlist is not performing space-time
    }

    if(m_musicPlaylist->playbackMode() == MusicObject::PlayRandom)
    {
        m_musicPlaylist->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playNext();
    }

    m_musicPlayer->stop();
    musicStatePlay();
}

void MusicApplication::musicPlayOrder()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PlayOrder);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PlayOrder);
}

void MusicApplication::musicPlayRandom()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PlayRandom);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PlayRandom);
}

void MusicApplication::musicPlaylistLoop()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PlaylistLoop);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PlaylistLoop);
}

void MusicApplication::musicPlayOneLoop()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PlayOneLoop);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PlayOneLoop);
}

void MusicApplication::musicPlayOnce()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PlayOnce);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PlayOnce);
}

void MusicApplication::musicVolumeMute()
{
    m_musicPlayer->setMuted(!m_musicPlayer->isMuted());
    const int volume = m_musicPlayer->volume();

    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_ui->musicSound->setValue(volume);
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, volume);
}

void MusicApplication::musicVolumeChanged(int volume)
{
    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_ui->musicSound->setValue(volume);
    m_musicPlayer->setVolume(volume);
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, volume);
}

void MusicApplication::musicImportSongs()
{
    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(tr("Open Files"), this, SLOT(musicImportSongsByFiles()));
    menu.addAction(tr("Open Dir"), this, SLOT(musicImportSongsByDir()));
    menu.addAction(tr("Open Url"), this, SLOT(musicImportSongsByUrl()));
    menu.addSeparator();
    menu.addAction(tr("Files Drag Drop"))->setEnabled(false);

    MusicUtils::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicApplication::musicImportSongsByFiles()
{
    const QStringList &files = MusicUtils::File::openFilesDialog(this, MusicFormats::supportMusicInputFormats());
    m_musicSongTreeWidget->importMusicSongsByPath(files);
}

void MusicApplication::musicImportSongsByDir()
{
    const QString &path = MusicUtils::File::openDirectoryDialog(this);
    if(path.isEmpty())
    {
        return;
    }

    m_musicSongTreeWidget->importMusicSongsByUrl(path);
}

void MusicApplication::musicImportSongsByUrl()
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

    m_musicSongTreeWidget->importMusicSongsByUrl(path);
}

void MusicApplication::musicImportSongsItemList()
{
    const QStringList &files = MusicUtils::File::openFilesDialog(this, MusicFormats::supportPlaylistInputFormats());
    if(files.isEmpty())
    {
        return;
    }

    MusicPlaylistManager manager;
    MusicSongItemList items;
    manager.musicSongItems(files, items);
    m_musicSongTreeWidget->appendMusicItemList(items);
}

void MusicApplication::musicExportSongsItemList(int index)
{
    const QString &path = MusicUtils::File::saveFileDialog(this, MusicFormats::supportPlaylistOutputFormats());
    if(path.isEmpty())
    {
        return;
    }

    const MusicSongItemList &items = m_musicSongTreeWidget->musicItemList();
    if(index < 0 || index >= items.count())
    {
        return;
    }

    MusicPlaylistManager manager;
    manager.setMusicSongItem(path, items[index]);
}

void MusicApplication::musicPlaySort(int row)
{
    const QString &path = m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, row);
    m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, path);
}

void MusicApplication::musicPlayedIndex(int row)
{
    m_musicPlaylist->setCurrentIndex(row);
    m_musicPlayer->stop();
    musicStatePlay();
}

void MusicApplication::musicPlayIndex(int row)
{
    m_musicPlayer->stop();
    if(row != -1)
    {
        setMusicPlayIndex();
        m_currentMusicSongTreeIndex = row;
        m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, row));
    }
    else
    {
        m_musicPlaylist->clear();
        m_currentMusicSongTreeIndex = row;
        m_musicPlaylist->setCurrentIndex(row);
    }
}

void MusicApplication::musicPlayIndex(int row, int)
{
    m_musicPlayer->stop();
    if(m_currentMusicSongTreeIndex != m_musicSongTreeWidget->currentIndex() || m_musicPlaylist->count() == 0)
    {
        setMusicPlayIndex();
        const MusicSongItemList items(m_musicSongTreeWidget->musicItemList());
        const int index = m_musicSongTreeWidget->currentIndex();
        if(0 <= index && index < items.count())
        {
            m_ui->musicPlayedList->append(items[index].m_songs);
        }
    }

    row = m_musicSongTreeWidget->cleanSearchResult(row);
    m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, row));
    musicStatePlay();
}

void MusicApplication::musicPlayIndexClicked(int row, int col)
{
    if(m_currentMusicSongTreeIndex == m_musicSongTreeWidget->currentIndex())
    {
        setMusicPlayIndex();
        const MusicSongItemList items(m_musicSongTreeWidget->musicItemList());
        const int index = m_musicSongTreeWidget->currentIndex();
        if(0 <= index && index < items.count())
        {
            m_ui->musicPlayedList->append(items[index].m_songs);
        }
    }
    musicPlayIndex(row, col);
}

void MusicApplication::musicPlayAnyTimeAt(int value)
{
    //Drag the progress indicator to rewind or fast-forward through the current song
    m_musicPlayer->setPosition(value);
    //Set lrc corrent to show
    m_rightAreaWidget->setSongSpeedAndSlow(value);
}

void MusicApplication::musicActionVolumeSub()
{
    int currentVol = m_musicPlayer->volume();
    currentVol -= 15;
    if(currentVol < 0)
    {
        currentVol = 0;   //reset music volume
    }
    musicVolumeChanged(currentVol);
}

void MusicApplication::musicActionVolumePlus()
{
    int currentVol = m_musicPlayer->volume();
    currentVol += 15;
    if(currentVol > 100)
    {
        currentVol = 100;   //reset music volume
    }
    musicVolumeChanged(currentVol);
}

void MusicApplication::musicSetting()
{
    m_rightAreaWidget->showSettingWidget();
}

void MusicApplication::musicCurrentPlayLocation()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;
    }

    const MusicPlayItem &item = m_musicPlaylist->currentItem();
    const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path);
    m_musicSongTreeWidget->playLocation(index);
}

void MusicApplication::musicAddSongToLovestList()
{
    musicAddSongToLovestList(true);
}

void MusicApplication::musicAddSongToLovestList(bool state)
{
    const MusicPlayItem &item = m_musicPlaylist->currentItem();
    const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path);
    if(m_musicPlaylist->isEmpty() || index < 0)
    {
        return;
    }

    bool contains = true;
    if(state)
    {
        contains = musicLovestContains();
        if(contains)
        {
            m_musicSongTreeWidget->musicSongToLovestListAt(false, index);
            m_leftAreaWidget->musictLoveStateClicked(false);
        }
        else
        {
            m_musicSongTreeWidget->musicSongToLovestListAt(true, index);
            m_leftAreaWidget->musictLoveStateClicked(true);
        }
    }
    else
    {
        m_musicSongTreeWidget->musicSongToLovestListAt(false, index);
        m_leftAreaWidget->musictLoveStateClicked(false);
    }

    MusicToastLabel::popup(!contains ? tr("Add music to lovest list done!") : tr("Remove music to lovest list done!"));
}

void MusicApplication::musicWindowConciseChanged()
{
    m_bottomAreaWidget->setWindowConcise();
    m_topAreaWidget->musicBackgroundThemeDownloadFinished();
}

void MusicApplication::musicEnhancedMusicChanged(int type)
{
    m_musicPlayer->setMusicEnhanced(TTKStatic_cast(MusicPlayer::Enhanced, type));
}

void MusicApplication::musicCreateRightMenu()
{
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    QMenu addNewFiles(tr("Add New Files"), &menu);
    menu.addMenu(&addNewFiles);
    addNewFiles.addAction(tr("Open Files"), this, SLOT(musicImportSongsByFiles()));
    addNewFiles.addAction(tr("Open Dir"), this, SLOT(musicImportSongsByDir()));
    addNewFiles.addAction(tr("Open Url"), this, SLOT(musicImportSongsByUrl()));
    MusicUtils::Widget::adjustMenuPosition(&addNewFiles);

    QMenu playbackMode(tr("Playback Mode"), &menu);
    menu.addMenu(&playbackMode);

    MusicObject::PlayMode mode = m_musicPlaylist->playbackMode();
    QList<QAction*> actions;
    actions << playbackMode.addAction(tr("Order Play"), this, SLOT(musicPlayOrder()));
    actions << playbackMode.addAction(tr("Random Play"), this, SLOT(musicPlayRandom()));
    actions << playbackMode.addAction(tr("List Cycle"), this, SLOT(musicPlaylistLoop()));
    actions << playbackMode.addAction(tr("Single Cycle"), this, SLOT(musicPlayOneLoop()));
    actions << playbackMode.addAction(tr("Play Once"), this, SLOT(musicPlayOnce()));

    int index = DEFAULT_NORMAL_LEVEL;
    switch(mode)
    {
        case MusicObject::PlayOrder: index = 0; break;
        case MusicObject::PlayRandom: index = 1; break;
        case MusicObject::PlaylistLoop: index = 2; break;
        case MusicObject::PlayOneLoop: index = 3; break;
        case MusicObject::PlayOnce: index = 4; break;
        default: break;
    }

    if(index > DEFAULT_NORMAL_LEVEL && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }

    menu.addSeparator();
    QMenu remoteControl(tr("Remote Control"), &menu);
    menu.addMenu(&remoteControl);
    remoteControl.addAction(tr("Square Remote"), m_topAreaWidget, SLOT(musicSquareRemote()));
    remoteControl.addAction(tr("Rectangle Remote"), m_topAreaWidget, SLOT(musicRectangleRemote()));
    remoteControl.addAction(tr("Simple Style Remote"), m_topAreaWidget, SLOT(musicSimpleStyleRemote()));
    remoteControl.addAction(tr("Complex Style Remote"), m_topAreaWidget, SLOT(musicComplexStyleRemote()));
    remoteControl.addAction(tr("Ripple Remote"), m_topAreaWidget, SLOT(musicRippleRemote()));
    remoteControl.addAction(tr("Delete Remote"), m_topAreaWidget, SLOT(musicDeleteRemote()));
    MusicUtils::Widget::adjustMenuPosition(&remoteControl);

    menu.addAction(QIcon(":/contextMenu/btn_equalizer"), tr("Equalizer"), m_applicationObject, SLOT(musicSetEqualizer()));
    menu.addAction(tr("Sound Effect"), m_applicationObject, SLOT(musicSetSoundEffect()));
    menu.addAction(tr("Timing Settings"), m_applicationObject, SLOT(musicTimerWidget()));
    menu.addAction(tr("Music Spectrum"), m_applicationObject, SLOT(musicSpectrumWidget()));
    menu.addSeparator();

    QAction *window = menu.addAction(tr("Window Top"), m_applicationObject, SLOT(musicSetWindowToTop()));
    window->setIcon(QIcon(m_applicationObject->windowToTop() ? ":/contextMenu/btn_selected" : QString()));
    menu.addAction(tr("Reset Window"), m_applicationObject, SLOT(musicResetWindow()));

    QMenu download(tr("Download"), &menu);
    menu.addMenu(&download);
    index = !G_SETTING_PTR->value(MusicSettingManager::DownloadLimitEnable).toInt();
    actions.clear();
    actions << download.addAction(tr("Full Download"), MusicRightAreaWidget::instance(), SLOT(musicChangeDownloadFulllyWidget()));
    actions << download.addAction(tr("Custom"), MusicRightAreaWidget::instance(), SLOT(musicChangeDownloadCustumWidget()));
    if(index > DEFAULT_NORMAL_LEVEL && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }

    if(!m_applicationObject->isLastedVersion())
    {
        menu.addAction(QIcon(":/contextMenu/btn_update"), tr("New Version"), m_applicationObject, SLOT(musicVersionUpdate()));
    }
    menu.addAction(QIcon(":/contextMenu/btn_setting"), tr("Settings"), this, SLOT(musicSetting()));

    QMenu information(tr("About"), &menu);
    menu.addMenu(&information)->setIcon(QIcon(":/contextMenu/btn_about"));
    information.addAction(QIcon(":/contextMenu/btn_bug_reoprt"), tr("Bug Report"), m_applicationObject, SLOT(musicBugReportView()));
    information.addAction(QIcon(":/contextMenu/btn_about"), tr("Version") + QString(TTK_VERSION_STR) + QString(TTK_VER_TIME_STR), m_applicationObject, SLOT(musicAboutUs()));

    menu.addSeparator();
    menu.addAction(QIcon(":/contextMenu/btn_quit"), tr("Quit"), this, SLOT(quitWindowClose()));
    menu.exec(QCursor::pos());
}

void MusicApplication::applySettingParameter()
{
    //This attribute is effective immediately.
    m_applicationObject->applySettingParameter();
    m_rightAreaWidget->applySettingParameter();
    m_bottomAreaWidget->applySettingParameter();
}

void MusicApplication::setLoveDeleteItemAt(const QString &path, bool current)
{
    setDeleteItemAt({path}, false, current, MUSIC_LOVEST_LIST);
}

void MusicApplication::setDeleteItemAt(const QStringList &path, bool remove, bool current, int toolIndex)
{
    if(path.isEmpty())
    {
        return;
    }

    if(current)
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        TTKIntList index;
        for(const QString &p : qAsConst(path))
        {
            int idx = -1;
            do
            {
                idx = m_musicPlaylist->find(item.m_toolIndex, p, idx + 1);
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
        int oldIndex = m_musicPlaylist->currentIndex();
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

        if(!contains && m_musicPlaylist->currentIndex() > index[0])
        {
            oldIndex -= index.count();
        }

        if(oldIndex == m_musicPlaylist->count()) ///Play index error correction
        {
            --oldIndex;
        }
        m_musicPlaylist->setCurrentIndex(oldIndex);

        if(contains)
        {
            //The corresponding item is deleted from the Playlist
            m_musicPlayer->stop();
            musicStatePlay();

            if(remove && !QFile::remove(item.m_path))
            {
                G_DISPATCH_PTR->dispatch(1, item.m_path);
            }
        }
    }
    else
    {
        for(const QString &p : qAsConst(path))
        {
            m_ui->musicPlayedList->remove(toolIndex, p);
        }
    }
}

void MusicApplication::musicCurrentLrcUpdated()
{
    const QString &fileName = currentFileName();
    QFile file(MusicUtils::String::lrcPrefix() + fileName + LRC_FILE);
    if(file.exists())
    {
        file.remove();
    }
    m_rightAreaWidget->checkLrcValid();
}

void MusicApplication::resetCurrentSongLrcIndex()
{
    const int pos = m_musicPlayer->position();
    if(pos != -1)
    {
        //Set lrc corrent to show
        m_rightAreaWidget->setSongSpeedAndSlow(pos);
    }
}

void MusicApplication::updateCurrentTime(qint64 pos)
{
    if(isPlaying())
    {
        m_musicPlayer->setPosition(pos);
    }
}

void MusicApplication::setPlaySongChanged(int index)
{
    if(m_musicPlaylist->isEmpty() || index <0 || index >= m_musicPlaylist->count())
    {
        return;
    }
    musicPlayIndex(index, 0);
}

void MusicApplication::currentPlaylist(QStringList &list)
{
    list = m_musicSongTreeWidget->musicSongsFileName(m_musicSongTreeWidget->currentIndex());
}

void MusicApplication::resizeEvent(QResizeEvent *event)
{
    if(!m_quitWindowClose)
    {
        G_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, size());
        m_topAreaWidget->musicBackgroundThemeChangedByResize();
        m_rightAreaWidget->resizeWindow();
        m_bottomAreaWidget->resizeWindow();
        MusicAbstractMoveResizeWidget::resizeEvent(event);
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
    MusicAbstractMoveResizeWidget::closeEvent(event);
    event->ignore();
    if(!m_bottomAreaWidget->systemCloseConfig() && m_bottomAreaWidget->systemTrayIsVisible())
    {
        hide();
        m_bottomAreaWidget->showMessage(tr("Prompt"), tr("TTKMusicPlayer will run in the background"));
    }
    else
    {
        quitWindowClose();
    }
}

void MusicApplication::dragEnterEvent(QDragEnterEvent *event)
{
    MusicAbstractMoveResizeWidget::dragEnterEvent(event);
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void MusicApplication::dragMoveEvent(QDragMoveEvent *event)
{
    MusicAbstractMoveResizeWidget::dragMoveEvent(event);
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void MusicApplication::dropEvent(QDropEvent *event)
{
    MusicAbstractMoveResizeWidget::dropEvent(event);
    const QMimeData *data = event->mimeData();
    QStringList files;

    for(const QUrl &url : data->urls())
    {
        files << url.toLocalFile();
    }

    m_musicSongTreeWidget->importMusicSongsByPath(files);
}

void MusicApplication::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractMoveResizeWidget::contextMenuEvent(event);
    musicCreateRightMenu();
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicApplication::enterEvent(QEnterEvent *event)
#else
void MusicApplication::enterEvent(QEvent *event)
#endif
{
    MusicAbstractMoveResizeWidget::enterEvent(event);
    m_applicationObject->sideAnimationByOff();
}

void MusicApplication::leaveEvent(QEvent *event)
{
    MusicAbstractMoveResizeWidget::leaveEvent(event);
    m_applicationObject->sideAnimationByOn();
}

void MusicApplication::mousePressEvent(QMouseEvent *event)
{
    MusicAbstractMoveResizeWidget::mousePressEvent(event);
    m_ui->musicSongSearchEdit->closePopWidget();
}

void MusicApplication::mouseReleaseEvent(QMouseEvent *event)
{
    MusicAbstractMoveResizeWidget::mouseReleaseEvent(event);
    m_applicationObject->sideAnimationByOn();
}

void MusicApplication::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->pos().y() <= m_ui->topWidget->height() && !G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        MusicAbstractMoveResizeWidget::mouseDoubleClickEvent(event);
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
    return MusicAbstractMoveResizeWidget::eventFilter(object, event);
}

void MusicApplication::setMusicPlayIndex()
{
    m_currentMusicSongTreeIndex = m_musicSongTreeWidget->currentIndex();
    m_musicPlaylist->clear();
    m_musicPlaylist->add(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->musicSongsFilePath(m_currentMusicSongTreeIndex));
    m_musicSongTreeWidget->setCurrentMusicSongTreeIndex(m_currentMusicSongTreeIndex);
}

void MusicApplication::readSystemConfigFromFile()
{
    int value = DEFAULT_NORMAL_LEVEL;
    //Path configuration song
    MusicSongItemList songs;
    MusicTKPLConfigManager manager;
    if(manager.fromFile())
    {
        manager.readBuffer(songs);
    }
    const bool success = m_musicSongTreeWidget->addMusicItemList(songs);

    MusicConfigManager xml;
    if(!xml.fromFile())
    {
        return;
    }
    xml.readBuffer();
    m_applicationObject->loadNetWorkSetting();

    switch(G_SETTING_PTR->value(MusicSettingManager::PlayMode).toInt())
    {
        case MusicObject::PlayOrder: musicPlayOrder();break;
        case MusicObject::PlayRandom: musicPlayRandom();break;
        case MusicObject::PlaylistLoop: musicPlaylistLoop();break;
        case MusicObject::PlayOneLoop: musicPlayOneLoop();break;
        case MusicObject::PlayOnce: musicPlayOnce();break;
        default:break;
    }

    value = G_SETTING_PTR->value(MusicSettingManager::RemoteWidgetMode).toInt();
    if(value != 0)
    {
        m_topAreaWidget->musicRemoteTypeChanged(value);
    }

    //The size of the volume of the allocation of songs
    musicVolumeChanged(G_SETTING_PTR->value(MusicSettingManager::Volume).toInt());

    //Configure playback mode
    m_ui->musicEnhancedButton->setEnhancedMusicConfig(G_SETTING_PTR->value(MusicSettingManager::EnhancedMusicIndex).toInt());
    m_applicationObject->musicEffectChanged();
    if(G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt() == 1)
    {
        m_musicPlayer->setEqualizerConfig();
    }

    //music hotkey
    if(G_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toBool())
    {
        QStringList hotkeys = G_SETTING_PTR->value(MusicSettingManager::HotkeyValue).toString().split(TTK_SPLITER);
        if(hotkeys.count() != G_HOTKEY_PTR->count())
        {
            hotkeys = G_HOTKEY_PTR->defaultKeys();
        }
        G_HOTKEY_PTR->setHotKeys(hotkeys);
        G_HOTKEY_PTR->enabledAll(true);
    }

    //musicSetting
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
    //Just always set fade false, because it is not finished yet.
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable, false);
#ifdef Q_OS_UNIX
    //Disable  window quit mode on unix
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode, false);
#endif
    //Set interior lrc mode always on
    G_SETTING_PTR->setValue(MusicSettingManager::ShowInteriorLrc, true);
    m_rightAreaWidget->setInteriorLrcVisible(true);

    //Set the desktop lrc should be shown
    m_rightAreaWidget->setWindowLrcTypeChanged();
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, xml.readShowDesktopLrcGeometry());

    //Set the current background color and alpha value
    m_topAreaWidget->setBackgroundParameter();

    //Configuration from next time also stopped at the last record.
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    //add new music file to playlist
    value = lastPlayIndex[1].toInt();
    m_musicPlaylist->add(value, m_musicSongTreeWidget->musicSongsFilePath(value));
    if(DEFAULT_NORMAL_LEVEL < value && value < songs.count())
    {
        m_ui->musicPlayedList->append(songs[value].m_songs);
    }

    if(success && lastPlayIndex[0] == "1")
    {
        QTimer::singleShot(MT_MS, m_musicSongTreeWidget, SLOT(updateCurrentIndex()));
        const int index = lastPlayIndex[2].toInt();
        m_currentMusicSongTreeIndex = (index == DEFAULT_NORMAL_LEVEL) ? DEFAULT_NORMAL_LEVEL : value;
        m_musicPlaylist->blockSignals(true);
        m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, index));
        m_musicPlaylist->blockSignals(false);
        m_ui->musicPlayedList->setCurrentIndex();
    }

    //Configure automatic playback
    if(G_SETTING_PTR->value(MusicSettingManager::AutoPlayMode).toInt() == 1)
    {
        musicStatePlay();
    }
    m_bottomAreaWidget->setCurrentPlayStatus(isPlaying());
    m_rightAreaWidget->setCurrentPlayStatus(isPlaying());
    m_topAreaWidget->setCurrentPlayStatus(isPlaying());

    //Set the lrc color the user set
    m_bottomAreaWidget->lockDesktopLrc(G_SETTING_PTR->value(MusicSettingManager::DLrcLockedMode).toInt());

    //init or reset the window
    value = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt();
    m_bottomAreaWidget->setDestopLrcVisible(value);
    m_rightAreaWidget->setDestopLrcVisible(value);

    //Reset geometry
    setGeometry(xml.readWindowGeometry());

    //Reset window concise
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        musicWindowConciseChanged();
    }

    //Update check on
    if(G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateEnable).toBool())
    {
        m_applicationObject->soureUpdateCheck();
    }

    m_rightAreaWidget->applySettingParameter();
    m_bottomAreaWidget->applySettingParameter();
    m_applicationObject->applySettingParameter();
}

void MusicApplication::writeSystemConfigToFile()
{
    MusicConfigManager xml;
    m_applicationObject->sideAnimationReset();
    G_SETTING_PTR->setValue(MusicSettingManager::WidgetPosition, pos());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicIndex, m_musicPlayer->musicEnhanced());
    G_SETTING_PTR->setValue(MusicSettingManager::PlayMode, m_musicPlaylist->playbackMode());
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, m_ui->musicSound->value());

    QStringList lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    if(lastPlayIndex.isEmpty())
    {
        lastPlayIndex << "1" << "-1" << "-1";
    }
    else
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        lastPlayIndex[1] = QString::number(item.m_toolIndex);
        lastPlayIndex[2] = QString::number(m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path));
    }

    G_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndex, lastPlayIndex);
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundThemeValue, m_topAreaWidget->backgroundPath());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparent, m_topAreaWidget->backgroundAlpha());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent, m_topAreaWidget->backgroundListAlpha());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable, m_topAreaWidget->backgroundTransparentEnable());
    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, m_rightAreaWidget->destopLrcVisible());
    xml.writeBuffer();

    MusicTKPLConfigManager manager;
    manager.writeBuffer(m_musicSongTreeWidget->musicItemList(), PLAYLIST_PATH_FULL);
}
