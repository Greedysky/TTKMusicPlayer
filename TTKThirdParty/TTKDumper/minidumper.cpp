#include "minidumper.h"
#include "mini.h"

#ifdef Q_OS_WIN
#include <wchar.h>

LPCWSTR MiniDumper::m_szAppName;
LPWSTR MiniDumper::m_szAppVersion;
LPWSTR MiniDumper::m_szAppBuildNumber;
LPWSTR MiniDumper::m_szDumpFilePath;

#define MAX_DUMP_FILE_NUMBER 9999

MiniDumper::MiniDumper(LPCWSTR szAppName, LPCWSTR szVersion, LPCWSTR szBuildNumber)
{
    m_szAppName = szAppName ? wcsdup(szAppName) : wcsdup(L"TTK");
    m_szAppVersion = szVersion ? wcsdup(szVersion) : wcsdup( L"1.0.0.0");
    m_szAppBuildNumber = szBuildNumber ? wcsdup(szBuildNumber) : wcsdup( L"0000");
	m_szDumpFilePath = NULL;
	
	::SetUnhandledExceptionFilter( TopLevelFilter );
}

void MiniDumper::SetVersion(LPCWSTR szVersion)
{
	if(szVersion)
	{
		free(m_szAppVersion);
		m_szAppVersion = wcsdup(szVersion);
	}	
}

void MiniDumper::SetBuildNumber(LPCWSTR szBuildNumber)
{
	if(szBuildNumber)
	{
		free(m_szAppBuildNumber);
		m_szAppBuildNumber = wcsdup(szBuildNumber);
	}
}

void MiniDumper::SetDumpFilePath(LPCWSTR szFilePath)
{
	free(m_szDumpFilePath);
	m_szDumpFilePath = NULL;
	if(szFilePath != NULL)
	{
		m_szDumpFilePath = wcsdup(szFilePath);
	}	
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
    checkExtraProcessQuit();

	LONG retval = EXCEPTION_CONTINUE_SEARCH;

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	HMODULE hDll = NULL;
	WCHAR szDbgHelpPath[_MAX_PATH];

    if (GetModuleFileNameW( NULL, szDbgHelpPath, _MAX_PATH ))
	{
		WCHAR *pSlash = wcsrchr( szDbgHelpPath, L'\\');
		if (pSlash)
		{
			wcscpy( pSlash+1, L"DBGHELP.DLL" );
            hDll = ::LoadLibraryW( szDbgHelpPath );
		}
	}

	if (hDll==NULL)
	{
		// load any version we can
        hDll = ::LoadLibraryW( L"DBGHELP.DLL");
	}

	LPCWSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			WCHAR szDumpPath[_MAX_PATH];
			WCHAR szDumpRootPath[_MAX_PATH];
			WCHAR szScratch[_MAX_PATH];

			// work out a good place for the dump file
			
			if(m_szDumpFilePath == NULL)
			{
                if (GetModuleFileNameW(NULL, szDbgHelpPath, _MAX_PATH))
				{
					WCHAR *pSlash = wcsrchr(szDbgHelpPath, L'\\');
					if (pSlash)
					{
						wcscpy(pSlash + 1, L"");
						wcscpy(szDumpPath, szDbgHelpPath);
					}
				}
                else if (!GetTempPathW( _MAX_PATH, szDumpPath ))
					wcscpy( szDumpPath, L"c:\\temp\\" );
			}
			else
			{				
				wcscpy( szDumpPath, m_szDumpFilePath );
			}
			wcscpy( szDumpRootPath, szDumpPath);					

			//PrintDebug(L"[MiniDumper] Mini Dump file:[%s]",szDumpPath);	

			// ask the user if they want to save a dump file
			//if (::MessageBox( NULL, _T("Something bad happened in your program, would you like to save a diagnostic file?"), m_szAppName, MB_YESNO )==IDYES)
			{
				HANDLE hFile = INVALID_HANDLE_VALUE;
				int i = 1;
				WCHAR szFileNumber[_MAX_PATH];
				while(hFile == INVALID_HANDLE_VALUE)
				{					
					swprintf(szFileNumber, sizeof(szFileNumber), L"_%04d",i);
					wcscpy( szDumpPath, szDumpRootPath);
					wcscat( szDumpPath, m_szAppName );
					wcscat( szDumpPath, L"_" );
					wcscat( szDumpPath, m_szAppVersion);
					wcscat( szDumpPath, L"_" );
					wcscat( szDumpPath, m_szAppBuildNumber);
					wcscat( szDumpPath, szFileNumber);
					wcscat( szDumpPath, L".dmp" );					
                    hFile = ::CreateFileW( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW,
                                        FILE_ATTRIBUTE_NORMAL, NULL );
					i++;
					if(i > MAX_DUMP_FILE_NUMBER)
					{
                        hFile = ::CreateFileW( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                                               FILE_ATTRIBUTE_NORMAL, NULL );
						break;
					}
				}
				// create the file				

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
                    ExInfo.ClientPointers = 0;

					// write the dump
					BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
					if (bOK)
					{
						swprintf( szScratch, sizeof(szScratch), L"Saved dump file to '%s'", szDumpPath );
						szResult = szScratch;
						retval = EXCEPTION_EXECUTE_HANDLER;
					}
					else
					{
						swprintf( szScratch, sizeof(szScratch),L"Failed to save dump file to '%s' (error %d)", szDumpPath, GetLastError() );
						szResult = szScratch;
					}
					::CloseHandle(hFile);

					WCHAR csOutMessage[MAX_WARNING_MESSAGE_PATH];
                    swprintf(csOutMessage, sizeof(csOutMessage), csOutMessage, m_szAppName, szDumpPath);
				}
				else
				{
					swprintf( szScratch, sizeof(szScratch),L"Failed to create dump file '%s' (error %d)", szDumpPath, GetLastError() );
					szResult = szScratch;
				}
			}

		}
		else
		{
			szResult = L"DBGHELP.DLL too old";
		}
	}
	else
	{
		szResult = L"DBGHELP.DLL not found";
	}

	return retval;
}

#elif defined Q_OS_UNIX
#include <signal.h>

void errorHandler(int type)
{
    qDebug() << "Error Type " << type;
    checkExtraProcessQuit();
    exit(0);
}

MiniDumper::MiniDumper()
{
    signal(SIGPIPE, errorHandler);
    signal(SIGSEGV, errorHandler);
    signal(SIGFPE, errorHandler);
    signal(SIGABRT, errorHandler);
}
#endif
