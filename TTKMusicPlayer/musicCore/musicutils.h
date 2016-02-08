#ifndef MUSICUTILS
#define MUSICUTILS

#include <QWidget>
#include "musicobject.h"

namespace MusicUtils
{
    void dirIsExist(const QString& name);
    void checkTheDirectoryExist();
    bool checkTheFileExist();

    QString getLanguageName(int index);

    void setLabelFont(QWidget *widget, int size);

    QString fileSize2Label(qint64 size);
    QString fileSize2Normal(qint64 size);

    qreal fileSizeByte2KByte(qint64 size);
    qreal fileSizeByte2MByte(qint64 size);

    quint64 dirSize(const QString &dirName);
    void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);

}
#endif // MUSICUTILS

