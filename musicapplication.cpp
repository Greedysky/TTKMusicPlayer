#include "musicapplication.h"
#include "ui_musicapplication.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsongssummarizied.h"
#include "musicxmlconfigmanager.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicbgthememanager.h"
#include "musicsettingmanager.h"
#include "musicversion.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"
#include "musictime.h"
#include "musicbottomareawidget.h"
#include "musictopareawidget.h"
#include "musicrightareawidget.h"
#include "musicleftareawidget.h"
#include "musicapplicationobject.h"
#include "musicconnectionpool.h"

#include <QDesktopWidget>

MusicApplication::MusicApplication(QWidget *parent) :
    MusicAbstractMoveWidget(parent),
    ui(new Ui::MusicApplication)
{
    ui->setupUi(this);

    QWidget *widget = QApplication::desktop();
    M_SETTING->setValue(MusicSettingManager::ScreenSize, widget->size());
    move((widget->width() - width())/2, (widget->height() - height())/2);
    M_Connection->setValue("MusicApplication", this);

    m_object = new MusicApplicationObject(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    drawWindowShadow(false);

    ////////////////////////////////////////////////
    m_musicPlayer = new MusicPlayer(this);
    m_musicList = new MusicPlaylist(this);
    m_musicSongTree = new MusicSongsSummarizied(this);
    ui->songsContainer->addWidget(m_musicSongTree);

    m_bottomAreaWidget = new MusicBottomAreaWidget(this);
    m_bottomAreaWidget->setupUi(ui);
    m_topAreaWidget = new MusicTopAreaWidget(this);
    m_topAreaWidget->setupUi(ui);
    m_rightAreaWidget = new MusicRightAreaWidget(this);
    m_rightAreaWidget->setupUi(ui);
    m_leftAreaWidget = new MusicLeftAreaWidget(this);
    m_leftAreaWidget->setupUi(ui);
    connect(m_topAreaWidget,SIGNAL(setTransparent(int)),m_musicSongTree,SLOT(setTransparent(int)));
    connect(m_topAreaWidget,SIGNAL(updateArtPicture()),m_musicSongTree,SLOT(updateArtPicture()));
    connect(m_topAreaWidget,SIGNAL(musicSearchButtonClicked()),m_rightAreaWidget,SLOT(musicSearchButtonSearched()));
    connect(m_rightAreaWidget,SIGNAL(updateBgThemeDownload()),m_topAreaWidget,SLOT(musicBgThemeDownloadFinished()));
    connect(m_rightAreaWidget,SIGNAL(updateBackgroundTheme()),m_topAreaWidget,SLOT(musicBgTransparentChanged()));
    connect(m_bottomAreaWidget,SIGNAL(setShowDesktopLrc(bool)),m_rightAreaWidget,SLOT(setDestopLrcVisible(bool)));
    connect(m_bottomAreaWidget,SIGNAL(setWindowLockedChanged()),m_rightAreaWidget,SLOT(setWindowLockedChanged()));
    connect(m_rightAreaWidget,SIGNAL(lockDesktopLrc(bool)),m_bottomAreaWidget,SLOT(lockDesktopLrc(bool)));
    connect(m_rightAreaWidget,SIGNAL(desktopLrcClosed()),m_bottomAreaWidget,SLOT(desktopLrcClosed()));

    initWindowSurface();
    setAcceptDrops(true);

    m_musicList->setPlaybackMode(MusicObject::MC_PlayOrder);
    //The default is the order of play
    m_musicPlayer->setPlaylist(m_musicList);
    m_musicPlayer->setVolume(100);
    //The default Volume is 100
    ui->musicSoundSlider->setRange(0,100);
    ui->musicSoundSlider->setValue(100);
    m_playControl = true;//The default in the suspended state
    m_currentMusicSongTreeIndex = 0;

    connect(m_musicPlayer,SIGNAL(positionChanged(qint64)),SLOT(positionChanged(qint64)));
    connect(m_musicPlayer,SIGNAL(durationChanged(qint64)),SLOT(durationChanged(qint64)));
    connect(m_musicPlayer,SIGNAL(stateChanged(MusicPlayer::State)),SLOT(stateChanged()));
    connect(m_musicList,SIGNAL(currentIndexChanged(int)), this, SLOT(showCurrentSong(int)));
    connect(m_musicList,SIGNAL(currentIndexChanged(int)), m_musicSongTree, SLOT(setMusicPlayCount(int)));

    connect(m_musicSongTree,SIGNAL(deleteItemAt(MIntList)),SLOT(setDeleteItemAt(MIntList)));
    connect(m_musicSongTree,SIGNAL(updatePlayLists(QString)),m_musicList,SLOT(appendMedia(QString)));
    connect(m_musicSongTree,SIGNAL(updateMediaLists(QStringList,int)),m_musicList,SLOT(updateMediaLists(QStringList,int)));

    ui->SurfaceStackedWidget->setCurrentIndex(0);
    ui->musicTimeWidget->setObject(this);

    readXMLConfigFromText();
}

bool MusicApplication::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    m_object->nativeEvent(eventType, message, result);
    return QWidget::nativeEvent(eventType, message, result);
}

