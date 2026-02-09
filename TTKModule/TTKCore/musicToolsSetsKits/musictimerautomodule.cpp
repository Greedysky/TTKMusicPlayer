#include "musictimerautomodule.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "musiccoreutils.h"

#include <QTime>
#ifdef Q_OS_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <qt_windows.h>
#else
#  include <signal.h>
#  include <unistd.h>
#  include <sys/reboot.h>
#endif

MusicTimerAutoModule::MusicTimerAutoModule(QObject *parent)
    : QObject(parent)
{
    Data play, stop, shutdown;
    play.m_index = 0;
    stop.m_index = 1;
    shutdown.m_index = 2;
    m_timeDatas << play << stop << shutdown;

    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicTimerAutoModule::~MusicTimerAutoModule()
{
    m_timer.stop();
}

void MusicTimerAutoModule::execute()
{
    m_timer.start(TTK_DN_S2MS);

    if(G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayMode).toInt() == 0)
    {
        m_timeDatas[0].m_state = true;
        m_timeDatas[0].m_hour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHour).toInt();
        m_timeDatas[0].m_minute = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecond).toInt();
    }

    if(G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopMode).toInt() == 0)
    {
        m_timeDatas[1].m_state = true;
        m_timeDatas[1].m_hour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHour).toInt();
        m_timeDatas[1].m_minute = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecond).toInt();
    }

    if(G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownMode).toInt() == 0)
    {
        m_timeDatas[2].m_state = true;
        m_timeDatas[2].m_hour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHour).toInt();
        m_timeDatas[2].m_minute = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecond).toInt();
    }
}

void MusicTimerAutoModule::timeout()
{
    int hour = 0, minute = 0;
    const QStringList &l = QTime::currentTime().toString(Qt::ISODate).split(":");
    if(l.count() != 3)
    {
        return;
    }

    hour = l[0].toInt();
    minute = l[1].toInt();

    for(int i = 0; i < m_timeDatas.count(); ++i)
    {
        Data *data = &m_timeDatas[i];
        if(data->m_state && hour == data->m_hour && minute == data->m_minute)
        {
            switch(data->m_index)
            {
                case 0:
                {
                    if(G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeat).toInt() == 0)
                    {
                        data->m_state = false;
                        G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayMode, 1);
                    }
                    MusicApplication::instance()->setPlaySongChanged(G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndex).toInt());
                    break;
                }
                case 1:
                {
                    if(G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeat).toInt() == 0)
                    {
                        data->m_state = false;
                        G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopMode, 1);
                    }
                    MusicApplication::instance()->switchToStopState();
                    break;
                }
                case 2:
                {
                    if(G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeat).toInt() == 0)
                    {
                        data->m_state = false;
                        G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownMode, 1);
                    }
                    setShutdown();
                    break;
                }
                default: break;
            }
        }
    }
}

void MusicTimerAutoModule::setShutdown()
{
#ifdef Q_OS_WIN
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, TTKStaticCast(PTOKEN_PRIVILEGES, nullptr), nullptr);
        ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF, 0);
    }
#elif defined Q_OS_LINUX
    /* first disable all our signals */
    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_BLOCK, &set, nullptr);
    /* send signals to all processes  _except_ pid 1 */
    kill(-1, SIGTERM);
    sync();
    TTK::Core::sleep(3 * TTK_DN_S2MS);

    kill(-1, SIGKILL);
    sync();
    TTK::Core::sleep(3 * TTK_DN_S2MS);
    /* shutdown */
    reboot(RB_POWER_OFF);
#elif defined Q_OS_MAC
    system("shutdown -h now");
#endif
    TTK_INFO_STREAM("Shutdown now");
}
