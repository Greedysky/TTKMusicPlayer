/***************************************************************************
 *   Copyright (C) 2010-2015 by Ilya Kotov                                 *
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

#include <QRegExp>
#include <QSettings>
#include <qmmp/qmmp.h>
#include "gmehelper.h"

GmeHelper::GmeHelper()
{
     m_emu = 0;

     QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
     m_fade_length = settings.value("GME/fadeout_length", 7000).toInt();
     if(settings.value("GME/fadeout", false).toBool())
         m_fade_length = 0;
}

GmeHelper::~GmeHelper()
{
    if(m_emu)
        gme_delete(m_emu);
    m_emu = 0;
}

Music_Emu *GmeHelper::load(const QString &url, int sample_rate)
{
    if(m_emu)
        gme_delete(m_emu);
    m_emu = 0;
    QString path = url;
    if(url.contains("://"))
    {
        path.remove("gme://");
        path.remove(QRegExp("#\\d+$"));
    }
    const char *err = 0;
    gme_type_t file_type;
    if((err = gme_identify_file(qPrintable(path),&file_type)))
    {
        qWarning("GmeHelper: %s", err);
        return 0;
    }
    if(!file_type)
    {
        qWarning("DecoderGme: unsupported music type");
        return 0;
    }
    if(!(m_emu = gme_new_emu(file_type, sample_rate)))
    {
        qWarning("GmeHelper: out of memory");
        return 0;
    }
    if((err = gme_load_file(m_emu, qPrintable(path))))
    {
        qWarning("GmeHelper: %s", err);
        return 0;
    }
    QString m3u_path = path.left(path.lastIndexOf("."));
    m3u_path.append(".m3u");
    gme_load_m3u(m_emu, qPrintable(m3u_path));
    m_path = path;
    return m_emu;
}

QList <FileInfo*> GmeHelper::createPlayList(bool meta)
{
    QList <FileInfo*> list;
    if(!m_emu)
        return list;
    int count = gme_track_count(m_emu);
    gme_info_t *track_info;
    for(int i = 0; i < count; ++i)
    {
        FileInfo *info = new FileInfo();
        if(!gme_track_info(m_emu, &track_info, i))
        {
            if(track_info->length <= 0)
                track_info->length = track_info->intro_length + track_info->loop_length * 2;
        }
        if(track_info->length <= 0)
            track_info->length = (long) (2.5 * 60 * 1000);
        if(track_info->length < m_fade_length)
            track_info->length += m_fade_length;
        if(meta)
        {
            info->setMetaData(Qmmp::TITLE, track_info->song);
            info->setMetaData(Qmmp::ARTIST, track_info->author);
            info->setMetaData(Qmmp::COMMENT, track_info->comment);
            info->setMetaData(Qmmp::TRACK, i+1);
        }
        info->setPath("gme://" + m_path + QString("#%1").arg(i+1));
        info->setLength(track_info->length/1000);
        gme_free_info(track_info);
        list << info;
    }
    return list;
}

int GmeHelper::fadeLength()
{
    return m_fade_length;
}
