#ifndef MUSICWEBDJRADIOFOUNDWIDGET_H
#define MUSICWEBDJRADIOFOUNDWIDGET_H

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

#include "musicfoundabstractwidget.h"

class QGridLayout;
class MusicPagingWidgetObject;
class MusicWebDJRadioInfoWidget;

/*! @brief The class of the dj radio music item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioFoundItemWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioFoundItemWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioFoundItemWidget(QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioFoundItemWidget();

    /*!
     * Set music results item.
     */
    void setMusicResultsItem(const MusicResultsItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultsItem &item);

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    MusicResultsItem m_itemData;
    QPushButton *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel, *m_creatorLabel;

};



/*! @brief The class of the dj radio music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioFoundWidget();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

Q_SIGNALS:
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenu();

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createProgramItem(const MusicResultsItem &item);
    /*!
     * Current radio clicked.
     */
    void currentRadioClicked(const MusicResultsItem &item);
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenuClicked();
    /*!
     * Paging widget button has changed.
     */
    void buttonClicked(int index);

protected:
    bool m_firstInit;
    QGridLayout *m_gridLayout;
    MusicPagingWidgetObject *m_pagingWidgetObject;
    MusicWebDJRadioInfoWidget *m_infoWidget;

};

#endif // MUSICWEBDJRADIOFOUNDWIDGET_H
