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

class QButtonGroup;
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


/*! @brief The class of the line background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLineBackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLineBackgroundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void transparent(bool state);
    /*!
     * Set backgrgound transparent.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_transparent;

};


/*! @brief The class of the animation base widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBaseAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBaseAnimationWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBaseAnimationWidget();

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
    virtual void switchToSelectedItemStyle(int index);
    /*!
     * Switch to selected item style.
     */
    void animationChanged(const QVariant &value);
    /*!
     * Animation state changed.
     */
    void finished();
    /*!
     * Animation finished.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    QPixmap m_pix;
    int m_curIndex, m_preIndex, m_x;
    float m_perWidth, m_totalWidth;
    bool m_isAnimation, m_showState, m_showLine;

    QPropertyAnimation *m_animation;
    QList<QWidget*> m_container;
    QButtonGroup *m_group;

};


/*! @brief The class of the function animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFuntionAnimationWidget : public MusicBaseAnimationWidget
{
    Q_OBJECT
public:
    explicit MusicFuntionAnimationWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual void switchToSelectedItemStyle(int index) override;
    /*!
     * Switch to selected item style.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

};


/*! @brief The class of the function option animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicOptionAnimationWidget : public MusicBaseAnimationWidget
{
    Q_OBJECT
public:
    explicit MusicOptionAnimationWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void musicButtonStyleClear(bool fore);
    /*!
     * Function button style clear.
     */
    void musicButtonStyle(int index);
    /*!
     * Function button style choice.
     */

public Q_SLOTS:
    virtual void switchToSelectedItemStyle(int index) override;
    /*!
     * Switch to selected item style.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

};

#endif // MUSICFUNTIONANIMATIONWIDGET_H
