#ifndef MUSICTRANSITIONANIMATIONLABEL_H
#define MUSICTRANSITIONANIMATIONLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

class QPropertyAnimation;

/*! @brief The class of the label transition animation.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicTransitionAnimationLabel : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTransitionAnimationLabel(QWidget *parent = 0);

    ~MusicTransitionAnimationLabel();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set no animation to start.
     */
    inline void setNoAnimation(bool on) { m_noAnimationSet = on; }
    /*!
     * Get no animation state.
     */
    inline bool getNoAnimation() const { return m_noAnimationSet; }

    /*!
     * Get renderer pixmap image.
     */
    QPixmap getRendererPixmap() const;

    /*!
     * Stop renderer pixmap animation.
     */
    void stop();

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

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    bool m_isAnimating;
    int m_currentValue;
    bool m_noAnimationSet;
    QPixmap m_rendererPixmap;
    QPixmap m_currentPixmap, m_previousPixmap;
    QPropertyAnimation *m_animation;

};

#endif // MUSICTRANSITIONANIMATIONLABEL_H
