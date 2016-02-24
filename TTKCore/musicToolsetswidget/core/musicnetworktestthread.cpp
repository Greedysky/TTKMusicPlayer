#include "musicnetworktestthread.h"

#ifdef Q_OS_WIN
# ifdef Q_CC_MINGW
#   include <winsock2.h>
# endif
# include <windows.h>
# include <cstdio>
# include <iphlpapi.h>
# if defined Q_CC_MSVC
#   pragma comment(lib, "Iphlpapi.lib")
# endif
#endif

MusicNetworkTestThread::MusicNetworkTestThread(QObject *parent)
    : QThread(parent)
{
    m_run = false;
}

void MusicNetworkTestThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicNetworkTestThread::setAvailableNewtworkNames(const QStringList &names)
{
    m_names = names;
}

QStringList MusicNetworkTestThread::getNewtworkNames()
{
    QStringList names;
#ifdef Q_OS_WIN
    PMIB_IFTABLE m_pTable = NULL;
    DWORD m_dwAdapters = 0;
    ULONG uRetCode = GetIfTable(m_pTable, &m_dwAdapters, TRUE);
    if(uRetCode == ERROR_NOT_SUPPORTED)
    {
        return names;
    }

    if (uRetCode == ERROR_INSUFFICIENT_BUFFER)
    {
        m_pTable = (PMIB_IFTABLE)new BYTE[65535];
    }

    GetIfTable(m_pTable, &m_dwAdapters, TRUE);
    for(UINT i = 0; i < m_pTable->dwNumEntries; i++)
    {
        MIB_IFROW Row = m_pTable->table[i];
        std::string s(MReinterpret_cast(char const*, Row.bDescr));
        QString qs = QString::fromStdString(s);
        if(Row.dwType == 71 && !names.contains(qs))
        {
            names << QString::fromStdString(s);
        }
    }
    delete[] m_pTable;
#endif
    return names;
}

void MusicNetworkTestThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicNetworkTestThread::run()
{
#ifdef Q_OS_WIN
    PMIB_IFTABLE m_pTable = NULL;
    DWORD m_dwAdapters = 0;
    ULONG uRetCode = GetIfTable(m_pTable, &m_dwAdapters, TRUE);
    if(uRetCode == ERROR_NOT_SUPPORTED)
    {
        return;
    }

    if (uRetCode == ERROR_INSUFFICIENT_BUFFER)
    {
        m_pTable = (PMIB_IFTABLE)new BYTE[65535];
    }

    DWORD dwLastIn = 0;
    DWORD dwLastOut = 0;
    DWORD dwBandIn = 0;
    DWORD dwBandOut = 0;
    while(m_run)
    {
        GetIfTable(m_pTable, &m_dwAdapters, TRUE);
        DWORD dwInOctets = 0;
        DWORD dwOutOctets = 0;

        for(UINT i = 0; i < m_pTable->dwNumEntries; i++)
        {
            MIB_IFROW Row = m_pTable->table[i];
            std::string s(MReinterpret_cast(char const*, Row.bDescr));
            if(Row.dwType == 71 && m_names.contains(QString::fromStdString(s)))
            {
                dwInOctets += Row.dwInOctets;
                dwOutOctets += Row.dwOutOctets;
            }
        }

        dwBandIn = dwInOctets - dwLastIn;
        dwBandOut = dwOutOctets - dwLastOut;
        if(dwLastIn <= 0)
        {
            dwBandIn = 0;
        }

        if (dwLastOut <= 0)
        {
            dwBandOut = 0;
        }
        dwLastIn = dwInOctets;
        dwLastOut = dwOutOctets;

        emit networkData(dwBandOut, dwBandIn);
        sleep(1);
    }
    delete[] m_pTable;
#endif
}
