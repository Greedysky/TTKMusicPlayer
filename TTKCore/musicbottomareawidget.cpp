#include "musicbottomareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musicsystemtraymenu.h"
#include "musicwindowextras.h"
#include "musicconnectionpool.h"
#include "musiclocalsongsearch.h"

MusicBottomAreaWidget::MusicBottomAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicLocalSongSearch(nullptr)
{
    m_supperClass = parent;
    m_systemCloseConfig = false;//Control the mode to exit

    createSystemTrayIcon();

    m_musicWindowExtras = new MusicWindowExtras(parent);
    M_CONNECTION->setValue("MusicBottomAreaWidget", this);
}

MusicBottomAreaWidget::~MusicBottomAreaWidget()
{
    delete m_systemTrayMenu;
    delete m_systemTray;
    delete m_musicWindowExtras;
    delete m_musicLocalSongSearch;
}

void MusicBottomAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    connect(ui->musicDesktopLrc, SIGNAL(clicked()), m_systemTrayMenu, SLOT(showDesktopLrc()));
}

void MusicBottomAreaWidget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::DoubleClick:
            break;
        case QSystemTrayIcon::Trigger:
            if(!m_supperClass->isVisible())
            {
                m_supperClass->show();
                m_supperClass->activateWindow();
            }
            break;
        default:
            break;
    }
}

void MusicBottomAreaWidget::setVolumeValue(int value) const
{
    m_ui->musicSoundSlider->setValue(value);
}

void MusicBottomAreaWidget::createSystemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(m_supperClass);
    m_systemTray->setIcon(QIcon(QString::fromUtf8(":/image/windowicon")));
    m_systemTray->setToolTip(tr("TTKMusicPlayer"));

    m_systemTrayMenu = new MusicSystemTrayMenu(m_supperClass);
    connect(m_systemTrayMenu, SIGNAL(setShowDesktopLrc(bool)), SIGNAL(setShowDesktopLrc(bool)));
    connect(m_systemTrayMenu, SIGNAL(setWindowLockedChanged()), SIGNAL(setWindowLockedChanged()));

    m_systemTray->setContextMenu(m_systemTrayMenu);
    m_systemTray->show();
    connect(m_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MusicBottomAreaWidget::setDestopLrcVisible(bool status) const
{
    m_systemTrayMenu->showDesktopLrc(status);
}

void MusicBottomAreaWidget::showPlayStatus(bool status) const
{
    m_systemTrayMenu->showPlayStatus(status);
#if defined MUSIC_DEBUG && defined Q_OS_WIN && defined MUSIC_WINEXTRAS
    m_musicWindowExtras->showPlayStatus(status);
#endif
}

void MusicBottomAreaWidget::setLabelText(const QString &name) const
{
    m_systemTrayMenu->setLabelText(name);
}

void MusicBottomAreaWidget::showMessage(const QString &title, const QString &text)
{
    m_systemTray->showMessage(title, text);
}

#if defined MUSIC_DEBUG && defined Q_OS_WIN && defined MUSIC_WINEXTRAS
void MusicBottomAreaWidget::setValue(int value) const
{
    m_musicWindowExtras->setValue(value);
}

void MusicBottomAreaWidget::setRange(int min, int max) const
{
    m_musicWindowExtras->setRange(min, max);
}
#endif

void MusicBottomAreaWidget::setWindowConcise()
{
    bool con = m_musicWindowExtras->isDisableBlurBehindWindow();
    m_supperClass->resize( con ? 390 : 990, m_supperClass->height());
    m_ui->musicWindowConcise->setGeometry(con ? 305 : 848, 27, 25, 25);
    m_ui->minimization->setGeometry(con ? 335 : 909, 27, 25, 25);
    m_ui->windowClose->setGeometry(con ? 360 : 937, 27, 25, 25);
    m_ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(con ? ":/image/conciseout" : ":/image/concisein")));
    m_ui->musicSongSearchLine->setVisible( !con );
    m_ui->resizeWindowLabel->setVisible( !con );
    ////////////////////////////////////////////////////////////
    m_ui->songsContainer->resize(331, con ? 460 : 490);
    m_ui->musicPrevious->setGeometry(con ? 35 : 64, con ? 530 : 556, con ? 30 : 50, con ? 30 : 50);
    m_ui->musicKey->setGeometry(con ? 65 : 127, con ? 530 : 556, con ? 30 : 50, con ? 30 : 50);
    m_ui->musicNext->setGeometry(con ? 95 : 190, con ? 530 : 556, con ? 30 : 50, con ? 30 : 50);
    m_ui->musicTimeWidget->move(con ? 15 : 230, 573);
    m_ui->verticalLayoutWidget->move(con ? 30 : 245, 563);
    m_ui->showCurrentSong->move(con ? 85 : 300, 558);
    m_ui->playCurrentTime->move(con ? 307 : 522, 568);
    m_ui->playTotalTime->move(con ? 342 : 556, 568);
    m_ui->musicBestLove->move(con ? 180 : 625, con ? 535 : 578);
    m_ui->musicPlayMode->move(con ? 202 : 655, con ? 532 : 575);
    m_ui->musicSimilarFound->move(con ? 225 : 685, con ? 532 : 575);
    m_ui->musicDownload->move(con ? 250 : 715, con ? 532 : 575);
    m_ui->musicDesktopLrc->move(con ? 275 : 747, con ? 534 : 577);
    m_ui->musicSound->move(con ? 295 : 840, con ? 535 : 578);
    m_ui->musicSoundSlider->move(con ? 315 : 865, con ? 535 : 579);
    ////////////////////////////////////////////////////////////
    m_ui->musicEnhancedButton->setVisible( !con );
    m_ui->lrcDisplayAllButton->setVisible(m_ui->SurfaceStackedWidget->currentIndex() == 2 && !con);
    m_musicWindowExtras->disableBlurBehindWindow( !con );
    ////////////////////////////////////////////////////////////
    if(m_musicLocalSongSearch)
    {
        m_musicLocalSongSearch->move(60, con ? 490 : 520);
    }
}

QString MusicBottomAreaWidget::getSearchedText() const
{
    return m_musicLocalSongSearch->getSearchedText();
}

void MusicBottomAreaWidget::clearSearchedText()
{
    m_musicLocalSongSearch->close();
}

void MusicBottomAreaWidget::musicSearch()
{
    if(m_musicLocalSongSearch == nullptr)
    {
        m_musicLocalSongSearch = new MusicLocalSongSearch(m_supperClass);
        m_musicLocalSongSearch->move(60, !m_musicWindowExtras->isDisableBlurBehindWindow() ? 490 : 520);
    }
    m_musicLocalSongSearch->setVisible(!m_musicLocalSongSearch->isVisible());
}

void MusicBottomAreaWidget::lockDesktopLrc(bool lock)
{
    m_systemTrayMenu->lockDesktopLrc(lock);
}

void MusicBottomAreaWidget::desktopLrcClosed()
{
    m_ui->musicDesktopLrc->setChecked(false);
    m_systemTrayMenu->showDesktopLrc(false);
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced, false);
}
