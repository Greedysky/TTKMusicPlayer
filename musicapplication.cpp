#include "musicapplication.h"
#include "ui_musicapplication.h"
#include "musicequalizerdialog.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsongsListwidget.h"
#include "musicsettingwidget.h"
#include "musicsystemtraymenu.h"
#include "musictimersliderwidget.h"
#include "musicwindowextras.h"
#include "musiclocalsongsearch.h"
#include "musicaudiorecorderwidget.h"
#include "musictimerwidget.h"
#include "musicmobiledeviceswidget.h"
#include "musicsongssummarizied.h"
#include "musicdownloadquerythread.h"
#include "musicdownloadstatuslabel.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicxmlconfigmanager.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "musictimerautoobject.h"
#include "musicvideoplayer.h"
#include "musicbgthememanager.h"
#include "musicsettingmanager.h"
#include <Dbt.h>

#include "musicuiobject.h"
#include "musicbottomareawidget.h"
#include "musictopareawidget.h"
#include "musicrightareawidget.h"
#include "musicleftareawidget.h"

MusicApplication::MusicApplication(QWidget *parent) :
    MusicMoveWidgetAbstract(parent),
    ui(new Ui::MusicApplication),
    m_setting(NULL),m_musicLocalSongSearch(NULL),
    m_mobileDevices(NULL), m_videoPlayer(NULL)
{
    ui->setupUi(this);
    /////////////////////////////////////////////////
    m_animation = new QPropertyAnimation(this, "windowOpacity");
    m_animation->setDuration(1000);
    m_animation->setStartValue(0);
    m_animation->setEndValue(1);
    m_animation->start();
    /////////////////////////////////////////////////
    setAttribute(Qt::WA_TranslucentBackground, true);
    //Set the window transparent for background
    drawWindowRoundedRect();
    //set window radius

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
    connect(m_topAreaWidget,SIGNAL(updateToolStyle()),m_musicSongTree,SLOT(updateToolStyle()));
    connect(m_topAreaWidget,SIGNAL(updateArtPicture()),m_musicSongTree,SLOT(updateArtPicture()));
    connect(m_rightAreaWidget,SIGNAL(theArtBgHasChanged()),m_topAreaWidget,SLOT(musicBgThemeDownloadFinished()));

    //This is the function to display the desktop lrc
    initWindowSurface();

    m_musicList->setPlaybackMode(MusicObject::MC_PlayOrder);
    //The default is the order of play
    m_musicPlayer->setPlaylist(m_musicList);
    m_musicPlayer->setVolume(100);
    //The default Volume is 100
    ui->musicSoundSlider->setRange(0,100);
    ui->musicSoundSlider->setValue(100);

    m_playControl = true;//The default in the suspended state
    m_systemCloseConfig = false;//Control the mode to exit
    m_setWindowToTop = false;
    m_currentMusicSongTreeIndex = 0;

    m_downloadStatusLabel = new MusicDownloadStatusLabel(this);
    connect(m_musicPlayer,SIGNAL(positionChanged(qint64)),SLOT(positionChanged(qint64)));
    connect(m_musicPlayer,SIGNAL(durationChanged(qint64)),SLOT(durationChanged(qint64)));
    connect(m_musicPlayer,SIGNAL(stateChanged()),SLOT(stateChanged()));
    connect(m_musicList,SIGNAL(currentIndexChanged(int)),this,SLOT(showCurrentSong(int)));

    connect(m_musicSongTree,SIGNAL(deleteItemAt(MIntList)),SLOT(setDeleteItemAt(MIntList)));
    connect(m_musicSongTree,SIGNAL(updatePlayLists(QString)),m_musicList,SLOT(appendMedia(QString)));
    connect(m_musicSongTree,SIGNAL(updateMediaLists(QStringList,int)),m_musicList,SLOT(updateMediaLists(QStringList,int)));

    connect(ui->songSearchWidget,SIGNAL(MuiscSongToPlayListChanged(QString)),
                                 m_musicSongTree, SLOT(addNetMusicSongToList(QString)));
    connect(ui->songSearchWidget,SIGNAL(showDownLoadInfoFor(DownLoadType)),
                                 m_downloadStatusLabel,SLOT(showDownLoadInfoFor(DownLoadType)));
    connect(ui->songSearchWidget,SIGNAL(showDownLoadInfoFinished(QString)),
                                 m_downloadStatusLabel,SLOT(showDownLoadInfoFinished(QString)));
    connect(ui->songSearchWidget,SIGNAL(musicBgDownloadFinished()),m_topAreaWidget,SLOT(musicBgThemeDownloadFinished()));
    connect(m_downloadStatusLabel,SIGNAL(musicBgDownloadFinished()),m_topAreaWidget,SLOT(musicBgThemeDownloadFinished()));

    m_setting = new MusicSettingWidget(this);
    connect(m_setting,SIGNAL(parameterSettingChanged()),SLOT(getParameterSetting()));

    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
    connect(m_musicTimerAutoObj,SIGNAL(setPlaySong(int)),SLOT(setPlaySongChanged(int)));
    connect(m_musicTimerAutoObj,SIGNAL(setStopSong()),SLOT(setStopSongChanged()));

    m_musicWindowExtras = new MusicWindowExtras(this);

    ui->SurfaceStackedWidget->setCurrentIndex(0);
    this->readXMLConfigFromText();

    ui->musicTimeWidget->setObject(this);

}

