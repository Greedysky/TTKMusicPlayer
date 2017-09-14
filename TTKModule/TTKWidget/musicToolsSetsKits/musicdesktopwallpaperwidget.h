#ifndef MUSICDESKTOPWALLPAPERWIDGET_H
#define MUSICDESKTOPWALLPAPERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicabstractmovewidget.h"

class MusicDesktopWallpaperThread;

namespace Ui {
class MusicDesktopWallpaperWidget;
}

/*! @brief The class of the desktop wallpaper widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicDesktopWallpaperWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDesktopWallpaperWidget(QWidget *parent = 0);

    ~MusicDesktopWallpaperWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

public Q_SLOTS:
    /*!
     * Net button clicked.
     */
    void netRadioButtonPressed();
    /*!
     * Local button clicked.
     */
    void localRadioButtonPressed();
    /*!
     * Play button clicked.
     */
    void playRadioButtonPressed();
    /*!
     * Find local file button clicked.
     */
    void viewButtonPressed();
    /*!
     * Confirm button clicked to start show wallpaper.
     */
    void confirmButtonPressed();
    /*!
     * Stop show wallpaper.
     */
    void stopButtonPressed();
    /*!
     * Close current widget.
     */
    void cancelButtonPressed();
    /*!
     * Set parameters.
     */
    void parameterFinished();
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Create all widget style in layout.
     */
    void initWidgetStyle() const;
    /*!
     * Init parameters.
     */
    void initParameters() const;
    /*!
     * Set auto thread to show wallpaper.
     */
    void setAutoStart(bool autoStart) const;

    Ui::MusicDesktopWallpaperWidget *m_ui;
    int m_currentMode;
    QStringList m_path;
    MusicDesktopWallpaperThread *m_wallThread;

};

#endif // MUSICDESKTOPWALLPAPERWIDGET_H
