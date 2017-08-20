#ifndef MINI_H
#define MINI_H

#include "musicobject.h"

#ifdef Q_OS_WIN
#include "psapi.h"
#include "tlhelp32.h"

#include <QDebug>

static inline QStringList getProcessLists()
{
    QStringList lprocess;
    unsigned long aProcesses[1024], cbNeeded, cProcesses;
    if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return lprocess;
    }

    cProcesses = cbNeeded / sizeof(unsigned long);

    for(unsigned int i = 0; i < cProcesses; i++)
    {
        if(aProcesses[i] == 0)
        {
            continue;
        }

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, aProcesses[i]);
        wchar_t buffer[50];
        GetModuleBaseName(hProcess, 0, buffer, 50);
        CloseHandle(hProcess);
        lprocess << QString::fromWCharArray(buffer);
    }

    return lprocess;
}

static inline bool killProcess(LPCWSTR processName)
{
   PROCESSENTRY32 pe;
   DWORD id = 0;
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   pe.dwSize = sizeof(PROCESSENTRY32);
   if(!Process32First(hSnapshot,&pe))
   {
       return false;
   }

   while(true)
   {
       pe.dwSize = sizeof(PROCESSENTRY32);
       if(Process32Next(hSnapshot, &pe) == FALSE)
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
   if(hProcess != NULL)
   {
       TerminateProcess(hProcess,0);
       CloseHandle(hProcess);
   }

   return true;
}

static inline void checkExtraProcessQuit()
{
    QStringList origin;
    origin << MAKE_TRANSFORM_PREFIX
           << MAKE_KRC2LRC_PREFIX
           << MAKE_PLAYER_PREFIX
           << MAKE_GAIN_PREFIX
           << MAKE_SOUNDTOUCH_PREFIX;

    QStringList list(getProcessLists());
    foreach(const QString &process, origin)
    {
        if(list.contains(process) && killProcess(process.toStdWString().c_str()))
        {
            qDebug() << "Windows Kill Process " << process << " Successed!";
        }
    }
}
#endif

#endif
