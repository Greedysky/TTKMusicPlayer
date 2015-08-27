#include "musiccorealgorithm.h"

MusicCoreAlgorithm::MusicCoreAlgorithm(QObject *parent)
    : QObject(parent)
{

}

QString MusicCoreAlgorithm::msecTime2LabelJustified(qint64 time)
{
    int minute = time / (60 * 1000);
    int second = time % (60 * 1000);
    return QString("%1:%2")
          .arg(QString::number(minute).rightJustified(2, '0'))
          .arg(QString::number(second / 1000).rightJustified(2, '0'));
}

QString MusicCoreAlgorithm::normalTime2LabelJustified(qint64 time)
{
    int hour = time / 3600;
    int min = time % 3600 / 60;
    int sec = time % 3600 % 60;
    return QString("%1:%2:%3")
          .arg(QString::number(hour).rightJustified(2, '0'))
          .arg(QString::number(min).rightJustified(2, '0'))
          .arg(QString::number(sec).rightJustified(2, '0'));
}

QString MusicCoreAlgorithm::fileSzie2Label(int size)
{
    if( size < 1024)
    {
        return QString("%1 Byte").arg(size);
    }
    else if( 1024 <= size && size < 1024*1024)
    {
        return QString("%1 KByte").arg((int)(size*1.0/1024*100)/100.0);
    }
    else if( 1024*1024 <= size && size < 1024*1024*1024)
    {
        return QString("%1 MByte").arg((int)(size*1.0/1024/1024*100)/100.0);
    }
    else
    {
        return QString();
    }
}

QString MusicCoreAlgorithm::normalTime2Label(qint64 time)
{
    if(time < 60)
    {
        return QString::number(time) + tr("ss");
    }
    else if(60 <= time && time < 60*60)
    {
        return QString::number(time/60) + tr("mm");
    }
    else if(60*60 <= time && time < 60*60*24)
    {
        return QString::number(time/60/60) + tr("hh");
    }
    else
    {
        return QString::number(time/60/60/24) + tr("day");
    }
}
