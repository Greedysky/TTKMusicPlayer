#if !defined(AFX_MDUMP_H__FCECFEFE6E_FA36_4693_B07C_F8JHT75BB0101B__INCLUDED_)
#define AFX_MDUMP_H__FCECFEFE6E_FA36_4693_B07C_F8JHT75BB0101B__INCLUDED_

#include "musicextrasglobaldefine.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include "dbghelp.h"

// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
                                    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
                                    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
                                    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

#define MAX_WARNING_MESSAGE_PATH 1024

/*! @brief The class of the mini dumper win32.
 * @author DoubleLi <517712484@qq.com>
 */
class MUSIC_EXTRAS_EXPORT MiniDumper
{
public:
    MiniDumper(LPCWSTR szAppName, LPCWSTR szVersion, LPCWSTR szBuildNumber = nullptr);
    ~MiniDumper();

    static void SetVersion(LPCWSTR szVersion);
    static void SetBuildNumber(LPCWSTR szBuildNumber);
    static void SetDumpFilePath(LPCWSTR szFilePath);

private:
    static LPCWSTR m_szAppName;
    static LPWSTR m_szAppVersion;
    static LPWSTR m_szAppBuildNumber;
    static LPWSTR m_szDumpFilePath;
    static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);

};

#elif defined Q_OS_UNIX
/*! @brief The class of the mini dumper linux.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT MiniDumper
{
public:
    MiniDumper();
    ~MiniDumper();

};

#endif

#endif
