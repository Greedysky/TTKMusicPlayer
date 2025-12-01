#include <QTextStream>
#include "qmmptextcodec.h"
#include "cueparser.h"

CueParser::CueParser(const QByteArray &data, const QByteArray &codecName)
{
    loadData(data, codecName);
}

CueParser::~CueParser()
{
    clear();
}

void CueParser::loadData(const QByteArray &data, const QByteArray &codecName)
{
    QmmpTextCodec codec(codecName);
    loadData(data, &codec);
}

void CueParser::loadData(const QByteArray &data, QmmpTextCodec *codec)
{
    clear();

    QString artist, album, genre, date, comment, file;
    double album_peak = 0.0, album_gain = 0.0;

    QString str = codec->toUnicode(data);
    QTextStream textStream(&str, QIODevice::ReadOnly);

    while(!textStream.atEnd())
    {
        const QString &line = textStream.readLine().trimmed();
        const QStringList &words = splitLine(line);
        if(words.count() < 2)
            continue;

        if(words[0] == "FILE")
        {
            file = words[1];
            m_files << file;
        }
        else if(words[0] == "PERFORMER")
        {
            if(m_tracks.isEmpty())
                artist = words[1];
            else
                m_tracks.last()->info.setValue(Qmmp::ARTIST, words[1]);
        }
        else if(words[0] == "TITLE")
        {
            if(m_tracks.isEmpty())
                album = words[1];
            else
                m_tracks.last()->info.setValue(Qmmp::TITLE, words[1]);
        }
        else if(words[0] == "TRACK")
        {
            TrackInfo info;
            info.setValue(Qmmp::TRACK, words[1].toInt());
            info.setValue(Qmmp::ALBUM, album);
            info.setValue(Qmmp::GENRE, genre);
            info.setValue(Qmmp::YEAR, date);
            info.setValue(Qmmp::COMMENT, comment);
            info.setValue(Qmmp::ARTIST, artist);
            info.setValue(Qmmp::ALBUMARTIST, artist);
            info.setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, album_gain);
            info.setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, album_peak);

            m_tracks << new CUETrack;
            m_tracks.last()->info = info;
            m_tracks.last()->offset = 0;
        }
        else if(words[0] == "INDEX" && words[1] == "01")
        {
            if(m_tracks.isEmpty())
                continue;
            m_tracks.last()->offset = getLength(words[2]);
            m_tracks.last()->file = file;
        }
        else if(words[0] == "REM")
        {
            if(words.count() < 3)
                continue;
            if(words[1] == "GENRE")
            {
                if(m_tracks.isEmpty())
                    genre = words[2];
                else
                    m_tracks.last()->info.setValue(Qmmp::GENRE, words[2]);
            }
            else if(words[1] == "DATE")
            {
                 if(m_tracks.isEmpty())
                     date = words[2];
                 else
                     m_tracks.last()->info.setValue(Qmmp::YEAR, words[2]);
            }
            else if(words[1] == "COMMENT")
            {
                 if(m_tracks.isEmpty())
                     comment = words[2];
                 else
                     m_tracks.last()->info.setValue(Qmmp::COMMENT, words[2]);
            }
            else if(words[1] == "REPLAYGAIN_ALBUM_GAIN")
                album_gain = words[2].toDouble();
            else if(words[1] == "REPLAYGAIN_ALBUM_PEAK")
                album_peak = words[2].toDouble();
            else if(words[1] == "REPLAYGAIN_TRACK_GAIN" && !m_tracks.isEmpty())
                m_tracks.last()->info.setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, words[2].toDouble());
            else if(words[1] == "REPLAYGAIN_TRACK_PEAK" && !m_tracks.isEmpty())
                m_tracks.last()->info.setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, words[2].toDouble());
        }
    }

    if(m_tracks.isEmpty())
        qWarning("CueParser: invalid cue data");
}

QList<TrackInfo*> CueParser::createPlayList(int track) const
{
    QList<TrackInfo*> playlist;
    if(track <= 0)
    {
        for(const CUETrack *track : m_tracks)
        {
            playlist << new TrackInfo(track->info);
        }
    }
    else if(track > m_tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return playlist;
    }
    else
    {
        playlist << new TrackInfo(m_tracks.at(track - 1)->info);
    }
    return playlist;
}

