#ifndef KUGOUWINDOW_H
#define KUGOUWINDOW_H

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

#include <QWidget>
#include "kugouurl.h"
#include "ttkprivate.h"

class KugouWindowPrivate;

/*! @brief The namespace of the kugou window widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT KugouWindow : public QWidget
{
    Q_OBJECT
public:
    enum KuGouType
    {
        KuGouSong,      /*!< KuGou Song*/
        KuGouRadio,     /*!< KuGou Radio*/
        KuGouList,      /*!< KuGou List*/
        KugouMv,        /*!< Kugou Mv*/
        KuGouLive,      /*!< KuGou Live*/
        KuGouLrc,       /*!< KuGou Lrc*/
        KuGouSingle     /*!< KuGou Single*/
    };

    /*!
     * Object contsructor.
     */
    explicit KugouWindow(KuGouType type, QWidget *parent = nullptr);

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

protected:
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
     * Create kugou mv widget.
     */
    void createKugouMVWidget();
    /*!
     * Create kugou single widget.
     */
    void createKugouSingleWidget();
    /*!
     * Change clicked button style by index.
     */
    void changeClickedButtonStyle(int index);

private:
    TTK_DECLARE_PRIVATE(KugouWindow)

};

#endif // KUGOUWINDOW_H
