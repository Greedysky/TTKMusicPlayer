#ifndef MUSICWINDOWEXTRAS_H
#define MUSICWINDOWEXTRAS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QObject>
#include "musicobject.h"
#include "musicglobaldefine.h"

#if defined Q_OS_WIN && defined TTK_WINEXTRAS
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
    TTK_DECLARE_MODULE(MusicWindowExtras)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWindowExtras(QObject *parent = nullptr);

    ~MusicWindowExtras();

    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current value.
     */
    void setValue(int value) const;
    /*!
     * Set current range from start to end.
     */
    void setRange(int start, int end) const;
    /*!
     * Enable or disable blur behind window.
     */
    void disableBlurBehindWindow(bool enable);
    /*!
     * Get blur behind window state.
     */
    inline bool isDisableBlurBehindWindow() const { return m_disableBlurBehindWindow; }

protected:
#if defined Q_OS_WIN && defined TTK_WINEXTRAS
    /*!
     * Create jump list.
     */
    void createJumpList() const;
    /*!
     * Create taskbar.
     */
    void createTaskbar();
    /*!
     * Create thumbnail toolBar.
     */
    void createThumbnailToolBar();

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