bool MusicApplication::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    if(msg->message == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
            case DBT_DEVICETYPESPECIFIC:
                break;
            case DBT_DEVICEARRIVAL:
                if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if (lpdbv->dbcv_flags == 0)
                    {
                        DWORD unitmask = lpdbv ->dbcv_unitmask;
                        int i;
                        for(i = 0; i < 26; ++i)
                        {
                            if(unitmask & 0x1)
                                break;
                            unitmask = unitmask >> 1;
                        }
                        qDebug() << "USB_Arrived and The USBDisk is: "<<(char)(i + 'A');
                        delete m_mobileDevices;
                        m_mobileDevices = new MusicMobileDevicesWidget;
                        m_mobileDevices->show();
                    }
                }
                break;
            case DBT_DEVICEREMOVECOMPLETE:
                if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if (lpdbv -> dbcv_flags == 0)
                    {
                        qDebug() << "USB_remove";
                        delete m_mobileDevices;
                        m_mobileDevices = NULL;
                    }
                }
                break;
        }
    }
    return QWidget::nativeEvent(eventType,msg, result);
}

MusicApplication::~MusicApplication()
{
    delete m_bottomAreaWidget;
    delete m_topAreaWidget;
    delete m_rightAreaWidget;
    delete m_leftAreaWidget;

    delete m_musicWindowExtras;
    delete m_videoPlayer;
    delete m_mobileDevices;
    delete m_animation;
    delete m_downloadStatusLabel;
    delete m_musicPlayer;
    delete m_musicList;
    delete m_musicSongTree;
    delete m_systemTray;
    delete m_systemTrayMenu;
    delete m_setting;
    delete m_musicLocalSongSearch;

    delete ui;
}

void MusicApplication::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    if(!m_systemCloseConfig && m_systemTray->isVisible())
    {
       this->hide();
       m_systemTray->showMessage(tr("Prompt"),tr("QMusicPlayer will run in the background"));
       event->ignore();
    }
    else
    {
       event->accept();
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
    QStringList sfx;
    sfx<<"mp3"<<"mp2"<<"mp1"<<"wav"<<"ogg"<<"flac"
       <<"ac3"<<"aac"<<"oga"<<"pcm";
    foreach(QUrl url,data->urls())
    {
        suffix = QFileInfo(url.toLocalFile()).suffix();
        if( sfx.contains(suffix.toLower()) )
            fileList<<url.toLocalFile();
        else
            QMessageBox::information(this,tr("QMusicPlayer"),
                                     url.toString().split('/').back() + tr("not supported"));
    }
    musicImportSongsSettingPath(fileList);
}

void MusicApplication::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle01);
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

    rightClickMenu.addAction(QIcon(":/contextMenu/equalizer"),tr("Equalizer"),this,SLOT(musicSetEqualizer()));
    rightClickMenu.addAction(tr("AudioRecorder"),this,SLOT(musicAudioRecorder()));
    rightClickMenu.addAction(tr("TimingSettings"),this,SLOT(musicTimerWidget()));
    rightClickMenu.addAction(tr("ShowingSpectrum"), m_leftAreaWidget, SLOT(musicSpectrumWidget()));
    rightClickMenu.addSeparator();

    QAction *window = rightClickMenu.addAction(tr("WindowTop"),this,SLOT(musicSetWindowToTop()));
    m_setWindowToTop ? window->setIcon(QIcon(":/share/selected")) : window->setIcon(QIcon());

    rightClickMenu.addAction(QIcon(":/contextMenu/setting"),tr("Setting"),this,SLOT(musicSetting()));
    rightClickMenu.addAction(QIcon(":/contextMenu/location"),tr("musicLocation"),this,SLOT(musicCurrentPlayLocation()));

    QMenu musicInfo(tr("musicAbout"),&rightClickMenu);
    rightClickMenu.addMenu(&musicInfo)->setIcon(QIcon(":/contextMenu/about"));
    musicInfo.addAction(QIcon(":/contextMenu/about"),tr("musicAbout"),this,SLOT(musicAboutUs()));
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
        musicKey();
}

