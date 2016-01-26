/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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

#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <QTextStream>
#include <QTextCodec>
#include <qmmp/decoder.h>
#include <qmmp/metadatamanager.h>
#include "cueparser.h"

CUEParser::CUEParser(const QByteArray &array, const QString &fileName)
{
    QString album, genre, date, comment;
    QTextStream textStream (array);
    textStream.setCodec("UTF-8");
    m_filePath = fileName;
    QString artist;
    double album_peak = 0.0, album_gain = 0.0;
    while (!textStream.atEnd())
    {
        QString line = textStream.readLine().trimmed();
        QStringList words = splitLine(line);
        if (words.size() < 2)
            continue;

        if (words[0] == "PERFORMER")
        {
            if(m_tracks.isEmpty())
                artist = words[1];
            else
                m_tracks.last()->info.setMetaData(Qmmp::ARTIST, words[1]);
        }
        else if (words[0] == "TITLE")
        {
            if(m_tracks.isEmpty())
                album = words[1];
            else
                m_tracks.last()->info.setMetaData(Qmmp::TITLE, words[1]);
        }
        else if (words[0] == "TRACK")
        {
            FileInfo info("flac://" + fileName + QString("#%1").arg(words[1].toInt()));
            info.setMetaData(Qmmp::TRACK, words[1].toInt());
            info.setMetaData(Qmmp::ALBUM, album);
            info.setMetaData(Qmmp::GENRE, genre);
            info.setMetaData(Qmmp::YEAR, date);
            info.setMetaData(Qmmp::COMMENT, comment);
            info.setMetaData(Qmmp::ARTIST, artist);

            m_tracks << new CUETrack;
            m_tracks.last()->info = info;
            m_tracks.last()->offset = 0;
            m_tracks.last()->replayGain.insert(Qmmp::REPLAYGAIN_ALBUM_GAIN, album_gain);
            m_tracks.last()->replayGain.insert(Qmmp::REPLAYGAIN_ALBUM_PEAK, album_peak);
        }
        else if (words[0] == "INDEX" && words[1] == "01")
        {
            if (m_tracks.isEmpty())
                continue;
            m_tracks.last()->offset = getLength(words[2]);
            int c = m_tracks.count();
            if(c > 1)
                m_tracks[c - 2]->info.setLength(m_tracks[c - 1]->offset - m_tracks[c - 2]->offset);
        }
        else if (words[0] == "REM")
        {
            if (words.size() < 3)
                continue;
            if (words[1] == "GENRE")
                genre = words[2];
            else if (words[1] == "DATE")
                date = words[2];
            else if (words[1] == "COMMENT")
                comment = words[2];
            else if (words[1] == "REPLAYGAIN_ALBUM_GAIN")
                album_gain = words[2].toDouble();
            else if (words[1] == "REPLAYGAIN_ALBUM_PEAK")
                album_peak = words[2].toDouble();
            else if (words[1] == "REPLAYGAIN_TRACK_GAIN" && !m_tracks.isEmpty())
                m_tracks.last()->replayGain.insert(Qmmp::REPLAYGAIN_TRACK_GAIN, words[2].toDouble());
            else if (words[1] == "REPLAYGAIN_TRACK_PEAK" && !m_tracks.isEmpty())
                m_tracks.last()->replayGain.insert(Qmmp::REPLAYGAIN_TRACK_PEAK, words[2].toDouble());
        }
    }
    if(m_tracks.isEmpty())
    {
        qWarning("CUEParser: invalid cue file");
        return;
    }
    //calculate last item length
    QList <FileInfo *> f_list = MetaDataManager::instance()->createPlayList(m_filePath, false);
    qint64 l = f_list.isEmpty() ? 0 : f_list.at(0)->length() * 1000;
    if (l > m_tracks.last()->offset)
        m_tracks.last()->info.setLength(l - m_tracks.last()->offset);
    else
        m_tracks.last()->info.setLength(0);
}

CUEParser::~CUEParser()
{
    qDeleteAll(m_tracks);
    m_tracks.clear();
}

QList<FileInfo*> CUEParser::createPlayList()
{
    QList<FileInfo*> list;
    foreach(CUETrack *track, m_tracks)
    {
        list << new FileInfo(track->info);
        list.last()->setLength(track->info.length()/1000);
    }
    return list;
}

const QString CUEParser::filePath() const
{
    return m_filePath;
}

qint64 CUEParser::offset(int track) const
{
    return m_tracks.at(track - 1)->offset;
}

qint64 CUEParser::length(int track) const
{
    return m_tracks.at(track - 1)->info.length();
}

int CUEParser::count() const
{
    return m_tracks.count();
}

FileInfo *CUEParser::info(int track)
{
    return &m_tracks.at(track - 1)->info;
}

const QString CUEParser::trackURL(int track) const
{
    return m_tracks.at(track - 1)->info.path();
}

const QMap<Qmmp::ReplayGainKey, double>  CUEParser::replayGain(int track) const
{
    return m_tracks.at(track - 1)->replayGain;
}

QStringList CUEParser::splitLine(const QString &line)
{
    //qDebug("raw string = %s",qPrintable(line));
    QStringList list;
    QString buf = line.trimmed();
    if (buf.isEmpty())
        return list;
    while (!buf.isEmpty())
    {
        //qDebug(qPrintable(buf));
        if (buf.startsWith('"'))
        {
            int end = buf.indexOf('"',1);
            if(end == -1) //ignore invalid line
            {
                list.clear();
                qWarning("CUEParser: unable to parse line: %s",qPrintable(line));
                return list;
            }
            list << buf.mid (1, end - 1);
            buf.remove (0, end+1);
        }
        else
        {
            int end = buf.indexOf(' ', 0);
            if (end < 0)
                end = buf.size();
            list << buf.mid (0, end);
            buf.remove (0, end);
        }
        buf = buf.trimmed();
    }
    return list;
}

qint64 CUEParser::getLength(const QString &str)
{
    QStringList list = str.split(":");
    if (list.size() == 2)
        return (qint64)list.at(0).toInt()*60000 + list.at(1).toInt()*1000;
    else if (list.size() == 3)
        return (qint64)list.at(0).toInt()*60000 + list.at(1).toInt()*1000 + list.at(2).toInt()*1000/75;
    return 0;
}

QString CUEParser::getDirtyPath(const QString &cue, const QString &path)
{

    if (Decoder::findByPath(path) || ! m_dirty)
        return path;

    QStringList candidates;
    QDirIterator it(QFileInfo(path).dir().path(), QDir::Files);
    while (it.hasNext())
    {
        it.next();
        QString f = it.filePath();
        if ((f != cue) && Decoder::findByPath(f))
            candidates.push_back(f);
    }

    if (candidates.empty())
        return path;
    else if (candidates.count() == 1)
        return candidates.first();

    int dot = cue.lastIndexOf('.');
    if (dot != -1)
    {
        QRegExp r(QRegExp::escape(cue.left(dot)) + "\\.[^\\.]+$");

        int index = candidates.indexOf(r);
        int rindex = candidates.lastIndexOf(r);

        if ((index != -1) && (index == rindex))
            return candidates[index];
    }
    dot = path.lastIndexOf('.');
    if (dot != -1)
    {
        QRegExp r(QRegExp::escape(path.left(dot)) + "\\.[^\\.]+$");

        int index = candidates.indexOf(r);
        int rindex = candidates.lastIndexOf(r);

        if ((index != -1) && (index == rindex))
            return candidates[index];
    }

    return path;
}
