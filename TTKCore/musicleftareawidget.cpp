#include "musicleftareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuiobject.h"
#include "musicleftitemlistuiobject.h"
#include "musicfunctionuiobject.h"
#include "musicdownloadmgmtwidget.h"
#include "musictoolsetswidget.h"
#include "musicdownloadrecordwidget.h"
#include "musicwebmusicradiolistview.h"
#include "musicconnectmobilewidget.h"
#include "musiccloudsharedsongwidget.h"
#include "musicqualitychoicepopwidget.h"
#include "musicsoundkmicrowidget.h"
#include "musicrightareawidget.h"
///qmmp incldue
#include "visual.h"

MusicLeftAreaWidget *MusicLeftAreaWidget::m_instance = nullptr;

MusicLeftAreaWidget::MusicLeftAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_instance = this;
    m_stackedWidget = nullptr;
    m_soundKMicroWidget = nullptr;
    m_qualityChoiceWidget = nullptr;
    m_cloudSharedSongWidget = nullptr;
    m_currentIndex = 0;
    m_isFullOrNormal = true;

    Visual::initialize(MusicApplication::instance());
}

MusicLeftAreaWidget::~MusicLeftAreaWidget()
{
    delete m_soundKMicroWidget;
    delete m_qualityChoiceWidget;
    delete m_cloudSharedSongWidget;
    delete m_stackedWidget;
}

QString MusicLeftAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

MusicLeftAreaWidget *MusicLeftAreaWidget::instance()
{
    return m_instance;
}

void MusicLeftAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    m_qualityChoiceWidget = new MusicQualityChoicePopWidget(this);
    m_ui->musicQualityWindow->addWidget(m_qualityChoiceWidget);
    m_ui->songsContainer->setLength(320, MusicAnimationStackedWidget::LeftToRight);

    connect(ui->musicKey, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicStatePlay()));
    connect(ui->musicPrevious, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicPlayPrevious()));
    connect(ui->musicNext, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicPlayNext()));
    connect(ui->musicSound, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicVolumeMute()));
    connect(ui->musicSound, SIGNAL(musicVolumeChanged(int)), MusicApplication::instance(), SLOT(musicVolumeChanged(int)));
    connect(ui->musicBestLove, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicAddSongToLovestListAt()));
    connect(ui->musicDownload, SIGNAL(clicked()), this, SLOT(musicDownloadSongToLocal()));
    connect(ui->musicButton_playlist, SIGNAL(clicked()), this, SLOT(musicStackedSongListWidgetChanged()));
    connect(ui->musicButton_tools, SIGNAL(clicked()), this, SLOT(musicStackedToolsWidgetChanged()));
    connect(ui->musicButton_radio, SIGNAL(clicked()), this, SLOT(musicStackedRadioWidgetChanged()));
    connect(ui->musicButton_mydownl, SIGNAL(clicked()), this, SLOT(musicStackedMyDownWidgetChanged()));
    connect(ui->musicButton_mobile, SIGNAL(clicked()), this, SLOT(musicStackedMobileWidgetChanged()));
    connect(ui->musicButton_cloud, SIGNAL(clicked()), this, SLOT(musicStackedCloudWidgetChanged()));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), MusicApplication::instance(),
                                     SLOT(musicEnhancedMusicChanged(int)));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), ui->musicTimeWidget,
                                     SLOT(setSliderStyleByType(int)));

    ui->musicPrevious->setStyleSheet(MusicUIObject::MKGBtnPrevious);
    ui->musicNext->setStyleSheet(MusicUIObject::MKGBtnNext);
    ui->musicKey->setStyleSheet(MusicUIObject::MKGBtnPlay);

    switchToSelectedItemStyle(0);
    ui->musicBestLove->setStyleSheet(MusicUIObject::MKGBtnUnLove);
    ui->musicDesktopLrc->setStyleSheet(MusicUIObject::MKGBtnDKLrc);
    ui->musicDownload->setStyleSheet(MusicUIObject::MKGBtnUnDownload);
    ui->musicMoreFunction->setStyleSheet(MusicUIObject::MKGBtnMore);

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

void MusicLeftAreaWidget::musictLoveStateClicked(bool state)
{
    m_ui->musicBestLove->setStyleSheet(state ? MusicUIObject::MKGBtnLove : MusicUIObject::MKGBtnUnLove);
    emit currentLoveStateChanged();
}

void MusicLeftAreaWidget::createSoundKMicroWidget(const QString &name)
{
    if(m_soundKMicroWidget == nullptr)
    {
        m_soundKMicroWidget = new MusicSoundKMicroWidget(this);
    }
    m_soundKMicroWidget->startSeachKMicro(name);
    m_soundKMicroWidget->show();
}

bool MusicLeftAreaWidget::isFullOrNormal() const
{
    return !m_isFullOrNormal;
}

void MusicLeftAreaWidget::musicDownloadSongToLocal()
{
    MusicDownloadMgmtWidget mgmt(this);
    mgmt.setSongName(m_ui->showCurrentSong->text(), MusicDownLoadQueryThreadAbstract::MusicQuery);
}

void MusicLeftAreaWidget::musicDownloadSongFinished()
{
    bool state = false;
    MusicApplication::instance()->musicDownloadContains(state);
    m_ui->musicDownload->setStyleSheet(state ? MusicUIObject::MKGBtnDownload : MusicUIObject::MKGBtnUnDownload);
    emit currentDownloadStateChanged();
}