void MusicApplication::initWindowSurface()
{
    createSystemTrayIcon();
    connect(ui->musicDesktopLrc,SIGNAL(clicked(bool)), m_rightAreaWidget, SLOT(setDestopLrcVisible(bool)));
    connect(ui->musicDesktopLrc,SIGNAL(clicked()),m_systemTrayMenu,SLOT(showDesktopLrc()));
}

void MusicApplication::createSystemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(QString::fromUtf8(":/image/windowicon")));
    m_systemTray->setToolTip(tr("QMusicPlayer"));

    createMenuActions();

    m_systemTrayMenu = new MusicSystemTrayMenu(this);
    connect(m_systemTrayMenu,SIGNAL(setShowDesktopLrc(bool)), SLOT(setShowDesktopLrc(bool)));
    connect(m_systemTrayMenu,SIGNAL(setWindowLockedChanged()), m_rightAreaWidget, SLOT(setWindowLockedChanged()));
//    connect(m_musiclrcfordesktop,SIGNAL(setWindowLockedChanged(bool)),m_systemTrayMenu,SLOT(lockDesktopLrc(bool)));

    m_systemTray->setContextMenu(m_systemTrayMenu);

    ui->musicPlayMode->setMenu(&m_playModeMenu);
    ui->musicPlayMode->setPopupMode(QToolButton::InstantPopup);

    m_systemTray->show();
    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
                       SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MusicApplication::createPlayModeMenuIcon(QMenu& menu)
{
    QList<QAction*> as = menu.actions();
    MusicObject::SongPlayType songplaymode = m_musicList->playbackMode();
    (songplaymode == MusicObject::MC_PlayOrder) ? as[0]->setIcon(QIcon(":/share/selected")) : as[0]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayRandom) ? as[1]->setIcon(QIcon(":/share/selected")) : as[1]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayListLoop) ? as[2]->setIcon(QIcon(":/share/selected")) : as[2]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayOneLoop) ? as[3]->setIcon(QIcon(":/share/selected")) : as[3]->setIcon(QIcon());
    (songplaymode == MusicObject::MC_PlayOnce) ? as[4]->setIcon(QIcon(":/share/selected")) : as[4]->setIcon(QIcon());
}

void MusicApplication::createPlayModeMenu(QMenu& menu)
{
    menu.setStyleSheet(MusicUIObject::MMenuStyle01);
    menu.addAction(tr("OrderPlay"),this,SLOT(musicPlayOrder()));
    menu.addAction(tr("RandomPlay"),this,SLOT(musicPlayRandom()));
    menu.addAction(tr("ListCycle"),this,SLOT(musicPlayListLoop()));
    menu.addAction(tr("SingleCycle"),this,SLOT(musicPlayOneLoop()));
    menu.addAction(tr("PlayOnce"),this,SLOT(musicPlayItemOnce()));
    createPlayModeMenuIcon(menu);
}

void MusicApplication::createMenuActions()
{
    connect(ui->action_ImportSongs,SIGNAL(triggered()),this,SLOT(musicImportSongs()));
    connect(ui->action_Setting,SIGNAL(triggered()),this,SLOT(musicSetting()));
    connect(ui->action_Quit,SIGNAL(triggered()),this,SLOT(quitWindowClose()));
    connect(ui->action_Next,SIGNAL(triggered()),this,SLOT(musicPlayNext()));
    connect(ui->action_Play,SIGNAL(triggered()),this,SLOT(musicKey()));
    connect(ui->action_Privious,SIGNAL(triggered()),this,SLOT(musicPlayPrivious()));
    connect(ui->action_VolumeSub,SIGNAL(triggered()),this,SLOT(musicActionVolumeSub()));
    connect(ui->action_VolumePlus,SIGNAL(triggered()),this,SLOT(musicActionVolumePlus()));
    connect(ui->action_OrderPlay,SIGNAL(triggered()),this,SLOT(musicPlayOrder()));
    connect(ui->action_RandomPlay,SIGNAL(triggered()),this,SLOT(musicPlayRandom()));
    connect(ui->action_SingleCycle,SIGNAL(triggered()),this,SLOT(musicPlayOneLoop()));
    connect(ui->action_ListCycle,SIGNAL(triggered()),this,SLOT(musicPlayListLoop()));
    connect(ui->action_About,SIGNAL(triggered()),this,SLOT(musicAboutUs()));
}

