#include "musicapplicationmodule.h"
#include "musicmobiledeviceswidget.h"
#include "musicfileassociationwidget.h"
#include "musictimerwidget.h"
#include "musicspectrumwidget.h"
#include "musictimerautomodule.h"
#include "musicbackupmodule.h"
#include "musicmessagebox.h"
#include "musictoastlabel.h"
#include "musicequalizerdialog.h"
#include "musicplatformmanager.h"
#include "musicsourceupdatewidget.h"
#include "musicsoundeffectswidget.h"
#include "musicmessageaboutdialog.h"
#include "musicapplication.h"
#include "musictopareawidget.h"
#include "musicimageutils.h"
#include "musicgiflabelwidget.h"
#include "musicfileutils.h"
#include "musicresourcerequest.h"
#include "musicpvcounterrequest.h"
#include "musicsourceupdaterequest.h"
#include "musicscreensaverwidget.h"
#include "musichotkeymanager.h"
#include "ttklibrary.h"
#include "ttklogoutput.h"
#include "ttkconcurrent.h"
#include "ttkdesktopscreen.h"
#include "ttkfileassociation.h"
#include "ttkplatformsystem.h"

#include "qdevicewatcher.h"
#include "qsync/qsyncconfig.h"

static constexpr int MARGIN_SIDE = 10;
static constexpr const char *SYNC_HOST_URL = "VDVnYUdYMW9xNnVWSnd6L0J6NHI2MFZ5d0R3R2NiRVF4VW5WckpNcUhnUT0=";

MusicApplicationModule *MusicApplicationModule::m_instance = nullptr;

MusicApplicationModule::MusicApplicationModule(QObject *parent)
    : QObject(parent),
      m_setWindowToTop(false),
      m_direction(TTK::Direction::No),
      m_mobileDeviceWidget(nullptr),
      m_quitContainer(nullptr),
      m_screenSaverWidget(nullptr)
{
    m_instance = this;

    resetWindowGeometry();

    m_quitAnimation = new QPropertyAnimation(this);
    m_sideAnimation = new QPropertyAnimation(parent, "pos", this);
    m_quitAnimation->setTargetObject(parent);
    m_sideAnimation->setDuration(TTK_DN_S2MS / 4);

    m_backupModule = new MusicBackupModule;
    m_timerAutoModule = new MusicTimerAutoModule(this);

    m_deviceWatcher = new QDeviceWatcher(this);
    connect(m_deviceWatcher, SIGNAL(deviceChanged(bool)), SLOT(deviceChanged(bool)));
    connect(m_deviceWatcher, SIGNAL(deviceAdded(QString)), SLOT(deviceNameChanged(QString)));
    m_deviceWatcher->appendEventReceiver(this);
    m_deviceWatcher->start();

    m_resourceRequest = new MusicResourceRequest(this);
    m_counterRequest = new MusicPVCounterRequest(this);
    m_sourceUpdateRequest = new MusicSourceUpdateRequest(this);

    runTimerAutoModule();
}

MusicApplicationModule::~MusicApplicationModule()
{
    Q_CLEANUP_RESOURCE(TTKModule);
    G_HOTKEY_PTR->unsetShortcut();
    m_deviceWatcher->stop();
    //
    delete m_backupModule;
    delete m_timerAutoModule;
    delete m_screenSaverWidget;
    delete m_quitAnimation;
    delete m_sideAnimation;
    delete m_deviceWatcher;
    delete m_mobileDeviceWidget;
    delete m_quitContainer;
    delete m_resourceRequest;
    delete m_counterRequest;
    delete m_sourceUpdateRequest;
}

MusicApplicationModule *MusicApplicationModule::instance()
{
    return m_instance;
}

void MusicApplicationModule::cleanup()
{
    if(G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool() && m_direction != TTK::Direction::No)
    {
        m_direction = TTK::Direction::No;
        MusicApplication *w = MusicApplication::instance();
        const QRect &rect = TTKDesktopScreen::screenGeometry();
        w->move((rect.width() - w->width()) / 2, w->y());
    }
}

bool MusicApplicationModule::isLastedVersion() const
{
    return m_sourceUpdateRequest->isLastedVersion();
}

void MusicApplicationModule::loadNetWorkSetting()
{
#ifndef QT_NO_SSL
    // ssl support check
    TTK_INFO_STREAM("Application network support ssl:" << (QSslSocket::supportsSsl() ? "true" : "false"));
#endif
    // sync host init
    QSyncConfig::HOST = TTK::Algorithm::mdII(SYNC_HOST_URL, false);
    //
    m_resourceRequest->startToRequest();
    m_counterRequest->startToRequest();
    m_sourceUpdateRequest->startToRequest();
}

