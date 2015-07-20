#include "musicapplication.h"
#include "ui_musicapplication.h"
#include "musicbackgroundskindialog.h"
#include "musicequalizerdialog.h"
#include "musicsongsearchonlinewidget.h"
#include "musicsongsListwidget.h"
#include "musicsettingwidget.h"
#include "musicsystemtraymenu.h"
#include "musictimersliderwidget.h"
#include "musicwindowextras.h"
#include "localsearch/musiclocalsongsearch.h"
#include "lrcmanager/musiclrccontainerfordesktop.h"
#include "remotewidget/musicremotewidgetforsquare.h"
#include "remotewidget/musicremotewidgetforrectangle.h"
#include "remotewidget/musicremotewidgetfordiamond.h"
#include "remotewidget/musicremotewidgetforcircle.h"
#include "toolsetswidget/musicaudiorecorderwidget.h"
#include "toolsetswidget/musictimerwidget.h"
#include "toolsetswidget/musicmobiledeviceswidget.h"
#include "toolsetswidget/musicspectrumwidget.h"
#include "toolswidget/musictoolsetswidget.h"
#include "toolswidget/musicsongssummarizied.h"
#include "toolswidget/musicwebradiolistview.h"
#include "toolswidget/musicmydownloadrecordwidget.h"
#include "core/musicdownloadmanagerthread.h"
#include "core/musicdownloadstatuslabel.h"
#include "core/musiclrcdownloadthread.h"
#include "core/musicsongdownloadthread.h"
#include "core/musicxmlconfigmanager.h"
#include "core/musicplayer.h"
#include "core/musicplaylist.h"
#include "core/musictimerautoobject.h"
#include "usermanager/musicuserwindow.h"
#include "videokits/musicvideoplayer.h"
#include <Dbt.h>

