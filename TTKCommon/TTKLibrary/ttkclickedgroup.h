#ifndef TTKCLICKEDGROUP_H
#define TTKCLICKEDGROUP_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QWidget>
#include "ttkmoduleexport.h"

/*! @brief The class of the label widget click group mapper.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKClickedGroup : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKClickedGroup(QObject *parent = nullptr);

    /*!
     * Add the clicked widget.
     */
    void addWidget(QWidget *widget, int id = -1);
    /*!
     * Remove the clicked widget.
     */
    void removeWidget(QWidget *widget);

    /*!
     * Get all container items.
     */
    QList<QWidget*> items() const;
    /*!
     * Get item widget by id.
     */
    QWidget *widget(int id) const;

    /*!
     * Set widget item id.
     */
    void setId(QWidget *widget, int id);
    /*!
     * Get widget item id.
     */
    int id(QWidget *widget) const;

Q_SIGNALS:
    /*!
     * Index widget click.
     */
    void clicked(int index);

private Q_SLOTS:
    /*!
     * Update the current clicked state.
     */
    void update();

private:
    struct Data
    {
        int m_id;
        QWidget *m_widget;

        inline bool operator ==(const Data &other) const noexcept
        {
            return m_widget == other.m_widget;
        }
    };

    QList<Data> m_container;

};

#endif // TTKCLICKEDGROUP_H