MusicApplication::~MusicApplication()
{
    delete m_musicPlayer;
    delete m_musicList;
    delete m_musicSongTree;
    delete m_bottomAreaWidget;
    delete m_topAreaWidget;
    delete m_rightAreaWidget;
    delete m_leftAreaWidget;
    delete m_object;
    delete ui;
}

void MusicApplication::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    event->ignore();
    if(!m_bottomAreaWidget->getSystemCloseConfig() &&
        m_bottomAreaWidget->systemTrayIsVisible() )
    {
       hide();
       m_bottomAreaWidget->showMessage(tr("Prompt"),
                                       tr("QMusicPlayer will run in the background"));
    }
    else
    {
       quitWindowClose();
    }
}

void MusicApplication::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget::dragEnterEvent(event);
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void MusicApplication::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget::dragMoveEvent(event);
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void MusicApplication::dropEvent(QDropEvent *event)
{
    QWidget::dropEvent(event);
    const QMimeData *data = event->mimeData();
    QStringList fileList;
    QString suffix;
    QStringList sfx = MusicPlayer::supportFormatsString();

    foreach(QUrl url, data->urls())
    {
        suffix = QFileInfo(url.toLocalFile()).suffix();
        if( sfx.contains(suffix.toLower()) )
        {
            fileList<<url.toLocalFile();
        }
        else
        {
            MusicMessageBox message;
            message.setText(url.toString().split('/').back() + tr("not supported"));
            message.exec();
        }
    }
    musicImportSongsSettingPath(fileList);
}

void MusicApplication::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/login"),tr("logout"),m_topAreaWidget,SLOT(musicUserContextLogin()));
    rightClickMenu.addSeparator();

    QMenu musicAddNewFiles(tr("addNewFiles"),&rightClickMenu);
    rightClickMenu.addMenu(&musicAddNewFiles)->setIcon(QIcon(":/contextMenu/add"));
    musicAddNewFiles.addAction(tr("openOnlyFiles"),this,SLOT(musicImportSongsOnlyFile()));
    musicAddNewFiles.addAction(tr("openOnlyDir"),this,SLOT(musicImportSongsOnlyDir()));

    QMenu musicPlaybackMode(tr("playbackMode"),&rightClickMenu);
    rightClickMenu.addMenu(&musicPlaybackMode);
    createPlayModeMenu(musicPlaybackMode);

    rightClickMenu.addSeparator();
    QMenu musicRemoteControl(tr("RemoteControl"),&rightClickMenu);
    rightClickMenu.addMenu(&musicRemoteControl)->setIcon(QIcon(":/contextMenu/remote"));
    musicRemoteControl.addAction(tr("SquareRemote"), m_topAreaWidget, SLOT(musicSquareRemote()));
    musicRemoteControl.addAction(tr("RectangleRemote"), m_topAreaWidget, SLOT(musicRectangleRemote()));
    musicRemoteControl.addAction(tr("DiamondRemote"), m_topAreaWidget, SLOT(musicDiamondRemote()));
    musicRemoteControl.addAction(tr("CircleRemote"), m_topAreaWidget, SLOT(musicCircleRemote()));
    musicRemoteControl.addAction(tr("DeleteRemote"), m_topAreaWidget, SLOT(musicDeleteRemote()));

    rightClickMenu.addAction(QIcon(":/contextMenu/equalizer"),tr("Equalizer"), m_object, SLOT(musicSetEqualizer()));
    rightClickMenu.addAction(tr("AudioRecorder"), m_object, SLOT(musicAudioRecorder()));
    rightClickMenu.addAction(tr("TimingSettings"), m_object, SLOT(musicTimerWidget()));
    rightClickMenu.addAction(tr("ShowingSpectrum"), m_leftAreaWidget, SLOT(musicSpectrumWidget()));
    rightClickMenu.addSeparator();

    QAction *window = rightClickMenu.addAction(tr("WindowTop"), m_object, SLOT(musicSetWindowToTop()));
    window->setIcon(QIcon(m_object->getWindowToTop() ? ":/share/selected" : QString()));

    rightClickMenu.addAction(QIcon(":/contextMenu/setting"),tr("Setting"),this,SLOT(musicSetting()));
    rightClickMenu.addAction(QIcon(":/contextMenu/location"),tr("musicLocation"),this,SLOT(musicCurrentPlayLocation()));

    QMenu musicInfo(tr("musicAbout"),&rightClickMenu);
    rightClickMenu.addMenu(&musicInfo)->setIcon(QIcon(":/contextMenu/about"));
    musicInfo.addAction(QIcon(":/contextMenu/about"),tr("musicAbout"), m_object, SLOT(musicAboutUs()));
    musicInfo.addAction(QIcon(":/contextMenu/edition"), QMUSICPLAYER_VERSION);

    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/quit"),tr("quit"),this,SLOT(quitWindowClose()));
    rightClickMenu.exec(QCursor::pos());
}