void MusicApplication::readXMLConfigFromText()
{
    MusicXMLConfigManager xml;
    int value;
    QString key;
    if(!xml.readMusicXMLConfig())//open music file
        return;
    //////////////////////////////////////////////////////////////
    musicToolSetsParameter();
    //////////////////////////////////////////////////////////////
    //Path configuration song
    MStringLists names,songs;
    xml.readMusicSongsConfig(names, songs);
    m_musicSongTree->musicSongsFileNameAndPath(names, songs);

    //////////////////////////////////////////////////////////////
    if(!xml.readXMLConfig())//open file
        return;
    //Configure playback mode
    if(xml.read3DMusicPlayConfig())
    {
        musicSetPlay3DMusic();
    }
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
    M_SETTING.setValue(MusicSettingManager::VolumeChoiced, value);
    ////////////////////////musicSetting
    //////////////////////////////////////////////////////////////
    //Set the inline lrc should be shown
    key = xml.readShowInlineLrc();
    M_SETTING.setValue(MusicSettingManager::ShowInlineLrcChoiced, key);
    m_rightAreaWidget->setInlineLrcVisible(key);
    //////////////////////////////////////////////////////////////
    //Set the desktop lrc should be shown
    key = xml.readShowDesktopLrc();
    M_SETTING.setValue(MusicSettingManager::ShowDesktopLrcChoiced, key);
    m_systemTrayMenu->showDesktopLrc(key);
    m_rightAreaWidget->setDestopLrcVisible(key);
    //////////////////////////////////////////////////////////////
    //Configure automatic playback
    key = xml.readSystemAutoPlayConfig();
    M_SETTING.setValue(MusicSettingManager::AutoPlayChoiced, key);
    m_systemTrayMenu->showPlayStatus(m_playControl);
    m_rightAreaWidget->showPlayStatus(m_playControl);
    m_musicWindowExtras->showPlayStatus(m_playControl);
    if(key == "true")
    {
        musicKey();
    }
    //////////////////////////////////////////////////////////////
    //Configuration from next time also stopped at the last record.
    QStringList keyList;
    xml.readSystemLastPlayIndexConfig(keyList);
    M_SETTING.setValue(MusicSettingManager::LastPlayIndexChoiced, keyList);
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
    M_SETTING.setValue(MusicSettingManager::CloseEventChoiced, key);
    if(key == "true")
    {
        m_systemCloseConfig = true;
    }
    //////////////////////////////////////////////////////////////
    //Set the current background color
    //Set the current alpha value
    m_topAreaWidget->setParameters(xml.readBackgroundTheme(),
                                   xml.readBackgroundTransparent().toInt());
    //////////////////////////////////////////////////////////////
    xml.readTimeAutoConfig();
    //Set the lrc color the user set
    M_SETTING.setValue(MusicSettingManager::LrcColorChoiced,xml.readShowLrcColor());
    M_SETTING.setValue(MusicSettingManager::LrcFgColorChoiced,xml.readShowLrcFgColor());
    M_SETTING.setValue(MusicSettingManager::LrcBgColorChoiced,xml.readShowLrcBgColor());
    M_SETTING.setValue(MusicSettingManager::LrcFamilyChoiced,xml.readShowLrcFamily());
    M_SETTING.setValue(MusicSettingManager::LrcTypeChoiced,xml.readShowLrcType());
    M_SETTING.setValue(MusicSettingManager::LrcColorTransChoiced,xml.readShowLrcTransparent());
    M_SETTING.setValue(MusicSettingManager::LrcSizeChoiced,xml.readShowLrcSize());
    //////////////////////////////////////////////////////////////
    //Set the lrc size the user set
    M_SETTING.setValue(MusicSettingManager::DLrcColorChoiced,xml.readShowDLrcColor());
    M_SETTING.setValue(MusicSettingManager::DLrcFgColorChoiced,xml.readShowDLrcFgColor());
    M_SETTING.setValue(MusicSettingManager::DLrcBgColorChoiced,xml.readShowDLrcBgColor());
    M_SETTING.setValue(MusicSettingManager::DLrcFamilyChoiced,xml.readShowDLrcFamily());
    M_SETTING.setValue(MusicSettingManager::DLrcTypeChoiced,xml.readShowDLrcType());
    M_SETTING.setValue(MusicSettingManager::DLrcColorTransChoiced,xml.readShowDLrcTransparent());
    M_SETTING.setValue(MusicSettingManager::DLrcSizeChoiced,xml.readShowDLrcSize());
    M_SETTING.setValue(MusicSettingManager::DLrcLockedChoiced,xml.readShowDLrcLocked());
    M_SETTING.setValue(MusicSettingManager::DLrcGeometryChoiced,xml.readShowDLrcGeometry());
    m_systemTrayMenu->lockDesktopLrc(xml.readShowDLrcLocked());
    m_rightAreaWidget->setSettingParameter();
    //////////////////////////////////////////////////////////////
    M_SETTING.setValue(MusicSettingManager::EqualizerEnableChoiced,xml.readEqualizerEnale());
    M_SETTING.setValue(MusicSettingManager::EqualizerValueChoiced,xml.readEqualizerValue());
    M_SETTING.setValue(MusicSettingManager::EqualizerIndexChoiced,xml.readEqualizerIndex());
    m_musicPlayer->setEqInformation();
    //////////////////////////////////////////////////////////////
    M_SETTING.setValue(MusicSettingManager::CurrentLanIndexChoiced,xml.readLanguageIndex());

}

