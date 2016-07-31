#include "musicbottomareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musicsystemtraymenu.h"
#include "musicwindowextras.h"
#include "musicconnectionpool.h"
#include "musiclocalsongsearchdialog.h"

MusicBottomAreaWidget::MusicBottomAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicLocalSongSearch(nullptr)
{
    m_supperClass = parent;
    m_systemCloseConfig = false;

    createSystemTrayIcon();

    m_musicWindowExtras = new MusicWindowExtras(parent);
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicBottomAreaWidget::~MusicBottomAreaWidget()
{
    delete m_systemTrayMenu;
    delete m_systemTray;
    delete m_musicWindowExtras;
    delete m_musicLocalSongSearch;
}

QString MusicBottomAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicBottomAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->showCurrentSong->setEffectOnResize(true);
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

void MusicBottomAreaWidget::createSystemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(m_supperClass);
    m_systemTray->setIcon(QIcon(QString::fromUtf8(":/image/lb_player_logo")));
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

void MusicBottomAreaWidget::setVolumeValue(int value) const
{
    m_systemTrayMenu->setVolumeValue(value);
}

void MusicBottomAreaWidget::setLabelText(const QString &name) const
{
    m_systemTrayMenu->setLabelText(name);
    m_systemTray->setToolTip(name);
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
//    bool con = m_musicWindowExtras->isDisableBlurBehindWindow();
//    m_supperClass->resize( con ? 380 : 1033, m_supperClass->height());
//    m_ui->musicWindowConcise->setGeometry(con ? 295 : 888, 30, 25, 25);
//    m_ui->minimization->setGeometry(con ? 325 : 949, 30, 25, 25);
//    m_ui->windowClose->setGeometry(con ? 350 : 977, 30, 25, 25);
////    m_ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(con ? ":/image/conciseout" : ":/image/concisein")));
//    m_ui->musicSongSearchLine->setVisible( !con );
//    m_ui->resizeWindowLabel->setVisible( !con );
//    ////////////////////////////////////////////////////////////
//    m_ui->songsContainer->resize(320, con ? 460 : 490);
//    m_ui->musicPrevious->setGeometry(con ? 35 : 64, con ? 550 : 576, con ? 30 : 50, con ? 30 : 50);
//    m_ui->musicKey->setGeometry(con ? 65 : 127, con ? 550 : 576, con ? 30 : 50, con ? 30 : 50);
//    m_ui->musicNext->setGeometry(con ? 95 : 190, con ? 550 : 576, con ? 30 : 50, con ? 30 : 50);
//    m_ui->musicTimeWidget->move(con ? 15 : 240, 593);
////    m_ui->verticalLayoutWidget->move(con ? 30 : 255, 583);
//    m_ui->showCurrentSong->move(con ? 85 : 310, 578);
//    m_ui->playCurrentTime->move(con ? 307 : 532, 588);
//    m_ui->playTotalTime->move(con ? 342 : 566, 588);
//    m_ui->musicBestLove->move(con ? 175 : 695, con ? 555 : 598);
//    m_ui->musicPlayMode->move(con ? 197 : 735, con ? 552 : 595);
//    m_ui->musicSimilarFound->move(con ? 220 : 775, con ? 552 : 595);
//    m_ui->musicDownload->move(con ? 245 : 815, con ? 552 : 595);
//    m_ui->musicDesktopLrc->move(con ? 270 : 857, con ? 554 : 597);
//    m_ui->musicSound->move(con ? 290 : 890, con ? 555 : 598);
//    m_ui->musicSoundSlider->move(con ? 310 : 915, con ? 555 : 599);
//    ////////////////////////////////////////////////////////////
//    m_ui->musicEnhancedButton->setVisible( !con );
//    m_ui->lrcDisplayAllButton->setVisible(m_ui->SurfaceStackedWidget->currentIndex() == 2 && !con);
//    m_musicWindowExtras->disableBlurBehindWindow( !con );
//    ////////////////////////////////////////////////////////////
//    if(m_musicLocalSongSearch)
//    {
//        m_musicLocalSongSearch->move(60, con ? 505 : 535);
//    }
}

QString MusicBottomAreaWidget::getSearchedText() const
{
    return m_musicLocalSongSearch->getSearchedText();
}

void MusicBottomAreaWidget::resizeWindow()
{
    int h = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height() - WINDOW_HEIGHT_MIN;
    if(m_musicLocalSongSearch)
    {
        m_musicLocalSongSearch->move(51, 554 + h);
    }
    m_ui->musicSongSearchLine->resizeWindow();

    h = m_ui->musiclrccontainerforinline->size().height() - m_ui->lrcDisplayAllButton->height();
    m_ui->lrcDisplayAllButton->move(m_ui->lrcDisplayAllButton->x(), h/2);
}

void MusicBottomAreaWidget::clearSearchedText()
{
    m_musicLocalSongSearch->close();
}

void MusicBottomAreaWidget::musicSearch()
{
    if(m_musicLocalSongSearch == nullptr)
    {
        m_musicLocalSongSearch = new MusicLocalSongSearchDialog(m_supperClass);
        resizeWindow();
//        m_musicLocalSongSearch->move(51, !m_musicWindowExtras->isDisableBlurBehindWindow() ? 505 : 535);
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
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, false);
}
