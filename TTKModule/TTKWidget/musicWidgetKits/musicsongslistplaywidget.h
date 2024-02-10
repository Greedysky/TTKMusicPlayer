#ifndef MUSICSONGSLISTPLAYWIDGET_H
#define MUSICSONGSLISTPLAYWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QLabel>
#include <QPushButton>
#include "musicglobaldefine.h"

class QMenu;
class MusicSplitItemClickedLabel;
class MusicItemRenameEidt;

/*! @brief The class of the song list play widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsListPlayWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsListPlayWidget)
public:
    /*!
     * Object constructor by index.
     */
    explicit MusicSongsListPlayWidget(int index, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsListPlayWidget();

    /*!
     * Update item duration label.
     */
    void updateDurationLabel(const QString &current, const QString &total);
    /*!
     * Update current artist when it download finished.
     */
    void updateCurrentArtist();

    /*!
     * Update item artist label by name.
     */
    void initialize(const QString &name, const QString &path, QString &time);
    /*!
     * Rename item artist label name.
     */
    void enableRenameMode();

    /*!
     * Get rename item artist label state.
     */
    inline bool isRenameMode() { return m_renameEdit != nullptr; }
    /*!
     * Set current play index.
     */
    inline void setCurrentPlayIndex(int index) { m_currentPlayIndex = index; }
    /*!
     * Get current play index.
     */
    inline int currentPlayIndex() const { return m_currentPlayIndex; }

Q_SIGNALS:
    /*!
     * Rename item artist label is finised it.
     */
    void renameFinished(const QString &text);
    /*!
     * Mouse is the item geometry.
     */
    void enterChanged(int row, int column);

public Q_SLOTS:
    /*!
     * Delete rename item.
     */
    void deleteRenameItem();
    /*!
     * Rename item artist label is finised.
     */
    void changItemName(const QString &name);
    /*!
     * Reset current music love icon state.
     */
    void currentLoveStateClicked();
    /*!
     * Reset current music download icon state.
     */
    void currentDownloadStateClicked();

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    /*!
     * Create more menu information.
     */
    void createMoreMenu(QMenu *menu);
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */
    bool showArtistPicture(const QString &name) const;

    bool m_noCover;
    int m_currentPlayIndex;
    QLabel *m_artistPictureLabel, *m_durationLabel;
    MusicSplitItemClickedLabel *m_songNameLabel;
    QPushButton *m_loveButton, *m_deleteButton, *m_showMVButton;
    QPushButton *m_downloadButton, *m_moreButton;
    MusicItemRenameEidt *m_renameEdit;

};

#endif // MUSICSONGSLISTPLAYWIDGET_H