void MusicApplication::writeXMLConfigToText()
{
    MusicXMLConfigManager xml;
    QStringList lastPlayIndexChoiced;

    bool flag;
    m_musicPlayer->setPlay3DMusicFlag(flag);
    M_SETTING.setValue(MusicSettingManager::Play3DMusicChoiced, flag ? 1 : 0);
    M_SETTING.setValue(MusicSettingManager::PlayModeChoiced, m_musicList->playbackMode());
    M_SETTING.setValue(MusicSettingManager::VolumeChoiced,ui->musicSoundSlider->value());
    lastPlayIndexChoiced = M_SETTING.value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    lastPlayIndexChoiced.replace(1,QString::number(m_musicSongTree->getCurrentPlayToolIndex()));
    lastPlayIndexChoiced.replace(2,QString::number(m_musicList->currentIndex()));
    M_SETTING.setValue(MusicSettingManager::LastPlayIndexChoiced,lastPlayIndexChoiced);
    M_SETTING.setValue(MusicSettingManager::BgThemeChoiced, m_topAreaWidget->getBgSkin());
    M_SETTING.setValue(MusicSettingManager::BgTransparentChoiced, m_topAreaWidget->getBgSkinAlpha());
    M_SETTING.setValue(MusicSettingManager::ShowDesktopLrcChoiced, m_rightAreaWidget->getDestopLrcVisible());
    xml.writeXMLConfig();
    xml.writeMusicSongsConfig( m_musicSongTree->getMusicAllSongsFileName(),
                               m_musicSongTree->getMusicAllSongsFilePath() );
}

void MusicApplication::quitWindowClose()
{
    writeXMLConfigToText();
    //Write configuration files
    qApp->quit();
}

QString MusicApplication::musicTimeTransToLabel(qint64 time)
{
    int min = time/60000;
    time = time%60000;
    return QString("%1:%2").arg(QString::number(min).rightJustified(2,'0'))
            .arg(QString::number(time/1000).rightJustified(2,'0'));
}

void MusicApplication::positionChanged(qint64 position)
{
    UpdateCurrentLrc(position);
    ui->musicTimeWidget->setValue(position);

    if(m_musicList->isEmpty())
        ui->playCurrentTime->setText("00:00");
    else
        ui->playCurrentTime->setText(musicTimeTransToLabel(position));
    //Show the current play time
    m_musicSongTree->setTimerLabel(ui->playCurrentTime->text());
#ifdef MUSIC_DEBUG
    m_musicWindowExtras->setValue(position);
#endif
}

void MusicApplication::durationChanged(qint64 duration)
{
    //Show the current play total time
    ui->musicTimeWidget->setRange(0,duration);
    ui->playTotalTime->setText(musicTimeTransToLabel(duration));
    //Loading the current song lrc
    musicLoadCurrentSongLrc();
#ifdef MUSIC_DEBUG
    m_musicWindowExtras->setRange(0,duration);
#endif
}

void MusicApplication::showCurrentSong(int index)
{
    if( index > -1 ) //The list to end
    {
        m_musicSongTree->selectRow(index);
        QString name = m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex())[index];
        ui->showCurrentSong->setText(tr("Playing")+QString(": %1 = = %2")
            .arg(index + 1)
            .arg(name));
        //Show the current play song information
        m_downloadStatusLabel->musicCheckHasLrcAlready();
        m_systemTrayMenu->setLabelText(name);
        m_topAreaWidget->setLabelText(name);
        //display current ArtTheme pic
        M_ARTBG.setArtName(getCurrentFileName());
        m_topAreaWidget->musicBgThemeDownloadFinished();
    }
    else
    {
        ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
        m_playControl = true;
        m_musicPlayer->stop();
        m_rightAreaWidget->stopLrcMask();

        m_systemTrayMenu->showPlayStatus(m_playControl);
        m_rightAreaWidget->showPlayStatus(m_playControl);
        m_musicWindowExtras->showPlayStatus(m_playControl);
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
      return;//The playlist is not performing space-time
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
    m_systemTrayMenu->showPlayStatus(m_playControl);
    m_rightAreaWidget->showPlayStatus(m_playControl);
    m_musicWindowExtras->showPlayStatus(m_playControl);
    m_topAreaWidget->showPlayStatus(m_playControl);
    ui->musicTimeWidget->setPlayState(m_playControl);
}

