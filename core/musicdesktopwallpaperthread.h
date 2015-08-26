#ifndef MUSICDESKTOPWALLPAPERTHREAD_H
#define MUSICDESKTOPWALLPAPERTHREAD_H

#include <QThread>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicDesktopWallpaperThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicDesktopWallpaperThread(QObject *parent = 0);
    ~MusicDesktopWallpaperThread();

    void setParamters(const MStriantMap &p);
    void stopAndQuitThread();

public slots:
    void start();
    void run();

protected:
    void setWallpaper(const QString &path, int type) const;

    bool m_run;
    bool m_returnToOrigin;
    int m_currentImageIndex;
    int m_originType;
    MStriantMap m_paramter;
    QString m_originPath;

};

#endif // MUSICDESKTOPWALLPAPERTHREAD_H
