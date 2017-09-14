#ifndef MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
#define MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

/*! @brief The class of the desktop complex remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForComplexStyle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRemoteWidgetForComplexStyle(QWidget *parent = 0);

    virtual ~MusicRemoteWidgetForComplexStyle();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &value) override;

protected:
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */
    bool showArtPicture(const QString &name);

    QLabel *m_iconLabel, *m_songName, *m_songArtist;

};

#endif // MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
