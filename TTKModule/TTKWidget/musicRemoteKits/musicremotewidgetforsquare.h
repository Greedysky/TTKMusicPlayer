#ifndef MUSICREMOTEWIDGETFORSQUARE_H
#define MUSICREMOTEWIDGETFORSQUARE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

/*! @brief The class of the desktop square remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForSquare : public MusicRemoteWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRemoteWidgetForSquare(QWidget *parent = 0);

    virtual ~MusicRemoteWidgetForSquare();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Mouse enter the geometry time out.
     */
    void enterTimeout();
    /*!
     * Mouse leave the geometry time out.
     */
    void leaveTimeout();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    QGraphicsOpacityEffect *m_effect[4];
    QTimer m_enterTimer;
    QTimer m_leaveTimer;
    float m_interval;

};

#endif // MUSICREMOTEWIDGETFORSQUARE_H
