#ifndef MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
#define MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
    explicit MusicRemoteWidgetForComplexStyle(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRemoteWidgetForComplexStyle();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void setLabelText(const QString &value) override;
    /*!
     * Set current song text.
     */

protected:
    bool showArtPicture(const QString &name);
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */

    QLabel *m_iconLabel, *m_songName, *m_songArtist;

};

#endif // MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