void MusicApplication::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
}

void MusicApplication::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
    if(event->key() == Qt::Key_Space)
    {
        musicKey();
    }
}

void MusicApplication::initWindowSurface()
{
    connect(ui->action_About, SIGNAL(triggered()), m_object, SLOT(musicAboutUs()));
    connect(ui->musicSearch,SIGNAL(clicked()), m_leftAreaWidget, SLOT(musicSearch()));
    connect(ui->musicDesktopLrc, SIGNAL(clicked(bool)), m_rightAreaWidget, SLOT(setDestopLrcVisible(bool)));
    ui->musicPlayMode->setMenu(&m_playModeMenu);
    ui->musicPlayMode->setPopupMode(QToolButton::InstantPopup);
}

void MusicApplication::createPlayModeMenuIcon(QMenu &menu)
{
    QList<QAction*> as = menu.actions();
    MusicObject::SongPlayType songplaymode = m_musicList->playbackMode();
    (songplaymode == MusicObject::MC_PlayOrder) ? as[0]->setIcon(QIcon(":/share/selected")) : as[0]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayRandom) ? as[1]->setIcon(QIcon(":/share/selected")) : as[1]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayListLoop) ? as[2]->setIcon(QIcon(":/share/selected")) : as[2]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayOneLoop) ? as[3]->setIcon(QIcon(":/share/selected")) : as[3]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayOnce) ? as[4]->setIcon(QIcon(":/share/selected")) : as[4]->setIcon(QIcon());
}

void MusicApplication::createPlayModeMenu(QMenu &menu)
{
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("OrderPlay"),this,SLOT(musicPlayOrder()));
    menu.addAction(tr("RandomPlay"),this,SLOT(musicPlayRandom()));
    menu.addAction(tr("ListCycle"),this,SLOT(musicPlayListLoop()));
    menu.addAction(tr("SingleCycle"),this,SLOT(musicPlayOneLoop()));
    menu.addAction(tr("PlayOnce"),this,SLOT(musicPlayItemOnce()));
    createPlayModeMenuIcon(menu);
}

