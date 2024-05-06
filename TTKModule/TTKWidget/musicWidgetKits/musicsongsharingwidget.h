#ifndef MUSICSONGSHARINGWIDGET_H
#define MUSICSONGSHARINGWIDGET_H

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

#include "musicabstractmovedialog.h"

static constexpr const char *WY_SG_SHARE = "TEVOTmViQm1OblVEM2JMUlhIM0ZoTk9sK2Y3aDJMb1I5cTRSQWYxemlVR0xyRVJsR1o3TUhRPT0=";
static constexpr const char *KG_SG_SHARE = "U3g3a2NLTnAvbGJjNXlqTjZuSkhHL0FDS2NMdTZ4U0UwN09jZEhLU0dYYnNMOXl1b3NNb3VRPT0=";
static constexpr const char *KW_SG_SHARE = "bThPbExsSllKcjU2MVpHa3NhaElaODNNczMyOHJ3N0RYakkzb1lUUEliYz0=";

static constexpr const char *WY_PL_SHARE = "cVhNOXVnOHp2ZVI1S21rd1RHOC8vWkE2ZWhLa0NFaHZuZlpXc2RBaU5KRWRnd091VVI2VDFMd21Pbms9";
static constexpr const char *KG_PL_SHARE = "ZEhtM3BHZUZ3UDJoWE5Kbi9Bc1JVcWxlbXdLZFIrc1JIbHlmR09CR2xEYzQ4K0JYN3JWWHBrbzYyUGpKOHBRZjArL0thQT09";
static constexpr const char *KW_PL_SHARE = "dG1JTHFsTms5UjdjbExUK0Q2emMwZnVpMS82blU3Tkt4OXRQcngvcS9SVFl6bWVFa2NjbnduTXltZnM9";

static constexpr const char *WY_AR_SHARE = "NHdtOUsxRjZ5YkdEOWhHVGcwelVwWVUwQTY5THZvUnk5d3hyZkdmdGhBN3BmU0htdllzeU13PT0=";
static constexpr const char *KG_AR_SHARE = "dVdUSGtqTEw0Z29vVHo4WUZEcWVsOVNBQ0hTNkhoQ0E3NnoyYlJ1R0xlc3VVQzlwamcrMm1CUUFmR2tJY1laQQ==";
static constexpr const char *KW_AR_SHARE = "YU4zcFJ3bndBRjZWMXVDeFdzbGpVV3Y4dm5aelV6STdPT3d3M3VQTGxjTUhmb2hvUUN1YkZ1QVZEaFk9";

static constexpr const char *WY_AL_SHARE = "WmJQamdNaS9CL1hWUmhsejFYMDdZZlpMd002MlhRdnBWSjlVcWtVZjJmNUtEMXRNcGZ6enFnPT0=";
static constexpr const char *KG_AL_SHARE = "dnFzTzNVaU1hUmJkTWwyTFNsdXNoNVB6SWZDN1MvRnRUTjlGQ05TZDIzanJZOVk0OVhyS2JwbHIzSE9sanErSA==";
static constexpr const char *KW_AL_SHARE = "SW5jUzJnLy9keTEraGNPWU9va2NGRjFmaFFidU5DTHFpTmZMa2NaQUtZbz0=";

static constexpr const char *WY_MV_SHARE = "Qjh6UWM4QjB4QkpBS3FWRVI5STZSZnZDVkRvMlhQLzZ5aklZMkFmbzRGM3BUNVJI";
static constexpr const char *KG_MV_SHARE = "Y1d2TGY3MVVJa0tXK2dweGEvdzVYRTVmWGFhdjE0VFZPRlhUc3dNZ0NjWWRsek5DdDFZcDRKVlN2NXc9";
static constexpr const char *KW_MV_SHARE = "ZVdIcDFqSnJHbGhWZDl4L01sNmtvTDRZeWVNMzNMc2FZQU9CemtHaldnND0=";

static constexpr const char *QQ_SHARE = "OXJEQURWRlk3c2ZJYjE2RXRtVVJyWVduaUNVMlFFdTNpY3l4U1pudS9CcFdudEZXdVFRL0VHZ3Ryc3NjWnBWM0JPeVY3UUIvWnNkNklVSEdhL3JzVEZTOEY3b015SnZIMkN2cXE1VytEWjhYOURMQ0FtZS9HWWlKd0xwaUJ0Rk4=";
static constexpr const char *QZONE_SHARE = "ck9FZHQzNmJkdzZBMC9KeGhCVGJNODlJUHYyYU5VUmF5eDFPdTJrZGQzZ2NzdFJlTkVRUjRPUGo0OTk0Zk5LWmY3S3BrRGQ2Vk15cUxYQXpraTZ3Vm45WGtMemJrL0FJQmJ1TXdTYTJoREJxcmEza2RHVktBS09HUGlGYVlFVTNrVHF5SVE9PQ==";
static constexpr const char *SINA_SHARE = "SDhiNk5UNFdDTnRneVBqcmNCOVdoOENsMGtXZGxZdVZJTWJncmFNbjRHZnpJVXdXZS9xM25tYW1hTDVsUlhDKzgzcHREK1M1Zlo1QkJsNnFNUk1WMEhPeldDUT0=";

class QRCodeQWidget;

namespace Ui {
class MusicSongSharingWidget;
}

/*! @brief The class of the song share widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongSharingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongSharingWidget)
public:
    enum class Module
    {
        Song,       /*!< song type*/
        Movie,      /*!< Movie type*/
        Artist,     /*!< artist type*/
        Album,      /*!< album type*/
        Playlist,   /*!< playlist type*/
        Null        /*!< null type*/
    };

    struct Item
    {
        QString m_id;
        QString m_name;
        QString m_cover;
        QString m_server;
    };

    /*!
     * Object constructor.
     */
    explicit MusicSongSharingWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongSharingWidget();

    /*!
     * Set current data to share.
     */
    void initialize(Module type, const MusicSongSharingWidget::Item &data);

public Q_SLOTS:
    /*!
     * User custom the current share text.
     */
    void textAreaChanged();
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();
    /*!
     * Query url time out, that means share failed.
     */
    void shareTimeout();
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &bytes);

private:
    /*!
     * Send to shared on web.
     */
    void sendToShare(const QString &playUrl, const QString &imageUrl);
    /*!
     * Send to shared on web.
     */
    void sendToShare(const QString &a, const QString &b, const QString &c);

private:
    Ui::MusicSongSharingWidget *m_ui;
    Module m_type;
    QRCodeQWidget *m_qrCodeWidget;
    MusicSongSharingWidget::Item m_data;

};

#endif // MUSICSONGSHARINGWIDGET_H
