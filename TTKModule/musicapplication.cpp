#include "musicapplication.h"
#include "ui_musicapplication.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicsysconfigmanager.h"
#include "musicplayer.h"
#include "musicformats.h"
#include "musicplaylist.h"
#include "musicbackgroundmanager.h"
#include "musicsettingmanager.h"
#include "ttkversion.h"
#include "musicmessagebox.h"
#include "musictime.h"
#include "musicbottomareawidget.h"
#include "musictopareawidget.h"
#include "musicrightareawidget.h"
#include "musicleftareawidget.h"
#include "musicapplicationobject.h"
#include "musichotkeymanager.h"
#include "musicfunctionuiobject.h"
#include "musictoastlabel.h"
#include "musiccoreutils.h"
#include "musicfileutils.h"
#include "musicplaylistmanager.h"
#include "musicotherdefine.h"
#include "musictinyuiobject.h"
#include "musicdispatchmanager.h"
#include "musictkplconfigmanager.h"

#include <QMimeData>
#include <QFileDialog>

MusicApplication *MusicApplication::m_instance = nullptr;

MusicApplication::MusicApplication(QWidget *parent)
    : MusicAbstractMoveResizeWidget(parent),
      m_ui(new Ui::MusicApplication)
{
    m_instance = this;
//#ifdef TTK_GREATER_NEW
    setAttribute(Qt::WA_TranslucentBackground, false);
//#endif

    m_applicationObject = new MusicApplicationObject(this);
    m_topAreaWidget = new MusicTopAreaWidget(this);
    m_bottomAreaWidget = new MusicBottomAreaWidget(this);
    m_rightAreaWidget = new MusicRightAreaWidget(this);
    m_leftAreaWidget = new MusicLeftAreaWidget(this);
    //
    m_ui->setupUi(this);
    const QSize &size = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
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
    m_topAreaWidget->musicBackgroundSliderStateChanged(false);

    connect(m_topAreaWidget, SIGNAL(setTransparent(int)), m_leftAreaWidget, SLOT(setTransparent(int)));
    connect(m_rightAreaWidget, SIGNAL(updateBackgroundTheme()), m_topAreaWidget, SLOT(musicBackgroundTransparentChanged()));
    connect(m_rightAreaWidget, SIGNAL(updateBackgroundThemeDownload()), m_topAreaWidget, SLOT(musicBackgroundThemeDownloadFinished()));

    setAcceptDrops(true);

    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOrder);
    //The default is the order of play

    m_ui->musicPlayedList->setPlaylist(m_musicPlaylist);
    m_musicPlayer->setPlaylist(m_musicPlaylist);
    m_musicPlayer->setVolume(100);
    //The default Volume is 100

    m_playControl = true;//The default in the suspended state
    m_quitWindowClose = false;
    m_currentMusicSongTreeIndex = DEFAULT_LEVEL_LOWER;

    connect(m_musicPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_musicPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_musicPlayer, SIGNAL(stateChanged(MusicObject::PlayState)), SLOT(stateChanged()));
    connect(m_musicPlaylist, SIGNAL(currentIndexChanged(int)), SLOT(showCurrentSong(int)));

    connect(m_ui->musicDesktopLrc, SIGNAL(clicked(bool)), m_rightAreaWidget, SLOT(setDestopLrcVisible(bool)));

    m_ui->lrcDisplayAllButton->hide();
    m_ui->musicKey->setFocus();
    m_ui->functionsContainer->setCurrentIndex(APP_WINDOW_INDEX_0);

    m_ui->musicTimeWidget->setObject(this);
    M_HOTKEY_PTR->connectParentObject(this);

    /////////// Mouse tracking
    m_ui->background->installEventFilter(this);
    m_ui->background->setMouseTracking(true);

    readXMLConfigFromText();

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

QString MusicApplication::getCurrentFileName() const
{
    if(m_musicPlaylist->currentIndex() < 0 || m_currentMusicSongTreeIndex < 0)
    {
        return QString();
    }

    const MusicSongItems items(m_musicSongTreeWidget->getMusicLists());
    if(0 <= m_currentMusicSongTreeIndex && m_currentMusicSongTreeIndex < items.count())
    {
        const MusicSongs &songs = items[m_currentMusicSongTreeIndex].m_songs;
        const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(m_currentMusicSongTreeIndex, m_musicPlaylist->currentMediaString());
        return (index != -1) ?songs[index].getMusicName() : QString();
    }
    return QString();
}

