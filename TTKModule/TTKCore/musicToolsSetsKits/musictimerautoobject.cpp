#include "musictimerautoobject.h"
#include "musicsettingmanager.h"
#include "musicnumberdefine.h"
#include "musicapplication.h"

#include <QTime>
#ifdef Q_OS_WIN
#  include <qt_windows.h>
#else
#  include <QProcess>
#endif

MusicTimerAutoObject::MusicTimerAutoObject(QObject *parent)
    : QObject(parent)
{
    MusicTimeObject play, stop, shutdown;
    play.m_index = 0;
    stop.m_index = 1;
    shutdown.m_index = 2;
    m_timeObjects << play << stop << shutdown;

    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicTimerAutoObject::~MusicTimerAutoObject()
{
    m_timer.stop();
}

void MusicTimerAutoObject::runTimerAutoConfig()
{
    m_timer.start(MT_S2MS);

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayChoiced).toInt() == 0)
    {
        m_timeObjects[0].m_state = true;
        m_timeObjects[0].m_hour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt();
        m_timeObjects[0].m_minute = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt();
    }

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopChoiced).toInt() == 0)
    {
        m_timeObjects[1].m_state = true;
        m_timeObjects[1].m_hour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt();
        m_timeObjects[1].m_minute = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt();
    }

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt() == 0)
    {
        m_timeObjects[2].m_state = true;
        m_timeObjects[2].m_hour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt();
        m_timeObjects[2].m_minute = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt();
    }
}

void MusicTimerAutoObject::timeout()
{
    int hour = 0, minute = 0;
    const QStringList &l = QTime::currentTime().toString(Qt::ISODate).split(':');
    if(l.count() != 3)
    {
        return;
    }

    hour = l[0].toInt();
    minute = l[1].toInt();

    for(int i=0; i<m_timeObjects.count(); ++i)
    {
        MusicTimeObject *pair = &m_timeObjects[i];
        if(pair->m_state && hour == pair->m_hour && minute == pair->m_minute)
        {
            switch(pair->m_index)
            {
                case 0:
                    {
                        if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt() == 0)
                        {
                            pair->m_state = false;
                            M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayChoiced, 1);
                        }
                        MusicApplication::instance()->setPlaySongChanged(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt());
                    }
                    break;
                case 1:
                    {
                        if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt() == 0)
                        {
                            pair->m_state = false;
                            M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopChoiced, 1);
                        }
                        MusicApplication::instance()->setStopSongChanged();
                    }
                    break;
                case 2:
                    {
                        if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt() == 0)
                        {
                            pair->m_state = false;
                            M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownChoiced, 1);
                        }
                        setShutdown();
                    }
                    break;
                default: break;
            }
        }
    }
}

void MusicTimerAutoObject::setShutdown()
{
#ifdef Q_OS_WIN
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, MStatic_cast(PTOKEN_PRIVILEGES, nullptr), 0);
        ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF, 0);
    }
#elif defined Q_OS_UNIX
    QProcess::execute("shutdown", QStringList() << "now");
#endif
    M_LOGGER_INFO("shutdown now");
}
