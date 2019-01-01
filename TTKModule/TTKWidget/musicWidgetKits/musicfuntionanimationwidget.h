#ifndef MUSICFUNTIONANIMATIONWIDGET_H
#define MUSICFUNTIONANIMATIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    TTK_DECLARE_MODULE(MusicBackgroundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundWidget(QWidget *parent = nullptr);

    /*!
     * Set background transparent.
     */
    void setTransparent(int alpha);

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    int m_backgroundAlpha;

};


/*! @brief The class of the line background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLineBackgroundWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLineBackgroundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLineBackgroundWidget(QWidget *parent = nullptr);

    /*!
     * Set backgrgound transparent.
     */
    void transparent(bool state);

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    bool m_transparent;

};


/*! @brief The class of the animation base widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBaseAnimationWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBaseAnimationWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBaseAnimationWidget(QWidget *parent = nullptr);

    ~MusicBaseAnimationWidget();

Q_SIGNALS:
    /*!
     * Switch to selected item style.
     */
    void buttonClicked(int index);

public Q_SLOTS:
    /*!
     * Switch to selected item style.
     */
    virtual void switchToSelectedItemStyle(int index);
    /*!
     * Animation state changed.
     */
    void animationChanged(const QVariant &value);
    /*!
     * Animation finished.
     */
    void finished();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

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
    TTK_DECLARE_MODULE(MusicFuntionAnimationWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFuntionAnimationWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    /*!
     * Switch to selected item style.
     */
    virtual void switchToSelectedItemStyle(int index) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

};


/*! @brief The class of the function option animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicOptionAnimationWidget : public MusicBaseAnimationWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicOptionAnimationWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicOptionAnimationWidget(QWidget *parent = nullptr);

    /*!
     * Function button style clear.
     */
    void musicButtonStyleClear(bool fore);
    /*!
     * Function button style choice.
     */
    void musicButtonStyle(int index);

public Q_SLOTS:
    /*!
     * Switch to selected item style.
     */
    virtual void switchToSelectedItemStyle(int index) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

};


/*! @brief The class of the function skin animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSkinAnimationWidget : public MusicBaseAnimationWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSkinAnimationWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSkinAnimationWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    /*!
     * Switch to selected item style.
     */
    virtual void switchToSelectedItemStyle(int index) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

};

#endif // MUSICFUNTIONANIMATIONWIDGET_H
