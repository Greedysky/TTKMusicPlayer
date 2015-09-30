#include "musicapplicationobject.h"
#include <Windows.h>
#include <Dbt.h>
#include "musicmobiledeviceswidget.h"
#include "musicaudiorecorderwidget.h"
#include "musictimerwidget.h"
#include "musictimerautoobject.h"
#include "musicmessagebox.h"
#include "musicequalizerdialog.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QPropertyAnimation>
#include <QApplication>
#include <QDebug>

MusicApplicationObject::MusicApplicationObject(QObject *parent)
    : QObject(parent), m_mobileDevices(NULL)
{
    m_supperClass = static_cast<QWidget*>(parent);

    windowStartAnimationOpacity();

    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
    connect(m_musicTimerAutoObj, SIGNAL(setPlaySong(int)), parent,
                                 SLOT(setPlaySongChanged(int)));
    connect(m_musicTimerAutoObj, SIGNAL(setStopSong()), parent,
                                 SLOT(setStopSongChanged()));

    m_setWindowToTop = false;
    M_Connection->setValue("MusicApplicationObject", this);
    M_Connection->connect("MusicApplicationObject", "MusicApplication");
    M_Connection->connect("MusicApplicationObject", "MusicEnhancedWidget");

    musicToolSetsParameter();
}

MusicApplicationObject::~MusicApplicationObject()
{
    delete m_mobileDevices;
    delete m_musicTimerAutoObj;
    delete m_animation;
}

void MusicApplicationObject::windowStartAnimationOpacity()
{
    m_animation = new QPropertyAnimation(m_supperClass, "windowOpacity");
    m_animation->setDuration(1000);
    m_animation->setStartValue(0);
    m_animation->setEndValue(1);
    m_animation->start();
}

void MusicApplicationObject::windowCloseAnimationOpacity()
{
    m_animation->stop();
    m_animation->setDuration(1000);
    m_animation->setStartValue(1);
    m_animation->setEndValue(0);
    m_animation->start();
    QTimer::singleShot(1000, qApp, SLOT(quit()));
}

void MusicApplicationObject::nativeEvent(const QByteArray &,
                                         void *message, long *)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
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
                        M_LOOGER << "USB_Arrived and The USBDisk is: "<<(char)(i + 'A');
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
                        M_LOOGER << "USB_remove";
                        delete m_mobileDevices;
                        m_mobileDevices = NULL;
                    }
                }
                break;
            default: break;
        }
    }
}

void MusicApplicationObject::musicAboutUs()
{
    MusicMessageBox message;
    message.setText(tr("QMusicPlayer") + QString("\n\n") +
                    tr("Directed By Greedysky") +
                    QString("\nCopyright© 2014-2015") +
                    QString("\nMail:Greedysky@163.com"));
    message.exec();
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
    if(M_SETTING->value(MusicSettingManager::EnhancedMusicChoiced).toInt() != 0)
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