MusicApplication::MusicApplication(QWidget *parent) :
    MusicMoveWidgetAbstract(parent),
    ui(new Ui::MusicApplication),
    m_setting(NULL),m_musicbgskin(NULL),m_musicLocalSongSearch(NULL),
    m_musicRemoteWidget(NULL),m_stackedWidget(NULL),m_mobileDevices(NULL),
    m_videoPlayer(NULL),m_musicSpectrumWidget(NULL)
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
    m_musiclrcfordesktop = new MusicLrcContainerForDesktop(this);
    m_musicPlayer = new MusicPlayer(this);
    m_musicList = new MusicPlaylist(this);
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
    m_pictureCarouselIndex = 0;
    connect(&m_pictureCarouselTimer,SIGNAL(timeout()),SLOT(musicBackgroundChanged()));

    m_musicSongTree = new MusicSongsSummarizied(this);
    ui->songsContainer->addWidget(m_musicSongTree);

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
    connect(ui->songSearchWidget,SIGNAL(musicBgDownloadFinished()),SLOT(musicBgThemeDownloadFinished()));
    connect(m_downloadStatusLabel,SIGNAL(musicBgDownloadFinished()),SLOT(musicBgThemeDownloadFinished()));

    connect(ui->musiclrccontainerforinline,SIGNAL(theCurrentLrcUpdated()),SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline,SIGNAL(theArtBgHasChanged()),SLOT(musicBgThemeDownloadFinished()));
    connect(m_musiclrcfordesktop,SIGNAL(theCurrentLrcUpdated()),SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline,SIGNAL(changeCurrentLrcColorSetting()),SLOT(musicSetting()));
    connect(m_musiclrcfordesktop,SIGNAL(changeCurrentLrcColorSetting()),SLOT(musicSetting()));
    connect(m_musiclrcfordesktop,SIGNAL(desktopLrcClosed()),SLOT(desktopLrcClosed()));
    connect(ui->musiclrccontainerforinline,SIGNAL(updateCurrentTime(qint64)),SLOT(updateCurrentTime(qint64)));

    m_setting = new MusicSettingWidget(this);
    connect(m_setting,SIGNAL(parameterSettingChanged()),SLOT(getParameterSetting()));
    connect(ui->musiclrccontainerforinline,SIGNAL(changeCurrentLrcColorCustom()),m_setting,SLOT(changeInlineLrcWidget()));
    connect(m_musiclrcfordesktop,SIGNAL(changeCurrentLrcColorCustom()),m_setting,SLOT(changeDesktopLrcWidget()));

    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
    connect(m_musicTimerAutoObj,SIGNAL(setPlaySong(int)),SLOT(setPlaySongChanged(int)));
    connect(m_musicTimerAutoObj,SIGNAL(setStopSong()),SLOT(setStopSongChanged()));

    m_musicWindowExtras = new MusicWindowExtras(this);

    ui->SurfaceStackedWidget->setCurrentIndex(0);
    this->readXMLConfigFromText();

    ui->userWindow->addWidget(m_msuicUserWindow = new MusicUserWindow(this));
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
    delete m_musicSpectrumWidget;
    delete m_musicWindowExtras;
    delete m_videoPlayer;
    delete m_mobileDevices;
    delete m_animation;
    delete m_stackedWidget;
    delete m_musicRemoteWidget;
    delete m_msuicUserWindow;
    delete m_downloadStatusLabel;
    delete m_musicbgskin;
    delete m_musicPlayer;
    delete m_musicList;
    delete m_musicSongTree;
    delete m_systemTray;
    delete m_systemTrayMenu;
    delete m_setting;
    delete m_musiclrcfordesktop;
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
    rightClickMenu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    rightClickMenu.addAction(QIcon(":/contextMenu/login"),tr("logout"),m_msuicUserWindow,SLOT(musicUserContextLogin()));
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
    musicRemoteControl.addAction(tr("SquareRemote"),this,SLOT(musicSquareRemote()));
    musicRemoteControl.addAction(tr("RectangleRemote"),this,SLOT(musicRectangleRemote()));
    musicRemoteControl.addAction(tr("DiamondRemote"),this,SLOT(musicDiamondRemote()));
    musicRemoteControl.addAction(tr("CircleRemote"),this,SLOT(musicCircleRemote()));
    musicRemoteControl.addAction(tr("DeleteRemote"),this,SLOT(musicDeleteRemote()));

    rightClickMenu.addAction(QIcon(":/contextMenu/equalizer"),tr("Equalizer"),this,SLOT(musicSetEqualizer()));
    rightClickMenu.addAction(tr("AudioRecorder"),this,SLOT(musicAudioRecorder()));
    rightClickMenu.addAction(tr("TimingSettings"),this,SLOT(musicTimerWidget()));
    rightClickMenu.addAction(tr("ShowingSpectrum"),this,SLOT(musicSpectrumWidget()));
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
    setMenuBarButton();

    setButtonTips();
    setButtonCursor();

    connect(ui->minimization,SIGNAL(clicked()),this,SLOT(hide()));
    connect(ui->windowClose,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->musicKey,SIGNAL(clicked()),this,SLOT(musicKey()));
    connect(ui->musicPrivious,SIGNAL(clicked()),this,SLOT(musicPlayPrivious()));
    connect(ui->musicNext,SIGNAL(clicked()),this,SLOT(musicPlayNext()));
    connect(ui->musicSound,SIGNAL(clicked()),this,SLOT(musicVolumeNULL()));
    connect(ui->musicSoundSlider,SIGNAL(valueChanged(int)),this,SLOT(musicVolumeChanged(int)));
    connect(ui->musicImport,SIGNAL(clicked()),this,SLOT(musicImportSongs()));
    connect(ui->musicSetting,SIGNAL(clicked()),this,SLOT(musicSetting()));
    connect(ui->musicSearch,SIGNAL(clicked()),this,SLOT(musicSearch()));
    connect(ui->musicCurrentLocation,SIGNAL(clicked()),this,SLOT(musicCurrentPlayLocation()));
    connect(ui->musicSearchButton,SIGNAL(clicked()),this,SLOT(musicSearchButtonSearched()));
    connect(ui->musicIndexWidgetButton,SIGNAL(clicked()),this,SLOT(musicIndexWidgetButtonSearched()));
    connect(ui->musicSearchWidgetButton,SIGNAL(clicked()),this,SLOT(musicSearchWidgetButtonSearched()));
    connect(ui->musicLrcWidgetButton,SIGNAL(clicked()),this,SLOT(musicLrcWidgetButtonSearched()));
    connect(ui->musicSearchRefreshButton,SIGNAL(clicked()),SLOT(musicSearchRefreshButtonRefreshed()));
    connect(ui->vedioWidgetButton,SIGNAL(clicked()),SLOT(musicVedioWidgetButtonSearched()));

    connect(ui->musicWindowChangeSkin,SIGNAL(clicked()),this,SLOT(musicShowSkinChangedWindow()));
    connect(ui->musicBestLove,SIGNAL(clicked()),this,SLOT(musicAddSongToLovestListAt()));
    connect(ui->musicDesktopLrc,SIGNAL(clicked(bool)),m_musiclrcfordesktop,SLOT(setVisible(bool)));
    connect(ui->musicDesktopLrc,SIGNAL(clicked()),m_systemTrayMenu,SLOT(showDesktopLrc()));
    connect(ui->music3DPlayButton,SIGNAL(clicked()),SLOT(musicSetPlay3DMusic()));
    connect(ui->musicWindowRemote,SIGNAL(clicked()),SLOT(musicSquareRemote()));
    connect(ui->musicWindowConcise,SIGNAL(clicked()),SLOT(musicWindowConciseChanged()));

    connect(ui->musicButton_playlist,SIGNAL(clicked()),SLOT(musicStackedSongListWidgetChanged()));
    connect(ui->musicButton_tools,SIGNAL(clicked()),SLOT(musicStackedToolsWidgetChanged()));
    connect(ui->musicButton_radio,SIGNAL(clicked()),SLOT(musicStackedRadioWidgetChanged()));
    connect(ui->musicButton_mydownl,SIGNAL(clicked()),SLOT(musicStackedMyDownWidgetChanged()));
}

