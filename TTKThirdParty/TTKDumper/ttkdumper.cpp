#include "ttkdumper.h"
#include "ttkversion.h"
#include "minidumper.h"

TTKDumper::TTKDumper()
{
#ifdef Q_OS_WIN
    m_winDump = new MiniDumper(L"TTK", TTKMUSIC_VERSION_WSTR);
#elif defined Q_OS_UNIX
    m_winDump = new MiniDumper();
#endif
}

TTKDumper::~TTKDumper()
{
    delete m_winDump;
}

void TTKDumper::run()
{
    ///do nothing
}
