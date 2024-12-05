#ifndef MUSICSONGCHECKTOOLSWIDGET_H
#define MUSICSONGCHECKTOOLSWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include "musicsongchecktoolsunit.h"
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSongCheckToolsWidget;
}

class MusicSongCheckToolsRenameThread;
class MusicSongCheckToolsDuplicateThread;
class MusicSongCheckToolsQualityThread;

/*! @brief The class of the song check tools widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongCheckToolsWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongCheckToolsWidget();

public Q_SLOTS:
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
    void renameCheckFinished(const MusicSongCheckToolsRenameList &items);
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
    void qualityCheckFinished(const MusicSongCheckToolsQualityList &items);
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
    void duplicateCheckFinished(const MusicSongCheckToolsDuplicateList &items);

private:
    /*!
     * Rename widget init.
     */
    void initRenameWidget();
    /*!
     * Quality widget init.
     */
    void initQualityWidget();
    /*!
     * Duplicate widget init.
     */
    void initDuplicateWidget();
    /*!
     * Switch to selected item style.
     */
    void switchToSelectedItemStyle(int index);

    Ui::MusicSongCheckToolsWidget *m_ui;
    MusicSongList m_localSongs;
    MusicSongCheckToolsRenameThread *m_renameThread;
    MusicSongCheckToolsDuplicateThread *m_duplicateThread;
    MusicSongCheckToolsQualityThread *m_qualityThread;

};

#endif // MUSICSONGCHECKTOOLSWIDGET_H
