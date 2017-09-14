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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatSettingWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

Q_SIGNALS:
    /*!
     * Widget close it emit.
     */
    void widgetClose();

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Change current lrc size up.
     */
    void lrcSizeUpChanged();
    /*!
     * Change current lrc size down.
     */
    void lrcSizeLowChanged();
    /*!
     * Change current background to default.
     */
    void lrcMusicBackgroundChanged();
    /*!
     * Change current background to artist.
     */
    void lrcArtBackgroundChanged();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Create pushButton by given index.
     */
    QPushButton *createPushButton(int index);

};

#endif // MUSICLRCFLOATSETTINGWIDGET_H
