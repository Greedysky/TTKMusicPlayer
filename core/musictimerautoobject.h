#ifndef MUSICTIMERAUTOOBJECT_H
#define MUSICTIMERAUTOOBJECT_H

#include <QObject>
#include <QTimer>

#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_EXPORT MusicTimerAutoObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicTimerAutoObject(QObject *parent = 0);
    ~MusicTimerAutoObject();

    void runTimerAutoConfig();

signals:
    void setPlaySong(int);
    void setStopSong();

public slots:
    void timeout();

protected:
    bool systemIs64bit();
    void setShutdown();
    QTimer m_timer;
    int m_hour;
    int m_second;
    int m_funcIndex;

};

#endif // MUSICTIMERAUTOOBJECT_H
