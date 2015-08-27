#ifndef MUSICCOREALGORITHM_H
#define MUSICCOREALGORITHM_H

#include <QObject>
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicCoreAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit MusicCoreAlgorithm(QObject *parent = 0);
    
    // xx:xx
    static QString msecTime2LabelJustified(qint64 time);
    // xx:xx:xx
    static QString normalTime2LabelJustified(qint64 time);
    //ss / mm / hh / dd
    static QString normalTime2Label(qint64 time);
    // xx MB
    static QString fileSzie2Label(int size);
    // xx KB
    static qreal fileSzieByte2KByte(qint64 size)
    {
        return size / 1024.0;
    }
    // xx MB
    static qreal fileSzieByte2MByte(qint64 size)
    {
        return fileSzieByte2KByte(size) / 1024.0;
    }


};

#endif // MUSICCOREALGORITHM_H
