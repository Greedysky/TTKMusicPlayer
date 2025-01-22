#ifndef QKUGOUWINDOW_H
#define QKUGOUWINDOW_H

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

#include <QLabel>
#include "qkugouurl.h"

class QKugouWindowPrivate;

/*! @brief The class of the kugou window widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QKugouWindow : public QLabel
{
    Q_OBJECT
public:
    enum Module
    {
        None,   /*!< KuGou None*/
        Song,   /*!< KuGou Song*/
        Radio,  /*!< KuGou Radio*/
        Movie,  /*!< Kugou Movie*/
        Single  /*!< KuGou Single*/
    };

    /*!
     * Object constructor.
     */
    explicit QKugouWindow(Module type, QWidget *parent = nullptr);

    /*!
     * Set current web viewer url to show.
     */
    void setUrl(const QString &url);

Q_SIGNALS:
    /*!
     * Switch to selected item style.
     */
    void buttonClicked(int index);

public Q_SLOTS:
    /*!
     * Set web index refresh.
     */
    void refresh();

private Q_SLOTS:
    /*!
     * Kugou song index changed.
     */
    void kugouSongIndexChanged(int index);

private:
    /*!
     * Create web view widget.
     */
    void createWebViewer();
    /*!
     * Create web view widget.
     */
    void createWebViewer(Module type);
    /*!
     * Create kugou song widget.
     */
    void createKugouSongWidget(bool power);

private:
    TTK_DECLARE_PRIVATE(QKugouWindow)

};

#endif // QKUGOUWINDOW_H
