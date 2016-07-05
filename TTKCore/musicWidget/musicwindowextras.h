#ifndef MUSICWINDOWEXTRAS_H
#define MUSICWINDOWEXTRAS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicobject.h"
#include "musicglobaldefine.h"

#if defined Q_OS_WIN && defined MUSIC_WINEXTRAS
class MusicApplication;
class QWinTaskbarButton;
class QWinTaskbarProgress;
class QWinThumbnailToolBar;
class QWinThumbnailToolButton;
#endif

/*! @brief The class of the windows extras.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicWindowExtras : public QObject
{
    Q_OBJECT
public:
    explicit MusicWindowExtras(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicWindowExtras();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    void setValue(int value) const;
    /*!
     * Set current value.
     */
    void setRange(int start, int end) const;
    /*!
     * Set current range from start to end.
     */
    void disableBlurBehindWindow(bool enable);
    /*!
     * Enable or disable blur behind window.
     */
    inline bool isDisableBlurBehindWindow() const { return m_disableBlurBehindWindow; }
    /*!
     * Get blur behind window state.
     */

protected:
#if defined Q_OS_WIN && defined MUSIC_WINEXTRAS
    void createJumpList() const;
    /*!
     * Create jump list.
     */
    void createTaskbar();
    /*!
     * Create taskbar.
     */
    void createThumbnailToolBar();
    /*!
     * Create thumbnail toolBar.
     */

    MusicApplication *m_superClass;
    QWinTaskbarButton *m_taskbarButton;
    QWinTaskbarProgress *m_taskbarProgress;
    QWinThumbnailToolBar *m_thumbnailToolBar;
    QWinThumbnailToolButton *m_playToolButton;
    QWinThumbnailToolButton *m_forwardToolButton;
    QWinThumbnailToolButton *m_backwardToolButton;
#endif
    bool m_disableBlurBehindWindow;

};

#endif // MUSICWINDOWEXTRAS_H
