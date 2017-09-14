#ifndef MUSICVOLUMEPOPWIDGET_H
#define MUSICVOLUMEPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

class MusicClickedSlider;

/*! @brief The class of the volume popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicVolumePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVolumePopWidget(QWidget *parent = 0);

    virtual ~MusicVolumePopWidget();

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
     * Current play volume changed.
     */
    void musicVolumeChanged(int volume);

public Q_SLOTS:
    /*!
     * Timer ot reset show slider flag.
     */
    void timeToResetFlag();

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    bool m_menuShown;
    MusicClickedSlider *m_volumeSlider;

};

#endif // MUSICVOLUMEPOPWIDGET_H
