/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef FFMPEGMETADATAMODEL_H
#define FFMPEGMETADATAMODEL_H

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#if (LIBAVUTIL_VERSION_INT >= ((51<<16)+(32<<8)+0))
#include <libavutil/dict.h>
#endif
}

#include <qmmp/metadatamodel.h>

class FFmpegMetaDataModel : public MetaDataModel
{
Q_OBJECT
public:
    FFmpegMetaDataModel(const QString &path, QObject *parent);
    ~FFmpegMetaDataModel();
    QHash<QString, QString> audioProperties();

private:
    AVFormatContext *m_in;
};

#endif // FFMPEGMETADATAMODEL_H
