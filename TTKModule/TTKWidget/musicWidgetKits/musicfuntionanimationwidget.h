#ifndef MUSICFUNTIONANIMATIONWIDGET_H
#define MUSICFUNTIONANIMATIONWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
 ***************************************************************************/

#include <QWidget>

#include "musicglobaldefine.h"

class QButtonGroup;
class QPropertyAnimation;

/*! @brief The class of the background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundWidget : public QWidget
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
    void backgroundTransparent(int value);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    int m_backgroundAlpha;

};


/*! @brief The class of the line background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLineBackgroundWidget : public QWidget
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

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    bool m_transparent;

};


/*! @brief The class of the animation base widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractAnimationWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractAnimationWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAbstractAnimationWidget(QWidget *parent = nullptr);
    ~MusicAbstractAnimationWidget();

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
    QWidgetList m_container;
    QButtonGroup *m_buttonGroup;

};


/*! @brief The class of the function animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFuntionAnimationWidget : public MusicAbstractAnimationWidget
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
    virtual void switchToSelectedItemStyle(int index) override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};


/*! @brief The class of the function option animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicOptionAnimationWidget : public MusicAbstractAnimationWidget
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
    void buttonStyleClear(bool fore);
    /*!
     * Function button style switch.
     */
    void buttonStyleSwitch(int index);

public Q_SLOTS:
    /*!
     * Switch to selected item style.
     */
    virtual void switchToSelectedItemStyle(int index) override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};


/*! @brief The class of the function skin animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSkinAnimationWidget : public MusicAbstractAnimationWidget
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
    virtual void switchToSelectedItemStyle(int index) override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};

#endif // MUSICFUNTIONANIMATIONWIDGET_H
