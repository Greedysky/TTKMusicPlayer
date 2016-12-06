#ifndef MUSICSONGCHECKTOOLSWIDGET_H
#define MUSICSONGCHECKTOOLSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSongCheckToolsWidget;
}

/*! @brief The class of the song check tools widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongCheckToolsWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void renameButtonClicked();
    /*!
     * Rename button clicked.
     */
    void qualityButtonClicked();
    /*!
     * Quality button clicked.
     */
    void duplicateButtonClicked();
    /*!
     * Duplicate button clicked.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    void switchToSelectedItemStyle(int index);
    /*!
     * Switch to selected item style.
     */

    Ui::MusicSongCheckToolsWidget *m_ui;

};

#endif // MUSICSONGCHECKTOOLSWIDGET_H
