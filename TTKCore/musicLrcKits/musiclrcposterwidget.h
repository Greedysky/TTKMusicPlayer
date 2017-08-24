#ifndef MUSICLRCPOSTERWIDGET_H
#define MUSICLRCPOSTERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcPosterWidget;
}

/*! @brief The class of the lrc art poster maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcPosterWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcPosterWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicLrcPosterWidget *m_ui;

};

#endif // MUSICLRCPOSTERWIDGET_H
