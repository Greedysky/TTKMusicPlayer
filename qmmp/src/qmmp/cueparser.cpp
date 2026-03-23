#include <QTextStream>
#include "qmmptextcodec.h"
#include "cueparser.h"

class CueParserPrivate
{
public:
    QStringList splitLine(const QString &line) const
    {
        QStringList list;
        QString buf = line.trimmed();
        if(buf.isEmpty())
            return list;

        while(!buf.isEmpty())
        {
            if(buf.startsWith('"'))
            {
                int end = buf.indexOf('"', 1);
                if(end == -1) //ignore invalid line
                {
                    list.clear();
                    qWarning("CueParser: unable to parse line: %s",qPrintable(line));
                    return list;
                }
                list << buf.mid(1, end - 1);
                buf.remove(0, end + 1);
            }
            else
            {
                int end = buf.indexOf(' ', 0);
                if(end < 0)
                    end = buf.length();
                list << buf.mid(0, end);
                buf.remove(0, end);
            }
            buf = buf.trimmed();
        }
        return list;
    }

    qint64 getLength(const QString &str) const
    {
        const QStringList &list = str.split(":");
        if(list.count() == 2)
            return (qint64)list.at(0).toInt() * 60000 + list.at(1).toInt() * 1000;
        else if(list.count() == 3)
            return (qint64)list.at(0).toInt() * 60000 + list.at(1).toInt() * 1000 + list.at(2).toInt() * 1000 / 75;
        return 0;
    }

    struct CUETrack
    {
        TrackInfo info;
        QString file;
        qint64 offset = 0;
    };

    QList<CUETrack *> tracks;
    QStringList files;

};


CueParser::CueParser()
    : d(new CueParserPrivate)
{

}

CueParser::CueParser(const QByteArray &data, const QByteArray &codecName)
{
    loadData(data, codecName);
}

CueParser::~CueParser()
{
    clear();
    delete d;
}

void CueParser::loadData(const QByteArray &data, const QByteArray &codecName)
{
    QmmpTextCodec codec(codecName);
    loadData(data, &codec);
}

void CueParser::loadData(const QByteArray &data, QmmpTextCodec *codec)
{
    clear();

    if(data.isEmpty())
    {
        return;
    }

    QString artist, album, genre, date, comment, file;
    double album_peak = 0.0, album_gain = 0.0;

    QString str = codec->toUnicode(data);
    QTextStream textStream(&str, QIODevice::ReadOnly);

    while(!textStream.atEnd())
    {
        const QString &line = textStream.readLine().trimmed();
        const QStringList &words = d->splitLine(line);
        if(words.count() < 2)
            continue;

        if(words[0] == "FILE")
        {
            file = words[1];
            d->files << file;
        }
        else if(words[0] == "PERFORMER")
        {
            if(d->tracks.isEmpty())
                artist = words[1];
            else
                d->tracks.last()->info.setValue(Qmmp::ARTIST, words[1]);
        }
        else if(words[0] == "TITLE")
        {
            if(d->tracks.isEmpty())
                album = words[1];
            else
                d->tracks.last()->info.setValue(Qmmp::TITLE, words[1]);
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

            d->tracks << new CueParserPrivate::CUETrack;
            d->tracks.last()->info = info;
            d->tracks.last()->offset = 0;
        }
        else if(words[0] == "INDEX" && words[1] == "01")
        {
            if(d->tracks.isEmpty())
                continue;

            d->tracks.last()->offset = d->getLength(words[2]);
            d->tracks.last()->file = file;
        }
        else if(words[0] == "REM")
        {
            if(words.count() < 3)
                continue;
            if(words[1] == "GENRE")
            {
                if(d->tracks.isEmpty())
                    genre = words[2];
                else
                    d->tracks.last()->info.setValue(Qmmp::GENRE, words[2]);
            }
            else if(words[1] == "DATE")
            {
                 if(d->tracks.isEmpty())
                     date = words[2];
                 else
                     d->tracks.last()->info.setValue(Qmmp::YEAR, words[2]);
            }
            else if(words[1] == "COMMENT")
            {
                 if(d->tracks.isEmpty())
                     comment = words[2];
                 else
                     d->tracks.last()->info.setValue(Qmmp::COMMENT, words[2]);
            }
            else if(words[1] == "REPLAYGAIN_ALBUM_GAIN")
                album_gain = words[2].toDouble();
            else if(words[1] == "REPLAYGAIN_ALBUM_PEAK")
                album_peak = words[2].toDouble();
            else if(words[1] == "REPLAYGAIN_TRACK_GAIN" && !d->tracks.isEmpty())
                d->tracks.last()->info.setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, words[2].toDouble());
            else if(words[1] == "REPLAYGAIN_TRACK_PEAK" && !d->tracks.isEmpty())
                d->tracks.last()->info.setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, words[2].toDouble());
        }
    }

    if(d->tracks.isEmpty())
        qWarning("CueParser: invalid cue data");
}

