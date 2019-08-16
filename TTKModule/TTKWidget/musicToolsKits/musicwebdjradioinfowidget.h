#ifndef MUSICWEBDJRADIOINFOWIDGET_H
#define MUSICWEBDJRADIOINFOWIDGET_H

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

/*! @brief The class of dj radio music info table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioInfoTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioInfoTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioInfoTableWidget(QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioInfoTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicDownLoadQueryThreadAbstract *query) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

};


/*! @brief The class of dj radio music info widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioInfoWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioInfoWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioInfoWidget(QWidget *parent = nullptr);

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
    void queryAllFinished();
    /*!
     * Create the current category info item.
     */
    void createCategoryInfoItem(const MusicResultsItem &item);

protected:
    /*!
     * Create init interface lables.
     */
    void createLabels();

};

#endif // MUSICWEBDJRADIOINFOWIDGET_H
