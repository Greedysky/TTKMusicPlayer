#ifndef MUSICSOUNDTOUCHWIDGET_H
#define MUSICSOUNDTOUCHWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSoundTouchWidget;
}
/*! @brief The class of the sound touch widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSoundTouchWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSoundTouchWidget(QWidget *parent = 0);
    virtual ~MusicSoundTouchWidget();

Q_SIGNALS:

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicSoundTouchWidget *ui;

};

#endif // MUSICSOUNDTOUCHWIDGET_H
