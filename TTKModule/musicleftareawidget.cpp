#include "musicleftareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuiobject.h"
#include "musicleftitemlistuiobject.h"
#include "musicfunctionuiobject.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdownloadrecordwidget.h"
#include "musicwebradioview.h"
#include "musicconnectmobilewidget.h"
#include "musiccloudsharedsongwidget.h"
#include "musicqualitychoicepopwidget.h"
#include "musicsoundkmicrowidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicrightareawidget.h"

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
}

MusicLeftAreaWidget::~MusicLeftAreaWidget()
{
    delete m_soundKMicroWidget;
    delete m_qualityChoiceWidget;
    delete m_cloudSharedSongWidget;
    delete m_stackedWidget;
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
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), MusicApplication::instance(), SLOT(musicEnhancedMusicChanged(int)));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), ui->musicTimeWidget, SLOT(setSliderStyleByType(int)));
    connect(ui->functionAnimationWidget, SIGNAL(buttonClicked(int)), SLOT(switchToSelectedItemStyle(int)));

    ui->musicPrevious->setStyleSheet(MusicUIObject::MKGBtnPrevious);
    ui->musicNext->setStyleSheet(MusicUIObject::MKGBtnNext);
    ui->musicKey->setStyleSheet(MusicUIObject::MKGBtnPlay);

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
}

void MusicLeftAreaWidget::radioExecuteOuter(const QString &path)
{
    musicStackedRadioWidgetChanged();
    MusicWebRadioView *w = MStatic_cast(MusicWebRadioView*, m_stackedWidget);
    w->init(path.toInt());
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

void MusicLeftAreaWidget::setTransparent(int index)
{
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparentChoiced, index);
    m_ui->centerLeftWidget->setTransparent(index);
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
}

void MusicLeftAreaWidget::musicStackedRadioWidgetChanged()
{
    if(m_currentIndex == 2)
    {
        return;
    }
    m_currentIndex = 2;

    delete m_stackedWidget;
    MusicWebRadioView *w = new MusicWebRadioView(this);
    w->init(DEFAULT_LEVEL_LOWER);
    m_stackedWidget = w;

    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
}

void MusicLeftAreaWidget::musicStackedMyDownWidgetChanged()
{
    if(m_currentIndex == 4)
    {
        return;
    }
    m_currentIndex = 4;

    delete m_stackedWidget;
    m_stackedWidget = new MusicDownloadToolBoxWidget(this);
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
}

void MusicLeftAreaWidget::musicStackedMobileWidgetChanged()
{
    if(m_currentIndex == 3)
    {
        return;
    }
    m_currentIndex = 3;

    delete m_stackedWidget;
    m_stackedWidget = new MusicConnectMobileWidget(this);
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
}

void MusicLeftAreaWidget::musicStackedCloudWidgetChanged()
{
    if(m_currentIndex == 1)
    {
        return;
    }
    m_currentIndex = 1;

    delete m_stackedWidget;
    m_stackedWidget = new QWidget(this);
    m_stackedWidget->hide();

    if(!m_cloudSharedSongWidget)
    {
        m_cloudSharedSongWidget = new MusicCloudSharedSongWidget(this);
        m_ui->songsContainer->addWidget(m_cloudSharedSongWidget);
    }
    m_cloudSharedSongWidget->showMainWindow();
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
}

void MusicLeftAreaWidget::switchToSelectedItemStyle(int index)
{
    switch(index)
    {
        case 0: musicStackedSongListWidgetChanged(); break;
        case 1: musicStackedCloudWidgetChanged(); break;
        case 2: musicStackedRadioWidgetChanged(); break;
        case 3: musicStackedMobileWidgetChanged(); break;
        case 4: musicStackedMyDownWidgetChanged(); break;
        default: break;
    }

    if(m_ui->musiclrccontainerforinline->lrcDisplayExpand())
    {
        MusicRightAreaWidget::instance()->musicLrcDisplayAllButtonClicked();
    }
}
