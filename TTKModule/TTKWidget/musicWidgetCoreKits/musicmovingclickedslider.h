#ifndef MUSICMOVINGCLICKEDSLIDER_H
#define MUSICMOVINGCLICKEDSLIDER_H

#include "musicclickedslider.h"

/*! @brief The class of the slider moving clicked slider.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMovingClickedSlider : public MusicClickedSlider
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicMovingClickedSlider(QWidget *parent = 0);
    /*!
     * Object contsructor by slider orientation.
     */
    explicit MusicMovingClickedSlider(Qt::Orientation orientation, QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Slider is now moving or not.
     */
    inline bool isMoving() const { return m_isMoving;}
    /*!
     * Set slider moving or not.
     */
    inline void setMoving(bool m) { m_isMoving = m;}

Q_SIGNALS:
    /*!
     * Slider handle release at pos emit.
     */
    void sliderReleasedAt(int pos);

public Q_SLOTS:
    /*!
     * Set slider current value.
     */
    void setValue(int value);

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    bool m_isMoving;
    Qt::Orientation m_orientation;

};

#endif // MUSICMOVINGCLICKEDSLIDER_H
