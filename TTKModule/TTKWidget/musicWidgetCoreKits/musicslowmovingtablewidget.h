#ifndef MUSICSLOWMOVINGTABLEWIDGET_H
#define MUSICSLOWMOVINGTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"

class QPropertyAnimation;

/*! @brief The class of the slider slow moving table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSlowMovingTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSlowMovingTableWidget(QWidget *parent = 0);

    virtual ~MusicSlowMovingTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current moved scroll bar.
     */
    void setMovedScrollBar(QScrollBar *bar);

public Q_SLOTS:
    /*!
     * Time out to start animation.
     */
    void timeToAnimation();
    /*!
     * Current scroll bar value changed.
     */
    void valueChanged(int value);

protected:
    /*!
     * Override the widget event.
     */
    virtual void wheelEvent(QWheelEvent *event) override;

    bool m_isFirstInit;
    int m_previousValue, m_deltaValue;
    QScrollBar *m_scrollBar;
    QTimer *m_animationTimer;
    QPropertyAnimation *m_slowAnimation;

};

#endif // MUSICSLOWMOVINGTABLEWIDGET_H
