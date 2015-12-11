#ifndef MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
#define MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"
#include <QLabel>

class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForComplexStyle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForComplexStyle(QWidget *parent = 0);
    virtual ~MusicRemoteWidgetForComplexStyle();

    virtual void setLabelText(const QString &value) override;

protected:
    bool showArtPicture(const QString &name);

    QLabel *m_iconLabel, *m_songName, *m_songArtist;

};

#endif // MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
