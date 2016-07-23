#ifndef MUSICVOLUMEPOPWIDGET_H
#define MUSICVOLUMEPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

class QSlider;

/*! @brief The class of the volume popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicVolumePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicVolumePopWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVolumePopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setValue(int value);

    int value() const;

Q_SIGNALS:
    void musicVolumeChanged(int volume);
    /*!
     * Current play volume changed.
     */

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void initWidget();
    /*!
     * Create all widget in layout.
     */


    bool m_menuShown;
    QSlider *m_volumeSlider;

};

#endif // MUSICVOLUMEPOPWIDGET_H
