#ifndef MUSICSONGSEARCHPOPTABLEWIDGET_H
#define MUSICSONGSEARCHPOPTABLEWIDGET_H

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

#include "musicabstracttablewidget.h"

class QPushButton;

/*! @brief The class of the search popup table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongSearchPopTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongSearchPopTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongSearchPopTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongSearchPopTableWidget();

    /*!
     * Select search history table items.
     */
    void selectRow(bool up);

    /*!
     * Create cell item by input data.
     */
    void addCellItem(int index, const QString &name, const QString &time);

Q_SIGNALS:
    /*!
     * User click the current item.
     */
    void setText(const QString &text, bool update);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;

};


/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongSearchPopWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongSearchPopWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongSearchPopWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongSearchPopWidget();

    /*!
     * Create popup search history table items.
     */
    void initialize(QWidget *parent);

    /*!
     * Select search history table items.
     */
    void selectRow(bool up);

    /*!
     * set control module enabled.
     */
    void setControlEnabled(bool enabled);
    /*!
     * Create popup search suggest table items.
     */
    void addCellItems(const QStringList &names);

Q_SIGNALS:
    /*!
     * User click the current item.
     */
    void setText(const QString &text);

public Q_SLOTS:
    /*!
     * Clear the search history.
     */
    void clearButtonClicked();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    /*!
     * Transform utc time to local time before.
     */
    QString utcTimeToLocal(const QString &time) const;

    QFrame *m_frame;
    QPushButton *m_clearButton;
    MusicSongSearchPopTableWidget *m_popTableWidget;

};

#endif // MUSICSONGSEARCHPOPTABLEWIDGET_H
