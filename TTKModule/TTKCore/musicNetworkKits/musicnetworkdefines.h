#ifndef MUSICNETWORKDEFINES_H
#define MUSICNETWORKDEFINES_H

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

/*! @brief The namespace of the network data.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum class Download
    {
        Music,             /*!< type of dwonlaod music*/
        Lrc,               /*!< type of dwonlaod lrc*/
        Cover,             /*!< type of dwonlaod cover*/
        Background,        /*!< type of dwonlaod background*/
        Video,             /*!< type of dwonlaod video*/
        Extra              /*!< type of dwonlaod extra user module*/
    };

    enum class Record
    {
        NormalDownload,    /*!< Local Download File Config*/
        CloudDownload,     /*!< Cloud Download File Config*/
        CloudUpload,       /*!< Cloud Upload Failed File Config*/
        Null               /*!< None File Config*/
    };
}

static constexpr const char *DOWNLOAD_KEY_MUSIC = "DownloadMusic";
static constexpr const char *DOWNLOAD_KEY_LRC = "DownloadLrc";
static constexpr const char *DOWNLOAD_KEY_COVER = "DownloadCover";
static constexpr const char *DOWNLOAD_KEY_BACKGROUND = "DownloadBackground";
static constexpr const char *DOWNLOAD_KEY_VIDEO = "DownloadVideo";
static constexpr const char *DOWNLOAD_KEY_EXTRA = "DownloadExtra";

#endif // MUSICNETWORKDEFINES_H
