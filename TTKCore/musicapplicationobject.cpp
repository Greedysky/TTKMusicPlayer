#include "musicapplicationobject.h"
#ifdef Q_OS_WIN
# include <Windows.h>
# include <Dbt.h>
#endif
#include "musicmobiledeviceswidget.h"
#include "musicaudiorecorderwidget.h"
#include "musictimerwidget.h"
#include "musictimerautoobject.h"
#include "musicmessagebox.h"
#include "musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicregeditmanager.h"
#include "musicmobiledevicesthread.h"
#include "musicsourceupdatewidget.h"
#include "musicsoundeffectswidget.h"
#include "musicmessageaboutdialog.h"
#include "musicnumberdefine.h"
#include "musicapplication.h"
#include "musictopareawidget.h"

#include <QPropertyAnimation>
#include <QApplication>
#include <QDesktopWidget>

MusicApplicationObject *MusicApplicationObject::m_instance = nullptr;

MusicApplicationObject::MusicApplicationObject(QObject *parent)
    : QObject(parent), m_mobileDevices(nullptr)
{
    Q_INIT_RESOURCE(MusicPlayer);
    m_instance = this;

    musicResetWindow();
    windowStartAnimationOpacity();

    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
#ifdef Q_OS_UNIX
    m_mobileDevicesLinux = new MusicMobileDevicesThread(this);
    connect(m_mobileDevicesLinux, SIGNAL(devicesChanged(bool)), SLOT(musicDevicesLinuxChanged(bool)));
    m_mobileDevicesLinux->start();
#elif defined Q_OS_WIN
    m_mobileDevicesLinux = nullptr;
#endif
    m_setWindowToTop = false;

    musicToolSetsParameter();
}

MusicApplicationObject::~MusicApplicationObject()
{
    Q_CLEANUP_RESOURCE(MusicPlayer);
    delete m_mobileDevicesLinux;
    delete m_mobileDevices;
    delete m_musicTimerAutoObj;
    delete m_animation;
}

QString MusicApplicationObject::getClassName()
{
    return staticMetaObject.className();
}

MusicApplicationObject *MusicApplicationObject::instance()
{
    return m_instance;
}

void MusicApplicationObject::getParameterSetting()
{
#ifdef Q_OS_WIN
    if(M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt())
    {
        MusicRegeditManager regeditManager;
        regeditManager.setMusicRegeditAssociateFileIcon();
    }
#endif
}

void MusicApplicationObject::windowStartAnimationOpacity()
{
    m_animation = new QPropertyAnimation(MusicApplication::instance(), "windowOpacity", this);
    m_animation->setDuration(MT_S2MS);
    m_animation->setStartValue(0);
    m_animation->setEndValue(1);
    m_animation->start();
    QTimer::singleShot(MT_S2MS, this, SLOT(musicBackgroundSliderStateChanged()));
}

void MusicApplicationObject::windowCloseAnimationOpacity()
{
    m_animation->stop();
    m_animation->setDuration(MT_S2MS);
    m_animation->setStartValue(1);
    m_animation->setEndValue(0);
    m_animation->start();
    QTimer::singleShot(MT_S2MS, qApp, SLOT(quit()));
}

#if defined(Q_OS_WIN)
#  ifdef MUSIC_GREATER_NEW
void MusicApplicationObject::nativeEvent(const QByteArray &,
                                         void *message, long *)
{
    MSG* msg = MReinterpret_cast(MSG*, message);
#  else
void MusicApplicationObject::winEvent(MSG *msg, long *)
{
#  endif
    if(msg->message == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
            case DBT_DEVICETYPESPECIFIC:
                break;
            case DBT_DEVICEARRIVAL:
                if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if (lpdbv->dbcv_flags == 0)
                    {
                        DWORD unitmask = lpdbv ->dbcv_unitmask;
                        int i;
                        for(i = 0; i < 26; ++i)
                        {
                            if(unitmask & 0x1)
                                break;
                            unitmask = unitmask >> 1;
                        }
                        QString dev((char)(i + 'A'));
                        M_LOGGER_INFO(QString("USB_Arrived and The USBDisk is: %1").arg(dev));
                        M_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePathChoiced, dev + ":/");
                        delete m_mobileDevices;
                        m_mobileDevices = new MusicMobileDevicesWidget;
                        m_mobileDevices->show();
                    }
                }
                break;
            case DBT_DEVICEREMOVECOMPLETE:
                if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if (lpdbv -> dbcv_flags == 0)
                    {
                        M_LOGGER_INFO("USB_remove");
                        M_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePathChoiced, QString());
                        delete m_mobileDevices;
                        m_mobileDevices = nullptr;
                    }
                }
                break;
            default: break;
        }
    }
}
#endif

void MusicApplicationObject::musicAboutUs()
{
    MusicMessageAboutDialog().exec();
}

void MusicApplicationObject::musicVersionUpdate()
{
    MusicSourceUpdateWidget(MusicApplication::instance()).exec();
}

void MusicApplicationObject::musicAudioRecorder()
{
    MusicAudioRecorderWidget recorder;
    recorder.exec();
}

void MusicApplicationObject::musicTimerWidget()
{
    MusicTimerWidget timer;
    QStringList list;
    MusicApplication::instance()->getCurrentPlayList(list);
    timer.setSongStringList(list);
    timer.exec();
}

void MusicApplicationObject::musicSetWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    Qt::WindowFlags flags = MusicApplication::instance()->windowFlags();
    MusicApplication::instance()->setWindowFlags( m_setWindowToTop ?
                              (flags | Qt::WindowStaysOnTopHint) :
                              (flags & ~Qt::WindowStaysOnTopHint) );
    MusicApplication::instance()->show();
}

void MusicApplicationObject::musicResetWindow()
{
    QWidget *widget = QApplication::desktop();
    M_SETTING_PTR->setValue(MusicSettingManager::ScreenSize, widget->size());
    M_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, QSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN));

    MusicApplication::instance()->setGeometry((widget->width() - WINDOW_WIDTH_MIN)/2,
                                (widget->height() - WINDOW_HEIGHT_MIN)/2,
                                WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
}

void MusicApplicationObject::musicToolSetsParameter()
{
    m_musicTimerAutoObj->runTimerAutoConfig();
}

void MusicApplicationObject::musicDevicesLinuxChanged(bool state)
{
    delete m_mobileDevices;
    m_mobileDevices = nullptr;
    if(state)
    {
        m_mobileDevices = new MusicMobileDevicesWidget;
        m_mobileDevices->show();
    }
}

void MusicApplicationObject::musicSetEqualizer()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }
    MusicEqualizerDialog equalizer;
    equalizer.exec();
}

void MusicApplicationObject::musicSetSoundEffect()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }
    MusicSoundEffectsWidget sound;
    sound.setParentConnect(this);
    sound.exec();
}

void MusicApplicationObject::musicBackgroundSliderStateChanged()
{
    MusicTopAreaWidget::instance()->musicBackgroundSliderStateChanged(false);
}

bool MusicApplicationObject::closeCurrentEqualizer()
{
    if(M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt() != 0)
    {
        MusicMessageBox message;
        message.setText(tr("we are opening the magic sound, if you want to close?"));
        if(message.exec())
        {
            return false;
        }
        emit enhancedMusicChanged(0);
    }
    return true;
}
