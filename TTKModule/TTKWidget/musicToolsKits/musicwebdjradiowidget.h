#ifndef MUSICWEBDJRADIOWIDGET_H
#define MUSICWEBDJRADIOWIDGET_H

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

#include "musicwidgetheaders.h"
#include "musicabstracttablewidget.h"
#include "musicdjradioprogramthread.h"

class MusicWebDJRadioCategoryWidget;
class MusicWebDJRadioFoundWidget;

/*! @brief The class of music dj radio program table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioProgramTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioProgramTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioProgramTableWidget(QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioProgramTableWidget();

    /*!
     * Init widget.
     */
    void init(MusicObject::Program type);
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

Q_SIGNALS:
    /*!
     * Program item clicked.
     */
    void programItemClicked(const QString &rid, const QString &cid);

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Create the current program info item.
     */
    void createProgramItem(const MusicResultsItem &item);
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data, const QVariantMap &ext);

protected:
    MusicDJRadioProgramThread *m_programThread;

};


/*! @brief The class of music dj radio program widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioProgramWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioProgramWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioProgramWidget(MusicObject::Program type, QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioProgramWidget();

    /*!
     * Init widget.
     */
    void init();
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

protected:
    MusicObject::Program m_type;
    MusicWebDJRadioProgramTableWidget *m_tableWidget;

};


/*! @brief The class of music dj radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioWidget : public QStackedWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioWidget(QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioWidget();

    /*!
     * Init widget.
     */
    void init();
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenu();
    /*!
     * Create recommend widget.
     */
    void createRecommendWidget();
    /*!
     * Create program widget.
     */
    void createProgramWidget();
    /*!
     * Program item clicked.
     */
    void programItemClicked(const QString &rid, const QString &cid);
    /*!
     * Current category clicked.
     */
    void currentCategoryClicked(const MusicResultsItem &item);

protected:
    /*!
     * Init the first widget.
     */
    void initFirstWidget();
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    MusicWebDJRadioCategoryWidget *m_categoryWidget;
    MusicWebDJRadioProgramWidget *m_recommendWidget, *m_programWidget;
    MusicWebDJRadioFoundWidget *m_foundWidget;

};

#endif // MUSICWEBDJRADIOWIDGET_H
