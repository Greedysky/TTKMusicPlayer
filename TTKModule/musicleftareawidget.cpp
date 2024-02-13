#include "musicleftareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicleftitemlistuiobject.h"
#include "musicfunctionuiobject.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdownloadrecordwidget.h"
#include "musicwebradioview.h"
#include "musicconnectlocalwidget.h"
#include "musiccloudsharedsongwidget.h"
#include "musicsongscontainerwidget.h"
#include "musicrightareawidget.h"

MusicLeftAreaWidget *MusicLeftAreaWidget::m_instance = nullptr;

MusicLeftAreaWidget::MusicLeftAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_currentIndex(0),
      m_stackedWidget(nullptr),
      m_localSharedSongWidget(nullptr),
      m_cloudSharedSongWidget(nullptr)
{
    m_instance = this;
}

MusicLeftAreaWidget::~MusicLeftAreaWidget()
{
    delete m_localSharedSongWidget;
    delete m_cloudSharedSongWidget;
    delete m_stackedWidget;
}

MusicLeftAreaWidget *MusicLeftAreaWidget::instance()
{
    return m_instance;
}

void MusicLeftAreaWidget::setupUi(Ui::MusicApplication *ui)
{
    m_ui = ui;
    //
    ui->songsContainer->setLength(LEFT_SIDE_WIDTH_MIN, MusicAnimationStackedWidget::Module::LeftToRight);

    ui->musicPrevious->setStyleSheet(TTK::UI::BtnPrevious);
    ui->musicNext->setStyleSheet(TTK::UI::BtnNext);
    ui->musicKey->setStyleSheet(TTK::UI::BtnPlay);

    ui->musicBestLove->setStyleSheet(TTK::UI::BtnUnLove);
    ui->musicDesktopLrc->setStyleSheet(TTK::UI::BtnDKLrc);
    ui->musicDownload->setStyleSheet(TTK::UI::BtnUnDownload);
    ui->musicMoreFunction->setStyleSheet(TTK::UI::BtnMore);

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
    ui->musicBestLove->setToolTip(tr("Best Love"));
    ui->musicDownload->setToolTip(tr("Download"));
    ui->musicMoreFunction->setToolTip(tr("More"));
    ui->musicDesktopLrc->setToolTip(tr("Show Desktop Lrc"));
    ui->musicPlayMode->setToolTip(tr("Play Mode"));

    connect(ui->musicKey, SIGNAL(clicked()), MusicApplication::instance(), SLOT(switchToPlayState()));
    connect(ui->musicPrevious, SIGNAL(clicked()), MusicApplication::instance(), SLOT(playPrevious()));
    connect(ui->musicNext, SIGNAL(clicked()), MusicApplication::instance(), SLOT(playNext()));
    connect(ui->musicSound, SIGNAL(clicked()), MusicApplication::instance(), SLOT(volumeMute()));
    connect(ui->musicSound, SIGNAL(volumeChanged(int)), MusicApplication::instance(), SLOT(volumeChanged(int)));
    connect(ui->musicBestLove, SIGNAL(clicked()), MusicApplication::instance(), SLOT(addSongToLovestList()));
    connect(ui->musicDownload, SIGNAL(clicked()), this, SLOT(downloadSongToLocal()));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), MusicApplication::instance(), SLOT(enhancedMusicChanged(int)));
    connect(ui->musicEnhancedButton, SIGNAL(enhancedMusicChanged(int)), ui->musicTimeWidget, SLOT(setSliderStyleByType(int)));
    connect(ui->userOptionWidget, SIGNAL(buttonClicked(int)), SLOT(switchToSelectedItemStyle(int)));
}

void MusicLeftAreaWidget::setLoveState(bool state)
{
    m_ui->musicBestLove->setStyleSheet(state ? TTK::UI::BtnLove : TTK::UI::BtnUnLove);
    Q_EMIT currentLoveStateChanged();
}

void MusicLeftAreaWidget::downloadSongToLocal()
{
    MusicDownloadMgmtWidget mgmt(this);
    mgmt.initialize(m_ui->musicSongTitle->text(), MusicAbstractQueryRequest::QueryType::Music);
}

void MusicLeftAreaWidget::downloadSongFinished()
{
    bool state = false;
    MusicApplication::instance()->containsDownloadItem(state);
    m_ui->musicDownload->setStyleSheet(state ? TTK::UI::BtnDownload : TTK::UI::BtnUnDownload);
    Q_EMIT currentDownloadStateChanged();
}

void MusicLeftAreaWidget::stackedSongListWidgetChanged()
{
    if(m_currentIndex == 0)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = nullptr;

    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(0);
    m_currentIndex = 0;
}

void MusicLeftAreaWidget::stackedLocalWidgetChanged()
{
    if(m_currentIndex == 1)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = new QWidget(this);
    m_stackedWidget->hide();

    if(m_cloudSharedSongWidget)
    {
        m_ui->songsContainer->removeWidget(m_cloudSharedSongWidget);
        m_cloudSharedSongWidget->hide();
    }

    if(!m_localSharedSongWidget)
    {
        m_localSharedSongWidget = new MusicConnectLocalWidget(this);
    }

    m_ui->songsContainer->addWidget(m_localSharedSongWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    m_currentIndex = 1;
}

void MusicLeftAreaWidget::stackedCloudWidgetChanged()
{
    if(m_currentIndex == 2)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = new QWidget(this);
    m_stackedWidget->hide();

    if(m_localSharedSongWidget)
    {
        m_ui->songsContainer->removeWidget(m_localSharedSongWidget);
        m_localSharedSongWidget->hide();
    }

    if(!m_cloudSharedSongWidget)
    {
        m_cloudSharedSongWidget = new MusicCloudSharedSongWidget(this);
    }

    m_ui->songsContainer->addWidget(m_cloudSharedSongWidget);
    m_cloudSharedSongWidget->initialize();
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    m_currentIndex = 2;
}

void MusicLeftAreaWidget::stackedRadioWidgetChanged()
{
    if(m_currentIndex == 3)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = new MusicWebFMRadioWidget(this);

    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    m_currentIndex = 3;
}

void MusicLeftAreaWidget::stackedMyDownWidgetChanged()
{
    if(m_currentIndex == 4)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = new MusicDownloadToolBoxWidget(this);

    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->setIndex(0, 0);
    m_ui->songsContainer->start(1);
    m_currentIndex = 4;
}

void MusicLeftAreaWidget::switchToSelectedItemStyle(int index)
{
    switch(index)
    {
        case 0: stackedSongListWidgetChanged(); break;
        case 1: stackedLocalWidgetChanged(); break;
        case 2: stackedCloudWidgetChanged(); break;
        case 3: stackedRadioWidgetChanged(); break;
        case 4: stackedMyDownWidgetChanged(); break;
        default: break;
    }

    if(m_ui->musiclrccontainerforinterior->lrcDisplayExpand())
    {
        MusicRightAreaWidget::instance()->lrcDisplayAllClicked();
    }
}