void MusicApplication::musicPlayPrivious()
{
    if(m_musicList->isEmpty())
      return;//The playlist is not performing space-time
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
      return;//The playlist is not performing space-time
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
    M_SETTING.setValue(MusicSettingManager::VolumeChoiced, m_musicPlayer->volume());
}

void MusicApplication::musicVolumeChanged(int volume)
{
    m_topAreaWidget->setVolumeValue(volume);
    m_musicPlayer->setVolume(volume);
    ui->volumeValue->setText(QString("%1%").arg(volume));
    (volume > 0) ? ui->musicSound->setStyleSheet(MusicUIObject::MCustomStyle24)
                 : ui->musicSound->setStyleSheet(MusicUIObject::MCustomStyle25);
    M_SETTING.setValue(MusicSettingManager::VolumeChoiced, volume);
}

void MusicApplication::musicActionVolumeSub()
{
    int currentVol = m_musicPlayer->volume();
    currentVol -= 15;
    if( currentVol < 0)
      currentVol = 0;   //reset music volume
    ui->musicSoundSlider->setValue(currentVol);
    musicVolumeChanged( currentVol );
}

void MusicApplication::musicActionVolumePlus()
{
    int currentVol = m_musicPlayer->volume();
    currentVol += 15;
    if( currentVol > 100)
      currentVol = 100;   //reset music volume
    ui->musicSoundSlider->setValue(currentVol);
    musicVolumeChanged( currentVol );
}

void MusicApplication::musicImportSongsOnlyFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles );
    dialog.setViewMode(QFileDialog::Detail);
    QStringList varietyMusic;
    varietyMusic <<"Mp3 File(*.mp3)"<<"Mp2 File(*.mp2)"<<"Mp1 File(*.mp1)"<<"Wav File(*.wav)"
                 <<"Ogg File(*.ogg)"<<"Flac File(*.flac)"<<"Ac3 File(*.ac3)"<<"Aac File(*.aac)"
                 <<"Oga File(*.oga)"<<"Pcm File(*.pcm)";
    dialog.setNameFilters(varietyMusic);
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
               fileList<<file[i].absoluteFilePath();
        }
        musicImportSongsSettingPath(fileList);
    }
}

void MusicApplication::musicImportSongsSettingPath(const QStringList & path)
{
    m_musicSongTree->importOtherMusicSongs(path);//append in songsList
    if(m_currentMusicSongTreeIndex == 0)
    {
        m_musicList->appendMedia(path);
    }
    if(path.count() > 0 && m_musicList->currentIndex() < 0)
        m_musicList->setCurrentIndex(0);
}

void MusicApplication::musicImportSongs()
{
    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MMenuStyle01);
    menu.addAction(tr("openOnlyFiles"),this,SLOT(musicImportSongsOnlyFile()));
    menu.addAction(tr("openOnlyDir"),this,SLOT(musicImportSongsOnlyDir()));
    menu.addSeparator();
    menu.addAction(tr("dragAnddrop"))->setEnabled(false);
    menu.exec(QCursor::pos());

    ///////////Repair tray program exit problems
    if(!this->isVisible())
    {
        this->show();
        this->activateWindow();
    }
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
        int count = m_musicLocalSongSearch->getSearchedText().count();
        row = m_searchfileListCache.value(count)[row];
        m_musicLocalSongSearch->clearSearchedText();
        m_searchfileListCache.clear();
    }

    m_musicList->setCurrentIndex(row);
    m_playControl = true;
    this->musicKey();
    m_playControl = false;
}

void MusicApplication::musicPlayAnyTimeAt(int posValue)
{
    //Drag the progress indicator to rewind or fast-forward through the current song
    m_musicPlayer->setPosition( posValue);
    //Set lrc corrent to show
    m_rightAreaWidget->setSongSpeedAndSlow(posValue);
}

void MusicApplication::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
      case QSystemTrayIcon::DoubleClick:
          break;
      case QSystemTrayIcon::Trigger:
          if(!this->isVisible())
          {
              this->show();
              this->activateWindow();
          }
          break;
      default:
          break;
    }
}

void MusicApplication::musicAboutUs()
{
    QMessageBox::about(this, tr("About QMusicPlayer"),
       tr("QMusicPlayer") + QString("\n\n") +
       tr("Directed By Greedysky")+
       QString("\nCopyright© 2014-2015")+
       QString("\nMail:Greedysky@163.com"));

    ///////////Repair tray program exit problems
    if(!this->isVisible())
    {
        this->show();
        this->activateWindow();
    }
}

void MusicApplication::musicSetting()
{
    m_setting->initControllerParameter();
    m_setting->exec();

    ///////////Repair tray program exit problems
    if(!this->isVisible())
    {
        this->show();
        this->activateWindow();
    }
}

