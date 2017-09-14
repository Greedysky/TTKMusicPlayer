#ifndef MUSICLRCFLOATWIDGET_H
#define MUSICLRCFLOATWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfloatabstractwidget.h"

class QPushButton;
class MusicLrcFloatPhotoWidget;
class MusicLrcFloatSettingWidget;

/*! @brief The class of the lrc float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatWidget(QWidget *parent = 0);

    virtual ~MusicLrcFloatWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

public Q_SLOTS:
    /*!
     * Show float setting widget.
     */
    void showFloatSettingWidget();
    /*!
     * Close float setting widget.
     */
    void closeFloatSettingWidget();
    /*!
     * Lrc desktop wallpaper button clicked.
     */
    void musicContainerForWallpaperClicked();

protected:
    QPushButton *m_more, *m_update, *m_search;
    QPushButton *m_wallp, *m_photo;
    MusicLrcFloatPhotoWidget *m_floatPhotoWidget;
    MusicLrcFloatSettingWidget *m_floatSettingWidget;

};

#endif // MUSICLRCFLOATWIDGET_H
