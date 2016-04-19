#include "musictimerautoobject.h"
#include "musicsettingmanager.h"

#include <QTime>
#include <QProcess>

#ifdef Q_OS_WIN
#   include <windows.h>
#endif

MusicTimerAutoObject::MusicTimerAutoObject(QObject *parent)
    : QObject(parent)
{
    m_hour = m_second = -1;
    m_funcIndex = -1;
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicTimerAutoObject::~MusicTimerAutoObject()
{
    m_timer.stop();
}

void MusicTimerAutoObject::runTimerAutoConfig()
{
    m_timer.start(1000);
    switch(m_funcIndex = M_SETTING->value(MusicSettingManager::TimerAutoIndexChoiced).toInt())
    {
        case 0:
            if(M_SETTING->value(MusicSettingManager::TimerAutoPlayChoiced).toInt() == 0)
            {
                m_hour = M_SETTING->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt();
                m_second = M_SETTING->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt();
            }
            break;
        case 1:
            if(M_SETTING->value(MusicSettingManager::TimerAutoStopChoiced).toInt() == 0)
            {
                m_hour = M_SETTING->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt();
                m_second = M_SETTING->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt();
            }
            break;
        case 2:
            if(M_SETTING->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt() == 0)
            {
                m_hour = M_SETTING->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt();
                m_second = M_SETTING->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt();
            }
            break;
        default: break;
    }
}

void MusicTimerAutoObject::timeout()
{
    int hour = 0,second = 0;
    QStringList l = QTime::currentTime().toString(Qt::ISODate).split(':');
    if(l.count() != 3 )
    {
        return;
    }

    hour = l[0].toInt();
    second = l[1].toInt();
    if( hour == m_hour && second == m_second )
    {
        m_hour = m_second = -1;
        if(m_funcIndex == 0)
        {
            if(M_SETTING->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt() == 0)
            {
               M_SETTING->setValue(MusicSettingManager::TimerAutoPlayChoiced, 1);
            }
            emit setPlaySong(M_SETTING->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt());
        }
        else if(m_funcIndex == 1)
        {
            if(M_SETTING->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt() == 0)
            {
               M_SETTING->setValue(MusicSettingManager::TimerAutoStopChoiced, 1);
            }
            emit setStopSong();
        }
        else if(m_funcIndex == 2)
        {
            if(M_SETTING->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt() == 0)
            {
               M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownChoiced, 1);
            }
            setShutdown();
        }
    }
}

bool MusicTimerAutoObject::systemIs64bit() const
{
    bool isWin64 = false;
#ifdef Q_OS_WIN
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
       si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64 )
    {
        isWin64 = true;
    }
    else
    {
        isWin64 = false;
    }
#elif defined Q_OS_UNIX
    isWin64 = QProcess::execute("uname", QStringList() << "-p");
#endif
    return isWin64;
}

void MusicTimerAutoObject::setShutdown()
{
#ifdef Q_OS_WIN
    bool isWind64 = systemIs64bit();
    QString item;
    QStringList lists = QProcess::systemEnvironment();
    foreach(QString var, lists)
    {
        item = var.toUpper();
        if(item.startsWith("PATH="))
        {
            lists = item.split(';');
            foreach(QString var, lists)
            {
                if(var.contains("\\WINDOWS\\SYSTEM32"))
                {   ///x86 or x64
                    item = var;
                    M_LOGGER_INFO(QString("x86_x64 %1").arg(item));
                    break;
                }
                if(isWind64 && var.contains("\\WINDOWS\\SYSWOW64"))
                {   ///x64
                    item = var;
                    M_LOGGER_INFO(QString("x64 %1").arg(item));
                    break;
                }
            }
            break;
        }
    }

    QString program = item + "\\shutdown.exe";
    QProcess::execute(program, QStringList() << "-s" << "-t" << "1");
#elif defined Q_OS_UNIX
    QProcess::execute("shutdown", QStringList() << "now");
#endif
    M_LOGGER_INFO("shutdown now");
}