const QStringList &CueParser::files() const
{
    return m_files;
}

qint64 CueParser::offset(int track) const
{
    if(track < 1 || track > m_tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return 0;
    }
    return m_tracks.at(track - 1)->offset;
}

qint64 CueParser::duration(int track) const
{
    if(track < 1 || track > m_tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return 0;
    }
    return m_tracks.at(track - 1)->info.duration();
}

QString CueParser::file(int track) const
{
    if(track < 1 || track > m_tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return QString();
    }
    return m_tracks.at(track - 1)->file;
}

QString CueParser::url(int track) const
{
    if(track < 1 || track > m_tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return QString();
    }
    return m_tracks.at(track - 1)->info.path();
}

int CueParser::count() const
{
    return m_tracks.count();
}

bool CueParser::isEmpty() const
{
    return m_tracks.isEmpty();
}

const TrackInfo *CueParser::info(int track) const
{
    if(track < 1 || track > m_tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return nullptr;
    }
    return &m_tracks.at(track - 1)->info;
}

void CueParser::setDuration(const QString &file, qint64 duration)
{
    for(int i = 0; i < m_tracks.count(); ++i)
    {
        CUETrack *track = m_tracks.at(i);
        if(track->file == file)
        {
            if((i == m_tracks.count() - 1) || (m_tracks.at(i + 1)->file != track->file))
                track->info.setDuration(duration - track->offset);
            else
                track->info.setDuration(m_tracks.at(i + 1)->offset - track->offset);

            if(track->info.duration() < 0)
                track->info.setDuration(0);
        }
    }
}

void CueParser::setDuration(qint64 duration)
{
    for(int i = 0; i < m_tracks.count(); ++i)
    {
        CUETrack *track = m_tracks.at(i);

        if(i == m_tracks.count() - 1)
            track->info.setDuration(duration - track->offset);
        else
            track->info.setDuration(m_tracks.at(i + 1)->offset - track->offset);

        if(track->info.duration() < 0)
            track->info.setDuration(0);
    }
}

void CueParser::setProperties(const QString &file, const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(CUETrack *track : qAsConst(m_tracks))
    {
        if(track->file == file)
            track->info.setValues(properties);
    }
}

void CueParser::setProperties(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(CUETrack *track : qAsConst(m_tracks))
        track->info.setValues(properties);
}

void CueParser::setMetaData(int track, Qmmp::MetaData key, const QVariant &value)
{
    if(track < 1 || track > m_tracks.count())
        qWarning("CueParser: invalid track number: %d", track);

    m_tracks.at(track - 1)->info.setValue(key, value);
}

void CueParser::setUrl(const QString &scheme, const QString &path)
{
    for(int i = 0; i < m_tracks.count(); ++i)
        m_tracks.at(i)->info.setPath(QString("%1://%2#%3").arg(scheme, path, m_tracks.at(i)->info.value(Qmmp::TRACK)));
}

void CueParser::clear()
{
    qDeleteAll(m_tracks);
    m_tracks.clear();
    m_files.clear();
}

QStringList CueParser::splitLine(const QString &line)
{
    //qDebug("raw string = %s",qPrintable(line));
    QStringList list;
    QString buf = line.trimmed();
    if(buf.isEmpty())
        return list;
    while(!buf.isEmpty())
    {
        //qDebug(qPrintable(buf));
        if(buf.startsWith('"'))
        {
            int end = buf.indexOf('"', 1);
            if(end == -1) //ignore invalid line
            {
                list.clear();
                qWarning("CueParser: unable to parse line: %s",qPrintable(line));
                return list;
            }
            list << buf.mid (1, end - 1);
            buf.remove (0, end + 1);
        }
        else
        {
            int end = buf.indexOf(' ', 0);
            if(end < 0)
                end = buf.length();
            list << buf.mid (0, end);
            buf.remove (0, end);
        }
        buf = buf.trimmed();
    }
    return list;
}

qint64 CueParser::getLength(const QString &str)
{
    const QStringList &list = str.split(":");
    if(list.count() == 2)
        return (qint64)list.at(0).toInt() * 60000 + list.at(1).toInt() * 1000;
    else if(list.count() == 3)
        return (qint64)list.at(0).toInt() * 60000 + list.at(1).toInt() * 1000 + list.at(2).toInt() * 1000 / 75;
    return 0;
}