void MusicApplication::setMenuBarButton()
{
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    ui->minimization->setIcon(QIcon(minPix.scaled(25,25)));
    ui->windowClose->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/close")).scaled(25,25)));

    ui->musicSetting->setIcon(QIcon(QString::fromUtf8(":/image/setting")));
    ui->musicImport->setIcon(QIcon(QString::fromUtf8(":/image/import")));
    ui->musicSearch->setIcon(QIcon(QString::fromUtf8(":/image/search")));
    ui->musicCurrentLocation->setIcon(QIcon(QString::fromUtf8(":/image/location")));
    ui->musicPrivious->setIcon(QIcon(QString::fromUtf8(":/image/privious")));
    ui->musicNext->setIcon(QIcon(QString::fromUtf8(":/image/next")));
    ui->menuSetting->setIcon(QIcon(QString::fromUtf8(":/image/menu")));
    ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
    ui->musicWindowChangeSkin->setIcon(QIcon(QString::fromUtf8(":/image/windowskin")));
    ui->musicWindowRemote->setIcon(QIcon(QString::fromUtf8(":/image/windowremote")));
    ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(":/image/concisein")));
    ui->musicBestLove->setIcon(QIcon(QString::fromUtf8(":/image/bestlove")));
    ui->music3DPlayButton->setIcon(QIcon(QString::fromUtf8(":/equalizer/3doff")));
    ui->musicButton_cloud->setIcon(QIcon(QString::fromUtf8(":/image/buttoncloud")));
    ui->musicButton_mydownl->setIcon(QIcon(QString::fromUtf8(":/image/buttonmydownl")));
    ui->musicButton_playlist->setIcon(QIcon(QString::fromUtf8(":/image/buttonplaylist")));
    ui->musicButton_radio->setIcon(QIcon(QString::fromUtf8(":/image/buttonradio")));
    ui->musicButton_tools->setIcon(QIcon(QString::fromUtf8(":/image/buttontools")));
    ui->musicSearchRefreshButton->setIcon(QIcon(QString::fromUtf8(":/image/flash")));

    ui->musicWindowChangeSkin->setIconSize(QSize(22,22));
    ui->musicSetting->setIconSize(QSize(40,40));
    ui->musicImport->setIconSize(QSize(40,40));
    ui->musicSearch->setIconSize(QSize(40,40));
    ui->musicCurrentLocation->setIconSize(QSize(40,40));
    ui->musicPrivious->setIconSize(QSize(45,45));
    ui->musicNext->setIconSize(QSize(45,45));
    ui->menuSetting->setIconSize(QSize(50,50));
    ui->musicKey->setIconSize(QSize(45,45));
    ui->musicBestLove->setIconSize(QSize(25,25));
    ui->music3DPlayButton->setIconSize(QSize(40,20));
    ui->musicButton_cloud->setIconSize(QSize(35,35));
    ui->musicButton_mydownl->setIconSize(QSize(35,35));
    ui->musicButton_playlist->setIconSize(QSize(35,35));
    ui->musicButton_radio->setIconSize(QSize(35,35));
    ui->musicButton_tools->setIconSize(QSize(35,35));
    ui->musicSearchRefreshButton->setIconSize(QSize(25,25));
    ui->musicPlayMode->setIconSize(QSize(25,25));

    ui->musicSoundSlider->setStyleSheet(MusicObject::MusicVolumeStyleHorizontal);
    ui->minimization->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->windowClose->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicSetting->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicImport->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicSearch->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicCurrentLocation->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicPrivious->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicNext->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->menuSetting->setStyleSheet(MusicObject::QToolButtonMenuPopStyle);
    ui->musicKey->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicWindowChangeSkin->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicWindowConcise->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicIndexWidgetButton->setStyleSheet(MusicObject::MusicMainFunctionButtonForStackWidget);
    ui->musicSearchWidgetButton->setStyleSheet(MusicObject::MusicMainFunctionButtonForStackWidget);
    ui->musicLrcWidgetButton->setStyleSheet(MusicObject::MusicMainFunctionButtonForStackWidget);
    ui->musicSearchRefreshButton->setStyleSheet(MusicObject::MusicMainFunctionButtonForStackWidget);
    ui->vedioWidgetButton->setStyleSheet(MusicObject::MusicMainFunctionButtonForStackWidget);
    ui->musicBestLove->setStyleSheet(MusicObject::MusicBestLoveToolButtonStyle);
    ui->musicDesktopLrc->setStyleSheet(MusicObject::MusicSettingCheckButton);
    ui->music3DPlayButton->setStyleSheet(MusicObject::MusicBestLoveToolButtonStyle);
    ui->musicWindowRemote->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicButton_cloud->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicButton_mydownl->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicButton_playlist->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicButton_radio->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicButton_tools->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicPlayMode->setStyleSheet(MusicObject::QToolButtonMenuPopStyle);
}

void MusicApplication::setButtonCursor()
{
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->menuSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicPrivious->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicKey->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicNext->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSound->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicImport->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearch->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicCurrentLocation->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicIndexWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicLrcWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchRefreshButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->vedioWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSoundSlider->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicBestLove->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicPlayMode->setCursor(QCursor(Qt::PointingHandCursor));
}