QString MusicApplication::getCurrentFilePath() const
{
    if(m_musicPlaylist->currentIndex() < 0 || m_currentMusicSongTreeIndex < 0)
    {
        return QString();
    }

    const MusicSongItems items(m_musicSongTreeWidget->getMusicLists());
    if(0 <= m_currentMusicSongTreeIndex && m_currentMusicSongTreeIndex < items.count())
    {
        const MusicSongs &songs = items[m_currentMusicSongTreeIndex].m_songs;
        const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(m_currentMusicSongTreeIndex, m_musicPlaylist->currentMediaString());
        return (index != -1) ?songs[index].getMusicPath() : QString();
    }
    return QString();
}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return (m_musicPlaylist->currentIndex() == DEFAULT_LEVEL_LOWER);
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

    const QString &filename = getCurrentFileName();
    const QString &prefix = MusicUtils::String::lrcPrefix();
    const QString &path = QFile::exists(prefix + filename + LRC_FILE) ? (prefix + filename + LRC_FILE) : (prefix + filename + KRC_FILE);
    m_rightAreaWidget->loadCurrentSongLrc(filename, path);

    //reset current song lrc index.
    QTimer::singleShot(MT_S2MS, this, SLOT(resetCurrentSongLrcIndex()));
}

void MusicApplication::radioExecuteOuter(const QString &path)
{
    m_leftAreaWidget->radioExecuteOuter(path);
}

void MusicApplication::musicImportSongsSettingPathOuter(const QStringList &path, bool play)
{
    musicImportSongsSettingPath(path);

    if(play)
    {
        musicPlayIndex(m_musicPlaylist->mediaCount() - 1, 0);
    }
}

void MusicApplication::musicImportSongsSettingPath(const QStringList &items)
{
    if(items.isEmpty())
    {
        return;
    }

    QStringList files(items);
    const QStringList &sfx = MusicFormats::supportFormatsString();

    QString suffix;

    QStringList failedFiles;
    foreach(const QString &path, items)
    {
        suffix = QFileInfo(path).suffix();
        if(!sfx.contains(suffix.toLower()))
        {
            failedFiles << path;
            files.removeOne(path);
        }
    }

    if(!files.isEmpty())
    {
        m_musicSongTreeWidget->importOtherMusicSongs(files);
    }

    if(!failedFiles.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("not supported count %1").arg(failedFiles.count()));
        message.exec();

        QFile file("import_failed_file_log.txt");
        if(file.open(QFile::WriteOnly))
        {
            foreach(const QString &name, failedFiles)
            {
                file.write(name.toUtf8() + "\r\n");
            }
        }
        file.close();
    }
}

QString MusicApplication::musicDownloadContains(bool &contains) const
{
    contains = false;
    QString path;
    if(m_musicSongTreeWidget->getCurrentPlayToolIndex() != DEFAULT_LEVEL_LOWER)
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        if(item.isValid())
        {
            const MusicSong currentSong(item.m_path);
            path = QString("%1%2.%3").arg(M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString())
                                     .arg(currentSong.getMusicName()).arg(currentSong.getMusicType());
            contains = QFile::exists(path);
        }
    }
    return path;
}

bool MusicApplication::musicLovestContains() const
{
    if(m_musicSongTreeWidget->getCurrentPlayToolIndex() != DEFAULT_LEVEL_LOWER)
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        const MusicSongItems items(m_musicSongTreeWidget->getMusicLists());
        if(item.isValid() && item.m_toolIndex < items.count())
        {
            const MusicSongs &currentSongs = items[item.m_toolIndex].m_songs;
            const int index = m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path);
            const MusicSongs &loveSongs = items[MUSIC_LOVEST_LIST].m_songs;
            return index != -1 ? loveSongs.contains(currentSongs[index]) : false;
        }
    }
    return false;
}

