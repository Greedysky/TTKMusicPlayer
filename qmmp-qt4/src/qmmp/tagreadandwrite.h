#ifndef TAGREADANDWRITE_H
#define TAGREADANDWRITE_H

#include <QMap>
#include <QString>

class TagReadAndWrite
{
public:
    enum MusicTag
    {
        TAG_UNKNOWN = -1,
        TAG_TITLE = 0,   /*!< Title */
        TAG_ARTIST,      /*!< Artist  */
        TAG_ALBUMARTIST, /*!< Album artist  */
        TAG_ALBUM,       /*!< Album */
        TAG_COMMENT,     /*!< Comment */
        TAG_GENRE,       /*!< Genre */
        TAG_COMPOSER,    /*!< Composer */
        TAG_YEAR,        /*!< Year */
        TAG_TRACK,       /*!< Track number */
        TAG_DISCNUMBER,  /*!< Disc number */
        TAG_URL,         /*!< Stream url or local file path */
        TAG_LENGTH,      /*!< Length */
        TAG_FORMAT,      /*!< Format */
        TAG_SAMPLERATE,  /*!< Sample rate */
        TAG_MODE,        /*!< Mode */
        TAG_BITRATE,     /*!< Bitrate */
        TAG_CHANNEL,     /*!< Channel */
    };

    TagReadAndWrite(const QString &file);
    ~TagReadAndWrite();

    bool readFile();
    bool writeMusicTag(MusicTag tag, const QString &value);

    QMap<MusicTag, QString> getMusicTags() {return m_parameters;}

protected:
    QString m_path;
    QMap<MusicTag, QString> m_parameters;

};

#endif // TAGREADANDWRITE_H