void MusicApplication::setButtonTips()
{
    ui->minimization->setToolTip(tr("Minimization"));
    ui->windowClose->setToolTip(tr("Close"));
    ui->menuSetting->setToolTip(tr("Menu"));
    ui->musicKey->setToolTip(tr("Play"));
    ui->musicPrivious->setToolTip(tr("Privious"));
    ui->musicNext->setToolTip(tr("Next"));
    ui->musicImport->setToolTip(tr("Import"));
    ui->musicSetting->setToolTip(tr("Setting"));
    ui->musicSearch->setToolTip(tr("musicSearch"));
    ui->musicCurrentLocation->setToolTip(tr("musicLocation"));
    ui->musicWindowChangeSkin->setToolTip(tr("changeskin"));
    ui->musicWindowConcise->setToolTip(tr("concisein/out"));
    ui->musicWindowRemote->setToolTip(tr("remoteWindow"));
    ui->musicBestLove->setToolTip(tr("bestlove"));
    ui->musicButton_cloud->setToolTip(tr("musicCloud"));
    ui->musicButton_mydownl->setToolTip(tr("musicMydownl"));
    ui->musicButton_playlist->setToolTip(tr("musicPlaylist"));
    ui->musicButton_radio->setToolTip(tr("musicRadio"));
    ui->musicButton_tools->setToolTip(tr("musicTools"));
}

void MusicApplication::createSystemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(QString::fromUtf8(":/image/windowicon")));
    m_systemTray->setToolTip(tr("QMusicPlayer"));

    createMenuActions();
    createToolPopupMenu();

    m_systemTrayMenu = new MusicSystemTrayMenu(this);
    connect(m_systemTrayMenu,SIGNAL(setShowDesktopLrc(bool)),SLOT(setShowDesktopLrc(bool)));
    connect(m_systemTrayMenu,SIGNAL(setWindowLockedChanged()),m_musiclrcfordesktop,SLOT(setWindowLockedChanged()));
    connect(m_musiclrcfordesktop,SIGNAL(setWindowLockedChanged(bool)),m_systemTrayMenu,SLOT(lockDesktopLrc(bool)));

    m_systemTray->setContextMenu(m_systemTrayMenu);
    ui->menuSetting->setMenu(&m_toolPopupMenu);

    ui->musicPlayMode->setMenu(&m_playModeMenu);
    ui->musicPlayMode->setPopupMode(QToolButton::InstantPopup);

    m_systemTray->show();
    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
                       SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MusicApplication::createToolPopupMenu()
{
    m_toolPopupMenu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    m_toolPopupMenu.addAction(ui->action_ImportSongs);
    m_toolPopupMenu.addAction(ui->action_Setting);

    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(ui->action_Privious);
    m_toolPopupMenu.addAction(ui->action_Play);
    m_toolPopupMenu.addAction(ui->action_Next);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(ui->action_OrderPlay);
    m_toolPopupMenu.addAction(ui->action_RandomPlay);
    m_toolPopupMenu.addAction(ui->action_SingleCycle);
    m_toolPopupMenu.addAction(ui->action_ListCycle);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(ui->action_VolumeSub);
    m_toolPopupMenu.addAction(ui->action_VolumePlus);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(ui->action_About);
    m_toolPopupMenu.addAction(ui->action_Quit);
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
    menu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
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
    ////////////////////////musicSetting
    //////////////////////////////////////////////////////////////
    //Set the inline lrc should be shown
    key = xml.readShowInlineLrc();
    m_musicSettingParameter.setValue("SHOWINLINELRCCHOICED",key);
    if(key == "true")
      ui->musiclrccontainerforinline->show();
    else
      ui->musiclrccontainerforinline->close();
    //////////////////////////////////////////////////////////////
    //Set the desktop lrc should be shown
    key = xml.readShowDesktopLrc();
    m_musicSettingParameter.setValue("SHOWDESKTOPLRCCHOICED",key);
    m_systemTrayMenu->showDesktopLrc(key);
    if(key == "true")
    {
      ui->musicDesktopLrc->setChecked(true);
      m_musiclrcfordesktop->show();
    }
    else
    {
      ui->musicDesktopLrc->setChecked(false);
      m_musiclrcfordesktop->close();
    }
    //////////////////////////////////////////////////////////////
    //Configure automatic playback
    key = xml.readSystemAutoPlayConfig();
    m_musicSettingParameter.setValue("AUTOPLAYCHOICED",key);
    m_systemTrayMenu->showPlayStatus(m_playControl);
    m_musiclrcfordesktop->showPlayStatus(m_playControl);
    m_musicWindowExtras->showPlayStatus(m_playControl);
    if(key == "true")
    {
        musicKey();
    }
    //////////////////////////////////////////////////////////////
    //Configuration from next time also stopped at the last record.
    QStringList keyList;
    xml.readSystemLastPlayIndexConfig(keyList);
    m_musicSettingParameter.setValue("LASTPLAYINDEXCHOICED", keyList);
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
    m_musicSettingParameter.setValue("CLOSEEVENTCHOICED",key);
    if(key == "true")
    {
        m_systemCloseConfig = true;
    }
    //////////////////////////////////////////////////////////////
    //Set the current background color
    m_currentBgSkin = xml.readBackgroundTheme();
    //////////////////////////////////////////////////////////////
    //Set the current alpha value
    m_alpha = xml.readBackgroundTransparent().toInt();
    this->musicBgTransparentChanged(m_alpha);
    //////////////////////////////////////////////////////////////
    xml.readTimeAutoConfig();
    //Set the lrc color the user set
    m_musicSettingParameter.setValue("LRCCOLORCHOICED",xml.readShowLrcColor());
    m_musicSettingParameter.setValue("LRCFGCOLORCHOICED",xml.readShowLrcFgColor());
    m_musicSettingParameter.setValue("LRCBGCOLORCHOICED",xml.readShowLrcBgColor());
    m_musicSettingParameter.setValue("LRCFAMILYCHOICED",xml.readShowLrcFamily());
    m_musicSettingParameter.setValue("LRCTYPECHOICED",xml.readShowLrcType());
    m_musicSettingParameter.setValue("LRCCOLORTRANSCHOICED",xml.readShowLrcTransparent());
    m_musicSettingParameter.setValue("LRCSIZECHOICED",xml.readShowLrcSize());
    ui->musiclrccontainerforinline->setSettingParameter();
    //////////////////////////////////////////////////////////////
    //Set the lrc size the user set
    m_musicSettingParameter.setValue("DLRCCOLORCHOICED",xml.readShowDLrcColor());
    m_musicSettingParameter.setValue("DLRCFGCOLORCHOICED",xml.readShowDLrcFgColor());
    m_musicSettingParameter.setValue("DLRCBGCOLORCHOICED",xml.readShowDLrcBgColor());
    m_musicSettingParameter.setValue("DLRCFAMILYCHOICED",xml.readShowDLrcFamily());
    m_musicSettingParameter.setValue("DLRCTYPECHOICED",xml.readShowDLrcType());
    m_musicSettingParameter.setValue("DLRCCOLORTRANSCHOICED",xml.readShowDLrcTransparent());
    m_musicSettingParameter.setValue("DLRCSIZECHOICED",xml.readShowDLrcSize());
    m_musicSettingParameter.setValue("DLRCLOCKEDCHOICED",xml.readShowDLrcLocked());
    m_musicSettingParameter.setValue("DLRCGEOMETRYCHOICED",xml.readShowDLrcGeometry());
    m_systemTrayMenu->lockDesktopLrc(xml.readShowDLrcLocked());
    m_musiclrcfordesktop->setSettingParameter();
    //////////////////////////////////////////////////////////////
    m_musicSettingParameter.setValue("EQUALIZERENABLECHOICED",xml.readEqualizerEnale());
    m_musicSettingParameter.setValue("EQUALIZERVALUECHOICED",xml.readEqualizerValue());
    m_musicSettingParameter.setValue("EQUALIZERINDEXCHOICED",xml.readEqualizerIndex());
    m_musicPlayer->setEqInformation();
    //////////////////////////////////////////////////////////////
    m_musicSettingParameter.setValue("CURRENTLANINDEXCHOICED",xml.readLanguageIndex());

}

