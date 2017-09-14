#ifndef MUSICSONGSLISTFUNCTIONWIDGET_H
#define MUSICSONGSLISTFUNCTIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include <QLabel>
#include "musicglobaldefine.h"

class QPropertyAnimation;

/*! @brief The class of the songs list function widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsListFunctionWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsListFunctionWidget(QWidget *parent = 0);

    virtual ~MusicSongsListFunctionWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current animation active.
     */
    void active();
    /*!
     * Set current geometry position.
     */
    void setGeometry();

Q_SIGNALS:
    /*!
     * Delete self object.
     */
    void deleteObject();

private Q_SLOTS:
    /*!
     * Leave Time out.
     */
    void leaveTimeout();
    /*!
     * Animation finished.
     */
    void animationFinished();

protected:
    /*!
     * Start to turn on animation.
     */
    void start(bool play, int end);
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    QTimer m_timer;
    QWidget *m_mainWidget;
    QPropertyAnimation *m_animation;
    float m_currentAnimationValue;

};

#endif // MUSICSONGSLISTFUNCTIONWIDGET_H
