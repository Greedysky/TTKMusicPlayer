#ifndef MUSICREMOTEWIDGETFORSIMPLESTYLE_H
#define MUSICREMOTEWIDGETFORSIMPLESTYLE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

class MusicMarqueeWidget;

/*! @brief The class of the desktop simple remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForSimpleStyle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForSimpleStyle(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRemoteWidgetForSimpleStyle();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void setLabelText(const QString &value) override;
    /*!
     * Set current song text.
     */

protected:
    MusicMarqueeWidget *m_songNameLabel;

};

#endif // MUSICREMOTEWIDGETFORSIMPLESTYLE_H
