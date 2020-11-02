#include "musictimerautoobject.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "musiccoreutils.h"

#include <QTime>
#ifdef Q_OS_WIN
#  include <qt_windows.h>
#else
#  include <signal.h>
#  include <unistd.h>
#  include <sys/reboot.h>
#endif

MusicTimerAutoObject::MusicTimerAutoObject(QObject *parent)
    : QObject(parent)
{
    MusicTimeData play, stop, shutdown;
    play.m_index = 0;
    stop.m_index = 1;
    shutdown.m_index = 2;
    m_timeDatas << play << stop << shutdown;

    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicTimerAutoObject::~MusicTimerAutoObject()
{
    m_timer.stop();
}

void MusicTimerAutoObject::runTimerAutoConfig()
{
    m_timer.start(MT_S2MS);

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlay).toInt() == 0)
    {
        m_timeDatas[0].m_state = true;
        m_timeDatas[0].m_hour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHour).toInt();
        m_timeDatas[0].m_minute = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecond).toInt();
    }

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStop).toInt() == 0)
    {
        m_timeDatas[1].m_state = true;
        m_timeDatas[1].m_hour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHour).toInt();
        m_timeDatas[1].m_minute = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecond).toInt();
    }

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdown).toInt() == 0)
    {
        m_timeDatas[2].m_state = true;
        m_timeDatas[2].m_hour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHour).toInt();
        m_timeDatas[2].m_minute = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecond).toInt();
    }
}

void MusicTimerAutoObject::timeout()
{
    int hour = 0, minute = 0;
    const QStringList &l = QTime::currentTime().toString(Qt::ISODate).split(":");
    if(l.count() != 3)
    {
        return;
    }

    hour = l[0].toInt();
    minute = l[1].toInt();

    for(int i=0; i<m_timeDatas.count(); ++i)
    {
        MusicTimeData *pair = &m_timeDatas[i];
        if(pair->m_state && hour == pair->m_hour && minute == pair->m_minute)
        {
            switch(pair->m_index)
            {
                case 0:
                    {
                        if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeat).toInt() == 0)
                        {
                            pair->m_state = false;
                            M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlay, 1);
                        }
                        MusicApplication::instance()->setPlaySongChanged(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndex).toInt());
                        break;
                    }
                case 1:
                    {
                        if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeat).toInt() == 0)
                        {
                            pair->m_state = false;
                            M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStop, 1);
                        }
                        MusicApplication::instance()->setStopSongChanged();
                        break;
                    }
                case 2:
                    {
                        if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeat).toInt() == 0)
                        {
                            pair->m_state = false;
                            M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdown, 1);
                        }
                        setShutdown();
                        break;
                    }
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
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, TTKStatic_cast(PTOKEN_PRIVILEGES, nullptr), nullptr);
        ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF, 0);
    }
#elif defined Q_OS_UNIX
    /* first disable all our signals */
    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_BLOCK, &set, nullptr);
    /* send signals to all processes  _except_ pid 1 */
    kill(-1, SIGTERM);
    sync();
    MusicUtils::Core::sleep(3 * MT_S2MS);

    kill(-1, SIGKILL);
    sync();
    MusicUtils::Core::sleep(3 * MT_S2MS);
    /* shutdown */
    reboot(RB_POWER_OFF);
#endif
    TTK_LOGGER_INFO("shutdown now");
}