bool MusicApplication::musicListLovestContains(int index) const
{
    if(m_musicSongTreeWidget->currentIndex() != DEFAULT_LEVEL_LOWER && index > DEFAULT_LEVEL_LOWER)
    {
        const MusicSongItems items(m_musicSongTreeWidget->getMusicLists());
        if(m_musicSongTreeWidget->currentIndex() < items.count())
        {
            const MusicSongs &currentSongs = items[m_musicSongTreeWidget->currentIndex()].m_songs;
            const MusicSongs &loveSongs = items[MUSIC_LOVEST_LIST].m_songs;
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

MusicObject::PlayMode MusicApplication::getPlayMode() const
{
    return m_musicPlaylist->playbackMode();
}

void MusicApplication::quitWindowClose()
{
    //Write configuration files
    writeXMLConfigToText();
    m_quitWindowClose = true;
    m_applicationObject->windowCloseAnimation();
}

void MusicApplication::positionChanged(qint64 position)
{
    m_rightAreaWidget->updateCurrentLrc(position, m_musicPlayer->duration(), m_playControl);
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
#if defined TTK_DEBUG && defined Q_OS_WIN && defined TTK_WINEXTRAS
    m_bottomAreaWidget->setValue(position);
#endif
}

void MusicApplication::durationChanged(qint64 duration)
{
    //Show the current play total time
    m_ui->musicTimeWidget->setRange(0, duration);
    m_ui->playTotalTime->setText("/" + MusicTime::msecTime2LabelJustified(duration));
    //Loading the current song lrc
    musicLoadCurrentSongLrc();
#if defined TTK_DEBUG && defined Q_OS_WIN && defined TTK_WINEXTRAS
    m_bottomAreaWidget->setRange(0, duration);
#endif
}

void MusicApplication::stateChanged()
{
    m_playControl = true;
    const bool concise = M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool();
    m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MKGTinyBtnPlay : MusicUIObject::MKGBtnPlay);
}

void MusicApplication::showCurrentSong(int index)
{
    QString name;
    const MusicPlayItem &item = m_musicPlaylist->currentItem();
    index = m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path);
    m_currentMusicSongTreeIndex = item.m_toolIndex;
    m_musicSongTreeWidget->setCurrentMusicSongTreeIndex(item.m_toolIndex);

    if(index > DEFAULT_LEVEL_LOWER) //The list to end
    {
        name = getCurrentFileName();
        ///detecting whether the file has been downloaded
        bool exist = false;
        musicDownloadContains(exist);
        m_ui->musicDownload->setStyleSheet(exist ? MusicUIObject::MKGBtnDownload : MusicUIObject::MKGBtnUnDownload);
        //
        exist = musicLovestContains();
        m_ui->musicBestLove->setStyleSheet(exist ? MusicUIObject::MKGBtnLove : MusicUIObject::MKGBtnUnLove);
        //
        m_musicSongTreeWidget->selectRow(index);
    }
    else
    {
        const bool concise = M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool();
        m_ui->musicBestLove->setStyleSheet(MusicUIObject::MKGBtnUnLove);
        m_ui->musicDownload->setStyleSheet(MusicUIObject::MKGBtnUnDownload);
        m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MKGTinyBtnPlay : MusicUIObject::MKGBtnPlay);
        m_playControl = true;
        m_musicPlayer->stop();
        m_rightAreaWidget->stopLrcMask();

        m_bottomAreaWidget->showPlayStatus(m_playControl);
        m_rightAreaWidget->showPlayStatus(m_playControl);
        m_topAreaWidget->showPlayStatus(m_playControl);
        m_ui->musicTimeWidget->setPlayState(m_playControl);

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
    M_BACKGROUND_PTR->clearArtistName();
    m_rightAreaWidget->musicCheckHasLrcAlready();
    m_bottomAreaWidget->setLabelText(name);
    m_topAreaWidget->setLabelText(name);
    //display current ArtTheme pic
    M_BACKGROUND_PTR->setArtistName( getCurrentFileName() );
    m_topAreaWidget->musicBackgroundThemeDownloadFinished();
}

void MusicApplication::musicStatePlay()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;//The playlist is not performing space-time
    }

    const bool concise = M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool();
    if(m_playControl)
    {
        m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MKGTinyBtnPause : MusicUIObject::MKGBtnPause);
        m_playControl = false;
        m_musicPlayer->play();
        m_topAreaWidget->musicBackgroundThemeDownloadFinished();
        m_rightAreaWidget->startTimerClock();
    }
    else
    {
        m_ui->musicKey->setStyleSheet(concise ? MusicUIObject::MKGTinyBtnPlay : MusicUIObject::MKGBtnPlay);
        m_playControl = true;
        m_musicPlayer->pause();
        m_topAreaWidget->setTimerStop();
        m_rightAreaWidget->stopLrcMask();
    }
    m_bottomAreaWidget->showPlayStatus(m_playControl);
    m_rightAreaWidget->showPlayStatus(m_playControl);
    m_topAreaWidget->showPlayStatus(m_playControl);
    m_ui->musicTimeWidget->setPlayState(m_playControl);
}

void MusicApplication::musicPlayPrevious()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;//The playlist is not performing space-time
    }

    if(m_musicPlaylist->playbackMode() == MusicObject::PM_PlayRandom)
    {
        m_musicPlaylist->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playPrevious();
    }

    m_playControl = true;
    musicStatePlay();
    m_playControl = false;
}

void MusicApplication::musicPlayNext()
{
    if(m_musicPlaylist->isEmpty())
    {
        return;//The playlist is not performing space-time
    }

    if(m_musicPlaylist->playbackMode() == MusicObject::PM_PlayRandom)
    {
        m_musicPlaylist->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playNext();
    }

    m_playControl = true;
    musicStatePlay();
    m_playControl = false;
}

void MusicApplication::musicPlayOrder()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOrder);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PM_PlayOrder);
}

void MusicApplication::musicPlayRandom()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayRandom);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PM_PlayRandom);
}

void MusicApplication::musicPlaylistLoop()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlaylistLoop);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PM_PlaylistLoop);
}

void MusicApplication::musicPlayOneLoop()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOneLoop);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PM_PlayOneLoop);
}

