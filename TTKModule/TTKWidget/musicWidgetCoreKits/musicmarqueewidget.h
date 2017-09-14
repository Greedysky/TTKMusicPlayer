#ifndef MUSICMARQUEEWIDGET_H
#define MUSICMARQUEEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicMarqueeWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set effect on resize.
     */
    void setEffectOnResize(bool on) { m_effectOnResize = on; }
    /*!
     * Set marquee text.
     */
    void setText(const QString &newText);
    /*!
     * Get marquee text.
     */
    inline QString text() const { return m_myText.trimmed(); }
    /*!
     * Override size hint.
     */
    QSize sizeHint() const;

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QString m_myText;
    int m_offset,m_myTimerId;
    bool m_effectOnResize;

};

#endif // MUSICMARQUEEWIDGET_H
