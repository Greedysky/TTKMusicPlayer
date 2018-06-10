#ifndef MUSICNETWORKDEFINES_H
#define MUSICNETWORKDEFINES_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

/*! @brief The class of abstract downloading data.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicNetwork
{
    enum StateCode
    {
        Init = 0xFFFFF00,   /*!< Network state init*/
        Success = 0,        /*!< Network state success*/
        Error = -1,         /*!< Network state error*/
        UnKnow = 2,         /*!< Network state unknow*/
    };

    enum DownloadType
    {
        DownloadMusic,      /*!< type of dwonlaod music*/
        DownloadLrc,        /*!< type of dwonlaod lrc*/
        DownloadSmallBG,    /*!< type of dwonlaod small background*/
        DownloadBigBG,      /*!< type of dwonlaod big background*/
        DownloadVideo,      /*!< type of dwonlaod video*/
        DownloadOther       /*!< type of dwonlaod other user mod*/
    };

    enum RecordType
    {
        Null,               /*!< None File Config*/
        NormalDownload,     /*!< Local Download File Config*/
        CloudDownload,      /*!< Cloud Download File Config*/
        CloudUpload         /*!< Cloud Upload Failed File Config*/
    };

}

#endif // MUSICNETWORKDEFINES_H
