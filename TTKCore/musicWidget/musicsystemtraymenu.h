#ifndef MUSICSYSTEMTRAYMENU_H
#define MUSICSYSTEMTRAYMENU_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include "musicglobaldefine.h"

class QLabel;
class MusicClickedSlider;
class QWidgetAction;
class QToolButton;

/*! @brief The class of the system tray menu widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSystemTrayMenu : public QMenu
{
    Q_OBJECT
public:
    MusicSystemTrayMenu(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSystemTrayMenu();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setLabelText(const QString &text) const;
    /*!
     * Set tray menu song text.
     */
    void showDesktopLrc(bool show) const;
    /*!
     * Set show or not desktop lrc.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    void setVolumeValue(int value) const;
    /*!
     * Set current play volume.
     */

Q_SIGNALS:
    void setShowDesktopLrc(bool status);
    /*!
     * Set show desktop lrc state changed.
     */
    void setWindowLockedChanged();
    /*!
     * Lock current desktop lrc state changed.
     */

public Q_SLOTS:
    void showDesktopLrc();
    /*!
     * Set show desktop lrc.
     */
    void lockDesktopLrc(bool lock);
    /*!
     * Lock current desktop lrc or not.
     */

protected:
    void createPlayWidgetActions();
    /*!
     * Create play widget actions.
     */
    void createVolumeWidgetActions();
    /*!
     * Create volume widget actions.
     */

    QLabel *m_showText;
    QToolButton *m_PlayOrStop, *m_volumeButton;
    MusicClickedSlider *m_volumeSlider;
    QAction *m_showLrcAction, *m_lockLrcAction;

};

#endif // MUSICSYSTEMTRAYMENU_H
