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
    /*!
     * Object contsructor.
     */
    explicit MusicSystemTrayMenu(QWidget *parent = 0);

    ~MusicSystemTrayMenu();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set tray menu song text.
     */
    void setLabelText(const QString &text) const;
    /*!
     * Set show or not desktop lrc.
     */
    void showDesktopLrc(bool show) const;
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play volume.
     */
    void setVolumeValue(int value) const;

public Q_SLOTS:
    /*!
     * Set show desktop lrc.
     */
    void showDesktopLrc();
    /*!
     * Lock current desktop lrc or not.
     */
    void lockDesktopLrc(bool lock);
    /*!
     * Lock current desktop lrc state changed.
     */
    void setWindowLockedChanged();

protected:
    /*!
     * Override the widget event.
     */
    virtual void showEvent(QShowEvent *event) override;
    /*!
     * Create play widget actions.
     */
    void createPlayWidgetActions();
    /*!
     * Create volume widget actions.
     */
    void createVolumeWidgetActions();

    QLabel *m_showText;
    QToolButton *m_PlayOrStop, *m_volumeButton;
    MusicClickedSlider *m_volumeSlider;
    QAction *m_showLrcAction, *m_lockLrcAction, *m_loginAction;

};

#endif // MUSICSYSTEMTRAYMENU_H
