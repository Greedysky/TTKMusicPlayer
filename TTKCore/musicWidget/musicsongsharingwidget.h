#ifndef MUSICSONGSHARINGWIDGET_H
#define MUSICSONGSHARINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

#define WEB_PLAYER     "d0FpN1NXRzF1Tk11TGFjTk5YYlpzcTduKzkxRDlzb01FVVlKU3gybDh4dGtOeHZVV1hyZGp3PT0="

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
    explicit MusicSongSharingWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongSharingWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name);
    /*!
     * Set current name to share.
     */

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */
    void textAreaChanged();
    /*!
     * User custom the current share text.
     */
    void confirmButtonClicked();
    /*!
     * Confirm button clicked.
     */
    void downLoadDataChanged(const QString &playUrl, const QString &imageUrl);
    /*!
     * Data download finished and send to shared on web.
     */
    void queryUrlTimeout();
    /*!
     * Query url time out, that means share failed.
     */
    void close();
    /*!
     * Override close function.
     */

protected:
    Ui::MusicSongSharingWidget *ui;
    QRCodeQWidget *m_qrCodeWidget;

};

#endif // MUSICSONGSHARINGWIDGET_H
