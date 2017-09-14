#ifndef MUSICBACKGROUNDPOPWIDGET_H
#define MUSICBACKGROUNDPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

class MusicClickedSlider;

/*! @brief The class of the background popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundPopWidget(QWidget *parent = 0);

    virtual ~MusicBackgroundPopWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set slider value.
     */
    void setValue(int value);
    /*!
     * Get slider value.
     */
    int value() const;

Q_SIGNALS:
    /*!
     * Current play transparent changed.
     */
    void valueChanged(int value);
    /*!
     * Current slider state changed.
     */
    void sliderStateChanged(bool state);

private Q_SLOTS:
    /*!
     * Current slider pressed changed.
     */
    void sliderPressed();
    /*!
     * Current slider released changed.
     */
    void sliderReleased();

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    MusicClickedSlider *m_slider;

};

#endif // MUSICBACKGROUNDPOPWIDGET_H
