#ifndef MUSICLRCFLOATPLAYWIDGET_H
#define MUSICLRCFLOATPLAYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfloatabstractwidget.h"

class QToolButton;

/*! @brief The class of the lrc play float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatPlayWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatPlayWidget(QWidget *parent = 0);

    virtual ~MusicLrcFloatPlayWidget();

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
     * Set current player to play or not.
     */
    void musicStatePlay();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    /*!
     * Set current play state icon.
     */
    void setCurrentPlayState();

    QToolButton *m_musicPrevious, *m_musicNext, *m_musicKey;

};

#endif // MUSICLRCFLOATPLAYWIDGET_H
