#include "musicapplicationmodule.h"
#include "musicmobiledeviceswidget.h"
#include "musictimerwidget.h"
#include "musicspectrumwidget.h"
#include "musictimerautomodule.h"
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
#include "musicsourceupdaterequest.h"
#include "musicdownloadcounterpvrequest.h"
#include "musicscreensaverwidget.h"
#include "ttkdesktopwrapper.h"
#include "ttkglobalhelper.h"

#include "qdevicewatcher.h"
#include "qsync/qsyncconfig.h"

#define MARGIN_SIDE     10
#define MARGIN_SIDE_GAP 2
#define SYNC_HOST_URL   "VDVnYUdYMW9xNnVWSnd6L0J6NHI2MFZ5d0R3R2NiRVF4VW5WckpNcUhnUT0="

MusicApplicationModule *MusicApplicationModule::m_instance = nullptr;

MusicApplicationModule::MusicApplicationModule(QObject *parent)
    : QObject(parent),
      m_setWindowToTop(false),
      m_direction(TTKObject::Direction::No),
      m_mobileDeviceWidget(nullptr),
      m_quitContainer(nullptr),
      m_screenSaverWidget(nullptr)
{
    m_instance = this;

    musicResetWindow();

    m_quitAnimation = new QPropertyAnimation(this);
    m_sideAnimation = new QPropertyAnimation(parent, "pos", this);
    m_quitAnimation->setTargetObject(parent);
    m_sideAnimation->setDuration(250 * MT_MS);

    m_timerAutoModule = new MusicTimerAutoModule(this);

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

    delete m_timerAutoModule;
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

void MusicApplicationModule::cleanup()
{
    if(G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toBool() && m_direction != TTKObject::Direction::No)
    {
        m_direction = TTKObject::Direction::No;
        MusicApplication *w = MusicApplication::instance();
        const QRect &rect = TTKDesktopWrapper::screenGeometry();
        w->move((rect.width() - w->width()) / 2, w->y());
    }
}

bool MusicApplicationModule::isLastedVersion() const
{
    return m_sourceUpdatehread->isLastedVersion();
}

void MusicApplicationModule::loadNetWorkSetting()
{
#ifndef QT_NO_SSL
    // ssl support check
    TTK_INFO_STREAM(QString("Application network support ssl: %1").arg(QSslSocket::supportsSsl() ? "true" : "false"));
#endif
    // sync host init
    QSyncConfig::HOST = MusicUtils::Algorithm::mdII(SYNC_HOST_URL, false);
    //
    m_sourceUpdatehread->startRequest();
    m_counterPVThread->startRequest();
}

void MusicApplicationModule::applyParameter()
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
    m_screenSaverWidget->applyParameter();
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
        m_quitAnimation->setDuration(250 * MT_MS);
        m_quitAnimation->setStartValue(w->geometry());
        m_quitAnimation->setEndValue(QRect(w->x(), w->geometry().center().y(), w->width(), 0));
        m_quitAnimation->start();
        QTimer::singleShot(m_quitAnimation->duration(), this, SLOT(windowCloseAnimationFinished()));
    }
    else
    {
        float v = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
              v = MusicUtils::Image::reRenderValue<float>(1, 0.35, MV_MAX - v);
        m_quitAnimation->stop();
        m_quitAnimation->setPropertyName("windowOpacity");
        m_quitAnimation->setDuration(MT_S2MS);
        m_quitAnimation->setStartValue(v);
        m_quitAnimation->setEndValue(0);
        m_quitAnimation->start();
    }

    QTimer::singleShot(MT_S2MS, this, SLOT(quit()));
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

    const int &lpx = w->x();
    const int &rpx = w->x() + w->width();
    const int &tpy = w->y();
    const int &bpy = w->y() + w->height();
    const QRect &rect = TTKDesktopWrapper::screenGeometry();
    const TTKObject::Direction direction = TTKDesktopWrapper::screenTaskbar().m_direction;

    if(-MARGIN_SIDE <= lpx && lpx <= MARGIN_SIDE && direction != TTKObject::Direction::Left)
    {
        m_direction = TTKObject::Direction::Left;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(-w->width() + MARGIN_SIDE_GAP, w->y()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
    else if(-MARGIN_SIDE + rect.width() <= rpx && rpx <= MARGIN_SIDE + rect.width() && direction != TTKObject::Direction::Right)
    {
        m_direction = TTKObject::Direction::Right;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(rect.width() - MARGIN_SIDE_GAP, w->y()));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
    else if(-MARGIN_SIDE <= tpy && tpy <= MARGIN_SIDE && direction != TTKObject::Direction::Top)
    {
        m_direction = TTKObject::Direction::Top;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(w->x(), -w->height() + MARGIN_SIDE_GAP));
        m_sideAnimation->start();
        G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, true);
    }
    else if(-MARGIN_SIDE + rect.height() <= bpy && bpy <= MARGIN_SIDE + rect.height() && direction != TTKObject::Direction::Bottom)
    {
        m_direction = TTKObject::Direction::Bottom;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->pos());
        m_sideAnimation->setEndValue(QPoint(w->x(), rect.height() - MARGIN_SIDE_GAP));
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
    const QRect &rect = TTKDesktopWrapper::screenGeometry();

    switch(m_direction)
    {
        case TTKObject::Direction::Left:
        case TTKObject::Direction::Right:
        {
            m_sideAnimation->stop();
            m_sideAnimation->setStartValue(w->pos());
            m_sideAnimation->setEndValue(QPoint((rect.width() - w->width()) / 2, w->y()));
            m_sideAnimation->start();
            G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
            break;
        }
        case TTKObject::Direction::Top:
        case TTKObject::Direction::Bottom:
        {
            m_sideAnimation->stop();
            m_sideAnimation->setStartValue(w->pos());
            m_sideAnimation->setEndValue(QPoint(w->x(), (rect.height() - w->height()) / 2));
            m_sideAnimation->start();
            G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByInMode, false);
            break;
        }
        case TTKObject::Direction::LeftTop: break;
        case TTKObject::Direction::RightTop: break;
        case TTKObject::Direction::LeftBottom: break;
        case TTKObject::Direction::RightBottom: break;
        default: break;
    }

    m_direction = TTKObject::Direction::No;
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

void MusicApplicationModule::musicAbout()
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

    MusicTimerWidget widget;
    widget.setSongStringList(list);
    widget.exec();
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
    m_direction = TTKObject::Direction::No;

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
    m_timerAutoModule->runTimerAutoConfig();
#ifdef Q_OS_WIN
    MusicPlatformManager platform;
    const int version = platform.windowsIEVersion();
    if(version == -1 || version < 8)
    {
        MusicToastLabel::popup(version == -1 ? QObject::tr("No ie core version detected") : QObject::tr("IE core version less than 8"));
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

    MusicEqualizerDialog widget;
    widget.exec();
}

void MusicApplicationModule::musicSetSoundEffect()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }

    MusicSoundEffectsWidget widget;
    widget.setInputModule(this);
    widget.exec();
}

void MusicApplicationModule::musicEffectChanged()
{
    const QString &value = G_SETTING_PTR->value(MusicSettingManager::EnhancedEffectValue).toString();
    const QStringList &effects = value.split(";", QtSkipEmptyParts);
    for(const QString &effect : qAsConst(effects))
    {
        MusicUtils::TTKQmmp::enabledEffectPlugin(true, effect);
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
