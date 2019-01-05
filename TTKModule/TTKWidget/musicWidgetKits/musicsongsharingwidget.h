#ifndef MUSICSONGSHARINGWIDGET_H
#define MUSICSONGSHARINGWIDGET_H

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

#include "musicabstractmovedialog.h"

#define WY_SG_SHARE    "TEVOTmViQm1OblVEM2JMUlhIM0ZoTk9sK2Y3aDJMb1I5cTRSQWYxemlVR0xyRVJsR1o3TUhRPT0="
#define QQ_SG_SHARE    "a3c2T3BwbVljRllHNFMxaEpzb3h3aC8ydVdtc1JYdWtxL2dsbVBNZkh1UDE3TVhKSlMyQUVRPT0="
#define KG_SG_SHARE    "U3g3a2NLTnAvbGJjNXlqTjZuSkhHL0FDS2NMdTZ4U0UwN09jZEhLU0dYYnNMOXl1b3NNb3VRPT0="
#define BD_SG_SHARE    "aTVNd1ZVTHc0K3UxeXhzV3lvNGhEUEl6cGN1UWNMblhlL2dwS01xTHl1cWZxS1Rm"
#define KW_SG_SHARE    "bThPbExsSllKcjU2MVpHa3NhaElaODNNczMyOHJ3N0RYakkzb1lUUEliYz0="
#define XM_SG_SHARE    "KzRXK2xrV3ZFY0h4cUFua3lHclRiMHRiN0dLc1drdXIyeEI0Nm9IVzVHbz0="


#define WY_PL_SHARE    "cVhNOXVnOHp2ZVI1S21rd1RHOC8vWkE2ZWhLa0NFaHZuZlpXc2RBaU5KRWRnd091VVI2VDFMd21Pbms9"
#define QQ_PL_SHARE    "aXRnOVpIRitYNWhPQnQrd2VKeDB2M1RDWTlLL005emtNRXpwd0s2djJaT0d6N0JjS0dyS0lLREg3eU0zcURTU3hJWXB1ejQrNlRLMzVabktTR0xRMWo2MHk0cHBHTGh5"
#define KG_PL_SHARE    "ZEhtM3BHZUZ3UDJoWE5Kbi9Bc1JVcWxlbXdLZFIrc1JIbHlmR09CR2xEYzQ4K0JYN3JWWHBrbzYyUGpKOHBRZjArL0thQT09"
#define BD_PL_SHARE    "NGRqT1UvRnNaeE1sNWMzTEl4TUg3Z0JaUFNOaU5iT1JZMDArYzlRK2h6Mmd0RUpLa3pJemxnPT0="
#define KW_PL_SHARE    "dG1JTHFsTms5UjdjbExUK0Q2emMwZnVpMS82blU3Tkt4OXRQcngvcS9SVFl6bWVFa2NjbnduTXltZnM9"
#define XM_PL_SHARE    "WXk4Wi90bmN0N3QzUFFuUFZuOWY4cTJ5R2hUUDhkeDRIZm8yd0lueTAxYkI0YndV"


#define WY_AR_SHARE    "NHdtOUsxRjZ5YkdEOWhHVGcwelVwWVUwQTY5THZvUnk5d3hyZkdmdGhBN3BmU0htdllzeU13PT0="
#define QQ_AR_SHARE    "U3RUdnpKZFVQOWMxVzFYSDU3ei9iWUNiVFRabUxOYVc4NjlNUXBwZWlrc3Z1WHY0cUN5WWVxR0hQV0x6d0p4N2VDOUIzdEJWQWhuajlaTHJReEgxblVqRGF5N1lYYmZN"
#define KG_AR_SHARE    "dVdUSGtqTEw0Z29vVHo4WUZEcWVsOVNBQ0hTNkhoQ0E3NnoyYlJ1R0xlc3VVQzlwamcrMm1CUUFmR2tJY1laQQ=="
#define BD_AR_SHARE    "RGk5M1VMTFlaeTJPbEpDZ01JMGg1cUFDMHJIR3ZaMnB3RUpoWEtUUUdqOGxKaUszeXFrSk1nPT0="
#define KW_AR_SHARE    "OVhBOEs3RUtNZ1VleUVRcldtdUFVL21DVU90ZjJ2Rm1oVFhJT2hPOXAxWEtmUnFUZDA0Q29BelN0SEhiREtEL1dzZm52dz09"
#define XM_AR_SHARE    "QjRRMDM1ZldLY1Y0YWJ2eGpIaWRxWWUrT0ZGdGY3NVJYNlNMbEJqRlZTOStkL0VQ"