void MusicApplication::writeXMLConfigToText()
{
    MusicXMLConfigManager xml;
    QStringList lastPlayIndexChoiced;

    bool flag;
    m_musicPlayer->setPlay3DMusicFlag(flag);
    m_musicSettingParameter.setValue("PLAY3DMUSICCHOICED",flag ? 1 : 0);
    m_musicSettingParameter.setValue("PLAYMODECHOICED", m_musicList->playbackMode());
    m_musicSettingParameter.setValue("VOLUMECHOICED",ui->musicSoundSlider->value());
    lastPlayIndexChoiced = m_musicSettingParameter.value("LASTPLAYINDEXCHOICED").toStringList();
    lastPlayIndexChoiced.replace(1,QString::number(m_musicSongTree->getCurrentPlayToolIndex()));
    lastPlayIndexChoiced.replace(2,QString::number(m_musicList->currentIndex()));
    m_musicSettingParameter.setValue("LASTPLAYINDEXCHOICED",lastPlayIndexChoiced);
    m_musicSettingParameter.setValue("BGTHEMECHOICED",m_currentBgSkin);
    m_musicSettingParameter.setValue("BGTRANSPARENTCHOICED",m_alpha);
    m_musicSettingParameter.setValue("SHOWDESKTOPLRCCHOICED",m_musiclrcfordesktop->isVisible());
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
        if(m_musicRemoteWidget)
            m_musicRemoteWidget->setLabelText(name);
        //display current ArtTheme pic
        musicBgThemeDownloadFinished();
    }
    else
    {
        ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
        m_playControl = true;
        m_musicPlayer->stop();
        if( m_musicSettingParameter.value("SHOWINLINELRCCHOICED").toBool() ||
            m_musicSettingParameter.value("SHOWDESKTOPLRCCHOICED").toBool() )
        {
           ui->musiclrccontainerforinline->stopLrcMask();
           m_musiclrcfordesktop->stopLrcMask();
        }
        m_systemTrayMenu->showPlayStatus(m_playControl);
        m_musiclrcfordesktop->showPlayStatus(m_playControl);
        m_musicWindowExtras->showPlayStatus(m_playControl);
        if(m_musicRemoteWidget) m_musicRemoteWidget->showPlayStatus(m_playControl);
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
        musicBgThemeDownloadFinished();
        if( m_musicSettingParameter.value("SHOWINLINELRCCHOICED").toBool() ||
          m_musicSettingParameter.value("SHOWDESKTOPLRCCHOICED").toBool() )
        {
            ui->musiclrccontainerforinline->startTimerClock();
            m_musiclrcfordesktop->startTimerClock();
        }
    }
    else
    {
        ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
        m_playControl = true;
        m_musicPlayer->pause();
        m_pictureCarouselTimer.stop();
        if( m_musicSettingParameter.value("SHOWINLINELRCCHOICED").toBool() ||
          m_musicSettingParameter.value("SHOWDESKTOPLRCCHOICED").toBool() )
        {
            ui->musiclrccontainerforinline->stopLrcMask();
            m_musiclrcfordesktop->stopLrcMask();
        }
    }
    m_systemTrayMenu->showPlayStatus(m_playControl);
    m_musiclrcfordesktop->showPlayStatus(m_playControl);
    m_musicWindowExtras->showPlayStatus(m_playControl);
    if(m_musicRemoteWidget) m_musicRemoteWidget->showPlayStatus(m_playControl);
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
        ui->musicSound->setStyleSheet("border-image: url(:/image/soundmin);");
    }
    else
    {
        m_musicPlayer->setMuted(false);
        ui->musicSound->setStyleSheet("border-image: url(:/image/soundmax);");
    }
    ui->musicSoundSlider->blockSignals(true);
    ui->musicSoundSlider->setValue(m_musicPlayer->volume());
    ui->musicSoundSlider->blockSignals(false);
    ui->volumeValue->setText(QString("%1%").arg(m_musicPlayer->volume()));
    QSettings().setValue("VOLUMECHOICED",m_musicPlayer->volume());
}