void MusicApplication::readXMLConfigFromText()
{
    MusicXMLConfigManager xml;
    int value;
    QString key;
    if(!xml.readMusicXMLConfig())//open music file
    {
        return;
    }
    //////////////////////////////////////////////////////////////
    //Path configuration song
    MMusicList songs;
    xml.readMusicSongsConfig(songs);
    m_musicSongTree->setMusicLists(songs);
    //////////////////////////////////////////////////////////////
    if(!xml.readXMLConfig())//open file
    {
        return;
    }
    //Configure playback mode
    ui->musicEnhancedButton->setEnhancedMusicConfig(xml.readEnhancedMusicConfig());

    createPlayModeMenu(m_playModeMenu);
    switch( xml.readMusicPlayModeConfig() )
    {
        case MusicObject::MC_PlayOrder:
            musicPlayOrder();break;
        case MusicObject::MC_PlayRandom:
            musicPlayRandom();break;
        case MusicObject::MC_PlayListLoop:
            musicPlayListLoop();break;
        case MusicObject::MC_PlayOneLoop:
            musicPlayOneLoop();break;
        case MusicObject::MC_PlayOnce:
            musicPlayItemOnce();break;
        default:break;
    }
    //////////////////////////////////////////////////////////////
    //The size of the volume of the allocation of songs
    value = xml.readMusicPlayVolumeConfig();
    ui->musicSoundSlider->setValue(value);
    M_SETTING->setValue(MusicSettingManager::VolumeChoiced, value);
    ////////////////////////musicSetting
    //////////////////////////////////////////////////////////////
    //Set the inline lrc should be shown
    key = xml.readShowInlineLrc();
    M_SETTING->setValue(MusicSettingManager::ShowInlineLrcChoiced, key);
    m_rightAreaWidget->setInlineLrcVisible(key);
    //////////////////////////////////////////////////////////////
    //Set the desktop lrc should be shown
    key = xml.readShowDesktopLrc();
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced, key);
    m_bottomAreaWidget->setDestopLrcVisible(key);
    m_rightAreaWidget->setDestopLrcVisible(key);
    //////////////////////////////////////////////////////////////
    //Configure automatic playback
    key = xml.readSystemAutoPlayConfig();
    M_SETTING->setValue(MusicSettingManager::AutoPlayChoiced, key);
    m_bottomAreaWidget->showPlayStatus(m_playControl);
    m_rightAreaWidget->showPlayStatus(m_playControl);
    if(key == "true")
    {
        musicKey();
    }
    //////////////////////////////////////////////////////////////
    //Configuration from next time also stopped at the last record.
    QStringList keyList;
    xml.readSystemLastPlayIndexConfig(keyList);
    M_SETTING->setValue(MusicSettingManager::LastPlayIndexChoiced, keyList);
    //add new music file to playlist
    m_musicList->addMedia(m_musicSongTree->getMusicSongsFilePath(keyList[1].toInt()));
    if(keyList[0] == "1")
    {
        QTimer::singleShot(1, m_musicSongTree, SLOT(setCurrentIndex()));
        m_currentMusicSongTreeIndex = keyList[1].toInt();
        m_musicList->blockSignals(true);
        m_musicList->setCurrentIndex(keyList[2].toInt());
        m_musicList->blockSignals(false);
    }
    //////////////////////////////////////////////////////////////
    //When the configuration is close to the direct exit
    key = xml.readSystemCloseConfig();
    M_SETTING->setValue(MusicSettingManager::CloseEventChoiced, key);
    m_bottomAreaWidget->setSystemCloseConfig(key);
    //////////////////////////////////////////////////////////////
    //Set the current background color
    //Set the current alpha value
    m_topAreaWidget->setParameters(xml.readBackgroundTheme(),
                                   xml.readBackgroundTransparent().toInt(),
                                   xml.readBackgroundListTransparent().toInt());
    //////////////////////////////////////////////////////////////
    xml.readTimeAutoConfig();
    //Set the lrc color the user set
    M_SETTING->setValue(MusicSettingManager::LrcColorChoiced,xml.readShowLrcColor());
    M_SETTING->setValue(MusicSettingManager::LrcFgColorChoiced,xml.readShowLrcFgColor());
    M_SETTING->setValue(MusicSettingManager::LrcBgColorChoiced,xml.readShowLrcBgColor());
    M_SETTING->setValue(MusicSettingManager::LrcFamilyChoiced,xml.readShowLrcFamily());
    M_SETTING->setValue(MusicSettingManager::LrcTypeChoiced,xml.readShowLrcType());
    M_SETTING->setValue(MusicSettingManager::LrcColorTransChoiced,xml.readShowLrcTransparent());
    M_SETTING->setValue(MusicSettingManager::LrcSizeChoiced,xml.readShowLrcSize());
    //////////////////////////////////////////////////////////////
    //Set the lrc size the user set
    M_SETTING->setValue(MusicSettingManager::DLrcColorChoiced,xml.readShowDLrcColor());
    M_SETTING->setValue(MusicSettingManager::DLrcFgColorChoiced,xml.readShowDLrcFgColor());
    M_SETTING->setValue(MusicSettingManager::DLrcBgColorChoiced,xml.readShowDLrcBgColor());
    M_SETTING->setValue(MusicSettingManager::DLrcFamilyChoiced,xml.readShowDLrcFamily());
    M_SETTING->setValue(MusicSettingManager::DLrcTypeChoiced,xml.readShowDLrcType());
    M_SETTING->setValue(MusicSettingManager::DLrcColorTransChoiced,xml.readShowDLrcTransparent());
    M_SETTING->setValue(MusicSettingManager::DLrcSizeChoiced,xml.readShowDLrcSize());
    M_SETTING->setValue(MusicSettingManager::DLrcLockedChoiced,xml.readShowDLrcLocked());
    M_SETTING->setValue(MusicSettingManager::DLrcGeometryChoiced,xml.readShowDLrcGeometry());
    m_bottomAreaWidget->lockDesktopLrc(xml.readShowDLrcLocked());
    m_rightAreaWidget->setSettingParameter();
    //////////////////////////////////////////////////////////////
    M_SETTING->setValue(MusicSettingManager::EqualizerEnableChoiced,xml.readEqualizerEnale());
    M_SETTING->setValue(MusicSettingManager::EqualizerValueChoiced,xml.readEqualizerValue());
    M_SETTING->setValue(MusicSettingManager::EqualizerIndexChoiced,xml.readEqualizerIndex());
    m_musicPlayer->setEqInformation();
    //////////////////////////////////////////////////////////////
    M_SETTING->setValue(MusicSettingManager::CurrentLanIndexChoiced,xml.readLanguageIndex());

}

