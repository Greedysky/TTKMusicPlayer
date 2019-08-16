#ifndef MINI_H
#define MINI_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicobject.h"
#include "musicextrasglobaldefine.h"
#include <QDebug>

#ifdef Q_OS_WIN
#include "psapi.h"
#include "tlhelp32.h"

/*! @brief The functions of process wrapper.
 * @author Greedysky <greedysky@163.com>
 */

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
        GetModuleBaseNameW(hProcess, 0, buffer, 50);
        CloseHandle(hProcess);
        lprocess << QString::fromWCharArray(buffer);
    }

    return lprocess;
}

static inline bool killProcess(LPCWSTR processName)
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
#elif defined Q_OS_UNIX
#include <QProcess>

typedef struct MUSIC_EXTRAS_EXPORT PID_INFO
{
    int m_pid;
    QString m_path;
}PID_INFO;

static inline QList<PID_INFO> getProcessLists()
{
    QList<PID_INFO> lprocess;
    QProcess process;
    process.start("/bin/bash", QStringList() << "-c" << "ps -xu | awk '{print $2\";\"$11}'");
    if(process.waitForFinished())
    {
        QString data(process.readAll());
        if(!data.isEmpty())
        {
            QStringList sp = data.split("\n");
            foreach(QString var, sp)
            {
                QStringList line = var.split(";");
                if(line.count() == 2)
                {
                    PID_INFO info;
                    info.m_pid = line.first().toInt();
                    info.m_path = line.last();
                    lprocess << info;
                }
            }
        }
    }

    return lprocess;
}

static inline bool killProcess(int pid)
{
    QProcess::execute("kill", QStringList() << "-s" << "9" << QString::number(pid));
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

    QList<PID_INFO>  list(getProcessLists());
    foreach(const PID_INFO &info, list)
    {
        foreach(const QString &process, origin)
        {
            if(info.m_path.contains(process) && killProcess(info.m_pid))
            {
                qDebug() << "Unix Kill Process " << process << " PID" << info.m_pid << " Successed!";
            }
        }
    }
}
#endif

#endif
