#include "xsfhelper.h"

XSFHelper::XSFHelper(const QString &path)
    : m_path(path)
{

}

XSFHelper::~XSFHelper()
{
    deinit();
}

void XSFHelper::metaMode(bool meta)
{
    m_meta = meta;
}

void XSFHelper::deinit()
{
    delete m_input;
}

bool XSFHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("XSFHelper: open file failed");
        file.close();
        return false;
    }

    const qint64 size = file.size();
    file.close();

    m_input = XSFReader::makeReader(m_path);
    if(!m_input)
    {
        qWarning("XSFHelper: load file suffix error");
        return false;
    }

    if(!m_input->load(QmmpPrintable(m_path), m_meta))
    {
       qWarning("XSFHelper: load error");
       return false;
    }

    m_bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

QMap<Qmmp::MetaData, QString> XSFHelper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;
    if(!m_input)
    {
        return metaData;
    }

    const file_meta meta(m_input->get_meta_map());
    for(auto itr = meta.begin(); itr != meta.end(); ++itr)
    {
        if(itr->first == "title")
            metaData.insert(Qmmp::TITLE, QString::fromStdString(itr->second));
        else if(itr->first == "artist")
            metaData.insert(Qmmp::ARTIST, QString::fromStdString(itr->second));
        else if(itr->first == "album")
            metaData.insert(Qmmp::ALBUM, QString::fromStdString(itr->second));
        else if(itr->first == "year")
            metaData.insert(Qmmp::YEAR, QString::fromStdString(itr->second));
        else if(itr->first == "genre")
            metaData.insert(Qmmp::GENRE, QString::fromStdString(itr->second));
        else if(itr->first == "copyright")
            metaData.insert(Qmmp::COMMENT, QString::fromStdString(itr->second));
    }
    return metaData;
}
