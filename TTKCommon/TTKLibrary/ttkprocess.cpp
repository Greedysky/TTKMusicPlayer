#include "ttkprocess.h"
#include <QStringList>

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <qt_windows.h>
#include <psapi.h>
#include <tlhelp32.h>

static QStringList processList()
{
    QStringList lprocess;
    unsigned long aProcesses[1024], cbNeeded, cProcesses;
    if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return lprocess;
    }

    cProcesses = cbNeeded / sizeof(unsigned long);
    for(unsigned int i = 0; i < cProcesses; ++i)
    {
        if(aProcesses[i] == 0)
        {
            continue;
        }

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, aProcesses[i]);
        wchar_t buffer[50];
        GetModuleBaseNameW(hProcess, 0, buffer, 50);
        CloseHandle(hProcess);
        lprocess << QString::fromWCharArray(buffer);
    }

    return lprocess;
}

static bool killProcess(LPCWSTR processName)
{
   PROCESSENTRY32W pe;
   DWORD id = 0;
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   pe.dwSize = sizeof(PROCESSENTRY32W);
   if(!Process32FirstW(hSnapshot, &pe))
   {
       return false;
   }

   while(true)
   {
       pe.dwSize = sizeof(PROCESSENTRY32W);
       if(Process32NextW(hSnapshot, &pe) == FALSE)
       {
           break;
       }

       //find processName
       if(wcsicmp(pe.szExeFile, processName) == 0)
       {
           id = pe.th32ProcessID;
           break;
       }
   }

   CloseHandle(hSnapshot);

   //Kill The Process
   HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
   if(hProcess)
   {
       TerminateProcess(hProcess,0);
       CloseHandle(hProcess);
   }

   return true;
}

void TTK::killProcessByName(const QString &process, bool more)
{
    const QStringList infos(processList());
    for(const QString &info : qAsConst(infos))
    {
        if(info.contains(process) && killProcess(process.toStdWString().c_str()))
        {
            TTK_INFO_STREAM("Windows Kill Process" << process << "Successed");
            if(!more)
            {
                break;
            }
        }
    }
}

void TTK::killProcessByName(const QStringList &processes, bool more)
{
    const QStringList infos(processList());
    for(const QString &process : qAsConst(processes))
    {
        for(const QString &info : qAsConst(infos))
        {
            if(info.contains(process) && killProcess(process.toStdWString().c_str()))
            {
                TTK_INFO_STREAM("Windows Kill Process" << process << "Successed");
                if(!more)
                {
                    break;
                }
            }
        }
    }
}
#elif defined Q_OS_UNIX
#include <QProcess>

struct PIDInfo
{
    int m_pid;
    QString m_path;
};

static QList<PIDInfo> processList()
{
    QList<PIDInfo> lprocess;
    QProcess process;
    process.start("/bin/bash", {"-c", "ps -xu | awk '{print $2\";\"$11}'"});
    if(!process.waitForFinished())
    {
        return lprocess;
    }

    const QString data(process.readAll());
    if(data.isEmpty())
    {
        return lprocess;
    }

    const QStringList &sp = data.split(TTK_LINEFEED);
    for(const QString &var : qAsConst(sp))
    {
        const QStringList &line = var.split(";");
        if(line.count() != 2)
        {
            continue;
        }

        PIDInfo info;
        info.m_pid = line.front().toInt();
        info.m_path = line.back();
        lprocess << info;
    }

    return lprocess;
}

static bool killProcess(int pid)
{
    QProcess::execute("kill", {"-s", "9", QString::number(pid)});
    return true;
}

void TTK::killProcessByName(const QString &process, bool more)
{
    const QList<PIDInfo> infos(processList());
    for(const PIDInfo &info : qAsConst(infos))
    {
        if(info.m_path.contains(process) && killProcess(info.m_pid))
        {
            TTK_INFO_STREAM("Unix Kill Process" << process << "PID" << info.m_pid << "Successed");
            if(!more)
            {
                break;
            }
        }
    }
}

void TTK::killProcessByName(const QStringList &processes, bool more)
{
    const QList<PIDInfo> infos(processList());
    for(const QString &process : qAsConst(processes))
    {
        for(const PIDInfo &info : qAsConst(infos))
        {
            if(info.m_path.contains(process) && killProcess(info.m_pid))
            {
                TTK_INFO_STREAM("Unix Kill Process" << process << "PID" << info.m_pid << "Successed");
                if(!more)
                {
                    break;
                }
            }
        }
    }
}
#endif
