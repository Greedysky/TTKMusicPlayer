#ifndef MUSICSONGLISTENHANCELOSSLESSWIDGET_H
#define MUSICSONGLISTENHANCELOSSLESSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSongListEnhanceLosslessWidget;
}

/*! @brief The class of the song list enhance lossless widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongListEnhanceLosslessWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongListEnhanceLosslessWidget(QWidget *parent = 0);

    virtual ~MusicSongListEnhanceLosslessWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicSongListEnhanceLosslessWidget *m_ui;

};

#endif // MUSICSONGLISTENHANCELOSSLESSWIDGET_H