void MusicApplication::writeXMLConfigToText()
{
    MusicXMLConfigManager xml;
    QStringList lastPlayIndexChoiced;

    M_SETTING->setValue(MusicSettingManager::EnhancedMusicChoiced, m_musicPlayer->getMusicEnhanced());
    M_SETTING->setValue(MusicSettingManager::PlayModeChoiced, m_musicList->playbackMode());
    M_SETTING->setValue(MusicSettingManager::VolumeChoiced, ui->musicSoundSlider->value());
    lastPlayIndexChoiced = M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    lastPlayIndexChoiced[1] = QString::number(m_musicSongTree->getCurrentPlayToolIndex());
    lastPlayIndexChoiced[2] = QString::number(m_musicList->currentIndex());
    M_SETTING->setValue(MusicSettingManager::LastPlayIndexChoiced,lastPlayIndexChoiced);
    M_SETTING->setValue(MusicSettingManager::BgThemeChoiced, m_topAreaWidget->getBgSkin());
    M_SETTING->setValue(MusicSettingManager::BgTransparentChoiced, m_topAreaWidget->getBgSkinAlpha());
    M_SETTING->setValue(MusicSettingManager::BgListTransparentChoiced, m_topAreaWidget->getListBgSkinAlpha());
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced, m_rightAreaWidget->getDestopLrcVisible());
    xml.writeXMLConfig();
    xml.writeMusicSongsConfig( m_musicSongTree->getMusicLists() );
}

void MusicApplication::quitWindowClose()
{
    //Write configuration files
    writeXMLConfigToText();
    m_object->windowCloseAnimationOpacity();
}

void MusicApplication::positionChanged(qint64 position)
{
    m_rightAreaWidget->updateCurrentLrc(position, m_musicPlayer->duration(), m_playControl);
    ui->musicTimeWidget->setValue(position);
    if(m_musicList->isEmpty())
    {
        ui->playCurrentTime->setText("00:00");
    }
    else
    {
        ui->playCurrentTime->setText(MusicTime::msecTime2LabelJustified(position));
    }
    //Show the current play time
    m_musicSongTree->setTimerLabel(ui->playCurrentTime->text());
#ifdef MUSIC_DEBUG
    m_bottomAreaWidget->setValue(position);
#endif
}

void MusicApplication::durationChanged(qint64 duration)
{
    //Show the current play total time
    ui->musicTimeWidget->setRange(0,duration);
    ui->playTotalTime->setText(MusicTime::msecTime2LabelJustified(duration));
    //Loading the current song lrc
    musicLoadCurrentSongLrc();
#ifdef MUSIC_DEBUG
    m_bottomAreaWidget->setRange(0, duration);
#endif
}

void MusicApplication::showCurrentSong(int index)
{
    if( index > -1 ) //The list to end
    {
        m_musicSongTree->selectRow(index);
        QString name = m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex())[index];
        ui->showCurrentSong->setText(tr("Playing")+QString(": %1 = = %2").arg(index + 1).arg(name));
        //Show the current play song information
        M_BG_MANAGER->clearArtName();
        m_rightAreaWidget->musicCheckHasLrcAlready();
        m_bottomAreaWidget->setLabelText(name);
        m_topAreaWidget->setLabelText(name);
        //display current ArtTheme pic
        M_BG_MANAGER->setArtName(getCurrentFileName());
        m_topAreaWidget->musicBgThemeDownloadFinished();
    }
    else
    {
        ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
        m_playControl = true;
        m_musicPlayer->stop();
        m_rightAreaWidget->stopLrcMask();

        m_bottomAreaWidget->showPlayStatus(m_playControl);
        m_rightAreaWidget->showPlayStatus(m_playControl);
        m_topAreaWidget->showPlayStatus(m_playControl);
    }
}

