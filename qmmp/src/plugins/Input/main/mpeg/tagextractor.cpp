#include "tagextractor.h"

#include <QBuffer>
#include <QSettings>
#include <QTextCodec>

TagExtractor::TagExtractor(QIODevice *input)
    : m_input(input)
{

}

const QMap<Qmmp::MetaData, QString> TagExtractor::id3v2tag() const
{
    QByteArray array = m_input->peek(2048);
    int offset = array.indexOf("ID3");
    if(offset < 0)
        return QMap<Qmmp::MetaData, QString>();

    ID3v2Tag tag(&array, offset);
    if(tag.isEmpty())
        return QMap<Qmmp::MetaData, QString>();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");
    QByteArray codecName = settings.value("ID3v2_encoding", "UTF-8").toByteArray();

    QTextCodec *codec = nullptr;
    if(codecName.contains("UTF"))
        codec = QTextCodec::codecForName("UTF-8");
    else if(!codecName.isEmpty())
        codec = QTextCodec::codecForName(codecName);

    if(!codec)
        codec = QTextCodec::codecForName("UTF-8");

    settings.endGroup();

    bool utf = codec->name().contains("UTF");
    QMap<Qmmp::MetaData, QString> tags;
    tags.insert(Qmmp::ARTIST, CSTR_TO_QSTR(codec, tag.artist(), utf));
    tags.insert(Qmmp::ALBUM, CSTR_TO_QSTR(codec, tag.album(), utf));
    tags.insert(Qmmp::COMMENT, CSTR_TO_QSTR(codec, tag.comment(), utf));
    tags.insert(Qmmp::GENRE, CSTR_TO_QSTR(codec, tag.genre(), utf));
    tags.insert(Qmmp::TITLE, CSTR_TO_QSTR(codec, tag.title(), utf));
    tags.insert(Qmmp::YEAR, QString::number(tag.year()));
    tags.insert(Qmmp::TRACK, QString::number(tag.track()));

    if(!tag.frameListMap()["TCOM"].isEmpty())
    {
        TagLib::String composer = tag.frameListMap()["TCOM"].front()->toString();
        tags.insert(Qmmp::COMPOSER, CSTR_TO_QSTR(codec, composer, utf));
    }
    if(!tag.frameListMap()["TPOS"].isEmpty())
    {
        TagLib::String disc = tag.frameListMap()["TPOS"].front()->toString();
        tags.insert(Qmmp::DISCNUMBER, QString(disc.toCString()).trimmed());
    }
    return tags;
}

ID3v2Tag::ID3v2Tag(QByteArray *array, long offset)
    : TagLib::ID3v2::Tag(),
      m_offset(offset)
{
    m_buf = new QBuffer(array);
    m_buf->open(QIODevice::ReadOnly);
    read();
}

ID3v2Tag::~ID3v2Tag()
{
    delete m_buf;
}

void ID3v2Tag::read()
{
    m_buf->seek(m_offset);
    uint to_read = TagLib::ID3v2::Header::size();
    if(to_read > 2048 - uint(m_offset))
        return;
    header()->setData(TagLib::ByteVector(m_buf->read(to_read).data(), to_read));
    to_read = header()->tagSize();
    if(!to_read ||  2048 < m_offset + TagLib::ID3v2::Header::size())
        return;
    QByteArray array = m_buf->read(to_read);
    TagLib::ByteVector v(array.data(), array.length());
    parse(v);
}
