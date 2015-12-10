#ifndef MUSICREMOTEWIDGETFORRECTANGLE_H
#define MUSICREMOTEWIDGETFORRECTANGLE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

class MusicMarqueeWidget;

class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForRectangle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForRectangle(QWidget *parent = 0);
    virtual ~MusicRemoteWidgetForRectangle();

    virtual void setLabelText(const QString &value) override;

protected:
    MusicMarqueeWidget *m_songNameLabel;
    QWidget *m_toolWidget;

};

#endif // MUSICREMOTEWIDGETFORRECTANGLE_H
