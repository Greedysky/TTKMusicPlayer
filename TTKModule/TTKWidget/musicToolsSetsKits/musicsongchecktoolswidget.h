#ifndef MUSICSONGCHECKTOOLSWIDGET_H
#define MUSICSONGCHECKTOOLSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    TTK_DECLARE_MODULE(MusicSongCheckToolsWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsWidget(QWidget *parent = nullptr);

    virtual ~MusicSongCheckToolsWidget();

Q_SIGNALS:
    /*!
     * Get music datas from container.
     */
    void getMusicLists(MusicSongItems &songs);

public Q_SLOTS:
    /*!
     * Modified item button clicked.
     */
    void modifiedItemButtonClicked();
    /*!
     * Item lists changed.
     */
    void itemListsChanged(const MIntList &items);
    /*!
     * Rename button clicked.
     */
    void renameButtonClicked();
    /*!
     * Rename button check clicked.
     */
    void renameButtonCheckClicked();
    /*!
     * Rename reCheck button clicked.
     */
    void renameReCheckButtonClicked();
    /*!
     * Rename check finished.
     */
    void renameCheckFinished(const MusicSongCheckToolsRenames &items);
    /*!
     * Quality button clicked.
     */
    void qualityButtonClicked();
    /*!
     * Quality button check clicked.
     */
    void qualityButtonCheckClicked();
    /*!
     * Quality reCheck button clicked.
     */
    void qualityReCheckButtonClicked();
    /*!
     * Quality check finished.
     */
    void qualityCheckFinished(const MusicSongCheckToolsQualitys &items);
    /*!
     * Duplicate button clicked.
     */
    void duplicateButtonClicked();
    /*!
     * Duplicate button check clicked.
     */
    void duplicateButtonCheckClicked();
    /*!
     * Duplicate reCheck button clicked.
     */
    void duplicateReCheckButtonClicked();
    /*!
     * Duplicate check finished.
     */
    void duplicateCheckFinished(const MusicSongCheckToolsDuplicates &items);
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Get selected song items.
     */
    void getSelectedSongItems();
    /*!
     * Rename widget init.
     */
    void renameWidgetInit();
    /*!
     * Quality widget init.
     */
    void qualityWidgetInit();
    /*!
     * Duplicate widget init.
     */
    void duplicateWidgetInit();
    /*!
     * Switch to selected item style.
     */
    void switchToSelectedItemStyle(int index);

    Ui::MusicSongCheckToolsWidget *m_ui;

    MusicSongs m_localSongs;
    bool m_selectedItemIdFlag;
    MIntList m_selectedItemIds;
    MusicSongCheckToolsRenameCore *m_renameCore;
    MusicSongCheckToolsDuplicateCore *m_duplicateCore;
    MusicSongCheckToolsQualityCore *m_qualityCore;

};

#endif // MUSICSONGCHECKTOOLSWIDGET_H