#define WY_AL_SHARE    "WmJQamdNaS9CL1hWUmhsejFYMDdZZlpMd002MlhRdnBWSjlVcWtVZjJmNUtEMXRNcGZ6enFnPT0="
#define QQ_AL_SHARE    "N0E0WlJUeEM1aHFVN0E4RzE1dVV5THRrNlRHMXNFcHF6aHlVcHM4V0ZyekFnVXNiY2hWNFg0YWNNeXdXNDU4NVdNbWtLc0dxV3JzWThvcnNhWFdFOTB3ZldBS3FlOVVS"
#define KG_AL_SHARE    "dnFzTzNVaU1hUmJkTWwyTFNsdXNoNVB6SWZDN1MvRnRUTjlGQ05TZDIzanJZOVk0OVhyS2JwbHIzSE9sanErSA=="
#define BD_AL_SHARE    "Uk5yQzl5ZkYvNzlyWnA0bUt0K2RpMEpSQlM5dGZnMDRrajJGTFRFaDlRWk1wTCtm"
#define KW_AL_SHARE    "SW5jUzJnLy9keTEraGNPWU9va2NGRjFmaFFidU5DTHFpTmZMa2NaQUtZbz0="
#define XM_AL_SHARE    "MGkwUlpOVkRlb09Db09tS0pWZ1E5S2g0RklXYUNaa2lhM2ppRFZrdU5jTHlLQVo0"


#define WY_MV_SHARE    "Qjh6UWM4QjB4QkpBS3FWRVI5STZSZnZDVkRvMlhQLzZ5aklZMkFmbzRGM3BUNVJI"
#define QQ_MV_SHARE    "RTNiSlFxbEFrQ3lwaVgwNE5udHFwVXJNVEtsSTJscU90TnVBT0dvM1JzbUVTMUJlMGxyc3ZRPT0="
#define KG_MV_SHARE    "eU9FKy9aWitpa3MwNytKMGZHMHdkaUVMUmhuczl0ZmhMeHMrdE53eG5FYXRyVzE3ZkIzQ0VvTm00b3FPZTNkaQ=="
#define BD_MV_SHARE    "d1FRbnByT3NaTVVFS1R3M0JWVHNOV0xxWk5TVWs0bUJ2UGJ2WjNMelpBbz0="
#define KW_MV_SHARE    "L3ptNGdkZkVuVU56Mmt1ZVVhWjBwQjY2aU1hT1pFaWpvM0NPVWFIOE0wZz0="
#define XM_MV_SHARE    "Nm1WRVhtcituMU54SXJDdzlLMHJkWm1RdkE4ODVvWmcrQWRSZFp5V1BWRT0="
#define YT_MV_SHARE    "MUMydEZmZkVmL1RLaTY2YkRjNTBYRXhkbERGODl1cGNUc1lMMURTZFo0UUNzc2My"


#define QQ_SHARE       "OXJEQURWRlk3c2ZJYjE2RXRtVVJyWVduaUNVMlFFdTNpY3l4U1pudS9CcFdudEZXdVFRL0VHZ3Ryc3NjWnBWM0JPeVY3UUIvWnNkNklVSEdhL3JzVEZTOEY3b015SnZIMkN2cXE1VytEWjhYOURMQ0FtZS9HWWlKd0xwaUJ0Rk4="
#define RENREN_SHARE   "QzlIUFF1MGhOV1ZKR2MxRWdIRkNvODVpZkY3YVFUL0hqM09sYy9SUDVVL1pNYXlWRVhBcHVHcVIxV0VyeDdvWEdGOFN1L0NYN3FOMnlMWEQ4bE9DRUp4blQyeDhHQ3N4"
#define QQ_SPACE_SHARE "ck9FZHQzNmJkdzZBMC9KeGhCVGJNODlJUHYyYU5VUmF5eDFPdTJrZGQzZ2NzdFJlTkVRUjRPUGo0OTk0Zk5LWmY3S3BrRGQ2Vk15cUxYQXpraTZ3Vm45WGtMemJrL0FJQmJ1TXdTYTJoREJxcmEza2RHVktBS09HUGlGYVlFVTNrVHF5SVE9PQ=="
#define QQ_MICBG_SHARE "Ky9NWkJNeWJRWGtTVEMzNDgxd0JCREJzMitXV2ZiZnJoc2NHSVYvR3lmRzhyRkcyQVFRTmxoZkxUMnBzQnpYd1IxNmRmbGFQcTg1VGhubDZJWTAwSW1XemlEM1ZiT3M5L2JsNnNxOUtmZG89"
#define SINA_SHARE     "SDhiNk5UNFdDTnRneVBqcmNCOVdoOENsMGtXZGxZdVZJTWJncmFNbjRHZnpJVXdXZS9xM25tYW1hTDVsUlhDKzgzcHREK1M1Zlo1QkJsNnFNUk1WMEhPeldDUT0="

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
    TTK_DECLARE_MODULE(MusicSongSharingWidget)
public:
    enum Type
    {
        Song,       /*!< song type*/
        Movie,      /*!< Movie type*/
        Artist,     /*!< artist type*/
        Album,      /*!< album type*/
        Playlist,   /*!< playlist type*/
        Null        /*!< null type*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicSongSharingWidget(QWidget *parent = nullptr);

    virtual ~MusicSongSharingWidget();

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
