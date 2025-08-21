#ifndef MUSICTTKFMRADIOPLAYWIDGET_H
#define MUSICTTKFMRADIOPLAYWIDGET_H

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

#include "musicfmconfigmanager.h"
#include "musicabstractmovewidget.h"
#include "musicabstractmovedialog.h"

class QTreeWidgetItem;
class MusicCoreMPlayer;

namespace Ui {
class MusicTTKFMRadioPlayWidget;
class MusicTTKFMRadioInformationWidget;
}


/*! @brief The class of the web music radio information widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTTKFMRadioInformationWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicTTKFMRadioInformationWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTTKFMRadioInformationWidget();

    /*!
     * Set read only mode.
     */
    void setReadOnly(bool mode);
    /*!
     * Set channel information.
     */
    void setChannelInformation(const MusicFMChannel &channel);
    /*!
     * Get channel information.
     */
    MusicFMChannel channelInformation() const;

private:
    Ui::MusicTTKFMRadioInformationWidget *m_ui;
    bool m_readOnly;

};


/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTTKFMRadioPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicTTKFMRadioPlayWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTTKFMRadioPlayWidget();

public Q_SLOTS:
    /*!
     * Radio item clicked changed.
     */
    void radioItemChanged(QTreeWidgetItem *item, int column);
    /*!
     * Media play finished.
     */
    void mediaPlayFinished(int code);
    /*!
     * Set radio to play.
     */
    void radioPlay();
    /*!
     * Set radio to previous.
     */
    void radioPrevious();
    /*!
     * Set radio to next.
     */
    void radioNext();
    /*!
     * Set radio volume.
     */
    void radioVolume(int num);
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Add button clicked.
     */
    void addButtonClicked();
    /*!
     * Delete button clicked.
     */
    void deleteButtonClicked();
    /*!
     * Info button clicked.
     */
    void infoButtonClicked();

private:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override final;

    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Create core module.
     */
    void createCoreModule();
    /*!
     * Start to core module.
     */
    void startCoreModule();
    /*!
     * Check index valid.
     */
    int indexValid(QTreeWidgetItem *item) const;

    Ui::MusicTTKFMRadioPlayWidget *m_ui;
    bool m_isPlaying;
    bool m_statusChanged;
    int m_currentIndex;
    MusicCoreMPlayer *m_player;
    MusicFMChannelList m_items;
    MusicFMChannelList m_favItems;

};

#endif // MUSICTTKFMRADIOPLAYWIDGET_H
