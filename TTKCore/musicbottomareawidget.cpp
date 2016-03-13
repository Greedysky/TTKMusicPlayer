#include "musicbottomareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musicsystemtraymenu.h"
#include "musicwindowextras.h"

MusicBottomAreaWidget::MusicBottomAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_supperClass = parent;
    m_systemCloseConfig = false;//Control the mode to exit
    createSystemTrayIcon();

    m_musicWindowExtras = new MusicWindowExtras(parent);
}

MusicBottomAreaWidget::~MusicBottomAreaWidget()
{
    delete m_systemTrayMenu;
    delete m_systemTray;
    delete m_musicWindowExtras;
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

void MusicBottomAreaWidget::setDestopLrcVisible(const QString& status) const
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

void MusicBottomAreaWidget::setSystemCloseConfig(const QString &status)
{
    m_systemCloseConfig = (status == "true") ? true : false;
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
    m_ui->resizeWindowLabel->setGeometry(con ? 370 : 950, 620, 15, 15);
//    m_ui->musicWindowSpace->setGeometry(con ? 300 : 380, 20, con ? 100 : 590, con ? 38 : 45);
    m_ui->lrcDisplayAllButton->setVisible(m_ui->SurfaceStackedWidget->currentIndex() == 2 && !con);
    m_ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(con ? ":/image/conciseout" : ":/image/concisein")));
    m_musicWindowExtras->disableBlurBehindWindow( !con );
}

void MusicBottomAreaWidget::lockDesktopLrc(bool lock)
{
    m_systemTrayMenu->lockDesktopLrc(lock);
}

void MusicBottomAreaWidget::desktopLrcClosed()
{
    m_ui->musicDesktopLrc->setChecked(false);
    m_systemTrayMenu->showDesktopLrc("false");
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced, false);
}
