#include "minidumper.h"
#include "musicobject.h"
#include "musicotherdefine.h"
#include "miniprocess.h"

static void cleanAppicationCache()
{
    QFile::remove(MUSIC_COLOR_FILE);
    QFile::remove(MUSIC_IMAGE_FILE);
    QFile::remove(MUSIC_RECORD_FILE);
    QFile::remove(MUSIC_RECORD_DATA_FILE);

    ///clean thirdparty process
    QStringList process;
    process << MAKE_TRANSFORM_PREFIX
           << MAKE_KRC2LRC_PREFIX
           << MAKE_PLAYER_PREFIX
           << MAKE_GAIN_PREFIX;
    killProcessByName(process);
}

#ifdef Q_OS_WIN
#include <wchar.h>

LPCWSTR MiniDumper::m_appName;
LPWSTR MiniDumper::m_appVersion;
LPWSTR MiniDumper::m_dumpFilePath;

#define MAX_DUMP_FILE_NUMBER 9999

MiniDumper::MiniDumper(LPCWSTR name, LPCWSTR version)
{
    m_appName = name ? wcsdup(name) : wcsdup(L"TTK");
    m_appVersion = version ? wcsdup(version) : wcsdup(L"1.0.0.0");
    m_dumpFilePath = nullptr;

    ::SetUnhandledExceptionFilter(TopLevelFilter);
}

MiniDumper::~MiniDumper()
{
    cleanAppicationCache();
}

LONG MiniDumper::TopLevelFilter(EXCEPTION_POINTERS *info)
{
    LONG retval = EXCEPTION_CONTINUE_SEARCH;

    // firstly see if dbghelp.dll is around and has the function we need
    // look next to the EXE first, as the one in System32 might be old
    // (e.g. Windows 2000)
    HMODULE hDll = nullptr;
    WCHAR dbgHelpPath[_MAX_PATH];

    if(GetModuleFileNameW(nullptr, dbgHelpPath, _MAX_PATH))
    {
        WCHAR *slash = wcsrchr(dbgHelpPath, L'\\');
        if(slash)
        {
            wcscpy(slash + 1, L"DBGHELP.DLL");
            hDll = ::LoadLibraryW(dbgHelpPath);
        }
    }

    if(hDll == nullptr)
    {
        // load any version we can
        hDll = ::LoadLibraryW(L"DBGHELP.DLL");
    }

    if(hDll)
    {
        MINIDUMPWRITEDUMP dump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
        if(dump)
        {
            WCHAR dumpPath[_MAX_PATH];
            WCHAR dumpRootPath[_MAX_PATH];
            WCHAR scratch[_MAX_PATH];

            // work out a good place for the dump file
            if(m_dumpFilePath == nullptr)
            {
                if(GetModuleFileNameW(nullptr, dbgHelpPath, _MAX_PATH))
                {
                    WCHAR *slash = wcsrchr(dbgHelpPath, L'\\');
                    if(slash)
                    {
                        wcscpy(slash + 1, L"");
                        wcscpy(dumpPath, dbgHelpPath);
                    }
                }
                else if(!GetTempPathW(_MAX_PATH, dumpPath))
                {
                    wcscpy(dumpPath, L"c:\\temp\\");
                }
            }
            else
            {
                wcscpy(dumpPath, m_dumpFilePath);
            }
            wcscpy(dumpRootPath, dumpPath);

            // ask the user if they want to save a dump file
            //if(::MessageBox(nullptr, _T("Something bad happened in your program, would you like to save a diagnostic file?"), m_szAppName, MB_YESNO)==IDYES)
            {
                HANDLE file = INVALID_HANDLE_VALUE;
                int i = 1;
                WCHAR fileNumber[_MAX_PATH];
                while(file == INVALID_HANDLE_VALUE)
                {
                    swprintf(fileNumber, sizeof(fileNumber), L"_%04d",i);
                    wcscpy(dumpPath, dumpRootPath);
                    wcscat(dumpPath, m_appName);
                    wcscat(dumpPath, L"_");
                    wcscat(dumpPath, m_appVersion);
                    wcscat(dumpPath, fileNumber);
                    wcscat(dumpPath, L".dmp");

                    file = ::CreateFileW(dumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
                    i++;
                    if(i > MAX_DUMP_FILE_NUMBER)
                    {
                        file = ::CreateFileW(dumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
                        break;
                    }
                }
                // create the file
                if(file != INVALID_HANDLE_VALUE)
                {
                    MINIDUMP_EXCEPTION_INFORMATION exInfo;
                    exInfo.ThreadId = ::GetCurrentThreadId();
                    exInfo.ExceptionPointers = info;
                    exInfo.ClientPointers = 0;

                    // write the dump
                    BOOL ok = dump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, &exInfo, nullptr, nullptr);
                    if(ok)
                    {
                        swprintf(scratch, sizeof(scratch), L"Saved dump file to '%s'", dumpPath);
                        retval = EXCEPTION_EXECUTE_HANDLER;
                    }
                    else
                    {
                        swprintf(scratch, sizeof(scratch), L"Failed to save dump file to '%s' (error %d)", dumpPath, GetLastError());
                    }
                    ::CloseHandle(file);

                    WCHAR outMessage[MAX_WARNING_MESSAGE_PATH];
                    swprintf(outMessage, sizeof(outMessage), outMessage, m_appName, dumpPath);
                }
                else
                {
                    swprintf(scratch, sizeof(scratch), L"Failed to create dump file '%s' (error %d)", dumpPath, GetLastError());
                }
            }
        }
    }
    return retval;
}

#elif defined Q_OS_UNIX
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <fcntl.h>

static QString GlobalAppName;

static void errorHandler(int id)
{
    TTK_LOGGER_INFO("App error occurred, error code " << id);
    cleanAppicationCache();
    
    char stamp[50];
    sprintf(stamp, "%ld", time(nullptr));
    const std::string& file_name = GlobalAppName.toStdString() + "." + stamp + ".dmp";
    
    const int size = 512;
    void*     array[size];
    const int fd = open(file_name.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    backtrace_symbols_fd(array, backtrace(array, size), fd);
    close(fd);

    raise(id);
    exit(0);
}

MiniDumper::MiniDumper(const QString &name, const QString &version)
{
    GlobalAppName = name + '_' + version;

    signal(SIGPIPE, errorHandler);
    signal(SIGSEGV, errorHandler);
    signal(SIGFPE, errorHandler);
    signal(SIGABRT, errorHandler);
}

MiniDumper::~MiniDumper()
{
    cleanAppicationCache();
}

#endif
