#include "musicapplicationmodule.h"
#include "musicmobiledeviceswidget.h"
#include "musictimerwidget.h"
#include "musicspectrumwidget.h"
#include "musictimerautomodule.h"
#include "musicmessagebox.h"
#include "musictoastlabel.h"
#include "musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicplatformmanager.h"
#include "musicsourceupdatewidget.h"
#include "musicsoundeffectswidget.h"
#include "musicmessageaboutdialog.h"
#include "musicapplication.h"
#include "musictopareawidget.h"
#include "musicimageutils.h"
#include "musicgiflabelwidget.h"
#include "musicfileutils.h"
#include "musicsourceupdaterequest.h"
#include "musicdownloadcounterpvrequest.h"
#include "musicscreensaverwidget.h"
#include "ttkdesktopwrapper.h"

#include "qdevicewatcher.h"
#include "qsync/qsyncconfig.h"

#define MARGIN_SIDE     5
#define MARGIN_SIDE_BY  1
#define SYNC_HOST_URL   "VDVnYUdYMW9xNnVWSnd6L0J6NHI2MFZ5d0R3R2NiRVF4VW5WckpNcUhnUT0="

MusicApplicationModule *MusicApplicationModule::m_instance = nullptr;

MusicApplicationModule::MusicApplicationModule(QObject *parent)
    : QObject(parent)
{
    m_instance = this;

    musicResetWindow();

    m_quitAnimation = new QPropertyAnimation(this);
    m_sideAnimation = new QPropertyAnimation(parent, "geometry", this);
    m_quitAnimation->setTargetObject(parent);
    m_sideAnimation->setDuration(250 * MT_MS);

    m_MusicTimerAutoModule = new MusicTimerAutoModule(this);
    m_screenSaverWidget = nullptr;
    m_setWindowToTop = false;
    m_mobileDeviceWidget = nullptr;
    m_quitContainer = nullptr;

    m_deviceWatcher = new QDeviceWatcher(this);
    connect(m_deviceWatcher, SIGNAL(deviceChanged(bool)), SLOT(musicDeviceChanged(bool)));
    connect(m_deviceWatcher, SIGNAL(deviceAdded(QString)), SLOT(musicDeviceNameChanged(QString)));
    m_deviceWatcher->appendEventReceiver(this);
    m_deviceWatcher->start();

    m_sourceUpdatehread = new MusicSourceUpdateRequest(this);
    m_counterPVThread = new MusicDownloadCounterPVRequest(this);

    musicToolSetsParameter();
}

MusicApplicationModule::~MusicApplicationModule()
{
    Q_CLEANUP_RESOURCE(MusicPlayer);

    delete m_MusicTimerAutoModule;
    delete m_screenSaverWidget;
    delete m_quitAnimation;
    delete m_sideAnimation;
    delete m_deviceWatcher;
    delete m_mobileDeviceWidget;
    delete m_quitContainer;
    delete m_sourceUpdatehread;
    delete m_counterPVThread;
}

MusicApplicationModule *MusicApplicationModule::instance()
{
    return m_instance;
}

bool MusicApplicationModule::isLastedVersion() const
{
    return m_sourceUpdatehread->isLastedVersion();
}

void MusicApplicationModule::loadNetWorkSetting()
{
#ifndef QT_NO_SSL
    // ssl support check
    TTK_LOGGER_INFO(QString("Application network support ssl: %1").arg(QSslSocket::supportsSsl() ? "true" : "false"));
#endif
    // sync host init
    QSyncConfig::HOST = MusicUtils::Algorithm::mdII(SYNC_HOST_URL, false);
    //
    m_sourceUpdatehread->startToDownload();
    m_counterPVThread->startToDownload();
}

void MusicApplicationModule::applySettingParameter()
{
#ifdef Q_OS_WIN
    if(G_SETTING_PTR->value(MusicSettingManager::FileAssociationMode).toInt())
    {
        MusicPlatformManager platform;
        platform.setMusicRegeditAssociateFileIcon();
    }
#endif
    if(!m_screenSaverWidget)
    {
        m_screenSaverWidget = new MusicScreenSaverBackgroundWidget;
    }
    m_screenSaverWidget->applySettingParameter();
}

