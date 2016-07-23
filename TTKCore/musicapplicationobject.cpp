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
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicregeditmanager.h"
#include "musicmobiledevicesthread.h"
#include "musicsourceupdatewidget.h"
#include "musicnumberdefine.h"
#include "musicapplication.h"
#include "musicenhancedwidget.h"

#include <QPropertyAnimation>
#include <QApplication>
#include <QDesktopWidget>

MusicApplicationObject::MusicApplicationObject(QObject *parent)
    : QObject(parent), m_mobileDevices(nullptr)
{
    m_supperClass = MStatic_cast(QWidget*, parent);
    QWidget *widget = QApplication::desktop();
    m_supperClass->move( (widget->width() - m_supperClass->width())/2,
                         (widget->height() - m_supperClass->height())/2 );
    M_SETTING_PTR->setValue(MusicSettingManager::ScreenSize, widget->size());
    M_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, QSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN));

    windowStartAnimationOpacity();
    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
    connect(m_musicTimerAutoObj, SIGNAL(setPlaySong(int)), parent,
                                 SLOT(setPlaySongChanged(int)));
    connect(m_musicTimerAutoObj, SIGNAL(setStopSong()), parent,
                                 SLOT(setStopSongChanged()));
#ifdef Q_OS_UNIX
    m_mobileDevicesLinux = new MusicMobileDevicesThread(this);
    connect(m_mobileDevicesLinux, SIGNAL(devicesChanged(bool)), SLOT(musicDevicesLinuxChanged(bool)));
    m_mobileDevicesLinux->start();
#elif defined Q_OS_WIN
    m_mobileDevicesLinux = nullptr;
#endif

    m_setWindowToTop = false;
    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicApplication::getClassName());
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicEnhancedWidget::getClassName());

    musicToolSetsParameter();
}

MusicApplicationObject::~MusicApplicationObject()
{
    delete m_mobileDevicesLinux;
    delete m_mobileDevices;
    delete m_musicTimerAutoObj;
    delete m_animation;
}

QString MusicApplicationObject::getClassName()
{
    return staticMetaObject.className();
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
    m_animation = new QPropertyAnimation(m_supperClass, "windowOpacity");
    m_animation->setDuration(MT_S2MS);
    m_animation->setStartValue(0);
    m_animation->setEndValue(1);
    m_animation->start();
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
                        M_SETTING_PTR->setValue(MusicSettingManager::MobileDevicePathChoiced, dev + ":/");
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
                        M_SETTING_PTR->setValue(MusicSettingManager::MobileDevicePathChoiced, QString());
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
    MusicMessageBox message;
    message.setText(tr("TTK Music Player") + QString("\n\n") +
                    tr("Directed By Greedysky") +
                    QString("\nCopyright© 2014-2016") +
                    QString("\nMail:Greedysky@163.com"));
    message.exec();
}

void MusicApplicationObject::musicVersionUpdate()
{
    MusicSourceUpdateWidget(m_supperClass).exec();
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
    emit getCurrentPlayList(list);
    timer.setSongStringList(list);
    timer.exec();
}

void MusicApplicationObject::musicSetWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    Qt::WindowFlags flags = m_supperClass->windowFlags();
    m_supperClass->setWindowFlags( m_setWindowToTop ?
                  (flags | Qt::WindowStaysOnTopHint) :
                  (flags & ~Qt::WindowStaysOnTopHint) );
    m_supperClass->show();
}

void MusicApplicationObject::musicToolSetsParameter()
{
    m_musicTimerAutoObj->runTimerAutoConfig();
}

void MusicApplicationObject::musicSetEqualizer()
{
    if(M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt() != 0)
    {
        MusicMessageBox message;
        message.setText(tr("we are opening the magic sound, if you want to close?"));
        if(message.exec())
        {
            return;
        }
        emit enhancedMusicChanged(0);
    }
    MusicEqualizerDialog eq;
    eq.exec();
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
