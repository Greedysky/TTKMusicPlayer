#ifndef MUSICLAYOUTANIMATIONWIDGET_H
#define MUSICLAYOUTANIMATIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class QVBoxLayout;
class QPropertyAnimation;

/*! @brief The class of the layout float animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLayoutAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLayoutAnimationWidget(QWidget *parent = 0);

    ~MusicLayoutAnimationWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to anmiation.
     */
    void start();
    /*!
     * Stop to anmiation.
     */
    void stop();

    /*!
     * Connect to parent.
     */
    void connectTo(QObject *parent);
    /*!
     * Add widget layout stretch.
     */
    void addStretch(int stretch = 0);
    /*!
     * Add widget to container.
     */
    void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = 0);
    /*!
     * Remove widget to container.
     */
    void removeWidget(QWidget *widget);

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
    float m_currentValue;
    QWidget *m_mainWidget;
    QVBoxLayout *m_widgetLayout;
    QPropertyAnimation *m_animation;

};

#endif // MUSICLAYOUTANIMATIONWIDGET_H