void MusicApplication::musicSearch()
{
    if(m_musicList->isEmpty())
      return;

    if(m_musicLocalSongSearch == NULL)
      m_musicLocalSongSearch = new MusicLocalSongSearch(this);
    m_musicLocalSongSearch->exec();
}

void MusicApplication::musicCurrentPlayLocation()
{
    if(m_musicList->isEmpty())
      return;
    /*Repair when already in the original row, paging,
     cannot return to the original row */
    m_musicSongTree->selectRow(0);
    m_musicSongTree->selectRow(m_musicList->currentIndex());
}

void MusicApplication::setDeleteItemAt(const MIntList &index)
{
    for(int i=index.count() - 1; i>=0; --i)
    {
        m_musicList->removeMedia(index[i]);
    }
    int oldIndex = index[0];
    if( oldIndex == m_musicList->mediaCount()/*|| oldIndex == 0*/)  //Play index error correction
        --oldIndex;
    m_musicList->setCurrentIndex(oldIndex);
    //The corresponding item is deleted from the QMediaPlaylist
    m_playControl = true;
    this->musicKey();
    m_playControl = false;
//    showCurrentSong(oldIndex);
}

void MusicApplication::getParameterSetting()
{
    m_rightAreaWidget->getParameterSetting();
    m_systemCloseConfig = M_SETTING.value(MusicSettingManager::CloseEventChoiced).toBool();
    bool showDeskLrc = M_SETTING.value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
    m_systemTrayMenu->showDesktopLrc(showDeskLrc ? "true" : "false" );
    //This attribute is effective immediately.
}

void MusicApplication::musicSearchIndexChanged(int,int index)
{
    MIntList searchResult;
    QStringList searchedSongs(m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex()));
    for(int j=0; j<searchedSongs.count(); ++j)
      if(searchedSongs[j].contains(m_musicLocalSongSearch->getSearchedText(),Qt::CaseInsensitive))
      {
        searchResult<<j;
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

void MusicApplication::UpdateCurrentLrc(qint64 time)
{
    m_rightAreaWidget->updateCurrentLrc(time, m_musicPlayer->duration(), m_playControl);
}

void MusicApplication::musicSearchButtonSearched()
{
    QString searchedQString = ui->musicSongSearchLine->text().trimmed();
    //The string searched wouldn't allow to be none
    if( !searchedQString.isEmpty())
    {
        createVedioWidget(false);
        ui->SurfaceStackedWidget->setCurrentIndex(1);
        m_topAreaWidget->musicBgTransparentChanged();
        ui->songSearchWidget->startSearchQuery(searchedQString);
    }
}

void MusicApplication::musicIndexWidgetButtonSearched()
{
    createVedioWidget(false);
    //Show the first index of widget
    ui->SurfaceStackedWidget->setCurrentIndex(0);
    m_topAreaWidget->musicBgTransparentChanged();
}

void MusicApplication::musicSearchWidgetButtonSearched()
{
    createVedioWidget(false);
    //Show searched song lists
    ui->SurfaceStackedWidget->setCurrentIndex(1);
    m_topAreaWidget->musicBgTransparentChanged();
}

void MusicApplication::musicLrcWidgetButtonSearched()
{
    createVedioWidget(false);
    //Show lrc display widget
    ui->SurfaceStackedWidget->setCurrentIndex(2);
    m_topAreaWidget->musicBgThemeDownloadFinished();
}

void MusicApplication::musicSearchRefreshButtonRefreshed()
{
    createVedioWidget(false);
    //Refresh the search music song
    musicSearchButtonSearched();
}

void MusicApplication::createVedioWidget(bool create)
{
    if(create)
    {
        delete m_videoPlayer;
        m_videoPlayer = new MusicVideoPlayer;
        ui->SurfaceStackedWidget->addWidget(m_videoPlayer);
        if(!m_playControl) musicKey();
    }
    else
    {
        if(m_videoPlayer)
        {
            ui->SurfaceStackedWidget->removeWidget(m_videoPlayer);
            delete m_videoPlayer;
            m_videoPlayer = NULL;
        }
    }
}

void MusicApplication::musicVedioWidgetButtonSearched()
{
    createVedioWidget(true);
    ui->SurfaceStackedWidget->setCurrentIndex(3);
    m_topAreaWidget->musicBgTransparentChanged();
}

void MusicApplication::musicVedioWidgetButtonDoubleClicked()
{
//    ui->SurfaceStackedWidget->removeWidget(m_videoPlayer);
//    m_videoPlayer->showFullScreen();
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

//bool MusicApplication::checkSettingParameterValue() const
//{
//    return ( M_SETTING.value(MusicSettingManager::ShowInlineLrcChoiced).toBool() ||
//             M_SETTING.value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() ) ? true : false;
//}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return (m_musicList->currentIndex() == -1) ? true : false;
}

QLabel *&MusicApplication::getshowDownloadLabel()
{
    return ui->showDownloadGif;
}

void MusicApplication::musicCurrentLrcUpdated()
{
    QString filename = getCurrentFileName();
    QFile file(LRC_DOWNLOAD + filename + LRC_FILE);
    if(file.exists()) file.remove();

    m_downloadStatusLabel->musicCheckHasLrcAlready();
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
    int index = static_cast<MusicSongsListWidget*>(m_musicSongTree->currentWidget())->currentRow();
    if(m_musicList->isEmpty() || index < 0 || m_currentMusicSongTreeIndex != m_musicSongTree->currentIndex() )
        return;

    m_musicSongTree->addMusicSongToLovestListAt(index);
}

void MusicApplication::musicSetWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    if(m_setWindowToTop)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    show();
}

