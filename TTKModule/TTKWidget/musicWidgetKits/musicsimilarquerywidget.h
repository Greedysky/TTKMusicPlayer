#ifndef MUSICSIMILARQUERYWIDGET_H
#define MUSICSIMILARQUERYWIDGET_H

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
#include "musicitemquerytablewidget.h"

/*! @brief The class of the similar music query table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSimilarQueryTableWidget : public MusicItemQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSimilarQueryTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSimilarQueryTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSimilarQueryTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicAbstractQueryRequest *query) override final;

public Q_SLOTS:
    /*!
     * Create searched items.
     */
    virtual void createSearchedItem(const MusicResultInfoItem &songItem) override final;

};



/*! @brief The class of the similar music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSimilarQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSimilarQueryWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSimilarQueryWidget(QWidget *parent = nullptr);

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();

private:
    /*!
     * Create init interface lables.
     */
    void createLabels();

};

#endif // MUSICSIMILARQUERYWIDGET_H