void MusicApplication::stateChanged()
{
    m_playControl = true;
    ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
}

void MusicApplication::musicKey()
{
    if(m_musicList->isEmpty())
    {
        return;//The playlist is not performing space-time
    }
    if(m_playControl)
    {
        ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/stop")));
        m_playControl = false;
        m_musicPlayer->play();
        m_topAreaWidget->musicBgThemeDownloadFinished();
        m_rightAreaWidget->startTimerClock();
    }
    else
    {
        ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
        m_playControl = true;
        m_musicPlayer->pause();
        m_topAreaWidget->setTimerStop();
        m_rightAreaWidget->stopLrcMask();
    }
    m_bottomAreaWidget->showPlayStatus(m_playControl);
    m_rightAreaWidget->showPlayStatus(m_playControl);
    m_topAreaWidget->showPlayStatus(m_playControl);
    ui->musicTimeWidget->setPlayState(m_playControl);
}

void MusicApplication::musicPlayPrivious()
{
    if(m_musicList->isEmpty())
    {
        return;//The playlist is not performing space-time
    }
    if(m_musicList->playbackMode() == MusicObject::MC_PlayRandom)
    {
        m_musicList->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playPrivious();
    }
    m_playControl = true;
    this->musicKey();
    m_playControl = false;
}

void MusicApplication::musicPlayNext()
{
    if(m_musicList->isEmpty())
    {
        return;//The playlist is not performing space-time
    }
    if(m_musicList->playbackMode() == MusicObject::MC_PlayRandom)
    {
        m_musicList->setCurrentIndex();
    }
    else
    {
        m_musicPlayer->playNext();
    }
    m_playControl = true;
    this->musicKey();
    m_playControl = false;
}

void MusicApplication::musicPlayOrder()
{
    m_musicList->setPlaybackMode(MusicObject::MC_PlayOrder);
    ui->musicPlayMode->setIcon(QIcon(QString::fromUtf8(":/image/orderplay")));
    m_musicSongTree->setPlaybackMode(MusicObject::MC_PlayOrder);
    createPlayModeMenuIcon(m_playModeMenu);
}

void MusicApplication::musicPlayRandom()
{
    m_musicList->setPlaybackMode(MusicObject::MC_PlayRandom);
    ui->musicPlayMode->setIcon(QIcon(QString::fromUtf8(":/image/randomplay")));
    m_musicSongTree->setPlaybackMode(MusicObject::MC_PlayRandom);
    createPlayModeMenuIcon(m_playModeMenu);
}

void MusicApplication::musicPlayListLoop()
{
    m_musicList->setPlaybackMode(MusicObject::MC_PlayListLoop);
    ui->musicPlayMode->setIcon(QIcon(QString::fromUtf8(":/image/listloop")));
    m_musicSongTree->setPlaybackMode(MusicObject::MC_PlayListLoop);
    createPlayModeMenuIcon(m_playModeMenu);
}

void MusicApplication::musicPlayOneLoop()
{
    m_musicList->setPlaybackMode(MusicObject::MC_PlayOneLoop);
    ui->musicPlayMode->setIcon(QIcon(QString::fromUtf8(":/image/oneloop")));
    m_musicSongTree->setPlaybackMode(MusicObject::MC_PlayOneLoop);
    createPlayModeMenuIcon(m_playModeMenu);
}

void MusicApplication::musicPlayItemOnce()
{
    m_musicList->setPlaybackMode(MusicObject::MC_PlayOnce);
    ui->musicPlayMode->setIcon(QIcon(QString::fromUtf8(":/image/playonce")));
    m_musicSongTree->setPlaybackMode(MusicObject::MC_PlayOnce);
    createPlayModeMenuIcon(m_playModeMenu);
}

void MusicApplication::musicVolumeNULL()
{
    if(!m_musicPlayer->isMuted())
    {
        m_musicPlayer->setMuted(true);
        ui->musicSound->setStyleSheet(MusicUIObject::MCustomStyle24);
    }
    else
    {
        m_musicPlayer->setMuted(false);
        ui->musicSound->setStyleSheet(MusicUIObject::MCustomStyle25);
    }
    ui->musicSoundSlider->blockSignals(true);
    ui->musicSoundSlider->setValue(m_musicPlayer->volume());
    ui->musicSoundSlider->blockSignals(false);
    ui->volumeValue->setText(QString("%1%").arg(m_musicPlayer->volume()));
    M_SETTING->setValue(MusicSettingManager::VolumeChoiced, m_musicPlayer->volume());
}

