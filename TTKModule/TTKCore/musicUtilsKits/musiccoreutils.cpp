#include "musiccoreutils.h"

#if defined Q_OS_UNIX || defined Q_CC_MINGW
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

void MusicUtils::Core::sleep(int ms)
{
    usleep(ms * 1000);
}

bool MusicUtils::Core::appVersionCheck(const QString &o, const QString &d)
{
    const QStringList &ol = o.split(TTK_DOT);
    const QStringList &dl = d.split(TTK_DOT);

    if(ol.isEmpty() || dl.isEmpty() || ol.count() != dl.count())
    {
        return false;
    }

    return versionCheck(ol, dl, 0);
}
