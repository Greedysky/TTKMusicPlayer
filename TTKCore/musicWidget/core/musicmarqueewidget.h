#ifndef MUSICMARQUEEWIDGET_H
#define MUSICMARQUEEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the marquee widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMarqueeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicMarqueeWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    void setText(const QString &newText);
    /*!
     * Set marquee text.
     */
    inline QString text() const { return m_myText; }
    /*!
     * Get marquee text.
     */
    QSize sizeHint() const;
    /*!
     * Override size hint.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;
    /*!
     * Override the widget event.
     */

private:
    QString m_myText;
    int m_offset;
    int m_myTimerId;

};

#endif // MUSICMARQUEEWIDGET_H
