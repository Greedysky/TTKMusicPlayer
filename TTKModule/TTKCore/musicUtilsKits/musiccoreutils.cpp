#include "musiccoreutils.h"

#ifdef Q_CC_MSVC
#  include <qt_windows.h>
#elif defined Q_OS_UNIX || defined Q_CC_GNU
#  include <unistd.h>
#endif

static bool versionCheck(const QStringList &ol, const QStringList &dl, int depth)
{
    if(depth >= ol.count())
    {
        return false;
    }

    if(dl[depth].toInt() >= ol[depth].toInt())
    {
        if(dl[depth].toInt() == ol[depth].toInt())
        {
            return versionCheck(ol, dl, depth + 1);
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

void TTK::Core::sleep(int ms)
{
#ifdef Q_CC_MSVC
    ::Sleep(ms);
#elif defined Q_OS_UNIX || defined Q_CC_GNU
    usleep(ms * MT_MS2US);
#endif
}

bool TTK::Core::appVersionCheck(const QString &o, const QString &d)
{
    const QStringList &ol = o.split(TTK_DOT);
    const QStringList &dl = d.split(TTK_DOT);

    if(ol.isEmpty() || dl.isEmpty() || ol.count() != dl.count())
    {
        return false;
    }

    return versionCheck(ol, dl, 0);
}
