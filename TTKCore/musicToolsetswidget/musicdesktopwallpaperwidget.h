#ifndef MUSICDESKTOPWALLPAPERWIDGET_H
#define MUSICDESKTOPWALLPAPERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
    explicit MusicDesktopWallpaperWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicDesktopWallpaperWidget();

Q_SIGNALS:
public Q_SLOTS:
    void netRadioButtonPressed();
    /*!
     * Net button clicked.
     */
    void localRadioButtonPressed();
    /*!
     * Local button clicked.
     */
    void playRadioButtonPressed();
    /*!
     * Play button clicked.
     */
    void viewButtonPressed();
    /*!
     * Find local file button clicked.
     */
    void confirmButtonPressed();
    /*!
     * Confirm button clicked to start show wallpaper.
     */
    void stopButtonPressed();
    /*!
     * Stop show wallpaper.
     */
    void cancelButtonPressed();
    /*!
     * Close current widget.
     */
    void parameterFinished();
    /*!
     * Set parameters.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    void initWidgetStyle() const;
    /*!
     * Create all widget style in layout.
     */
    void initParameters() const;
    /*!
     * Init parameters.
     */
    void findFiles(const QString &path);
    /*!
     * Find current dir files.
     */
    void setAutoStart(bool autoStart) const;
    /*!
     * Set auto thread to show wallpaper.
     */

    Ui::MusicDesktopWallpaperWidget *ui;
    int m_currentMode;
    QStringList m_path;
    MusicDesktopWallpaperThread *m_wallThread;

};

#endif // MUSICDESKTOPWALLPAPERWIDGET_H
