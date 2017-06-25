#ifndef MUSICDESKTOPWALLPAPERTHREAD_H
#define MUSICDESKTOPWALLPAPERTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QThread>
#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the desktop wallpaper thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicDesktopWallpaperThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicDesktopWallpaperThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicDesktopWallpaperThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setParamters(const MusicObject::MStriantMap &p);
    /*!
     * Set paramters(Time\Type\Func\Close).
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    virtual void run() override;
    /*!
     * Thread run now.
     */

protected:
    void setWallpaper(const QString &path, int type) const;
    /*!
     * Set desktop wallpaper.
     */

    bool m_run, m_returnToOrigin;
    int m_currentImageIndex, m_originType;
    QString m_originPath;
    MusicObject::MStriantMap m_paramter;

};

#endif // MUSICDESKTOPWALLPAPERTHREAD_H
