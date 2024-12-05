#ifndef MUSICPAGEQUERYWIDGET_H
#define MUSICPAGEQUERYWIDGET_H

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

#include "musicglobaldefine.h"

class TTKClickedLabel;

/*! @brief The class of the page query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPageQueryWidget : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPageQueryWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPageQueryWidget(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPageQueryWidget();

    /*!
     * Create page items.
     */
    QWidget* createPageWidget(QWidget *parent, int total);
    /*!
     * Reset page to origin.
     */
    void reset(int total);
    /*!
     * Start to page by given index and total.
     */
    void page(int index, int total);
    /*!
     * Get current page index.
     */
    int currentIndex() const;

Q_SIGNALS:
    /*!
     * Mapped the clicked page index.
     */
    void clicked(int index);

private:
    /*!
     * Update button status.
     */
    void updateStatus();

private:
    int m_currentPage;
    QWidget *m_pageWidget;
    QList<TTKClickedLabel*> m_pageItems;

};

#endif // MUSICPAGEQUERYWIDGET_H
