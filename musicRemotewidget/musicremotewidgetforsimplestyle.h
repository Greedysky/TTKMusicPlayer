#ifndef MUSICREMOTEWIDGETFORSIMPLESTYLE_H
#define MUSICREMOTEWIDGETFORSIMPLESTYLE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

class MusicMarqueeWidget;

class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForSimpleStyle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForSimpleStyle(QWidget *parent = 0);
    virtual ~MusicRemoteWidgetForSimpleStyle();

    virtual void setLabelText(const QString &value) override;

protected:
    MusicMarqueeWidget *m_songNameLabel;

};

#endif // MUSICREMOTEWIDGETFORSIMPLESTYLE_H
