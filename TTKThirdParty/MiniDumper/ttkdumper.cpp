#include "ttkdumper.h"
#include "musicversion.h"
#include "minidumper.h"

TTKDumper::TTKDumper()
{
#ifdef Q_OS_WIN
    m_winDump = new MiniDumper(L"TTK", TTKMUSIC_VERSION_STRW);
#endif
}

TTKDumper::~TTKDumper()
{
#ifdef Q_OS_WIN
    delete m_winDump;
#endif
}

void TTKDumper::run()
{

}
