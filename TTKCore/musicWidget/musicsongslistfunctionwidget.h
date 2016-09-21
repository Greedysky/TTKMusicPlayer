#ifndef MUSICSONGSLISTFUNCTIONWIDGET_H
#define MUSICSONGSLISTFUNCTIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
    explicit MusicSongsListFunctionWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsListFunctionWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void active();
    /*!
     * Set current animation active.
     */
    void setGeometry();
    /*!
     * Set current geometry position.
     */

Q_SIGNALS:
    void musicSearch();
    /*!
     * Show searched text widget.
     */
    void musicCurrentPlayLocation();
    /*!
     * Show current play index.
     */
    void deleteObject();
    /*!
     * Delete self object.
     */

private Q_SLOTS:
    void leaveTimeout();
    /*!
     * Leave Time out.
     */
    void animationFinished();
    /*!
     * Animation finished.
     */

protected:
    void start(bool play, int end);
    /*!
     * Start to turn on animation.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */

    QTimer m_timer;
    QPropertyAnimation *m_animation;
    float m_currentAnimationValue;

};

#endif // MUSICSONGSLISTFUNCTIONWIDGET_H