void MusicApplication::musicVolumeChanged(int volume)
{
    if(m_musicRemoteWidget)
        m_musicRemoteWidget->setVolumeValue(volume);
    m_musicPlayer->setVolume(volume);
    ui->volumeValue->setText(QString("%1%").arg(volume));
    (volume > 0) ? ui->musicSound->setStyleSheet("border-image: url(:/image/soundmax);")
                 : ui->musicSound->setStyleSheet("border-image: url(:/image/soundmin);");
    QSettings().setValue("VOLUMECHOICED", volume);
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
    menu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
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
    ui->musiclrccontainerforinline->setSongSpeedAndSlow(posValue);

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
    ui->musiclrccontainerforinline->setSettingParameter();
    m_musiclrcfordesktop->setSettingParameter();
    m_systemCloseConfig = m_musicSettingParameter.value("CLOSEEVENTCHOICED").toBool();
    if(m_musicSettingParameter.value("SHOWINLINELRCCHOICED").toBool())
      ui->musiclrccontainerforinline->show();
    else
      ui->musiclrccontainerforinline->close();
    bool showDeskLrc = m_musicSettingParameter.value("SHOWDESKTOPLRCCHOICED").toBool();
    if(showDeskLrc)
      m_musiclrcfordesktop->show();
    else
      m_musiclrcfordesktop->close();
    ui->musicDesktopLrc->setChecked(showDeskLrc);
    m_systemTrayMenu->showDesktopLrc(showDeskLrc ? "true" : "false" );
    //This attribute is effective immediately.
}

void MusicApplication::musicShowSkinChangedWindow()
{
    if(m_musicbgskin == NULL)
        m_musicbgskin = new MusicBackgroundSkinDialog(this);
    m_musicbgskin->setCurrentBgTheme(m_currentBgSkin,m_alpha);
    m_musicbgskin->exec();
}

void MusicApplication::musicBgThemeDownloadFinished()
{
    if(ui->SurfaceStackedWidget->currentIndex() == 2  &&
       ui->musiclrccontainerforinline->artBackgroundIsShow() )
    {
        musicBackgroundChanged();
        m_musicSongTree->updateArtPicture();
        m_pictureCarouselTimer.start(5000);
    }
    else
        drawWindowBackgroundRect();
}

void MusicApplication::musicBgTransparentChanged(int index)
{
    if(ui->SurfaceStackedWidget->currentIndex() == 2) return;
    m_alpha = index;//save the alpha
    //Here set the picture transparency
    drawWindowBackgroundRect();
}

void MusicApplication::musicBackgroundChanged()
{
    int count = 0;
    QString filter = ART_BG + getCurrentFileName().split('-').front().trimmed()+ "%1" + SKN_FILE;
    for(int i=0; i<5; ++i)
        if(QFile::exists(filter.arg(i))) ++count;
    /////////////////////////////////////////////////////////////////////
    QString art_path = filter.arg(m_pictureCarouselIndex < count ? m_pictureCarouselIndex++ : m_pictureCarouselIndex = 0);
    QFile::exists(art_path) ? drawWindowBackgroundRectString(art_path) : drawWindowBackgroundRect();
}

