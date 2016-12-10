#ifndef MUSICLRCERRORWIDGET_H
#define MUSICLRCERRORWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcErrorWidget;
}

/*! @brief The class of the report lrc error widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcErrorWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcErrorWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcErrorWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void buttonClicked(int index);
    /*!
     * Differ radio button clicked by index.
     */
    void textAreaChanged();
    /*!
     * Text area text value changed.
     */
    void confirmButtonClicked();
    /*!
     * Confirm button clicked.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicLrcErrorWidget *m_ui;

};

#endif // MUSICLRCERRORWIDGET_H
