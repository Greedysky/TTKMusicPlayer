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
    ui->menuSetting->setIcon(QIcon(QString::fromUtf8(":/image/menu")));
    ui->menuSetting->setIconSize(QSize(50, 50));
    ui->menuSetting->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    ui->menuSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->menuSetting->setToolTip(tr("Menu"));
    ui->menuSetting->setMenu(&m_toolPopupMenu);
    
    ui->musicImport->setIcon(QIcon(QString::fromUtf8(":/appTools/import")));
    ui->musicImport->setIconSize(QSize(40, 40));
    ui->musicImport->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicImport->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicImport->setToolTip(tr("Import"));
    connect(ui->musicImport, SIGNAL(clicked()), m_supperClass, SLOT(musicImportSongs()));
    
    ui->musicSetting->setIcon(QIcon(QString::fromUtf8(":/appTools/setting")));
    ui->musicSetting->setIconSize(QSize(40, 40));
    ui->musicSetting->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSetting->setToolTip(tr("Setting"));
    connect(ui->musicSetting, SIGNAL(clicked()), m_supperClass, SLOT(musicSetting()));
    
    ui->musicSearch->setIcon(QIcon(QString::fromUtf8(":/appTools/search")));
    ui->musicSearch->setIconSize(QSize(40, 40));
    ui->musicSearch->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicSearch->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearch->setToolTip(tr("musicSearch"));

    ui->musicCurrentLocation->setIcon(QIcon(QString::fromUtf8(":/appTools/location")));
    ui->musicCurrentLocation->setIconSize(QSize(40, 40));
    ui->musicCurrentLocation->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->musicCurrentLocation->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicCurrentLocation->setToolTip(tr("musicLocation"));
    connect(ui->musicCurrentLocation, SIGNAL(clicked()), m_supperClass, SLOT(musicCurrentPlayLocation()));

    connect(ui->musicDesktopLrc, SIGNAL(clicked()), m_systemTrayMenu, SLOT(showDesktopLrc()));

    createToolPopupMenu();
}

void MusicBottomAreaWidget::createToolPopupMenu()
{
    m_toolPopupMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    m_toolPopupMenu.addAction(m_ui->action_ImportSongs);
    m_toolPopupMenu.addAction(m_ui->action_Setting);

    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_Privious);
    m_toolPopupMenu.addAction(m_ui->action_Play);
    m_toolPopupMenu.addAction(m_ui->action_Next);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_OrderPlay);
    m_toolPopupMenu.addAction(m_ui->action_RandomPlay);
    m_toolPopupMenu.addAction(m_ui->action_SingleCycle);
    m_toolPopupMenu.addAction(m_ui->action_ListCycle);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_VolumeSub);
    m_toolPopupMenu.addAction(m_ui->action_VolumePlus);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_About);
    m_toolPopupMenu.addAction(m_ui->action_Quit);
    createMenuActions();
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

void MusicBottomAreaWidget::createMenuActions() const
{
    connect(m_ui->action_ImportSongs, SIGNAL(triggered()), m_supperClass, SLOT(musicImportSongs()));
    connect(m_ui->action_Setting, SIGNAL(triggered()), m_supperClass, SLOT(musicSetting()));
    connect(m_ui->action_Quit, SIGNAL(triggered()), m_supperClass, SLOT(quitWindowClose()));
    connect(m_ui->action_Next, SIGNAL(triggered()), m_supperClass, SLOT(musicPlayNext()));
    connect(m_ui->action_Play, SIGNAL(triggered()), m_supperClass, SLOT(musicKey()));
    connect(m_ui->action_Privious, SIGNAL(triggered()), m_supperClass, SLOT(musicPlayPrivious()));
    connect(m_ui->action_VolumeSub, SIGNAL(triggered()), m_supperClass, SLOT(musicActionVolumeSub()));
    connect(m_ui->action_VolumePlus, SIGNAL(triggered()), m_supperClass, SLOT(musicActionVolumePlus()));
    connect(m_ui->action_OrderPlay, SIGNAL(triggered()), m_supperClass, SLOT(musicPlayOrder()));
    connect(m_ui->action_RandomPlay, SIGNAL(triggered()), m_supperClass, SLOT(musicPlayRandom()));
    connect(m_ui->action_SingleCycle, SIGNAL(triggered()), m_supperClass, SLOT(musicPlayOneLoop()));
    connect(m_ui->action_ListCycle, SIGNAL(triggered()), m_supperClass, SLOT(musicPlayListLoop()));
}

void MusicBottomAreaWidget::createSystemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(m_supperClass);
    m_systemTray->setIcon(QIcon(QString::fromUtf8(":/image/windowicon")));
    m_systemTray->setToolTip(tr("QMusicPlayer"));

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
#if defined MUSIC_DEBUG && defined Q_OS_WIN
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

#if defined MUSIC_DEBUG && defined Q_OS_WIN
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
    m_supperClass->resize( con ? 400 : 990, m_supperClass->height());
    m_ui->musicWindowConcise->setGeometry(con ? 315 : 848, 27, 25, 25);
    m_ui->minimization->setGeometry(con ? 345 : 909, 27, 25, 25);
    m_ui->windowClose->setGeometry(con ? 370 : 937, 27, 25, 25);
    m_ui->resizeWindowLabel->setGeometry(con ? 380 : 950, 620, 15, 15);
    m_ui->musicWindowSpace->setGeometry(con ? 310 : 380, 20, con ? 100 : 590, con ? 38 : 45);
    m_ui->lrcDisplayAllButton->setHidden(con);
    m_ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(con ? ":/image/conciseout"
                                                                  : ":/image/concisein")));
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
