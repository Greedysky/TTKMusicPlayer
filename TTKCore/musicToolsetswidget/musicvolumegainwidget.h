#ifndef MUSICVOLUMEGAINWIDGET_H
#define MUSICVOLUMEGAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicVolumeGainWidget;
}

/*! @brief The class of the volume gain widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicVolumeGainWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicVolumeGainWidget(QWidget *parent = 0);
    virtual ~MusicVolumeGainWidget();

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicVolumeGainWidget *ui;

};

#endif // MUSICVOLUMEGAINWIDGET_H
