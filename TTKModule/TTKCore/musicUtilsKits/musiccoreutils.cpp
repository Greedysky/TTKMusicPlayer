#include "musiccoreutils.h"

#include <QThread>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

void MusicUtils::Core::sleep(int msecond)
{
#if defined Q_OS_WIN && defined TTK_GREATER_NEW
    QThread::msleep(msecond);
#else
    usleep(msecond * 1000);
#endif
}

QString MusicUtils::Core::getLanguageName(int index)
{
    QString lan(LANGUAGE_DIR_FULL);
    switch(index)
    {
        case 0 : return lan.append("cn.ln");
        case 1 : return lan.append("cn_c.ln");
        case 2 : return lan.append("en.ln");
        default: return QString();
    }
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