void MusicApplication::drawWindowBackgroundRect()
{
    m_pictureCarouselTimer.stop();
    drawWindowBackgroundRectString(THEME_DOWNLOAD + m_currentBgSkin + SKN_FILE);
}

void MusicApplication::drawWindowBackgroundRectString(const QString& path)
{
    QPixmap origin(path);
    QPixmap afterDeal(size());
    afterDeal.fill(Qt::transparent);
    QPainter paint(&afterDeal);
    paint.fillRect(0,0,afterDeal.width(),afterDeal.height(),QColor(255,255,255,2.55*m_alpha));
    paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
    paint.drawPixmap(0,0,QPixmap::fromImage(origin.scaled(size(), Qt::KeepAspectRatioByExpanding).toImage()));
    paint.end();

    m_musicSongTree->setStyleSheet(MusicObject::QToolBoxItemStyle);
    ui->background->setPixmap(afterDeal);
}

void MusicApplication::musicBackgroundSkinChanged(const QString &filename)
{
    m_currentBgSkin = filename;
    musicBgTransparentChanged(m_alpha);
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
    //Loading the current song lrc
    if( m_musicSettingParameter.value("SHOWINLINELRCCHOICED").toBool() ||
        m_musicSettingParameter.value("SHOWDESKTOPLRCCHOICED").toBool() )
    {
       if(m_musicList->currentIndex() == -1)
          return;

       QString filename = LRC_DOWNLOAD + m_musicSongTree->getMusicSongsFileName( \
                  m_currentMusicSongTreeIndex)[m_musicList->currentIndex()] + LRC_FILE;
       ui->musiclrccontainerforinline->stopLrcMask();
       ui->musiclrccontainerforinline->setCurrentSongName(getCurrentFileName());
       ui->musiclrccontainerforinline->transLrcFileToTime(filename.trimmed());
       m_musiclrcfordesktop->setCurrentSongName(getCurrentFileName());
       m_musiclrcfordesktop->initCurrentLrc();
    }
    //display current ArtTheme pic
    musicBgThemeDownloadFinished();
}

void MusicApplication::UpdateCurrentLrc(qint64 time)
{
    qint64 totalTimeValue = m_musicPlayer->duration();
    //Direct access to the audio file is the total time, in milliseconds
    MIntStringMap lrcContainer(ui->musiclrccontainerforinline->getLrcContainer());
    //The corresponding access to current time lyrics
    if(!lrcContainer.isEmpty())
    {
      //After get the current time in the lyrics of the two time points
      qint64 previous = 0;
      qint64 later = 0;
      //Keys () method returns a list of lrcContainer
      foreach (qint64 value, lrcContainer.keys())
      {
        if(time >= value)
           previous = value;
        else
        {
          later = value;
          break;
        }
      }
      //To the last line, set the later to song total time value
      if(later == 0)
        later = totalTimeValue;
      //The lyrics content corresponds to obtain the current time
      QString currentLrc = lrcContainer.value(previous);
      QString laterLrc = lrcContainer.value(later);
      //No content
//      if(currentLrc.length() < 2)
//        currentLrc = tr("..............");
      //If this is a new line of the lyrics, then restart lyrics display mask
      if(currentLrc != ui->musiclrccontainerforinline->text())
      {
        qint64 intervalTime = later - previous;
        if(!m_playControl)
            ui->musiclrccontainerforinline->updateCurrentLrc(intervalTime);
        m_musiclrcfordesktop->updateCurrentLrc(currentLrc,laterLrc,intervalTime);
      }
    }
}

void MusicApplication::musicSearchButtonSearched()
{
    QString searchedQString = ui->musicSongSearchLine->text().trimmed();
    //The string searched wouldn't allow to be none
    if( !searchedQString.isEmpty())
    {
        createVedioWidget(false);
        ui->SurfaceStackedWidget->setCurrentIndex(1);
        musicBgTransparentChanged(m_alpha);
        ui->songSearchWidget->startSearchSong(searchedQString);
    }
}

void MusicApplication::musicIndexWidgetButtonSearched()
{
    createVedioWidget(false);
    //Show the first index of widget
    ui->SurfaceStackedWidget->setCurrentIndex(0);
    musicBgTransparentChanged(m_alpha);
}

void MusicApplication::musicSearchWidgetButtonSearched()
{
    createVedioWidget(false);
    //Show searched song lists
    ui->SurfaceStackedWidget->setCurrentIndex(1);
    musicBgTransparentChanged(m_alpha);
}

void MusicApplication::musicLrcWidgetButtonSearched()
{
    createVedioWidget(false);
    //Show lrc display widget
    ui->SurfaceStackedWidget->setCurrentIndex(2);
    musicBgThemeDownloadFinished();
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
    musicBgTransparentChanged(m_alpha);
}

void MusicApplication::musicVedioWidgetButtonDoubleClicked()
{
//    ui->SurfaceStackedWidget->removeWidget(m_videoPlayer);
//    m_videoPlayer->showFullScreen();
}