void MusicApplication::musicVolumeChanged(int volume)
{
    m_topAreaWidget->setVolumeValue(volume);
    m_bottomAreaWidget->setVolumeValue(volume);
    m_musicPlayer->setVolume(volume);
    ui->volumeValue->setText(QString("%1%").arg(volume));
    (volume > 0) ? ui->musicSound->setStyleSheet(MusicUIObject::MCustomStyle24)
                 : ui->musicSound->setStyleSheet(MusicUIObject::MCustomStyle25);
    M_SETTING->setValue(MusicSettingManager::VolumeChoiced, volume);
}

void MusicApplication::musicActionVolumeSub()
{
    int currentVol = m_musicPlayer->volume();
    currentVol -= 15;
    if( currentVol < 0)
    {
        currentVol = 0;   //reset music volume
    }
    ui->musicSoundSlider->setValue(currentVol);
    musicVolumeChanged( currentVol );
}

void MusicApplication::musicActionVolumePlus()
{
    int currentVol = m_musicPlayer->volume();
    currentVol += 15;
    if( currentVol > 100)
    {
        currentVol = 100;   //reset music volume
    }
    ui->musicSoundSlider->setValue(currentVol);
    musicVolumeChanged( currentVol );
}

void MusicApplication::musicImportSongsOnlyFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles );
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters( MusicPlayer::supportFormatsFilterDialogString() );

    if(dialog.exec())
    {
        musicImportSongsSettingPath(dialog.selectedFiles());
    }
}

void MusicApplication::musicImportSongsOnlyDir()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory );
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        QList<QFileInfo> file(dialog.directory().entryInfoList());
        QStringList fileList;
        for(int i=0; i<file.count(); ++i)
        {
            if(file[i].isFile())
            {
               fileList << file[i].absoluteFilePath();
            }
        }
        musicImportSongsSettingPath(fileList);
    }
}

void MusicApplication::musicImportSongsSettingPath(const QStringList &path)
{
    m_musicSongTree->importOtherMusicSongs(path);//append in songsList
    if(m_currentMusicSongTreeIndex == 0)
    {
        m_musicList->appendMedia(path);
    }
    if(path.count() > 0 && m_musicList->currentIndex() < 0)
    {
        m_musicList->setCurrentIndex(0);
    }
}

void MusicApplication::musicImportSongs()
{
    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("openOnlyFiles"),this,SLOT(musicImportSongsOnlyFile()));
    menu.addAction(tr("openOnlyDir"),this,SLOT(musicImportSongsOnlyDir()));
    menu.addSeparator();
    menu.addAction(tr("dragAnddrop"))->setEnabled(false);
    menu.exec(QCursor::pos());
}

void MusicApplication::musicPlayIndex(int row, int)
{
    m_musicPlayer->stop();//stop playing the privious song
    if(m_currentMusicSongTreeIndex != m_musicSongTree->currentIndex())
    {
        m_musicList->clear();
        m_musicList->addMedia(m_musicSongTree->getMusicSongsFilePath(m_musicSongTree->currentIndex()));
        m_currentMusicSongTreeIndex = m_musicSongTree->currentIndex();
        m_musicSongTree->currentMusicSongTreeIndexChanged(m_currentMusicSongTreeIndex);
    }
    if(!m_searchfileListCache.isEmpty())
    {
        int count = m_leftAreaWidget->getSearchedText().count();
        row = m_searchfileListCache.value(count)[row];
        m_leftAreaWidget->clearSearchedText();
        m_searchfileListCache.clear();
    }

    m_musicList->setCurrentIndex(row);
    m_playControl = true;
    musicKey();
    m_playControl = false;
}

void MusicApplication::musicPlayAnyTimeAt(int posValue)
{
    //Drag the progress indicator to rewind or fast-forward through the current song
    m_musicPlayer->setPosition( posValue);
    //Set lrc corrent to show
    m_rightAreaWidget->setSongSpeedAndSlow(posValue);
}

void MusicApplication::musicSetting()
{
    m_rightAreaWidget->showSettingWidget();
}

