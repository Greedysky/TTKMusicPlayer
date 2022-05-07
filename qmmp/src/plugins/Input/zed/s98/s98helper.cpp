#include "s98helper.h"

#include <QStringList>

S98Helper::S98Helper(const QString &path)
    : m_path(path)
{
    memset(&m_info, 0, sizeof(SOUNDINFO));
    m_info.dwSamplesPerSec = sampleRate();
}

S98Helper::~S98Helper()
{
    deinit();
}

void S98Helper::deinit()
{
    delete m_input;
}

bool S98Helper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("S98Helper: open file failed");
        return false;
    }

    m_input = new s98File;
    if(!m_input)
    {
        qWarning("S98Helper: create error");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray &module = file.readAll();
    file.close();

    if(!m_input->OpenFromBuffer((unsigned char *)module.constData(), size, &m_info))
    {
        qWarning("S98Helper: OpenFromBuffer error");
        return false;
    }

    const int length = totalTime();
    m_bitrate = length != 0 ? (size * 8.0 / length + 1.0f) : 0;
    return true;
}

qint64 S98Helper::read(unsigned char *data, qint64 maxSize)
{
    if(m_input->GetPosition() > totalTime())
    {
        return 0;
    }

    return m_input->Write((short *)data, maxSize / 4) * 4;
}

QMap<Qmmp::MetaData, QString> S98Helper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;

    char buffer[1024];
    m_input->getRawFileInfo((unsigned char*)buffer, 255, 0);
    if(strlen(buffer) == 0)
    {
        return metaData;
    }

    /*
    note: V3 files contain tagged info, e.g.
    [S98]
    "title=Opening" 0x0a
    "artist=Yuzo Koshiro" 0x0a
    "game=Sorcerian" 0x0a
    "year=1987" 0x0a
    "genre=game" 0x0a
    "comment=This is sample data." 0x0a
    "copyright=Nihon Falcom" 0x0a
    "s98by=foo" 0x0a
    "system=PC-8801" 0x0a
    */
    const char *pfx = "[S98]";
    const int valid = !strncmp(buffer, pfx, strlen(pfx));
    if(valid || m_info.dwIsV3)
    {
        const QString text(buffer + (valid ? strlen(pfx) : 0));
        for(const QString &v : text.split(0xa, QString::SkipEmptyParts))
        {
            const QStringList &parts = v.split('=', QString::SkipEmptyParts);
            if(parts.count() < 2)
            {
                continue;
            }

            const QString &key = parts[0].toUpper();
            const QString &value = parts[1];

            if(!key.compare("TITLE"))
            {
                metaData.insert(Qmmp::TITLE, value);
            }
            else if(!key.compare("ARTIST"))
            {
                metaData.insert(Qmmp::ARTIST, value);
            }
            else if(!key.compare("YEAR"))
            {
                metaData.insert(Qmmp::YEAR, value);
            }
            else if(!key.compare("GENRE"))
            {
                metaData.insert(Qmmp::GENRE, value);
            }
            else if(!key.compare("COMMENT"))
            {
                metaData.insert(Qmmp::COMMENT, value);
            }
        }
    }
    else
    {
        // some older format
        const QString text(buffer);
        const int index = text.indexOf("Copyright");	// some contain this..
        if(index != -1)
        {
            metaData.insert(Qmmp::TITLE, text.left(index));
            metaData.insert(Qmmp::COMMENT, text.mid(index));
        }
    }
    return metaData;
}