QString MusicApplication::getCurrentFileName() const
{
    if(m_musicList->currentIndex() == -1) return QString();
    return m_musicSongTree->getMusicSongsFileName( \
            m_currentMusicSongTreeIndex)[m_musicList->currentIndex()];
}

bool MusicApplication::checkSettingParameterValue() const
{
    return ( m_musicSettingParameter.value("SHOWINLINELRCCHOICED").toBool() ||
             m_musicSettingParameter.value("SHOWDESKTOPLRCCHOICED").toBool() ) ? true : false;
}

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
        m_musicPlayer->setPosition(pos);
}

void MusicApplication::musicAddSongToLovestListAt()
{
//    int index = m_musicList->currentIndex();
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
    ui->musicDesktopLrc->setChecked(show);
    m_musiclrcfordesktop->setVisible(show);
    m_musicSettingParameter.setValue("SHOWDESKTOPLRCCHOICED",show);
}

void MusicApplication::desktopLrcClosed()
{
    ui->musicDesktopLrc->setChecked(false);
    m_systemTrayMenu->showDesktopLrc("false");
    m_musicSettingParameter.setValue("SHOWDESKTOPLRCCHOICED",false);
}

void MusicApplication::musicAudioRecorder()
{
    MusicAudioRecorderWidget recorder(this);
    recorder.exec();
}

void MusicApplication::musicVolumeChangedFromRemote(int value)
{
    ui->musicSoundSlider->setValue(value);
}

void MusicApplication::createRemoteWidget()
{
    if(m_musicRemoteWidget == NULL) return;

    hide();
    m_musicRemoteWidget->showPlayStatus(m_playControl);
    m_musicRemoteWidget->setVolumeValue(ui->musicSoundSlider->value());
    connect(m_musicRemoteWidget,SIGNAL(musicWindowSignal()),SLOT(showNormal()));
    connect(m_musicRemoteWidget,SIGNAL(musicPlayPriviousSignal()),SLOT(musicPlayPrivious()));
    connect(m_musicRemoteWidget,SIGNAL(musicPlayNextSignal()),SLOT(musicPlayNext()));
    connect(m_musicRemoteWidget,SIGNAL(musicKeySignal()),SLOT(musicKey()));
    connect(m_musicRemoteWidget,SIGNAL(musicVolumeSignal(int)),SLOT(musicVolumeChangedFromRemote(int)));
    connect(m_musicRemoteWidget,SIGNAL(musicSettingSignal()),SLOT(musicSetting()));
    m_musicRemoteWidget->show();
}

void MusicApplication::musicDeleteRemote()
{
    delete m_musicRemoteWidget;
    m_musicRemoteWidget = NULL;
}

void MusicApplication::musicSquareRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForSquare;
    createRemoteWidget();
}

void MusicApplication::musicCircleRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForCircle;
    createRemoteWidget();
}

void MusicApplication::musicDiamondRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForDiamond;
    createRemoteWidget();
}

void MusicApplication::musicRectangleRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForRectangle;
    m_musicRemoteWidget->setLabelText(ui->showCurrentSong->text().split('=').back().trimmed());
    createRemoteWidget();
}

void MusicApplication::musicStackedSongListWidgetChanged()
{
    ui->songsContainer->setCurrentIndex(0);
}

void MusicApplication::musicStackedToolsWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicToolSetsWidget(this);

    ui->songsContainer->addWidget(m_stackedWidget);
    connect(m_stackedWidget,SIGNAL(setSpectrum(HWND,int,int)),
            m_musicPlayer,SLOT(setSpectrum(HWND,int,int)));
    static_cast<MusicToolSetsWidget*>(m_stackedWidget)->setSongStringList(
                                      m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex()));
    ui->songsContainer->setCurrentIndex(1);
}

void MusicApplication::musicStackedMyDownWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicMyDownloadRecordWidget(this);
    connect(static_cast<MusicMyDownloadRecordWidget*>(m_stackedWidget),SIGNAL(musicPlay(QStringList)),
            SLOT(addSongToPlayList(QStringList)));
    ui->songsContainer->addWidget(m_stackedWidget);
    ui->songsContainer->setCurrentIndex(1);
}

void MusicApplication::musicStackedRadioWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicWebRadioListView(this);

    ui->songsContainer->addWidget(m_stackedWidget);
    ui->songsContainer->setCurrentIndex(1);
}

void MusicApplication::musicTimerWidget()
{
    MusicTimerWidget timer(this);
    timer.setSongStringList(m_musicSongTree->getMusicSongsFileName(m_musicSongTree->currentIndex()));
    connect(&timer,SIGNAL(timerParameterChanged()),SLOT(musicToolSetsParameter()));
    timer.exec();
}

void MusicApplication::musicSpectrumWidget()
{
    if(!m_musicSpectrumWidget)
        m_musicSpectrumWidget = new MusicSpectrumWidget;
    m_musicSpectrumWidget->show();
    connect(m_musicSpectrumWidget,SIGNAL(setSpectrum(HWND,int,int)),
            m_musicPlayer,SLOT(setSpectrum(HWND,int,int)));
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
    musicBgThemeDownloadFinished();
}
