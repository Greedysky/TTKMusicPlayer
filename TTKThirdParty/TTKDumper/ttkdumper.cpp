#include "ttkdumper.h"
#include "ttkversion.h"
#include "minidumper.h"

TTKDumper::TTKDumper()
{
#ifdef Q_OS_WIN
    m_dump = new MiniDumper(L"TTK", TTKMUSIC_VERSION_WSTR);
#elif defined Q_OS_UNIX
    m_dump = new MiniDumper();
#endif
}

TTKDumper::~TTKDumper()
{
    delete m_dump;
}

void TTKDumper::run()
{
    ///do nothing
}
