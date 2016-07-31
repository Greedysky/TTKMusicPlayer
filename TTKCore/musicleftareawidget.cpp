#include "musicleftareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musicttkuiobject.h"
#include "musicdownloadmgmtwidget.h"
#include "musictoolsetswidget.h"
#include "musicmydownloadrecordwidget.h"
#include "musicwebradiotoolwidget.h"
#include "musicconnectmobilewidget.h"
#include "musiccloudsharedsongwidget.h"
#include "musicqualitychoicewidget.h"
#include "musicconnectionpool.h"
///qmmp incldue
#include "visual.h"
#include "visualfactory.h"
///

MusicLeftAreaWidget::MusicLeftAreaWidget(QWidget *parent)
    : QWidget(parent), m_qualityChoiceWidget(nullptr)
{
    m_supperClass = parent;
    m_stackedWidget = nullptr;
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicLeftAreaWidget::~MusicLeftAreaWidget()
{
    delete m_qualityChoiceWidget;
    delete m_stackedWidget;
}

QString MusicLeftAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLeftAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    m_qualityChoiceWidget = new MusicQualityChoiceWidget(this);
    m_ui->musicQualityWindow->addWidget(m_qualityChoiceWidget);
    m_ui->songsContainer->setLength(320, MusicAnimationStackedWidget::LeftToRight);

    connect(ui->musicKey, SIGNAL(clicked()), m_supperClass, SLOT(musicStatePlay()));
    connect(ui->musicPrevious, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayPrevious()));
    connect(ui->musicNext, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayNext()));
    connect(ui->musicSound, SIGNAL(clicked()), m_supperClass, SLOT(musicVolumeMute()));
    connect(ui->musicSound, SIGNAL(musicVolumeChanged(int)), m_supperClass, SLOT(musicVolumeChanged(int)));
    connect(ui->musicBestLove, SIGNAL(clicked()), m_supperClass, SLOT(musicAddSongToLovestListAt()));
    connect(ui->musicDownload, SIGNAL(clicked()), this, SLOT(musicDownloadSongToLocal()));
    connect(ui->musicButton_playlist, SIGNAL(clicked()), this, SLOT(musicStackedSongListWidgetChanged()));
    connect(ui->musicButton_tools, SIGNAL(clicked()), this, SLOT(musicStackedToolsWidgetChanged()));
    connect(ui->musicButton_radio, SIGNAL(clicked()), this, SLOT(musicStackedRadioWidgetChanged()));
    connect(ui->musicButton_mydownl, SIGNAL(clicked()), this, SLOT(musicStackedMyDownWidgetChanged()));
    connect(ui->musicButton_mobile, SIGNAL(clicked()), this, SLOT(musicStackedMobileWidgetChanged()));
    connect(ui->musicButton_cloud, SIGNAL(clicked()), this, SLOT(musicStackedCloudWidgetChanged()));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), m_supperClass,
                                     SLOT(musicEnhancedMusicChanged(int)));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), ui->musicTimeWidget,
                                     SLOT(setSliderStyleByType(int)));

    ui->musicPrevious->setStyleSheet(MusicTTKUIObject::MKGBtnPrevious);
    ui->musicNext->setStyleSheet(MusicTTKUIObject::MKGBtnNext);
    ui->musicKey->setStyleSheet(MusicTTKUIObject::MKGBtnPlay);

    switchToSelectedItemStyle(0);
    ui->musicBestLove->setStyleSheet(MusicTTKUIObject::MKGBtnUnLove);
    ui->musicDesktopLrc->setStyleSheet(MusicTTKUIObject::MKGBtnDKLrc);
    ui->musicDownload->setStyleSheet(MusicTTKUIObject::MKGBtnUnDownload);
    ui->musicMoreFunction->setStyleSheet(MusicTTKUIObject::MKGBtnMore);

    ui->musicPrevious->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicKey->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicNext->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSound->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicBestLove->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicDesktopLrc->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicPlayMode->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicDownload->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicMoreFunction->setCursor(QCursor(Qt::PointingHandCursor));

    ui->windowClose->setToolTip(tr("Close"));
    ui->musicKey->setToolTip(tr("Play"));
    ui->musicPrevious->setToolTip(tr("Previous"));
    ui->musicNext->setToolTip(tr("Next"));
    ui->musicBestLove->setToolTip(tr("bestlove"));
    ui->musicDownload->setToolTip(tr("download"));
    ui->musicMoreFunction->setToolTip(tr("moreFunction"));
    ui->musicDesktopLrc->setToolTip(tr("desktopLrc"));
    ui->musicPlayMode->setToolTip(tr("playMode"));
    ui->musicButton_cloud->setToolTip(tr("musicCloud"));
    ui->musicButton_mydownl->setToolTip(tr("musicMydownl"));
    ui->musicButton_playlist->setToolTip(tr("musicPlaylist"));
    ui->musicButton_radio->setToolTip(tr("musicRadio"));
    ui->musicButton_tools->setToolTip(tr("musicTools"));
    ui->musicButton_mobile->setToolTip(tr("musicMobile"));
}

