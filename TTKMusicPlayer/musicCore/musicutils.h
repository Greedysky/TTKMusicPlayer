#ifndef MUSICUTILS
#define MUSICUTILS

#include <QWidget>
#include "musicobject.h"
#include "musicglobaldefine.h"

class MUSIC_CORE_EXPORT MusicUtils
{
public:
    MusicUtils(){}

    static void dirIsExist(const QString& name);
    static void checkTheDirectoryExist();
    static bool checkTheFileExist();

    static QString getLanguageName(int index);

    static void setLabelFont(QWidget *widget, int size);

    static QString fileSize2Label(qint64 size);
    static QString fileSize2Normal(qint64 size);

    static qreal fileSizeByte2KByte(qint64 size);
    static qreal fileSizeByte2MByte(qint64 size);

    static quint64 dirSize(const QString &dirName);
    static void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);

};


#endif // MUSICUTILS

