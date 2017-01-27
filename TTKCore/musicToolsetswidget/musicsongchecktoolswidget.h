#ifndef MUSICSONGCHECKTOOLSWIDGET_H
#define MUSICSONGCHECKTOOLSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicsongchecktoolsunit.h"
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSongCheckToolsWidget;
}

class MusicSongCheckToolsRenameCore;
class MusicSongCheckToolsDuplicateCore;
class MusicSongCheckToolsQualityCore;

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

Q_SIGNALS:
    void getMusicLists(MusicSongItems &songs);
    /*!
     * Get music datas from container.
     */

public Q_SLOTS:
    void renameButtonClicked();
    /*!
     * Rename button clicked.
     */
    void renameButtonCheckClicked();
    /*!
     * Rename button check clicked.
     */
    void renameReCheckButtonClicked();
    /*!
     * Rename reCheck button clicked.
     */
    void renameCheckFinished(const SongCheckToolsRenames &items);
    /*!
     * Rename check finished.
     */
    void qualityButtonClicked();
    /*!
     * Quality button clicked.
     */
    void qualityButtonCheckClicked();
    /*!
     * Quality button check clicked.
     */
    void qualityReCheckButtonClicked();
    /*!
     * Quality reCheck button clicked.
     */
    void duplicateButtonClicked();
    /*!
     * Duplicate button clicked.
     */
    void duplicateButtonCheckClicked();
    /*!
     * Duplicate button check clicked.
     */
    void duplicateReCheckButtonClicked();
    /*!
     * Duplicate reCheck button clicked.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    void renameWidgetInit();
    /*!
     * Rename widget init.
     */
    void qualityWidgetInit();
    /*!
     * Quality widget init.
     */
    void duplicateWidgetInit();
    /*!
     * Duplicate widget init.
     */
    void switchToSelectedItemStyle(int index);
    /*!
     * Switch to selected item style.
     */

    Ui::MusicSongCheckToolsWidget *m_ui;

    MusicSongItems *m_localSongs;
    MusicSongCheckToolsRenameCore *m_renameCore;
    MusicSongCheckToolsDuplicateCore *m_duplicateCore;
    MusicSongCheckToolsQualityCore *m_qualityCore;

};

#endif // MUSICSONGCHECKTOOLSWIDGET_H