void MusicApplication::musicPlayItemOnce()
{
    m_musicPlaylist->setPlaybackMode(MusicObject::PM_PlayOnce);
    m_ui->musicPlayMode->setPlaybackMode(MusicObject::PM_PlayOnce);
}

void MusicApplication::musicVolumeMute()
{
    m_musicPlayer->setMuted(!m_musicPlayer->isMuted());
    const int volume = m_musicPlayer->volume();

    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_ui->musicSound->setValue(volume);
    M_SETTING_PTR->setValue(MusicSettingManager::VolumeChoiced, volume);
}

void MusicApplication::musicVolumeChanged(int volume)
{
    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_ui->musicSound->setValue(volume);
    m_musicPlayer->setVolume(volume);
    M_SETTING_PTR->setValue(MusicSettingManager::VolumeChoiced, volume);
}

void MusicApplication::musicImportSongs()
{
    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("openOnlyFiles"), this, SLOT(musicImportSongsOnlyFile()));
    menu.addAction(tr("openOnlyDir"), this, SLOT(musicImportSongsOnlyDir()));
    menu.addSeparator();
    menu.addAction(tr("dragAnddrop"))->setEnabled(false);
    menu.exec(QCursor::pos());
}

void MusicApplication::musicImportSongsOnlyFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters( MusicFormats::supportFormatsFilterDialogString() );

    if(dialog.exec())
    {
        musicImportSongsSettingPath(dialog.selectedFiles());
    }
}

void MusicApplication::musicImportSongsOnlyDir()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        QStringList fileList;
        foreach(const QFileInfo &info, MusicUtils::File::getFileListByDir(dialog.directory().absolutePath(), true))
        {
            if(MusicFormats::supportFormatsString().contains(info.suffix().toLower()))
            {
               fileList << info.absoluteFilePath();
            }
        }
        musicImportSongsSettingPath(fileList);
    }
}

void MusicApplication::musicImportSongsItemList()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters(MusicFormats::supportFormatsPlaylistDialogString());

    if(dialog.exec())
    {
        MusicPlaylistManager manager;
        MusicSongItems items;
        manager.getMusicSongItems(dialog.selectedFiles(), items);
        m_musicSongTreeWidget->appendMusicLists(items);
    }
}

void MusicApplication::musicExportSongsItemList(int index)
{
    const QString &fileName = MusicUtils::File::getSaveFileDialog(this, tr("Save List File"), MusicFormats::supportFormatsPlaylistString());
    if(!fileName.isEmpty())
    {
        const MusicSongItems &items = m_musicSongTreeWidget->getMusicLists();
        if(index < 0 || index >= items.count())
        {
            return;
        }

        MusicPlaylistManager manager;
        manager.setMusicSongItem(fileName, items[index]);
    }
}

void MusicApplication::musicPlaySort(int row)
{
    const QString &path = m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, row);
    m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, path);
}

void MusicApplication::musicPlayedIndex(int row)
{
    m_musicPlaylist->setCurrentIndex(row);

    m_playControl = true;
    musicStatePlay();
    m_playControl = false;
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

    if(m_currentMusicSongTreeIndex != m_musicSongTreeWidget->currentIndex() || m_musicPlaylist->mediaCount() == 0)
    {
        setMusicPlayIndex();
        const MusicSongItems items(m_musicSongTreeWidget->getMusicLists());
        const int index = m_musicSongTreeWidget->currentIndex();
        if(0 <= index && index < items.count())
        {
            m_ui->musicPlayedList->append(items[index].m_songs);
        }
    }

    if(!m_musicSongTreeWidget->searchFileListEmpty())
    {
        row = m_musicSongTreeWidget->getSearchFileListIndexAndClear(row);
    }

    m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, row));

    m_playControl = true;
    musicStatePlay();
    m_playControl = false;
}

void MusicApplication::musicPlayIndexClicked(int row, int col)
{
    if(m_currentMusicSongTreeIndex == m_musicSongTreeWidget->currentIndex())
    {
        setMusicPlayIndex();
        const MusicSongItems items(m_musicSongTreeWidget->getMusicLists());
        const int index = m_musicSongTreeWidget->currentIndex();
        if(0 <= index && index < items.count())
        {
            m_ui->musicPlayedList->append(items[index].m_songs);
        }
    }
    musicPlayIndex(row, col);
}

void MusicApplication::musicPlayAnyTimeAt(int posValue)
{
    //Drag the progress indicator to rewind or fast-forward through the current song
    m_musicPlayer->setPosition(posValue);
    //Set lrc corrent to show
    m_rightAreaWidget->setSongSpeedAndSlow(posValue);
}

void MusicApplication::musicActionVolumeSub()
{
    int currentVol = m_musicPlayer->volume();
    currentVol -= 15;
    if(currentVol < 0)
    {
        currentVol = 0;   //reset music volume
    }
    musicVolumeChanged( currentVol );
}

