#ifndef MUSICWEBMVRADIOINFOWIDGET_H
#define MUSICWEBMVRADIOINFOWIDGET_H

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

#include "musicabstractitemquerywidget.h"

/*! @brief The class of the music mv info table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebMVRadioInfoTableWidget : public MusicItemQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioInfoTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebMVRadioInfoTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebMVRadioInfoTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicAbstractQueryRequest *query) override final;
    /*!
     * Download query result data.
     */
    virtual void downloadQueryResult(int row) override final;

public Q_SLOTS:
    /*!
     * Subclass should implement this function.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Table widget item cell double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override final;
    /*!
     * Menu action type changed.
     */
    void actionChanged(QAction *action);

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

};


/*! @brief The class of the music mv info widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebMVRadioInfoWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioInfoWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebMVRadioInfoWidget(QWidget *parent = nullptr);

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

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
    void createMVRadioProgramItem(const MusicResultDataItem &item);
    /*!
     * Download button clicked now.
     */
    void downloadMVsButtonClicked();

private:
    /*!
     * Create init interface lables.
     */
    void createLabels();
    /*!
     * Init the third widget.
     */
    void initThirdWidget();

};

#endif // MUSICWEBMVRADIOINFOWIDGET_H
