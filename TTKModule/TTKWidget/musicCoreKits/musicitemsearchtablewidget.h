#ifndef MUSICITEMSEARCHTABLEWIDGET_H
#define MUSICITEMSEARCHTABLEWIDGET_H

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

#include "musicquerytablewidget.h"

/*! @brief The class of the search item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicItemSearchTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicItemSearchTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicItemSearchTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicItemSearchTableWidget();

    /*!
     * Start to search query by text.
     */
    virtual void startToSearchByText(const QString &text) override;

public Q_SLOTS:
    /*!
     * Search action menu clicked.
     */
    virtual void searchActionClicked(QAction *action);

protected:
    /*!
     * Create context menu.
     */
    void createContextMenu(QMenu &menu);
    /*!
     * Get current random simulation.
     */
    QString randSimulation() const;

    QActionGroup *m_actionGroup;

};

#endif // MUSICITEMSEARCHTABLEWIDGET_H
