#ifndef MUSICFUNTIONANIMATIONWIDGET_H
#define MUSICFUNTIONANIMATIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class QPropertyAnimation;

/*! @brief The class of the background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setTransparent(int alpha);
    /*!
     * Set background transparent.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_backgroundAlpha;

};


/*! @brief The class of the function animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFuntionAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicFuntionAnimationWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicFuntionAnimationWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

signals:
    void buttonClicked(int index);
    /*!
     * Switch to selected item style.
     */

public Q_SLOTS:
    void switchToSelectedItemStyle(int index);
    /*!
     * Switch to selected item style.
     */
    void animationChanged(const QVariant &value);
    void finished();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    QPixmap m_pix;
    int m_curIndex, m_preIndex, m_x;
    float m_perWidth;
    bool m_isAnimation;

    QPropertyAnimation *m_animation;
    QList<QWidget*> m_container;

};

#endif // MUSICFUNTIONANIMATIONWIDGET_H
