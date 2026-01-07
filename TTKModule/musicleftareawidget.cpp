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

    ui->previousButton->setStyleSheet(TTK::UI::BtnPrevious);
    ui->nextButton->setStyleSheet(TTK::UI::BtnNext);
    ui->playButton->setStyleSheet(TTK::UI::BtnPlay);

    ui->bestLoveButton->setStyleSheet(TTK::UI::BtnUnLove);
    ui->desktopLrcButton->setStyleSheet(TTK::UI::BtnDKLrc);
    ui->downloadButton->setStyleSheet(TTK::UI::BtnUnDownload);
    ui->moreFunctionButton->setStyleSheet(TTK::UI::BtnMore);

    ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->soundButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->bestLoveButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->desktopLrcButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playModeButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->moreFunctionButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->windowClose->setToolTip(tr("Close"));
    ui->playButton->setToolTip(tr("Play"));
    ui->previousButton->setToolTip(tr("Previous"));
    ui->nextButton->setToolTip(tr("Next"));
    ui->bestLoveButton->setToolTip(tr("Best Love"));
    ui->downloadButton->setToolTip(tr("Download"));
    ui->moreFunctionButton->setToolTip(tr("More"));
    ui->desktopLrcButton->setToolTip(tr("Show Desktop Lrc"));
    ui->playModeButton->setToolTip(tr("Play Mode"));

    connect(ui->playButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(switchToPlayState()));
    connect(ui->previousButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(playPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(playNext()));
    connect(ui->soundButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(volumeMute()));
    connect(ui->soundButton, SIGNAL(volumeChanged(int)), MusicApplication::instance(), SLOT(volumeChanged(int)));
    connect(ui->bestLoveButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(addSongToLovestList()));
    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(downloadSongToLocal()));
    connect(ui->enhancedButton, SIGNAL(enhancedSongChanged(int)), MusicApplication::instance(), SLOT(enhancedSongChanged(int)));
    connect(ui->enhancedButton, SIGNAL(enhancedSongChanged(int)), ui->timeSliderWidget, SLOT(setSliderStyleByType(int)));
    connect(ui->userOptionWidget, SIGNAL(buttonClicked(int)), SLOT(switchToSelectedItemStyle(int)));
}

void MusicLeftAreaWidget::setSongLoveState(bool state)
{
    m_ui->bestLoveButton->setStyleSheet(state ? TTK::UI::BtnLove : TTK::UI::BtnUnLove);
    Q_EMIT currentLoveStateChanged();
}

void MusicLeftAreaWidget::downloadSongToLocal()
{
    MusicDownloadMgmtWidget mgmt(this);
    mgmt.initialize(m_ui->songTitleLabel->text());
}

void MusicLeftAreaWidget::downloadSongFinished()
{
    bool exist = false;
    MusicApplication::instance()->containsDownloadItem(exist);
    m_ui->downloadButton->setStyleSheet(exist ? TTK::UI::BtnDownload : TTK::UI::BtnUnDownload);
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

    m_currentIndex = 0;
    m_ui->songsContainer->start(0, 0);
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

    m_currentIndex = 1;
    m_ui->songsContainer->addWidget(m_localSharedSongWidget);
    m_ui->songsContainer->start(0, 1);
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

    m_cloudSharedSongWidget->initialize();
    m_currentIndex = 2;
    m_ui->songsContainer->addWidget(m_cloudSharedSongWidget);
    m_ui->songsContainer->start(0, 1);
}

void MusicLeftAreaWidget::stackedRadioWidgetChanged()
{
    if(m_currentIndex == 3)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = new MusicWebFMRadioWidget(this);

    m_currentIndex = 3;
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->start(0, 1);
}

void MusicLeftAreaWidget::stackedMyDownWidgetChanged()
{
    if(m_currentIndex == 4)
    {
        return;
    }

    delete m_stackedWidget;
    m_stackedWidget = new MusicDownloadToolBoxWidget(this);

    m_currentIndex = 4;
    m_ui->songsContainer->insertWidget(1, m_stackedWidget);
    m_ui->songsContainer->start(0, 1);
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

    if(m_ui->lrcForInterior->lrcDisplayExpand())
    {
        MusicRightAreaWidget::instance()->lrcDisplayAllClicked();
    }
}