void MusicApplication::musicActionVolumePlus()
{
    int currentVol = m_musicPlayer->volume();
    currentVol += 15;
    if(currentVol > 100)
    {
        currentVol = 100;   //reset music volume
    }
    musicVolumeChanged( currentVol );
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

void MusicApplication::musicAddSongToLovestListAt()
{
    musicAddSongToLovestListAt(true);
}

void MusicApplication::musicAddSongToLovestListAt(bool state)
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

    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->defaultLabel(this, !contains ? tr("add music to lovest list done!") : tr("remove music to lovest list done!"));
}

void MusicApplication::musicWindowConciseChanged()
{
    m_bottomAreaWidget->setWindowConcise();
    m_topAreaWidget->musicBackgroundThemeDownloadFinished();
}

void MusicApplication::musicEnhancedMusicChanged(int type)
{
    m_musicPlayer->setMusicEnhanced(MStatic_cast(MusicPlayer::Enhanced, type));
}

void MusicApplication::musicCreateRightMenu()
{
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_login"), m_topAreaWidget->getUserLoginState() ? tr("logout") : tr("login"), m_topAreaWidget, SLOT(musicUserContextLogin()));
    rightClickMenu.addSeparator();

    QMenu musicAddNewFiles(tr("addNewFiles"), &rightClickMenu);
    rightClickMenu.addMenu(&musicAddNewFiles);
    musicAddNewFiles.addAction(tr("openOnlyFiles"), this, SLOT(musicImportSongsOnlyFile()));
    musicAddNewFiles.addAction(tr("openOnlyDir"), this, SLOT(musicImportSongsOnlyDir()));

    QMenu musicPlaybackMode(tr("playbackMode"), &rightClickMenu);
    rightClickMenu.addMenu(&musicPlaybackMode);

    MusicObject::PlayMode mode = m_musicPlaylist->playbackMode();
    QList<QAction*> actions;
    actions << musicPlaybackMode.addAction(tr("OrderPlay"), this, SLOT(musicPlayOrder()));
    actions << musicPlaybackMode.addAction(tr("RandomPlay"), this, SLOT(musicPlayRandom()));
    actions << musicPlaybackMode.addAction(tr("ListCycle"), this, SLOT(musicPlaylistLoop()));
    actions << musicPlaybackMode.addAction(tr("SingleCycle"), this, SLOT(musicPlayOneLoop()));
    actions << musicPlaybackMode.addAction(tr("PlayOnce"), this, SLOT(musicPlayItemOnce()));

    int index = DEFAULT_LEVEL_LOWER;
    switch(mode)
    {
        case MusicObject::PM_PlayOrder: index = 0; break;
        case MusicObject::PM_PlayRandom: index = 1; break;
        case MusicObject::PM_PlaylistLoop: index = 2; break;
        case MusicObject::PM_PlayOneLoop: index = 3; break;
        case MusicObject::PM_PlayOnce: index = 4; break;
        default: break;
    }
    if(index > DEFAULT_LEVEL_LOWER && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }

    rightClickMenu.addSeparator();
    QMenu musicRemoteControl(tr("RemoteControl"), &rightClickMenu);
    rightClickMenu.addMenu(&musicRemoteControl);
    musicRemoteControl.addAction(tr("CircleRemote"), m_topAreaWidget, SLOT(musicCircleRemote()));
    musicRemoteControl.addAction(tr("SquareRemote"), m_topAreaWidget, SLOT(musicSquareRemote()));
    musicRemoteControl.addAction(tr("RectangleRemote"), m_topAreaWidget, SLOT(musicRectangleRemote()));
    musicRemoteControl.addAction(tr("SimpleStyleRemote"), m_topAreaWidget, SLOT(musicSimpleStyleRemote()));
    musicRemoteControl.addAction(tr("ComplexStyleRemote"), m_topAreaWidget, SLOT(musicComplexStyleRemote()));
    musicRemoteControl.addAction(tr("DiamondRemote"), m_topAreaWidget, SLOT(musicDiamondRemote()));
    musicRemoteControl.addAction(tr("RipplesRemote"), m_topAreaWidget, SLOT(musicRipplesRemote()));
    musicRemoteControl.addAction(tr("RaysWaveRemote"), m_topAreaWidget, SLOT(musicRaysWaveRemote()));
    musicRemoteControl.addAction(tr("DeleteRemote"), m_topAreaWidget, SLOT(musicDeleteRemote()));

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_equalizer"), tr("Equalizer"), m_applicationObject, SLOT(musicSetEqualizer()));
    rightClickMenu.addAction(tr("SoundEffect"), m_applicationObject, SLOT(musicSetSoundEffect()));
    rightClickMenu.addAction(tr("TimingSettings"), m_applicationObject, SLOT(musicTimerWidget()));
    rightClickMenu.addAction(tr("MusicSpectrum"), m_applicationObject, SLOT(musicSpectrumWidget()));
    rightClickMenu.addSeparator();

    QAction *window = rightClickMenu.addAction(tr("windowTop"), m_applicationObject, SLOT(musicSetWindowToTop()));
    window->setIcon(QIcon(m_applicationObject->getWindowToTop() ? ":/contextMenu/btn_selected" : QString()));
    rightClickMenu.addAction(tr("resetWindow"), m_applicationObject, SLOT(musicResetWindow()));

    QMenu musicDownload(tr("Download"), &rightClickMenu);
    rightClickMenu.addMenu(&musicDownload);
    index = !M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt();
    actions.clear();
    actions << musicDownload.addAction(tr("Full Download"), MusicRightAreaWidget::instance(), SLOT(musicChangeDownloadFulllyWidget()));
    actions << musicDownload.addAction(tr("Custom"), MusicRightAreaWidget::instance(), SLOT(musicChangeDownloadCustumWidget()));
    if(index > DEFAULT_LEVEL_LOWER && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_setting"), tr("Setting"), this, SLOT(musicSetting()));

    QMenu musicInfo(tr("musicAbout"), &rightClickMenu);
    rightClickMenu.addMenu(&musicInfo)->setIcon(QIcon(":/contextMenu/btn_about"));
    musicInfo.addAction(QIcon(":/contextMenu/btn_bug_reoprt"), tr("Bug Report"), m_applicationObject, SLOT(musicBugReportView()));
    musicInfo.addAction(QIcon(":/contextMenu/btn_update"), tr("Update"), m_applicationObject, SLOT(musicVersionUpdate()));
    musicInfo.addAction(QIcon(":/contextMenu/btn_about"), tr("Version") + QString(TTKMUSIC_VERSION_STR) + QString(TTKMUSIC_VER_TIME_STR), m_applicationObject, SLOT(musicAboutUs()));

    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_quit"), tr("quit"), this, SLOT(quitWindowClose()));
    rightClickMenu.exec(QCursor::pos());
}

void MusicApplication::musicSearchIndexChanged(int, int index)
{
    m_musicSongTreeWidget->searchFileListCache(index);
}

void MusicApplication::getParameterSetting()
{
    //This attribute is effective immediately.
    m_applicationObject->getParameterSetting();
    m_rightAreaWidget->getParameterSetting();
    m_bottomAreaWidget->getParameterSetting();
}

void MusicApplication::setLoveDeleteItemAt(const QString &path, bool current)
{
    setDeleteItemAt(QStringList() << path, false, current, MUSIC_LOVEST_LIST);
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
        MIntList index;
        foreach(const QString &p, path)
        {
            int idx = -1;
            do
            {
                idx = m_musicPlaylist->find(item.m_toolIndex, p, idx + 1);
                if(idx != -1)
                {
                    index << idx;
                }
            }while(idx != -1);
        }

        if(index.isEmpty())
        {
            return;
        }
        qSort(index);

        bool contains = false; ///the play one is delete list
        int oldIndex = m_musicPlaylist->currentIndex();
        ///check if delete one that the play one
        if(index.count() == 1 && index.first() == oldIndex)
        {
            contains = true;
        }
        ///other ways
        for(int i=index.count() - 1; i>=0; --i)
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
        if(oldIndex == m_musicPlaylist->mediaCount()) ///Play index error correction
        {
            --oldIndex;
        }
        m_musicPlaylist->setCurrentIndex(oldIndex);

        if(contains)
        {
            //The corresponding item is deleted from the Playlist
            m_playControl = true;
            musicStatePlay();
            m_playControl = false;

            if(remove && !QFile::remove(item.m_path))
            {
                M_DISPATCH_PTR->dispatch(1, item.m_path);
            }
        }
    }
    else
    {
        foreach(const QString &p, path)
        {
            m_ui->musicPlayedList->remove(toolIndex, p);
        }
    }
}

