#ifndef MUSICSONGSHARINGWIDGET_H
#define MUSICSONGSHARINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include "musicabstractmovedialog.h"

#define WEB_PLAYER     "UnA2T0FHR3dMb2tnQ1JrcWZ4RHY3U1VIdW45RmlOTUNiY1dPd0owSlQxS2NSbEpK"

#define QQ_SHARE       "UlBic0d5OXhCUThoTWlLU3NGZlRUby9MVGhzdmZ2WGdva3ZzT3FzYXRpWXRjaS9MaXpiaG1GTmd6RzFTMG1BNlRaU3JNTEkySkJmd08vYVJ3c3hlNFczT2xUUnAvTWVrbE8yMzA4SkxGdFgvWUNqUUtWWXJydk9Jb1gyTUVYMDI="
#define RENREN_SHARE   "MXNzTTYvK3FOUGExUVBMdi9BOS9tMDlWNUJxSW9pbHpBQW9ucisxUWtNZG51UjhPSXBWa3R6Z3VsSXNaNjZ3TmVydml1NmpEanBuam1nTTlGWHh4NnNlZEFHQmtrUFRT"
#define QQ_SPACE_SHARE "WlA2UFBRQVJ2djExYlBwaTFzUVV6MlBLUDJxR0k0TzRudWZWbWdpb3pPL0JCb0dOdXRYcktvUVlFMFVmYnpCT3RVZVZ2VDZiSUNNKzMzYlpBVnZ0UEQxalc0S1ZlV2hjZFZONHVmczN2bmtVZFBuTXUxWXhDQzdveXFuSVByWFpDc1hyZEE9PQ=="
#define QQ_MICBG_SHARE "bVIzdk1WZ3ZaWEFFQW1QcXhhUU8rNHNjcGpGTkRGc0dITytJVUs4a0dnRFNBQkV6Y1NleUgwM3pCbkxtcitxM2ZvREVrSTBWSytISjMxb09QdmRyeHJVRHlmb0ExcVFyUXRnWmswN1dUemc9"
#define SINA_SHARE     "cGoxY3doN0ljQWRHZGR1Y1ZzZUhzci83S3NZSk95SG00RXFhdmFyQ1NGODVIa2FKT2UyYWIybGtWbTlnYW5qT3RCOGZNMTNuVGtUZnlOeVl2RWUzdFV5UXRHWT0="

class QRCodeQWidget;

namespace Ui {
class MusicSongSharingWidget;
}

/*! @brief The class of the song share widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongSharingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongSharingWidget(QWidget *parent = 0);

    virtual ~MusicSongSharingWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current name to share.
     */
    void setSongName(const QString &name);

public Q_SLOTS:
    /*!
     * Override exec function.
     */
    virtual int exec();
    /*!
     * User custom the current share text.
     */
    void textAreaChanged();
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();
    /*!
     * Data download finished and send to shared on web.
     */
    void downLoadDataChanged(const QString &playUrl, const QString &imageUrl);
    /*!
     * Query url time out, that means share failed.
     */
    void queryUrlTimeout();
    /*!
     * Override close function.
     */
    void close();

protected:
    Ui::MusicSongSharingWidget *m_ui;
    QRCodeQWidget *m_qrCodeWidget;

};

#endif // MUSICSONGSHARINGWIDGET_H
