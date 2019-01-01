#ifndef MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
#define MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H

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

#include "musicabstracttablewidget.h"

class QPushButton;

/*! @brief The class of the search popup table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongSearchPopTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchPopTableWidget(QWidget *parent = nullptr);

    virtual ~MusicLocalSongSearchPopTableWidget();

    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Create popup table item by index and name and time.
     */
    void createItems(int index, const QString &name, const QString &time);

Q_SIGNALS:
    /*!
     * User click the current item, emit the current text.
     */
    void setText(const QString &text);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

};


/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongSearchPopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchPopWidget(QWidget *parent = nullptr);

    ~MusicLocalSongSearchPopWidget();

    /*!
     * Create popup search history table items.
     */
    void createItems();
    /*!
     * Create popup search suggest table items.
     */
    void createSuggestItems(const QStringList &names);

Q_SIGNALS:
    /*!
     * User click the current item, emit the current text.
     */
    void setText(const QString &text);

public Q_SLOTS:
    /*!
     * Clear the search history.
     */
    void clearButtonClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Transform utc time to local time before.
     */
    QString utcTimeToLocal(const QString &time) const;

    QPushButton *m_clearButton;
    MusicLocalSongSearchPopTableWidget *m_popTableWidget;

};

#endif // MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
