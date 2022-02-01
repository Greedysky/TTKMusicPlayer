#if !defined(AFX_MDUMP_H__FCECFEFE6E_FA36_4693_B07C_F8JHT75BB0101B__INCLUDED_)
#define AFX_MDUMP_H__FCECFEFE6E_FA36_4693_B07C_F8JHT75BB0101B__INCLUDED_

#include "ttkglobaldefine.h"
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
class TTK_MODULE_EXPORT MiniDumper
{
public:
    MiniDumper(LPCWSTR name, LPCWSTR version);
    ~MiniDumper();

private:
    static LPCWSTR m_appName;
    static LPWSTR m_appVersion;
    static LPWSTR m_dumpFilePath;

    static LONG WINAPI TopLevelFilter(EXCEPTION_POINTERS *info);

};

#elif defined Q_OS_UNIX
/*! @brief The class of the mini dumper linux.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MiniDumper
{
public:
    MiniDumper(const QString &name, const QString &version);
    ~MiniDumper();

};

#endif

#endif
