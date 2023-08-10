#ifndef QKUGOUWINDOW_H
#define QKUGOUWINDOW_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QWidget>
#include "qkugouurl.h"

class QKugouWindowPrivate;

/*! @brief The class of the kugou window widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QKugouWindow : public QWidget
{
    Q_OBJECT
public:
    enum Module
    {
        KuGouSong,      /*!< KuGou Song*/
        KuGouRadio,     /*!< KuGou Radio*/
        KuGouList,      /*!< KuGou List*/
        KugouMovie,     /*!< Kugou Mv*/
        KuGouSingle     /*!< KuGou Single*/
    };

    /*!
     * Object constructor.
     */
    explicit QKugouWindow(Module type, QWidget *parent = nullptr);

    /*!
     * Set current web viewer url to show.
     */
    void setUrl(const QString &url);

public Q_SLOTS:
    /*!
     * Set web index go back.
     */
    void goBack();
    /*!
     * Set web index refresh.
     */
    void refresh();

private Q_SLOTS:
    /*!
     * Kugou song index changed.
     */
    void kugouSongIndexChanged(int index);
    /*!
     * Kugou radio index changed.
     */
    void kugouRadioIndexChanged(int index);
    /*!
     * Kugou MV index changed.
     */
    void kugouMVIndexChanged(int index);

private:
    /*!
     * Create web view widget.
     */
    void createWebViewer();
    /*!
     * Create kugou song widget.
     */
    void createKugouSongWidget();
    /*!
     * Create kugou radio widget.
     */
    void createKugouRadioWidget();
    /*!
     * Create kugou list widget.
     */
    void createKugouListWidget();
    /*!
     * Create kugou movie widget.
     */
    void createKugouMovieWidget();
    /*!
     * Create kugou single widget.
     */
    void createKugouSingleWidget();
    /*!
     * Change clicked button style by index.
     */
    void changeClickedButtonStyle(int index);

private:
    TTK_DECLARE_PRIVATE(QKugouWindow)

};

#endif // QKUGOUWINDOW_H
