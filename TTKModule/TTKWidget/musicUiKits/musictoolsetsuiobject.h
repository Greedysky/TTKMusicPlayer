#ifndef MUSICTOOLSETSUIOBJECT_H
#define MUSICTOOLSETSUIOBJECT_H

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

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace UI
    {
        static constexpr const char *SongsDetectStartBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_hover); }";

        static constexpr const char *SongsDetectStopBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_stop_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_stop_hover); }";

        static constexpr const char *SongsRedetectBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_redetect_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_redetect_hover); } \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_redetect_hover); }";

        static constexpr const char *SongsDetectDownloadBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_download_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_download_hover); } \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_download_disable); }";

        static constexpr const char *SongsDetectPlayBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_play_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_play_hover); } \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_play_disable); }";

        static constexpr const char *SongsDetectShareBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_share_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_share_hover); } \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_share_disable); }";

        static constexpr const char *SongsDetectUnloveBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_unlove_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_unlove_hover); } \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_unlove_disable); }";

        static constexpr const char *CheckTestDuplicate = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_duplicate_normal); } \
            QToolButton:hover{ background-image: url(:/toolSets/btn_duplicate_hover); }";

        static constexpr const char *CheckTestDuplicateClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_duplicate_clicked); }";

        static constexpr const char *CheckTestQuality = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_quality_normal); } \
            QToolButton:hover{ background-image: url(:/toolSets/btn_quality_hover); }";

        static constexpr const char *CheckTestQualityClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_quality_clicked); }";

        static constexpr const char *CheckTestRename = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_rename_normal); } \
            QToolButton:hover{ background-image: url(:/toolSets/btn_rename_hover); }";

        static constexpr const char *CheckTestRenameClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_rename_clicked); }";

        static constexpr const char *TimeToStop = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_timer_stop); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_timer_stop_hover); }";

        static constexpr const char *TimeToStopClicked = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_timer_stop_clicked); }";

        static constexpr const char *TimeToPlay = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_timer_play); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_timer_play_hover); }";

        static constexpr const char *TimeToPlayClicked = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_timer_play_clicked); }";

        static constexpr const char *TimeToDown = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_timer_down); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_timer_down_hover); }";

        static constexpr const char *TimeToDownClicked = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_timer_down_clicked); }";

        static constexpr const char *TransferSong = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_import_song_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_import_song_hover); }";

        static constexpr const char *TransferRing = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_import_ring_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_import_ring_hover); }";

        static constexpr const char *TransferChangedDev = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_change_dev_normal); } \
            QPushButton::menu-indicator{ image:none; } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_change_dev_hover); }";

        static constexpr const char *TransferRefreshDev = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_refresh_dev_normal); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_refresh_dev_hover); }";

        static constexpr const char *ScreenSaverOn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_saver_on); }";

        static constexpr const char *ScreenSaverOff = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_saver_off); }";

        static constexpr const char *ScreenItemEnable = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_saver_enable); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_saver_enable_hover); }";

        static constexpr const char *ScreenItemDisable = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_saver_disable); } \
            QPushButton:hover{ background-image: url(:/toolSets/btn_saver_disable_hover); }";

    }
}

#endif // MUSICTOOLSETSUIOBJECT_H
