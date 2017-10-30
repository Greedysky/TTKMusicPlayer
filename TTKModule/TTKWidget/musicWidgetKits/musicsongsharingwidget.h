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

#define WY_PL_SHARE    "Mm5CQXF6WFlZVmNKbHpsejFDZ2ZVZ0ZrSEQwSDRMRVJPQUdxcmdVaUMyQ3hJR2huMGhMSk04dkQxN3c9"
#define QQ_PL_SHARE    "dXBEejZlTGhhTmJvNGtlczdOSDJYNzVRZkVGM09CbmIzUGlwWGx3N28xOXB1MkpoT2c5WWVCTG9ISnRFd21YdW9JUkFKS2pqQW4xeFhEenpibG9GK0dtb1ptMFhBNnVBTEc5MUx3PT0="
#define KG_PL_SHARE    "blVpLzRYVFdhaXQzZjJhNzBMbWFPVWlRbm5qZTBqWFJ6Nmk4dDN3VGg1dkI4dmZZYWdNeXIxblFNVDFmUk10YjVzMEpkZz09"
#define BD_PL_SHARE    "YllZN3c4SmJkZ2xSZUdPWmRyWWRRRVhnbkVJQkNSZzN2L25IbVJjTVFFbXlBYmVXZm9FQy9BPT0="
#define KW_PL_SHARE    "OVJtRFhtWnZ3azB2RmhFM0ZCYlZmSVdCVFVVcDc0OHlocTRYKzdSL3NlWmd4UzcwWEVWZkYxK2J4N009"
#define XM_PL_SHARE    "N1JBdVpuUGJaVXZpK04wYVdEVHJiZkJhOHdPYzdHdkxTTGN6UEtMUUxmbkc1amFu"

#define WY_AR_SHARE    "bUlFS3FWQVIrajRTdktIblNyYjlzRnk5d0VOdVZMSnJpR05xUGFETE9SZXVJK2JOS1JVM3p3PT0="
#define QQ_AR_SHARE    "Q3R5bWRydnA3VzhtV2poV2MvanJjNTZxYm9ZelY0ZU1JU0xoRjZtems3Mmc2YlpsK21CUHpURVVKQkFnMEcvZy9PQURaZCtZalFURWJ4RmV1VUJpQ3JseTBCLzVIWVJN"
#define KG_AR_SHARE    "ZlFXdHRhenFreHA1QjFnMTFvWk8vZFdMTWpOOVA0RU5yWjBDTmxUMkJKMFhUOHJ2YVZhV1M1Q1BYMkFxTjliYQ=="
#define BD_AR_SHARE    "VUhPRG9SSnBGRW1lazhLR0I5YzlaUTF0RE90YTRlZWdpQ1lwUHAyNFNvQ2RsL1dF"
#define KW_AR_SHARE    "c25YQzhiU3dwRXc1djFtY2k4UCtNRDBmMVJmTFVGc3hvTE5pcmpVdkZxWGc0QzNLREcrNXhkZUtUZ0d5c0RoLw=="
#define XM_AR_SHARE    "TzFLM3Fqd1liTENOZG9najhqSkFNQVAyUTh5dTZiN1BndkVTbkx5aWR6Ni96M2hm"

#define WY_AL_SHARE    "REVSbCsrUjd2MXlmOWs3SjZqb2JSZzBZUWpwVWtweWRwK0dvREI5WFV6endQMUhOUDQrdThBPT0="
#define QQ_AL_SHARE    "QW9aQTJQTndxUENQSVVsV2xSbjkvdkxXVi9uVjlPanVoTFRVVS9EU3lKVGxIcFdUaUNRUTdrOHMxeW5JOFRsLyt1Nm5BcGc2VHM4K2xWVE5XSkZUZ0R0aThMOGl3WFdP"
#define KG_AL_SHARE    "NWdaK2VCeEtRSE1FMVoxbzVYVjdRR3NuSGlqaW9MZzREVlcybUR0STF5Y29ZMjhuVmIrcFdkb3lRbjlOSlFmKw=="
#define BD_AL_SHARE    "MWxsUFEvbENzRUQvM2E3OU5VMDBWK0F6NzVKTDZnTzJ4encvNGZpUDJIektTcFNK"
#define KW_AL_SHARE    "aWlVRVpnb2J3cXhPaVhrbkM5OHVRWmJiM1FuNW5aa3AyVU9GL2N5VmdZdz0="
#define XM_AL_SHARE    "RjcwU05kTXFsOG8xVW1hQVE3UGVZVFlHT1YrbmxqRzJsWG1vU25Md1Bub2UzUUhD"

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
    enum Type
    {
        Song,       /*!< song type*/
        Artist,     /*!< artist type*/
        Album,      /*!< album type*/
        Playlist,   /*!< playlist type*/
        Null        /*!< null type*/
    };

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
     * Set current data to share.
     */
    void setData(Type type, const QVariantMap &data);

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
    /*!
     * Download pixmap data finished.
     */
    void downLoadFinished(const QByteArray &data);

protected:
    Ui::MusicSongSharingWidget *m_ui;

    Type m_type;
    QVariantMap m_data;
    QRCodeQWidget *m_qrCodeWidget;

};

#endif // MUSICSONGSHARINGWIDGET_H
