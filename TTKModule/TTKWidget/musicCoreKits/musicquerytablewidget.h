#ifndef MUSICQUERYTABLEWIDGET_H
#define MUSICQUERYTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include <QMenu>
#include <QActionGroup>
#include "musicfillitemtablewidget.h"
#include "musicabstractqueryrequest.h"

class MusicGifLabelWidget;

/*! @brief The class of the query table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQueryTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicQueryTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicQueryTableWidget();

    /*!
     * Set network query input.
     */
    void setQueryInput(MusicAbstractQueryRequest *query);
    /*!
     * Get network query input.
     */
    MusicAbstractQueryRequest *queryInput() const;

    /*!
     * Start to search query by text.
     * Subclass should implement this function.
     */
    virtual void startToSearchByText(const QString &text) = 0;
    /*!
     * Download query result data.
     * Subclass should implement this function.
     */
    virtual void downloadQueryResult(int row) = 0;

Q_SIGNALS:
    /*!
     * Start to query by text.
     */
    void startToQueryByText(const QString &text);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;
    /*!
     * Table widget item cell double clicked.
     * Subclass should implement this function.
     */
    virtual void itemDoubleClicked(int row, int column) = 0;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override;
    /*!
     * Create search result items.
     * Subclass should implement this function.
     */
    virtual void createResultItem(const MusicResultInfoItem &result) = 0;
    /*!
     * Create the search finished item.
     */
    void createFinishedItem();

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Set loading label status.
     */
    void setLoadingStatus(bool status) const;

    using MusicFillItemTableWidget::isValid;
    /*!
     * Check item row is valid or not.
     */
    inline bool isValid(int row) const noexcept
    {
        return !(row < 0 || row >= rowCount() - 1);
    }

    MusicGifLabelWidget *m_loadingLabel;
    TTKLabelItemDelegate *m_labelDelegate;
    MusicAbstractQueryRequest *m_networkRequest;

};

#endif // MUSICQUERYTABLEWIDGET_H