void MusicLeftAreaWidget::musicStackedSongListWidgetChanged()
{
    if(m_currentIndex == 0)
    {
        return;
    }
    m_currentIndex = 0;

    delete m_stackedWidget;
    m_stackedWidget = nullptr;
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(0);
    switchToSelectedItemStyle(0);
}

void MusicLeftAreaWidget::musicStackedToolsWidgetChanged()
{
    if(m_currentIndex == 1)
    {
        return;
    }
    m_currentIndex = 1;

    delete m_stackedWidget;
    m_stackedWidget = new MusicToolSetsWidget(this);
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(5);
}

void MusicLeftAreaWidget::musicStackedRadioWidgetChanged()
{
    if(m_currentIndex == 2)
    {
        return;
    }
    m_currentIndex = 2;

    delete m_stackedWidget;
    MusicWebMusicRadioListView *w = new MusicWebMusicRadioListView(this);
    w->initListItems();
    m_stackedWidget = w;

    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(1);
}

void MusicLeftAreaWidget::musicStackedMyDownWidgetChanged()
{
    if(m_currentIndex == 3)
    {
        return;
    }
    m_currentIndex = 3;

    delete m_stackedWidget;
    m_stackedWidget = new MusicDownloadRecordWidget(this);
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(2);
}

void MusicLeftAreaWidget::musicStackedMobileWidgetChanged()
{
    if(m_currentIndex == 4)
    {
        return;
    }
    m_currentIndex = 4;

    delete m_stackedWidget;
    m_stackedWidget = new MusicConnectMobileWidget(this);
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(3);
}

void MusicLeftAreaWidget::musicStackedCloudWidgetChanged()
{
    if(m_currentIndex == 5)
    {
        return;
    }
    m_currentIndex = 5;

    delete m_stackedWidget;
    m_stackedWidget = new QWidget(this);
    m_stackedWidget->hide();

    if(!m_cloudSharedSongWidget)
    {
        m_cloudSharedSongWidget = new MusicCloudSharedSongWidget(this);
        m_ui->songsContainer->addWidget(m_cloudSharedSongWidget);
        m_cloudSharedSongWidget->getKey();
    }
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    switchToSelectedItemStyle(4);
}

void MusicLeftAreaWidget::cloudSharedSongUploadAllDone()
{
    if(m_currentIndex == 5)
    {
        return;
    }

    switch(m_currentIndex)
    {
        case 0: musicStackedSongListWidgetChanged(); break;
        case 1: musicStackedToolsWidgetChanged(); break;
        case 2: musicStackedRadioWidgetChanged(); break;
        case 3: musicStackedMyDownWidgetChanged(); break;
        case 4: musicStackedMobileWidgetChanged(); break;
    }

    delete m_cloudSharedSongWidget;
    m_cloudSharedSongWidget = nullptr;
}

void MusicLeftAreaWidget::showFullOrNormal()
{
    if(m_ui->musiclrccontainerforinline->lrcDisplayExpand())
    {
        MusicRightAreaWidget::instance()->musicLrcDisplayAllButtonClicked();
    }

    m_isFullOrNormal = !m_isFullOrNormal;
    m_ui->topWidget->setVisible(m_isFullOrNormal);
    m_ui->bottomWidget->setVisible(m_isFullOrNormal);
    m_ui->centerLeftWidget->setVisible(m_isFullOrNormal);
    m_ui->songsContainer->setVisible(m_isFullOrNormal);
    m_ui->stackedFunctionWidget->setVisible(m_isFullOrNormal);
    m_ui->lrcDisplayAllButton->setVisible(m_isFullOrNormal);

    m_ui->musiclrccontainerforinline->createFloatPlayWidget();
    m_isFullOrNormal ? MusicApplication::instance()->showNormal() : MusicApplication::instance()->showFullScreen();
    m_ui->musiclrccontainerforinline->showFullOrNormal();
}

void MusicLeftAreaWidget::switchToSelectedItemStyle(int index)
{
    m_ui->musicButton_cloud->setStyleSheet(MusicUIObject::MKGItemFavourite);
    m_ui->musicButton_mydownl->setStyleSheet(MusicUIObject::MKGItemDownload);
    m_ui->musicButton_playlist->setStyleSheet(MusicUIObject::MKGItemMusic);
    m_ui->musicButton_radio->setStyleSheet(MusicUIObject::MKGItemRadio);
    m_ui->musicButton_tools->setStyleSheet(MusicUIObject::MKGItemMore);
    m_ui->musicButton_mobile->setStyleSheet(MusicUIObject::MKGItemMobile);

    switch(index)
    {
        case 0: m_ui->musicButton_playlist->setStyleSheet(MusicUIObject::MKGItemMusicClicked); break;
        case 1: m_ui->musicButton_radio->setStyleSheet(MusicUIObject::MKGItemRadioClicked); break;
        case 2: m_ui->musicButton_mydownl->setStyleSheet(MusicUIObject::MKGItemDownloadClicked); break;
        case 3: m_ui->musicButton_mobile->setStyleSheet(MusicUIObject::MKGItemMobileClicked); break;
        case 4: m_ui->musicButton_cloud->setStyleSheet(MusicUIObject::MKGItemFavouriteClicked); break;
        case 5: m_ui->musicButton_tools->setStyleSheet(MusicUIObject::MKGItemMoreClicked); break;
        default: break;
    }

    if(m_ui->musiclrccontainerforinline->lrcDisplayExpand())
    {
        MusicRightAreaWidget::instance()->musicLrcDisplayAllButtonClicked();
    }
}