void MusicApplicationModule::windowCloseAnimation()
{
    if(G_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toBool())
    {
        MusicTopAreaWidget::instance()->setBackgroundAnimation(false);
        MusicApplication *w = MusicApplication::instance();
        w->setMinimumSize(0, 0); ///remove fixed size

        m_quitAnimation->stop();
        m_quitAnimation->setPropertyName("geometry");
        m_quitAnimation->setDuration(250 * MT_MS);
        m_quitAnimation->setStartValue(w->geometry());
        m_quitAnimation->setEndValue(QRect(w->x(), w->geometry().center().y(), w->width(), 0));
        m_quitAnimation->start();
        QTimer::singleShot(m_quitAnimation->duration(), this, SLOT(windowCloseAnimationFinished()));
    }
    else
    {
        float v = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
              v = MusicUtils::Image::reRenderValue<float>(1, 0.35, 100 - v);
        m_quitAnimation->stop();
        m_quitAnimation->setPropertyName("windowOpacity");
        m_quitAnimation->setDuration(MT_S2MS);
        m_quitAnimation->setStartValue(v);
        m_quitAnimation->setEndValue(0);
        m_quitAnimation->start();
    }
    QTimer::singleShot(MT_S2MS, qApp, SLOT(quit()));
}

void MusicApplicationModule::soureUpdateCheck()
{
    MusicSourceUpdateNotifyWidget *w = new MusicSourceUpdateNotifyWidget;
    w->start();
    w->show();
}

void MusicApplicationModule::sideAnimationByOn()
{
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool())
    {
        return;
    }

    MusicApplication *w = MusicApplication::instance();
    if(w->isFullScreen() || w->isMaximized())
    {
        return;
    }

    QPoint pt = w->mapToGlobal(w->rect().topLeft());
    if(-MARGIN_SIDE <= pt.x() && pt.x() <= MARGIN_SIDE)
    {
        m_leftSideByOn = true;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(-w->width() + MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }

    const QRect &rect = TTKDesktopWrapper::screenGeometry();
    pt = w->mapToGlobal(rect.topRight());
    if(-MARGIN_SIDE + rect.width() <= pt.x() && pt.x() <= MARGIN_SIDE + rect.width())
    {
        m_rightSideByOn = true;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(rect.width() - MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
}

void MusicApplicationModule::sideAnimationByOff()
{
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool())
    {
        return;
    }

    MusicApplication *w = MusicApplication::instance();
    if(m_leftSideByOn)
    {
        m_leftSideByOn = false;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
    }
    else if(m_rightSideByOn)
    {
        const QRect &rect = TTKDesktopWrapper::screenGeometry();
        m_rightSideByOn = false;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(rect.width() - w->width() - MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
    }
}

void MusicApplicationModule::sideAnimationReset()
{
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool())
    {
        return;
    }

    if(m_leftSideByOn)
    {
        MusicApplication *w = MusicApplication::instance();
        w->move(MARGIN_SIDE_BY, w->y());
    }
    else if(m_rightSideByOn)
    {
        MusicApplication *w = MusicApplication::instance();
        const QRect &rect = TTKDesktopWrapper::screenGeometry();
        w->move(rect.width() - w->width() - MARGIN_SIDE_BY, w->y());
    }
}

void MusicApplicationModule::windowCloseAnimationFinished()
{
    if(!m_quitContainer)
    {
        MusicApplication *w = MusicApplication::instance();
        w->hide();

        m_quitContainer = new QWidget;
        m_quitContainer->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        m_quitContainer->setAttribute(Qt::WA_TranslucentBackground);

        MusicGifLabelWidget *gifWidget = new MusicGifLabelWidget(m_quitContainer);
        gifWidget->setType(MusicGifLabelWidget::Gif_Close_White);
        gifWidget->setInterval(25 * MT_MS);
        gifWidget->setInfinited(false);
        m_quitContainer->resize(gifWidget->size());

        const QPoint &center = w->geometry().center();
        m_quitContainer->move(QPoint(center.x() - m_quitContainer->width() / 2, center.y() - m_quitContainer->height() / 2));

        m_quitContainer->raise();
        m_quitContainer->show();
        gifWidget->start();
        gifWidget->show();
    }
}

void MusicApplicationModule::musicAboutUs()
{
    MusicMessageAboutDialog().exec();
}

void MusicApplicationModule::musicBugReportView()
{
    MusicUtils::Url::openUrl(MusicUtils::Algorithm::mdII(REPORT_BUG_URL, false), false);
}

void MusicApplicationModule::musicVersionUpdate()
{
    MusicSourceUpdateWidget(MusicApplication::instance()).exec();
}

void MusicApplicationModule::musicTimerWidget()
{
    QStringList list;
    MusicApplication::instance()->currentPlaylist(list);

    MusicTimerWidget timer;
    timer.setSongStringList(list);
    timer.exec();
}

void MusicApplicationModule::musicSpectrumWidget()
{
    GENERATE_SINGLE_WIDGET_CLASS(MusicSpectrumWidget);
}

void MusicApplicationModule::musicSetWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    Qt::WindowFlags flags = MusicApplication::instance()->windowFlags();
    MusicApplication::instance()->setWindowFlags(m_setWindowToTop ? (flags | Qt::WindowStaysOnTopHint) : (flags & ~Qt::WindowStaysOnTopHint));
    MusicApplication::instance()->show();
}

