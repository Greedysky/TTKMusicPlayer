#include "musicleftareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musictoolsetswidget.h"
#include "musicmydownloadrecordwidget.h"
#include "musicwebradiolistview.h"
#include "musicspectrumwidget.h"
#include "musiclocalsongsearch.h"
#include "musicconnectmobilewidget.h"
#include "musicqualitychoicewidget.h"

MusicLeftAreaWidget::MusicLeftAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicLocalSongSearch(nullptr), m_qualityChoiceWidget(nullptr)
{
    m_supperClass = parent;
    m_stackedWidget = nullptr;
    m_musicSpectrumWidget = nullptr;
}

MusicLeftAreaWidget::~MusicLeftAreaWidget()
{
    delete m_qualityChoiceWidget;
    delete m_musicLocalSongSearch;
    delete m_stackedWidget;
    delete m_musicSpectrumWidget;
}

void MusicLeftAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    m_qualityChoiceWidget = new MusicQualityChoiceWidget(this);
    m_ui->musicQualityWindow->addWidget(m_qualityChoiceWidget);

    connect(ui->musicKey, SIGNAL(clicked()), m_supperClass, SLOT(musicKey()));
    connect(ui->musicPrivious, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayPrivious()));
    connect(ui->musicNext, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayNext()));
    connect(ui->musicSound, SIGNAL(clicked()), m_supperClass, SLOT(musicVolumeMute()));
    connect(ui->musicSoundSlider, SIGNAL(valueChanged(int)), m_supperClass, SLOT(musicVolumeChanged(int)));
    connect(ui->musicBestLove, SIGNAL(clicked()), m_supperClass, SLOT(musicAddSongToLovestListAt()));
    connect(ui->musicButton_playlist, SIGNAL(clicked()), this, SLOT(musicStackedSongListWidgetChanged()));
    connect(ui->musicButton_tools, SIGNAL(clicked()), this, SLOT(musicStackedToolsWidgetChanged()));
    connect(ui->musicButton_radio, SIGNAL(clicked()), this, SLOT(musicStackedRadioWidgetChanged()));
    connect(ui->musicButton_mydownl, SIGNAL(clicked()), this, SLOT(musicStackedMyDownWidgetChanged()));
    connect(ui->musicButton_mobile, SIGNAL(clicked()), this, SLOT(musicStackedMobileWidgetChanged()));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), m_supperClass,
                                     SLOT(musicEnhancedMusicChanged(int)));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), ui->musicTimeWidget,
                                     SLOT(setSliderStyleByType(int)));

    ui->musicPrivious->setIcon(QIcon(QString::fromUtf8(":/image/privious")));
    ui->musicNext->setIcon(QIcon(QString::fromUtf8(":/image/next")));
    ui->musicKey->setIcon(QIcon(QString::fromUtf8(":/image/play")));
    ui->musicBestLove->setIcon(QIcon(QString::fromUtf8(":/image/bestlove")));
    ui->musicButton_cloud->setIcon(QIcon(QString::fromUtf8(":/appTools/buttoncloud")));
    ui->musicButton_mydownl->setIcon(QIcon(QString::fromUtf8(":/appTools/buttonmydownl")));
    ui->musicButton_playlist->setIcon(QIcon(QString::fromUtf8(":/appTools/buttonplaylist")));
    ui->musicButton_radio->setIcon(QIcon(QString::fromUtf8(":/appTools/buttonradio")));
    ui->musicButton_tools->setIcon(QIcon(QString::fromUtf8(":/appTools/buttontools")));
    ui->musicButton_mobile->setIcon(QIcon(QString::fromUtf8(":/appTools/buttonmobile")));

    ui->musicPrivious->setIconSize(QSize(45, 45));
    ui->musicNext->setIconSize(QSize(45, 45));
    ui->musicKey->setIconSize(QSize(45, 45));
    ui->musicBestLove->setIconSize(QSize(25, 25));
    ui->musicButton_cloud->setIconSize(QSize(35, 35));
    ui->musicButton_mydownl->setIconSize(QSize(35, 35));
    ui->musicButton_playlist->setIconSize(QSize(35, 35));
    ui->musicButton_radio->setIconSize(QSize(35, 35));
    ui->musicButton_tools->setIconSize(QSize(35, 35));
    ui->musicButton_mobile->setIconSize(QSize(35, 35));
    ui->musicPlayMode->setIconSize(QSize(25, 25));

    ui->musicSoundSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->musicPrivious->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicNext->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicKey->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicBestLove->setStyleSheet(MusicUIObject::MToolButtonStyle06);
    ui->musicDesktopLrc->setStyleSheet(MusicUIObject::MCheckBoxStyle03);
    ui->musicButton_cloud->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicButton_mydownl->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicButton_playlist->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicButton_radio->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicButton_tools->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicButton_mobile->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicPlayMode->setStyleSheet(MusicUIObject::MToolButtonStyle04);

    ui->musicPrivious->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicKey->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicNext->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSound->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSoundSlider->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicBestLove->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicDesktopLrc->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicPlayMode->setCursor(QCursor(Qt::PointingHandCursor));

    ui->windowClose->setToolTip(tr("Close"));
    ui->musicKey->setToolTip(tr("Play"));
    ui->musicPrivious->setToolTip(tr("Privious"));
    ui->musicNext->setToolTip(tr("Next"));
    ui->musicBestLove->setToolTip(tr("bestlove"));
    ui->musicDesktopLrc->setToolTip(tr("desktopLrc"));
    ui->musicPlayMode->setToolTip(tr("playMode"));
    ui->musicButton_cloud->setToolTip(tr("musicCloud"));
    ui->musicButton_mydownl->setToolTip(tr("musicMydownl"));
    ui->musicButton_playlist->setToolTip(tr("musicPlaylist"));
    ui->musicButton_radio->setToolTip(tr("musicRadio"));
    ui->musicButton_tools->setToolTip(tr("musicTools"));
    ui->musicButton_mobile->setToolTip(tr("musicMobile"));
}

