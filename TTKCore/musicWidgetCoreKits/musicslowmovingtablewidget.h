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
    explicit MusicSlowMovingTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSlowMovingTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setMovedScrollBar(QScrollBar *bar);
    /*!
     * Set current moved scroll bar.
     */

public Q_SLOTS:
    void timeToAnimation();
    /*!
     * Time out to start animation.
     */
    void valueChanged(int value);
    /*!
     * Current scroll bar value changed.
     */

protected:
    virtual void wheelEvent(QWheelEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_isFirstInit;
    int m_priviousValue, m_deltaValue;
    QScrollBar *m_scrollBar;
    QTimer *m_animationTimer;
    QPropertyAnimation *m_slowAnimation;

};

#endif // MUSICSLOWMOVINGTABLEWIDGET_H
