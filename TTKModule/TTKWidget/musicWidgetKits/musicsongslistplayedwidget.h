#ifndef MUSICSONGSLISTPLAYEDWIDGET_H
#define MUSICSONGSLISTPLAYEDWIDGET_H

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

#include <QLabel>
#include "musicglobaldefine.h"

class QMenu;
class QPushButton;
class MusicGifLabelWidget;

/*! @brief The class of the song list played widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsListPlayedWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsListPlayedWidget(int index, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsListPlayedWidget();

    /*!
     * Update item artist label by name.
     */
    void initialize(const QString &name);

Q_SIGNALS:
    /*!
     * Mouse is the item geometry.
     */
    void enterChanged(int row, int column);

public Q_SLOTS:
    /*!
     * Remove item from list at current row.
     */
    void removeItemAt();

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    /*!
     * Create more menu information.
     */
    void createMoreMenu(QMenu *menu);

    int m_currentPlayIndex;
    QWidget *m_parent;
    QLabel *m_textLabel;
    MusicGifLabelWidget *m_gifLabel;
    QPushButton *m_deleteButton, *m_downloadButton, *m_moreButton;

};

#endif // MUSICSONGSLISTPLAYEDWIDGET_H