void MusicApplicationModule::windowCloseAnimation()
{
    sideAnimationByOff();

    if(G_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toBool())
    {
        MusicTopAreaWidget::instance()->setBackgroundAnimation(false);
        MusicApplication *w = MusicApplication::instance();
        w->setMinimumSize(0, 0); ///remove fixed size

        m_quitAnimation->stop();
        m_quitAnimation->setPropertyName("geometry");
        m_quitAnimation->setDuration(TTK_DN_S2MS / 4);
        m_quitAnimation->setStartValue(w->geometry());
        m_quitAnimation->setEndValue(QRect(w->x(), w->geometry().center().y(), w->width(), 0));
        m_quitAnimation->start();
        TTK_SIGNLE_SHOT(m_quitAnimation->duration(), this, windowCloseAnimationFinished, TTK_SLOT);
    }
    else
    {
        float v = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
              v = TTK::Image::boundValue<float>(1.0f, 0.35f, TTK_RN_MAX - v);

        m_quitAnimation->stop();
        m_quitAnimation->setPropertyName("windowOpacity");
        m_quitAnimation->setDuration(TTK_DN_S2MS);
        m_quitAnimation->setStartValue(v);
        m_quitAnimation->setEndValue(0.0f);
        m_quitAnimation->start();
    }

    TTK_SIGNLE_SHOT(TTK_DN_S2MS, this, quit, TTK_SLOT);
}