void MusicLeftAreaWidget::musictLoveStateClicked()
{
    bool state = !M_SETTING_PTR->value(MusicSettingManager::MuiscSongLovedChoiced).toBool();
    m_ui->musicBestLove->setStyleSheet(state ? MusicTTKUIObject::MKGBtnLove : MusicTTKUIObject::MKGBtnUnLove);
    M_SETTING_PTR->setValue(MusicSettingManager::MuiscSongLovedChoiced, state);
    emit currentLoveStateChanged();
}

void MusicLeftAreaWidget::switchToSelectedItemStyle(int index)
{
    m_ui->musicButton_cloud->setStyleSheet(MusicTTKUIObject::MKGItemFavourite);
    m_ui->musicButton_mydownl->setStyleSheet(MusicTTKUIObject::MKGItemDownload);
    m_ui->musicButton_playlist->setStyleSheet(MusicTTKUIObject::MKGItemMusic);
    m_ui->musicButton_radio->setStyleSheet(MusicTTKUIObject::MKGItemRadio);
    m_ui->musicButton_tools->setStyleSheet(MusicTTKUIObject::MKGItemMore);
    m_ui->musicButton_mobile->setStyleSheet(MusicTTKUIObject::MKGItemMobile);
    switch(index)
    {
        case 0: m_ui->musicButton_playlist->setStyleSheet(MusicTTKUIObject::MKGItemMusicClicked); break;
        case 1: m_ui->musicButton_radio->setStyleSheet(MusicTTKUIObject::MKGItemRadioClicked);break;
        case 2: m_ui->musicButton_mydownl->setStyleSheet(MusicTTKUIObject::MKGItemDownloadClicked);break;
        case 3: m_ui->musicButton_mobile->setStyleSheet(MusicTTKUIObject::MKGItemMobileClicked);break;
        case 4: m_ui->musicButton_cloud->setStyleSheet(MusicTTKUIObject::MKGItemFavouriteClicked);break;
        case 5: m_ui->musicButton_tools->setStyleSheet(MusicTTKUIObject::MKGItemMoreClicked);break;
        default: break;
    }
}

void MusicLeftAreaWidget::musicAnalyzerSpectrumWidget()
{
    Visual::initialize(m_supperClass);
    foreach(VisualFactory *v, Visual::factories())
    {
        if(v->properties().shortName.contains("analyzer"))
        {
            Visual::setEnabled(v);
        }
    }
}

void MusicLeftAreaWidget::musicProjectMSpectrumWidget()
{
    Visual::initialize(m_supperClass);
    foreach(VisualFactory *v, Visual::factories())
    {
        if(v->properties().shortName.contains("projectm"))
        {
            Visual::setEnabled(v);
        }
    }
}

void MusicLeftAreaWidget::musicDownloadSongToLocal()
{
    MusicDownloadMgmtWidget mgmt(this);
    mgmt.setSongName(m_ui->showCurrentSong->text(), MusicDownLoadQueryThreadAbstract::MusicQuery);
}

void MusicLeftAreaWidget::musicDownloadSongFinished()
{
    bool state = !M_SETTING_PTR->value(MusicSettingManager::DownloadMusicExistChoiced).toBool();
    if(state)
    {
        m_ui->musicDownload->setStyleSheet(state ? MusicTTKUIObject::MKGBtnDownload : MusicTTKUIObject::MKGBtnUnDownload);
        M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicExistChoiced, state);
        emit currentDownloadStateChanged();
    }
}

void MusicLeftAreaWidget::musicStackedSongListWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = nullptr;
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(0);
    switchToSelectedItemStyle(0);
}

void MusicLeftAreaWidget::musicStackedToolsWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicToolSetsWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(5);
}

void MusicLeftAreaWidget::musicStackedRadioWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicWebRadioToolWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(1);
}

void MusicLeftAreaWidget::musicStackedMyDownWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicMyDownloadRecordWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(2);
}

void MusicLeftAreaWidget::musicStackedMobileWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicConnectMobileWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(3);
}

void MusicLeftAreaWidget::musicStackedCloudWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicCloudSharedSongWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(4);
}
