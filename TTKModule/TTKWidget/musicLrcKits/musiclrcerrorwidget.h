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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcErrorWidget(QWidget *parent = 0);

    virtual ~MusicLrcErrorWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Differ radio button clicked by index.
     */
    void buttonClicked(int index);
    /*!
     * Text area text value changed.
     */
    void textAreaChanged();
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcErrorWidget *m_ui;

};

#endif // MUSICLRCERRORWIDGET_H