void MusicApplicationModule::soureUpdateCheck()
{
    MusicSourceUpdateNotifyWidget *widget = new MusicSourceUpdateNotifyWidget;
    widget->show();
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

    const int lpx = w->x();
    const int rpx = w->x() + w->width();
    const int tpy = w->y();
    const int bpy = w->y() + w->height();
#ifdef Q_OS_WIN
    constexpr int gap = 2;
#elif defined Q_OS_UNIX
    const int gap = TTKPlatformSystem::systemName() == TTKPlatformSystem::System::LinuxUbuntu ? 3 : 2;
#endif
    const QRect &rect = TTKDesktopScreen::screenGeometry();
    const TTK::Direction direction = TTKDesktopScreen::screenTaskbar().m_direction;

    if(direction != TTK::Direction::Left && -MARGIN_SIDE <= lpx && lpx <= MARGIN_SIDE)
    {
        m_direction = TTK::Direction::Left;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(-w->width() + gap, w->y()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
    else if(direction != TTK::Direction::Right && -MARGIN_SIDE + rect.width() <= rpx && rpx <= MARGIN_SIDE + rect.width())
    {
        m_direction = TTK::Direction::Right;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(rect.width() - gap, w->y()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
    else if(direction != TTK::Direction::Top && -MARGIN_SIDE <= tpy && tpy <= MARGIN_SIDE)
    {
        m_direction = TTK::Direction::Top;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(w->x(), -w->height() + gap));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
    else if(direction != TTK::Direction::Bottom && -MARGIN_SIDE + rect.height() <= bpy && bpy <= MARGIN_SIDE + rect.height())
    {
        m_direction = TTK::Direction::Bottom;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(w->x(), rect.height() - gap));
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
    const QRect &rect = TTKDesktopScreen::screenGeometry();

    switch(m_direction)
    {
        case TTK::Direction::Left:
        case TTK::Direction::Right:
        {
            m_sideAnimation->stop();
            m_sideAnimation->setStartValue(w->pos());
            m_sideAnimation->setEndValue(QPoint((rect.width() - w->width()) / 2, w->y()));
            m_sideAnimation->start();
            G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
            break;
        }
        case TTK::Direction::Top:
        case TTK::Direction::Bottom:
        {
            m_sideAnimation->stop();
            m_sideAnimation->setStartValue(w->pos());
            m_sideAnimation->setEndValue(QPoint(w->x(), (rect.height() - w->height()) / 2));
            m_sideAnimation->start();
            G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
            break;
        }
        case TTK::Direction::LeftTop: break;
        case TTK::Direction::RightTop: break;
        case TTK::Direction::LeftBottom: break;
        case TTK::Direction::RightBottom: break;
        default: break;
    }

    m_direction = TTK::Direction::No;
}

void MusicApplicationModule::soundEffectChanged()
{
    const QString &value = G_SETTING_PTR->value(MusicSettingManager::EnhancedEffectValue).toString();
    const QStringList &effects = value.split(";", QtSkipEmptyParts);
    for(const QString &effect : qAsConst(effects))
    {
        TTK::TTKQmmp::enabledEffectPlugin(true, effect);
    }
}

void MusicApplicationModule::applyParameter()
{
#ifdef Q_OS_WIN
    QFile file(TTK::applicationPath() + TTK_QT_CONFIG);
    if(file.open(QIODevice::ReadWrite))
    {
        if(!(G_SETTING_PTR->value(MusicSettingManager::OtherHighDpiScalingEnable).toBool() ? (file.write("[Platforms]\nWindowsArguments = dpiawareness=0\n") != -1) : file.remove()))
        {
            G_SETTING_PTR->setValue(MusicSettingManager::OtherHighDpiScalingEnable, Qt::PartiallyChecked);
        }
        file.close();
    }

    MusicPlatformManager manager;
    manager.windowsStartUpMode(G_SETTING_PTR->value(MusicSettingManager::StartUpMode).toBool());

    TTKConcurrent(
    {
        TTKFileAssociation association;
        const QStringList &keys = association.keys();

        for(const QString &format : TTK::unsupportAssociations())
        {
            const bool exist = keys.contains(format) && association.exist(format);
            if(exist)
            {
                 association.remove(format);
            }
        }

        for(const QString &format : TTK::supportAssociations())
        {
            const bool exist = keys.contains(format) && association.exist(format);
            const bool enable = G_SETTING_PTR->value(MusicSettingManager::FileAssociationMode).toBool();

            if(exist && !enable)
            {
                 association.remove(format);
            }
            else if(!exist && enable)
            {
                 association.append(format);
            }
        }
    });
#endif
    //
    TTK::initiailizeLog(TTK_APP_NAME);
    G_SETTING_PTR->value(MusicSettingManager::OtherLogTrackEnable).toBool() ? TTK::installLogHandler() : TTK::removeLogHandler();
    //
    if(!m_screenSaverWidget)
    {
        m_screenSaverWidget = new MusicScreenSaverBackgroundWidget;
    }
    m_screenSaverWidget->execute();
    //
    m_backupModule->execute();
}

void MusicApplicationModule::quit()
{
    qApp->exit();
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
        gifWidget->setType(MusicGifLabelWidget::Module::CloseWhite);
        gifWidget->setInterval(25 * TTK_DN_MS);
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

void MusicApplicationModule::showAboutWidget()
{
    MusicMessageAboutDialog().exec();
}

void MusicApplicationModule::showBugReportView()
{
    TTK::Url::openUrl(TTK::Algorithm::mdII(REPORT_BUG_URL, false), false);
}

void MusicApplicationModule::showVersionWidget()
{
    MusicSourceUpdateWidget(MusicApplication::instance()).exec();
}

void MusicApplicationModule::showTimerWidget()
{
    QStringList list;
    MusicApplication::instance()->currentPlaylist(list);

    MusicTimerWidget widget;
    widget.setSongStringList(list);
    widget.exec();
}

void MusicApplicationModule::showSpectrumWidget()
{
    TTKGenerateSingleWidget(MusicSpectrumWidget);
}

void MusicApplicationModule::setWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    const Qt::WindowFlags flags = MusicApplication::instance()->windowFlags();
    MusicApplication::instance()->setWindowFlags(m_setWindowToTop ? (flags | Qt::WindowStaysOnTopHint) : (flags & ~Qt::WindowStaysOnTopHint));
    MusicApplication::instance()->show();
}

void MusicApplicationModule::resetWindowGeometry()
{
    m_direction = TTK::Direction::No;

    const QRect &rect = TTKDesktopScreen::screenGeometry();
    G_SETTING_PTR->setValue(MusicSettingManager::ScreenSize, rect.size());
    G_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, QSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN));

    QWidget *w = MusicApplication::instance();
    if(w->isMaximized() || w->isMinimized() || w->isFullScreen())
    {
        w->showNormal();
    }
    w->setGeometry((rect.width() - WINDOW_WIDTH_MIN) / 2, (rect.height() - WINDOW_HEIGHT_MIN) / 2, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
}

void MusicApplicationModule::runTimerAutoModule()
{
    m_timerAutoModule->execute();
}

void MusicApplicationModule::deviceNameChanged(const QString &name)
{
    G_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePath, name);
}

void MusicApplicationModule::deviceChanged(bool state)
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

void MusicApplicationModule::showEqualizerWidget()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }

    MusicEqualizerDialog widget;
    widget.exec();
}

void MusicApplicationModule::showSoundEffectWidget()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }

    MusicSoundEffectsWidget widget;
    widget.setInputModule(this);
    widget.exec();
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

        Q_EMIT enhancedSongChanged(0);
    }
    return true;
}
