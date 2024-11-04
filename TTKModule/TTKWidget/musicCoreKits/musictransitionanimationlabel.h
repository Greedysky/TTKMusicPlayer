#ifndef MUSICTRANSITIONANIMATIONLABEL_H
#define MUSICTRANSITIONANIMATIONLABEL_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QLabel>
#include "musicglobaldefine.h"

class QPropertyAnimation;
namespace QAlgorithm
{
class ImageRender;
}

/*! @brief The class of the label transition animation.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTransitionAnimationLabel : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTransitionAnimationLabel)
public:
    enum class Module
    {
        FadeEffect,
        BlindsEffect,
        CubeEffect,
        WaterEffect,
        LeftToRightEffect,
        TopToBottomEffect,
        MAX = 6
    };

    /*!
     * Object constructor.
     */
    explicit MusicTransitionAnimationLabel(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTransitionAnimationLabel();

    /*!
     * Set animation to start.
     */
    inline void setAnimation(bool enable) { m_animationEnabled = enable; }
    /*!
     * Get animation state.
     */
    inline bool animation() const { return m_animationEnabled; }

    /*!
     * Get render pixmap image.
     */
    const QPixmap& renderPixmap() const;

    /*!
     * Start renderer pixmap animation.
     */
    void start();
    /*!
     * Stop renderer pixmap animation.
     */
    void stop();

    /*!
     * Get animation is in run state.
     */
    bool isRunning() const;

public Q_SLOTS:
    /*!
     * Override the setPixmap function.
     */
    void setPixmap(const QPixmap &pix);

private Q_SLOTS:
    /*!
     * Animation value changed.
     */
    void valueChanged(const QVariant &value);
    /*!
     * Animation finished.
     */
    void animationFinished();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    Module m_type;
    bool m_isAnimating;
    int m_currentValue;
    bool m_animationEnabled;
    QPixmap m_renderPixmap;
    QPixmap m_currentPixmap, m_previousPixmap;
    QPropertyAnimation *m_animation;
    QAlgorithm::ImageRender *m_imageRender;

};

#endif // MUSICTRANSITIONANIMATIONLABEL_H