void MusicApplicationModule::musicResetWindow()
{
    m_leftSideByOn = false;
    m_rightSideByOn = false;

    const QRect &rect = TTKDesktopWrapper::screenGeometry();
    G_SETTING_PTR->setValue(MusicSettingManager::ScreenSize, rect.size());
    G_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, QSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN));

    QWidget *w = MusicApplication::instance();
    if(w->isMaximized() || w->isMinimized() || w->isFullScreen())
    {
        w->showNormal();
    }
    w->setGeometry((rect.width() - WINDOW_WIDTH_MIN) / 2, (rect.height() - WINDOW_HEIGHT_MIN) / 2, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
}

void MusicApplicationModule::musicToolSetsParameter()
{
    m_MusicTimerAutoModule->runTimerAutoConfig();
#ifdef Q_OS_WIN
    MusicPlatformManager platform;
    const int version = platform.windowsIEVersion();
    if(version == -1 || version < 8)
    {
        MusicToastLabel::popup(version == -1 ? QObject::tr("No ie core version detected!") : QObject::tr("IE core version less than 8!"));
    }
#endif
}

void MusicApplicationModule::musicDeviceNameChanged(const QString &name)
{
    G_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePath, name);
}

void MusicApplicationModule::musicDeviceChanged(bool state)
{
    delete m_mobileDeviceWidget;
    m_mobileDeviceWidget = nullptr;

    if(state)
    {
        m_mobileDeviceWidget = new MusicMobileDevicesWidget;
        m_mobileDeviceWidget->show();
    }
    else
    {
        G_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePath, QString());
    }
}

void MusicApplicationModule::musicSetEqualizer()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }

    MusicEqualizerDialog equalizer;
    equalizer.exec();
}

void MusicApplicationModule::musicSetSoundEffect()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }

    MusicSoundEffectsWidget sound;
    sound.setInputModule(this);
    sound.exec();
}

void MusicApplicationModule::musicEffectChanged()
{
    const QString &value = G_SETTING_PTR->value(MusicSettingManager::EnhancedEffectValue).toString();
#if TTK_QT_VERSION_CHECK(5,15,0)
    const QStringList &effects = value.split(";", Qt::SkipEmptyParts);
#else
    const QStringList &effects = value.split(";", QString::SkipEmptyParts);
#endif
    for(const QString &effect : qAsConst(effects))
    {
        MusicSoundEffectsItemWidget::soundEffectChanged(effect, true);
    }
}

bool MusicApplicationModule::closeCurrentEqualizer()
{
    if(G_SETTING_PTR->value(MusicSettingManager::EnhancedMusicIndex).toInt() != 0)
    {
        MusicMessageBox message;
        message.setText(tr("We are opening the magic sound, if you want to close?"));
        if(!message.exec())
        {
            return false;
        }

        Q_EMIT enhancedMusicChanged(0);
    }
    return true;
}
