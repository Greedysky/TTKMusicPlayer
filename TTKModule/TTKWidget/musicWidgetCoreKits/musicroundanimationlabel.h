#ifndef MUSICROUNDANIMATIONLABEL_H
#define MUSICROUNDANIMATIONLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the round animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicRoundAnimationLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRoundAnimationLabel(QWidget *parent = 0);

    ~MusicRoundAnimationLabel();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set rotating widget background pixmap.
     */
    void setPixmap(const QPixmap &pix);
    /*!
     * Set rotating widget interval.
     */
    void setInterval(int value);

public Q_SLOTS:
    /*!
     * Start to rotating widget.
     */
    void start();
    /*!
     * Stop to rotating widget.
     */
    void stop();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    QPixmap m_pixmap;
    QTimer m_timer;
    int m_rotateAngle;

};

#endif // MUSICROUNDANIMATIONLABEL_H
