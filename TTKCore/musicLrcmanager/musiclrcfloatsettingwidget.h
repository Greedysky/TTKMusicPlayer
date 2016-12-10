#ifndef MUSICLRCFLOATSETTINGWIDGET_H
#define MUSICLRCFLOATSETTINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfloatabstractwidget.h"

class QPushButton;

/*! @brief The class of the lrc setting float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatSettingWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatSettingWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void resizeWindow(int width, int height) override;
    /*!
     * Resize window bound by given width and height.
     */

Q_SIGNALS:
    void widgetClose();
    /*!
     * Widget close it emit.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */
    void lrcSizeUpChanged();
    /*!
     * Change current lrc size up.
     */
    void lrcSizeLowChanged();
    /*!
     * Change current lrc size down.
     */
    void lrcMusicBackgroundChanged();
    /*!
     * Change current background to default.
     */
    void lrcArtBackgroundChanged();
    /*!
     * Change current background to artist.
     */

protected:
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */
    QPushButton *createPushButton(int index);
    /*!
     * Create pushButton by given index.
     */

};

#endif // MUSICLRCFLOATSETTINGWIDGET_H
