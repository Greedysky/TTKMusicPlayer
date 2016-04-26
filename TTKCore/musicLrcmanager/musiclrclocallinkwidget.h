#ifndef MUSICLRCLOCALLINKWIDGET_H
#define MUSICLRCLOCALLINKWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcLocalLinkWidget;
}

/*! @brief The class of the local lrc link widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcLocalLinkWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcLocalLinkWidget(QWidget *parent = 0);
    virtual ~MusicLrcLocalLinkWidget();

protected:
    Ui::MusicLrcLocalLinkWidget *ui;

};

#endif // MUSICLRCLOCALLINKWIDGET_H