void MusicApplication::musicSetEqualizer()
{
    MusicEqualizerDialog eq(this);
    connect(&eq,SIGNAL(setEqEffect(MIntList)),m_musicPlayer,SLOT(setEqEffect(MIntList)));
    connect(&eq,SIGNAL(setEnaleEffect(bool)),m_musicPlayer,SLOT(setEnaleEffect(bool)));
    connect(&eq,SIGNAL(setSpEqEffect(MusicObject::SpecialEQ)),m_musicPlayer,
                SLOT(setSpEqEffect(MusicObject::SpecialEQ)));
    connect(&eq,SIGNAL(resetEqEffect()),m_musicPlayer,SLOT(resetEqEffect()));
    eq.exec();
}

void MusicApplication::setShowDesktopLrc(bool show)
{
    m_rightAreaWidget->setDestopLrcVisible(show);
    M_SETTING.setValue(MusicSettingManager::ShowDesktopLrcChoiced, show);
}

void MusicApplication::desktopLrcClosed()
{
    ui->musicDesktopLrc->setChecked(false);
    m_systemTrayMenu->showDesktopLrc("false");
    M_SETTING.setValue(MusicSettingManager::ShowDesktopLrcChoiced, false);
}

void MusicApplication::musicAudioRecorder()
{
    MusicAudioRecorderWidget recorder(this);
    recorder.exec();
}

void MusicApplication::musicTimerWidget()
{
    MusicTimerWidget timer(this);
    timer.setSongStringList(m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex()));
    connect(&timer,SIGNAL(timerParameterChanged()),SLOT(musicToolSetsParameter()));
    timer.exec();
}

void MusicApplication::setPlaySongChanged(int index)
{
    if(m_musicList->isEmpty() || index <0 || index >= m_musicList->mediaCount())
        return;
    musicPlayIndex(index,0);
}

void MusicApplication::setStopSongChanged()
{
    m_musicPlayer->pause();
}

void MusicApplication::musicToolSetsParameter()
{
    m_musicTimerAutoObj->runTimerAutoConfig();
}

void MusicApplication::addSongToPlayList(const QStringList &item)
{
    musicImportSongsSettingPath(item);
    musicPlayIndex(m_musicList->mediaCount() - 1, 0);
}

void MusicApplication::musicSetPlay3DMusic()
{
    bool flag;
    m_musicPlayer->setPlay3DMusicFlag(flag);
    ui->music3DPlayButton->setIcon(QIcon(QString::fromUtf8(flag ? ":/equalizer/3doff" : ":/equalizer/3don")));
}

void MusicApplication::drawWindowRoundedRect()
{
    QBitmap bmp( size() );
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
}

void MusicApplication::musicWindowConciseChanged()
{
    bool con = m_musicWindowExtras->isDisableBlurBehindWindow();
    resize( con ? 380 : 950, height());
    ui->musicWindowConcise->setGeometry(con ? 295 : 828, 7, 25, 25);
    ui->minimization->setGeometry(con ? 325 : 889, 7, 25, 25);
    ui->windowClose->setGeometry(con ? 350 : 917, 7, 25, 25);
    m_musicWindowExtras->disableBlurBehindWindow( !con );
    ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(con ? ":/image/conciseout" : ":/image/concisein")));
    drawWindowRoundedRect();
    m_topAreaWidget->musicBgThemeDownloadFinished();
}

void MusicApplication::setSpectrum(HWND wnd, int w, int h)
{
    m_musicPlayer->setSpectrum(wnd, w, h);
}

void MusicApplication::getCurrentPlayList(QStringList& list)
{
    list = m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex());
}

void MusicApplication::changeDesktopLrcWidget()
{
    m_setting->changeDesktopLrcWidget();
}

void MusicApplication::changeInlineLrcWidget()
{
    m_setting->changeInlineLrcWidget();
}

void MusicApplication::lockDesktopLrc(bool lock)
{
    m_systemTrayMenu->lockDesktopLrc(lock);
}