void MusicApplication::musicCurrentPlayLocation()
{
    if(m_musicList->isEmpty())
    {
        return;
    }
    m_musicSongTree->selectRow(0);
    /*Repair when already in the original row, paging,
     cannot return to the original row */
    m_musicSongTree->selectRow(m_musicList->currentIndex());
}

void MusicApplication::setDeleteItemAt(const MIntList &index)
{
    for(int i=index.count() - 1; i>=0; --i)
    {
        m_musicList->removeMedia(index[i]);
    }
    int oldIndex = m_musicList->currentIndex();
    if( oldIndex == m_musicList->mediaCount())  //Play index error correction
    {
        --oldIndex;
    }

    m_musicList->setCurrentIndex(oldIndex);
    //The corresponding item is deleted from the QMediaPlaylist
    m_playControl = true;
    musicKey();
    m_playControl = false;
}

void MusicApplication::getParameterSetting()
{
    m_rightAreaWidget->getParameterSetting();
    bool config = M_SETTING->value(MusicSettingManager::CloseEventChoiced).toBool();
    m_bottomAreaWidget->setSystemCloseConfig(config);
         config = M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
    m_bottomAreaWidget->setDestopLrcVisible(config ? "true" : "false" );
    //This attribute is effective immediately.
}

void MusicApplication::musicSearchIndexChanged(int, int index)
{
    MIntList searchResult;
    QStringList searchedSongs(m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex()));
    for(int j=0; j<searchedSongs.count(); ++j)
    if(searchedSongs[j].contains(m_leftAreaWidget->getSearchedText(), Qt::CaseInsensitive))
    {
        searchResult << j;
    }
    m_searchfileListCache.insert(index,searchResult);
    m_musicSongTree->setMusicSongsSearchedFileName(searchResult);
}

void MusicApplication::musicLoadCurrentSongLrc()
{
    //display current ArtTheme pic
    m_topAreaWidget->musicBgThemeDownloadFinished();
    //Loading the current song lrc
    if(m_musicList->currentIndex() == -1)
    {
        return;
    }
    QString filename = LRC_DOWNLOAD + m_musicSongTree->getMusicSongsFileName( \
              m_currentMusicSongTreeIndex)[m_musicList->currentIndex()] + LRC_FILE;
    m_rightAreaWidget->loadCurrentSongLrc(getCurrentFileName(), filename);
}

QString MusicApplication::getCurrentFileName() const
{
    if(m_musicList->currentIndex() == -1)
    {
        return QString();
    }
    return m_musicSongTree->getMusicSongsFileName( \
           m_currentMusicSongTreeIndex)[m_musicList->currentIndex()];
}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return (m_musicList->currentIndex() == -1) ? true : false;
}

void MusicApplication::musicCurrentLrcUpdated()
{
    QString filename = getCurrentFileName();
    QFile file(LRC_DOWNLOAD + filename + LRC_FILE);
    if(file.exists())
    {
        file.remove();
    }
    m_rightAreaWidget->musicCheckHasLrcAlready();
}

void MusicApplication::updateCurrentTime(qint64 pos)
{
    if(!m_playControl) ///When pause just resume it
    {
        m_musicPlayer->setPosition(pos);
    }
}

void MusicApplication::musicAddSongToLovestListAt()
{
    int index = m_musicList->currentIndex();
    if(m_musicList->isEmpty() || index < 0 || m_currentMusicSongTreeIndex != m_musicSongTree->currentIndex() ||
       m_musicSongTree->currentIndex() == 1)
    {
        return;
    }
    m_musicSongTree->addMusicSongToLovestListAt(index);
}

void MusicApplication::setPlaySongChanged(int index)
{
    if(m_musicList->isEmpty() || index <0 || index >= m_musicList->mediaCount())
    {
        return;
    }
    musicPlayIndex(index,0);
}

void MusicApplication::setStopSongChanged()
{
    m_musicPlayer->pause();
}

void MusicApplication::addSongToPlayList(const QStringList &item)
{
    musicImportSongsSettingPath(item);
    musicPlayIndex(m_musicList->mediaCount() - 1, 0);
}

void MusicApplication::musicWindowConciseChanged()
{
    m_bottomAreaWidget->setWindowConcise();
    m_topAreaWidget->musicBgThemeDownloadFinished();
}

void MusicApplication::musicEnhancedMusicChanged(int type)
{
    m_musicPlayer->setMusicEnhanced(static_cast<MusicPlayer::Enhanced>(type));
}

void MusicApplication::getCurrentPlayList(QStringList &list)
{
    list = m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex());
}