QString MusicLeftAreaWidget::getSearchedText() const
{
    return m_musicLocalSongSearch->getSearchedText();
}

void MusicLeftAreaWidget::clearSearchedText() const
{
    m_musicLocalSongSearch->clearSearchedText();
}

void MusicLeftAreaWidget::musicSearch()
{
    if(m_musicLocalSongSearch == nullptr)
    {
        m_musicLocalSongSearch = new MusicLocalSongSearch(m_supperClass);
    }
    m_musicLocalSongSearch->setVisible(!m_musicLocalSongSearch->isVisible());
}

void MusicLeftAreaWidget::musicStackedSongListWidgetChanged()
{
    m_ui->songsContainer->setCurrentIndex(0);
    delete m_stackedWidget;
    m_stackedWidget = nullptr;
}

void MusicLeftAreaWidget::musicStackedToolsWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicToolSetsWidget(this);

    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setCurrentIndex(1);
}

void MusicLeftAreaWidget::musicStackedMyDownWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicMyDownloadRecordWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setCurrentIndex(1);
}

void MusicLeftAreaWidget::musicStackedMobileWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicConnectMobileWidget(this);
    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setCurrentIndex(1);
}

void MusicLeftAreaWidget::musicStackedRadioWidgetChanged()
{
    delete m_stackedWidget;
    m_stackedWidget = new MusicWebRadioListView(this);

    m_ui->songsContainer->addWidget(m_stackedWidget);
    m_ui->songsContainer->setCurrentIndex(1);
}

void MusicLeftAreaWidget::musicSpectrumWidget()
{
    if(m_musicSpectrumWidget == nullptr)
    {
        m_musicSpectrumWidget = new MusicSpectrumWidget;
    }
    m_musicSpectrumWidget->show();
}
