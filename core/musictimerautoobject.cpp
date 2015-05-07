#include "musictimerautoobject.h"
#include <QSettings>
#include <QTime>
#ifdef Q_OS_WIN
    #include <windows.h>
#endif
#include <QProcess>

MusicTimerAutoObject::MusicTimerAutoObject(QObject *parent)
            : QObject(parent)
{
    m_hour = m_second = -1;
    m_funcIndex = -1;
    connect(&m_timer,SIGNAL(timeout()),SLOT(timeout()));
}

MusicTimerAutoObject::~MusicTimerAutoObject()
{

}

void MusicTimerAutoObject::runTimerAutoConfig()
{
    m_timer.start(1000);
    QSettings setting;
    switch(m_funcIndex = setting.value("TIMERAUTOINDEXCHOICED").toInt())
    {
        case 0:
            if(setting.value("TIMERAUTOPLAYCHOICED").toInt() == 0)
            {
                m_hour = setting.value("TIMERAUTOPLAYHOURCHOICED").toInt();
                m_second = setting.value("TIMERAUTOPLAYSECONDCHOICED").toInt();
            }
            break;
        case 1:
            if(setting.value("TIMERAUTOSTOPCHOICED").toInt() == 0)
            {
                m_hour = setting.value("TIMERAUTOSTOPHOURCHOICED").toInt();
                m_second = setting.value("TIMERAUTOSTOPSECONDCHOICED").toInt();
            }
            break;
        case 2:
            if(setting.value("TIMERAUTOSHUTDOWNCHOICED").toInt() == 0)
            {
                m_hour = setting.value("TIMERAUTOSHUTDOWNHOURCHOICED").toInt();
                m_second = setting.value("TIMERAUTOSHUTDOWNSECONDCHOICED").toInt();
            }
            break;
    }
}

void MusicTimerAutoObject::timeout()
{
    int hour = 0,second = 0;
    QSettings setting;
    QStringList l = QTime::currentTime().toString(Qt::ISODate).split(':');
    if(l.count() !=3 )
        return;

    hour = l[0].toInt();
    second = l[1].toInt();
    if( hour == m_hour && second == m_second )
    {
        m_hour = m_second = -1;
        if(m_funcIndex == 0)
        {
            if(setting.value("TIMERAUTOPLAYREPEATCHOICED").toInt() == 0)
                setting.setValue("TIMERAUTOPLAYCHOICED",1);

            emit setPlaySong(setting.value("TIMERAUTOPLAYSONGINDEXCHOICED").toInt());
        }
        else if(m_funcIndex == 1)
        {
            if(setting.value("TIMERAUTOSTOPREPEATCHOICED").toInt() == 0)
                setting.setValue("TIMERAUTOSTOPCHOICED",1);

            emit setStopSong();
        }
        else if(m_funcIndex == 2)
        {
            if(setting.value("TIMERAUTOSHUTDOWNREPEATCHOICED").toInt() == 0)
                setting.setValue("TIMERAUTOSHUTDOWNCHOICED",1);
            setShutdown();
        }
    }
}

bool MusicTimerAutoObject::systemIs64bit()
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
#endif
    return isWin64;
}

void MusicTimerAutoObject::setShutdown()
{
    bool isWind64 = systemIs64bit();
    QString item;
    QStringList lists = QProcess::systemEnvironment();
    for(int i=0; i<lists.count(); ++i)
    {
        item = lists[i].toUpper();
        if(item.startsWith("PATH="))
        {
            lists = item.split(';');
            for(int j=0; j<lists.count(); ++j)
            {
                item = lists[j];
                if(item.contains("\\WINDOWS\\SYSTEM32"))
                {   ///x86 or x64
                    qDebug()<<"x86_x64"<<item;
                    break;
                }
                if(isWind64 && item.contains("\\WINDOWS\\SYSWOW64"))
                {   ///x64
                    qDebug()<<"x64"<<item;
                    break;
                }
            }
        }
    }
    QString program = item + "\\shutdown.exe";
    (new QProcess(this))->start(program, QStringList()<<"-s"<<"-t"<<"1");
    qDebug()<<"shutdown now";
}