void MusicApplication::musicCurrentLrcUpdated()
{
    const QString &filename = getCurrentFileName();
    QFile file(MusicUtils::String::lrcPrefix() + filename + LRC_FILE);
    if(file.exists())
    {
        file.remove();
    }
    m_rightAreaWidget->musicCheckHasLrcAlready();
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
    if(!m_playControl) ///When pause just resume it
    {
        m_musicPlayer->setPosition(pos);
    }
}

void MusicApplication::setPlaySongChanged(int index)
{
    if(m_musicPlaylist->isEmpty() || index <0 || index >= m_musicPlaylist->mediaCount())
    {
        return;
    }
    musicPlayIndex(index, 0);
}


void MusicApplication::setStopSongChanged()
{
    m_playControl = false;
    musicStatePlay();
}

void MusicApplication::getCurrentPlaylist(QStringList &list)
{
    list = m_musicSongTreeWidget->getMusicSongsFileName(m_musicSongTreeWidget->currentIndex());
}

void MusicApplication::resizeEvent(QResizeEvent *event)
{
    if(!m_quitWindowClose)
    {
        M_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, size());
        m_topAreaWidget->musicBackgroundThemeChangedByResize();
        m_rightAreaWidget->resizeWindow();
        m_bottomAreaWidget->resizeWindow();
        m_ui->musicPlayedList->resizeWindow();
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
    if(!m_bottomAreaWidget->getSystemCloseConfig() && m_bottomAreaWidget->systemTrayIsVisible() )
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
    QStringList fileList;

    foreach(const QUrl &url, data->urls())
    {
        fileList << url.toLocalFile();
    }
    musicImportSongsSettingPath(fileList);
}

