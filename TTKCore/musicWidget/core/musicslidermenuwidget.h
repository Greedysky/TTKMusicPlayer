#ifndef MUSICSLIDERMENUWIDGET_H
#define MUSICSLIDERMENUWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QLabel>
#include <QSlider>
#include <QWidgetAction>
#include "musicglobaldefine.h"

/*! @brief The class of the slider menu widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSliderMenuWidget : public QMenu
{
    Q_OBJECT
public:
    explicit MusicSliderMenuWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicSliderMenuWidget(Qt::Orientation orientation, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSliderMenuWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setValue(int value);
    /*!
     * Set slider value.
     */
    int getValue() const;
    /*!
     * Get slider value.
     */
    void setText(const QString &text);
    /*!
     * Set label text.
     */
    QString getText() const;
    /*!
     * Get label text.
     */

    void showTextLabel(bool show);
    /*!
     * Show text label or not
     */

Q_SIGNALS:
    void valueChanged(int value);
    /*!
     * Current slider value changed.
     */

protected:
    QLabel *m_textLabel;
    QSlider *m_slider;
    QWidgetAction *m_widgetAction;

};

#endif // MUSICSLIDERMENUWIDGET_H
