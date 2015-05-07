#ifndef QMUSICSONG_H
#define QMUSICSONG_H

#include <QStringList>
#include "../core/musiclibexportglobal.h"

class MUSIC_EXPORT QMusicSong
{
public:
    QMusicSong():m_musicName(""),m_musicType(""),m_musicSize(0){}
    explicit QMusicSong(const QString& musicName,
                        const QString& musicType,
                        const qint64 musicSize):
        m_musicName(musicName),m_musicType(musicType),
        m_musicSize(musicSize){}

    inline qreal transSizeByte2KByte() const
    {
        return m_musicSize/1000.0;
    }
    inline qreal transSizeByte2MByte() const
    {
        return m_musicSize/1000000.0;
    }
    inline QString getMusicArtistFront() const
    {
        return m_musicName.split('-').front().trimmed();
    }
    inline QString getMusicArtistBack() const
    {
        return m_musicName.split('-').back().trimmed();
    }
    inline QString getMusicAbName() const
    {
        return m_musicName.split('-').back().trimmed();
    }

    QString m_musicName;
    QString m_musicType;
    qint64  m_musicSize;

};

#endif // QMUSICSONG_H
