#ifndef MUSICWEBDJRADIOWIDGET_H
#define MUSICWEBDJRADIOWIDGET_H

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

#include "musicwidgetheaders.h"
#include "musicabstracttablewidget.h"
#include "musicdjradioprogramrequest.h"

class MusicWebDJRadioCategoryWidget;
class MusicWebDJRadioQueryWidget;

/*! @brief The class of the music dj radio program table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebDJRadioProgramTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioProgramTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebDJRadioProgramTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebDJRadioProgramTableWidget();

    /*!
     * Init widget.
     */
    void initialize(TTK::Program type);
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection() const;

Q_SIGNALS:
    /*!
     * Program item clicked.
     */
    void programItemClicked(const QString &rid, const QString &cid);

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Create the current program info item.
     */
    void createProgramItem(const MusicResultDataItem &item);
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &bytes);

private:
    MusicDJRadioProgramRequest *m_networkRequest;

};


/*! @brief The class of the music dj radio program widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebDJRadioProgramWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioProgramWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebDJRadioProgramWidget(TTK::Program type, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebDJRadioProgramWidget();

    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();

private:
    TTK::Program m_type;
    MusicWebDJRadioProgramTableWidget *m_tableWidget;

};


/*! @brief The class of the music dj radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebDJRadioWidget : public QStackedWidget, public TTKAbstractResizeInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebDJRadioWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebDJRadioWidget();

    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

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
    void currentCategoryClicked(const MusicResultDataItem &item);

private:
    /*!
     * Init the first widget.
     */
    void initFirstWidget();

    MusicWebDJRadioCategoryWidget *m_categoryWidget;
    MusicWebDJRadioProgramWidget *m_recommendWidget, *m_programWidget;
    MusicWebDJRadioQueryWidget *m_queryWidget;

};

#endif // MUSICWEBDJRADIOWIDGET_H