void MusicApplication::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractMoveResizeWidget::contextMenuEvent(event);
    musicCreateRightMenu();
}

void MusicApplication::enterEvent(QEvent *event)
{
    MusicAbstractMoveResizeWidget::enterEvent(event);
    m_applicationObject->sideAnimationByOff();
}

void MusicApplication::leaveEvent(QEvent *event)
{
    MusicAbstractMoveResizeWidget::leaveEvent(event);
    m_applicationObject->sideAnimationByOn();
}

void MusicApplication::mouseReleaseEvent(QMouseEvent *event)
{
    MusicAbstractMoveResizeWidget::mouseReleaseEvent(event);
    m_applicationObject->sideAnimationByOn();
}

void MusicApplication::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->pos().y() <= m_ui->topWidget->height() && !M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool())
    {
        MusicAbstractMoveResizeWidget::mouseDoubleClickEvent(event);
    }
    else
    {
        event->ignore();
    }
}

void MusicApplication::setMusicPlayIndex()
{
    m_currentMusicSongTreeIndex = m_musicSongTreeWidget->currentIndex();
    m_musicPlaylist->clear();
    m_musicPlaylist->addMedia(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->getMusicSongsFilePath(m_currentMusicSongTreeIndex));
    m_musicSongTreeWidget->setCurrentMusicSongTreeIndex(m_currentMusicSongTreeIndex);
}

void MusicApplication::readXMLConfigFromText()
{
    int value = DEFAULT_LEVEL_LOWER;

    //Path configuration song
    MusicSongItems songs;
    MusicTKPLConfigManager listXml;
    if(listXml.readConfig())
    {
        listXml.readPlaylistData(songs);
    }
    const bool success = m_musicSongTreeWidget->addMusicLists(songs);
    //
    MusicSysConfigManager xml;
    if(!xml.readConfig())
    {
        return;
    }
    xml.readSysConfigData();
    m_applicationObject->loadNetWorkSetting();

    //
    switch( M_SETTING_PTR->value(MusicSettingManager::PlayModeChoiced).toInt() )
    {
        case MusicObject::PM_PlayOrder:
            musicPlayOrder();break;
        case MusicObject::PM_PlayRandom:
            musicPlayRandom();break;
        case MusicObject::PM_PlaylistLoop:
            musicPlaylistLoop();break;
        case MusicObject::PM_PlayOneLoop:
            musicPlayOneLoop();break;
        case MusicObject::PM_PlayOnce:
            musicPlayItemOnce();break;
        default:break;
    }
    //
    value = M_SETTING_PTR->value(MusicSettingManager::RemoteWidgetModeChoiced).toInt();
    if(value != 0)
    {
        m_topAreaWidget->musicRemoteTypeChanged(value);
    }

    //The size of the volume of the allocation of songs
    musicVolumeChanged(M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt());
//    m_musicPlayer->setSoundEffectVolume(M_SETTING_PTR->value(MusicSettingManager::EnhancedBalanceChoiced).toInt());

    //Configure playback mode
    m_ui->musicEnhancedButton->setEnhancedMusicConfig(M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt());
    m_applicationObject->musicEffectChanged();
    if(M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt() == 1)
    {
        m_musicPlayer->setEqInformation();
    }

    //music hotkey
    if(M_SETTING_PTR->value(MusicSettingManager::HotkeyEnableChoiced).toBool())
    {
        QStringList hotkeys = M_SETTING_PTR->value(MusicSettingManager::HotkeyStringChoiced).toString().split(TTK_STR_SPLITER);
        if(hotkeys.count() != M_HOTKEY_PTR->count())
        {
            hotkeys = M_HOTKEY_PTR->getDefaultKeys();
        }
        M_HOTKEY_PTR->setHotKeys(hotkeys);
        M_HOTKEY_PTR->enabledAll(true);
    }

    //musicSetting
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInChoiced, false);
    //Just always set fade false, because it is not finished yet.
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnableChoiced, false);
#ifdef Q_OS_UNIX
    //Disable  window quit mode on unix
    M_SETTING_PTR->setValue(MusicSettingManager::WindowQuitModeChoiced, false);