TrackInfoList CueParser::createPlayList(int track) const
{
    TrackInfoList playlist;
    if(track <= 0)
    {
        for(const CueParserPrivate::CUETrack *track : d->tracks)
        {
            playlist << TrackInfo(track->info);
        }
    }
    else if(track > d->tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return playlist;
    }
    else
    {
        playlist << TrackInfo(d->tracks.at(track - 1)->info);
    }
    return playlist;
}

const QStringList &CueParser::files() const
{
    return d->files;
}

qint64 CueParser::offset(int track) const
{
    if(track < 1 || track > d->tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return 0;
    }
    return d->tracks.at(track - 1)->offset;
}

qint64 CueParser::duration(int track) const
{
    if(track < 1 || track > d->tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return 0;
    }
    return d->tracks.at(track - 1)->info.duration();
}

QString CueParser::file(int track) const
{
    if(track < 1 || track > d->tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return QString();
    }
    return d->tracks.at(track - 1)->file;
}

QString CueParser::url(int track) const
{
    if(track < 1 || track > d->tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return QString();
    }
    return d->tracks.at(track - 1)->info.path();
}

int CueParser::count() const
{
    return d->tracks.count();
}

bool CueParser::isEmpty() const
{
    return d->tracks.isEmpty();
}

TrackInfo CueParser::info(int track) const
{
    if(track < 1 || track > d->tracks.count())
    {
        qWarning("CueParser: invalid track number: %d", track);
        return TrackInfo();
    }
    return d->tracks.at(track - 1)->info;
}

void CueParser::setDuration(const QString &file, qint64 duration)
{
    for(int i = 0; i < d->tracks.count(); ++i)
    {
        CueParserPrivate::CUETrack *track = d->tracks.at(i);
        if(track->file == file)
        {
            if((i == d->tracks.count() - 1) || (d->tracks.at(i + 1)->file != track->file))
                track->info.setDuration(duration - track->offset);
            else
                track->info.setDuration(d->tracks.at(i + 1)->offset - track->offset);

            if(track->info.duration() < 0)
                track->info.setDuration(0);
        }
    }
}

void CueParser::setDuration(qint64 duration)
{
    for(int i = 0; i < d->tracks.count(); ++i)
    {
        CueParserPrivate::CUETrack *track = d->tracks.at(i);

        if(i == d->tracks.count() - 1)
            track->info.setDuration(duration - track->offset);
        else
            track->info.setDuration(d->tracks.at(i + 1)->offset - track->offset);

        if(track->info.duration() < 0)
            track->info.setDuration(0);
    }
}

void CueParser::setProperties(const QString &file, const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(CueParserPrivate::CUETrack *track : qAsConst(d->tracks))
    {
        if(track->file == file)
            track->info.setValues(properties);
    }
}

void CueParser::setProperties(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(CueParserPrivate::CUETrack *track : qAsConst(d->tracks))
        track->info.setValues(properties);
}

void CueParser::setMetaData(int track, Qmmp::MetaData key, const QVariant &value)
{
    if(track < 1 || track > d->tracks.count())
        qWarning("CueParser: invalid track number: %d", track);

    d->tracks.at(track - 1)->info.setValue(key, value);
}

void CueParser::setUrl(const QString &scheme, const QString &path)
{
    for(int i = 0; i < d->tracks.count(); ++i)
        d->tracks.at(i)->info.setPath(QString("%1://%2#%3").arg(scheme, path, d->tracks.at(i)->info.value(Qmmp::TRACK)));
}

void CueParser::clear()
{
    qDeleteAll(d->tracks);
    d->tracks.clear();
    d->files.clear();
}
