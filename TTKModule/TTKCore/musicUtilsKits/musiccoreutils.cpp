#include "musiccoreutils.h"

#include <QThread>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

void MusicUtils::Core::sleep(int ms)
{
#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,0,0)
    QThread::msleep(ms);
#else
    usleep(ms * 1000);
#endif
}

bool MusicUtils::Core::appVersionCheck(const QStringList &ol, const QStringList &dl, int depth)
{
    if(depth >= ol.count())
    {
        return false;
    }

    if(dl[depth].toInt() >= ol[depth].toInt())
    {
        if(dl[depth].toInt() == ol[depth].toInt())
        {
            return appVersionCheck(ol, dl, depth + 1);
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

bool MusicUtils::Core::appVersionCheck(const QString &o, const QString &d)
{
    const QStringList &ol = o.split(".");
    const QStringList &dl = d.split(".");

    if(ol.isEmpty() || dl.isEmpty() || ol.count() != dl.count())
    {
        return false;
    }

    return appVersionCheck(ol, dl, 0);
}