#endif
    //Set inline lrc mode always on
    M_SETTING_PTR->setValue(MusicSettingManager::ShowInlineLrcChoiced, true);
    m_rightAreaWidget->setInlineLrcVisible(true);

    //Set the desktop lrc should be shown
    m_rightAreaWidget->setWindowLrcTypeChanged();
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometryChoiced, xml.readShowDLrcGeometry());

    //Set the current background color and alpha value
    m_topAreaWidget->setBackgroundParams(M_SETTING_PTR->value(MusicSettingManager::BackgroundThemeChoiced).toString(),
                                         M_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentChoiced).toInt(),
                                         M_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparentChoiced).toInt());

    //Configuration from next time also stopped at the last record.
    QStringList keyList;
    xml.readSystemLastPlayIndexConfig(keyList);
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, keyList);
    //add new music file to playlist
    value = keyList[1].toInt();
    m_musicPlaylist->addMedia(value, m_musicSongTreeWidget->getMusicSongsFilePath(value));
    if(DEFAULT_LEVEL_LOWER < value && value < songs.count())
    {
        m_ui->musicPlayedList->append(songs[value].m_songs);
    }

    if(success && keyList[0] == "1")
    {
        QTimer::singleShot(MT_MS, m_musicSongTreeWidget, SLOT(setCurrentIndex()));
        const int index = keyList[2].toInt();
        m_currentMusicSongTreeIndex = (index == DEFAULT_LEVEL_LOWER) ? DEFAULT_LEVEL_LOWER : value;
        m_musicPlaylist->blockSignals(true);
        m_musicPlaylist->setCurrentIndex(m_currentMusicSongTreeIndex, m_musicSongTreeWidget->mapFilePathBySongIndex(m_currentMusicSongTreeIndex, index));
        m_musicPlaylist->blockSignals(false);
        m_ui->musicPlayedList->setCurrentIndex();
    }

    //Configure automatic playback
    if(M_SETTING_PTR->value(MusicSettingManager::AutoPlayChoiced).toInt() == 1)
    {
        m_playControl = true;
        musicStatePlay();
    }
    m_bottomAreaWidget->showPlayStatus(m_playControl);
    m_rightAreaWidget->showPlayStatus(m_playControl);
    m_topAreaWidget->showPlayStatus(m_playControl);

    //Set the lrc color the user set
    m_bottomAreaWidget->lockDesktopLrc(M_SETTING_PTR->value(MusicSettingManager::DLrcLockedChoiced).toInt());
    m_rightAreaWidget->setSettingParameter();

    //init or reset the window
    value = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toInt();
    m_bottomAreaWidget->setDestopLrcVisible(value);
    m_rightAreaWidget->setDestopLrcVisible(value);

    //Reset geometry
    setGeometry( xml.readWindowGeometry() );

    //Reset window concise
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool())
    {
        musicWindowConciseChanged();
    }

    //Update check on
    if(M_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateChoiced).toBool())
    {
        m_applicationObject->soureUpdateCheck();
    }

    m_bottomAreaWidget->getParameterSetting();

}

void MusicApplication::writeXMLConfigToText()
{
    MusicSysConfigManager xml;
    m_applicationObject->sideAnimationReset();
    M_SETTING_PTR->setValue(MusicSettingManager::WidgetPosition, pos());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicChoiced, m_musicPlayer->getMusicEnhanced());
    M_SETTING_PTR->setValue(MusicSettingManager::PlayModeChoiced, m_musicPlaylist->playbackMode());
    M_SETTING_PTR->setValue(MusicSettingManager::VolumeChoiced, m_ui->musicSound->value());

    QStringList lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    if(lastPlayIndexChoiced.isEmpty())
    {
        lastPlayIndexChoiced << "0" << "0" << "-1";
    }
    else
    {
        const MusicPlayItem &item = m_musicPlaylist->currentItem();
        lastPlayIndexChoiced[1] = QString::number(item.m_toolIndex);
        lastPlayIndexChoiced[2] = QString::number(m_musicSongTreeWidget->mapSongIndexByFilePath(item.m_toolIndex, item.m_path));
    }
    M_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndexChoiced, lastPlayIndexChoiced);
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundThemeChoiced, m_topAreaWidget->getBackgroundPath());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentChoiced, m_topAreaWidget->getBackgroundAlpha());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparentChoiced, m_topAreaWidget->getBackgroundListAlpha());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnableChoiced, m_topAreaWidget->getBackgroundTransparentEnable());
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, m_rightAreaWidget->getDestopLrcVisible());
    xml.writeSysConfigData();

    MusicTKPLConfigManager listXml;
    listXml.writePlaylistData(m_musicSongTreeWidget->getMusicLists());
}
