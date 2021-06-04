#include "s98helper.h"

#include <QStringList>

#define TEXT_MAX            255
#define SAMPLE_BUF_SIZE     1024

S98Helper::S98Helper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

S98Helper::~S98Helper()
{
    deinit();
}

void S98Helper::deinit()
{
    if(m_info->input)
    {
        m_info->input->Close();
        delete m_info->input;
    }
    free(m_info);
}

bool S98Helper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("S98Helper: open file failed");
        file.close();
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();
    file.close();

    m_info->input = new s98File();
    m_info->sound.dwSamplesPerSec = sampleRate();
    if(!m_info->input->OpenFromBuffer((unsigned char *)module.constData(), size, &m_info->sound))
    {
        qWarning("S98Helper: OpenFromBuffer error");
        return false;
    }

    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

int S98Helper::totalTime() const
{
    return m_info->sound.dwLength;
}

void S98Helper::seek(qint64 time)
{
    m_info->input->SetPosition(time);
}

int S98Helper::bitrate() const
{
    return m_info->bitrate;
}

int S98Helper::sampleRate() const
{
    return 44100;
}

int S98Helper::channels() const
{
    return 2;
}

int S98Helper::bitsPerSample() const
{
    return 2;
}

int S98Helper::read(unsigned char *buf, int)
{
    if(m_info->input->GetPosition() >= totalTime())
    {
        return 0;	// stop song
    }

    m_info->input->Write((short*)buf, SAMPLE_BUF_SIZE / 4);
    return SAMPLE_BUF_SIZE;
}

QMap<Qmmp::MetaData, QString> S98Helper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;
    if(!m_info->input)
    {
        return metaData;
    }

    char buffer[TEXT_MAX];
    m_info->input->getRawFileInfo((unsigned char*)buffer, TEXT_MAX, 0);
    if(strlen(buffer) != 0)
    {
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
        const int hasPrefix = !strncmp(buffer, pfx, strlen(pfx));
        if(hasPrefix || m_info->sound.dwIsV3)
        {
            QString meta(buffer + (hasPrefix ? strlen(pfx) : 0));
            for(const QString &item : meta.split("\n", QString::SkipEmptyParts))
            {
                if(!item.contains("="))
                {
                    continue;
                }

                const QStringList metaPair(item.split("=", QString::SkipEmptyParts));
                if(metaPair.count() < 2)
                {
                    continue;
                }

                const QString &metaName = metaPair[0].toUpper();
                if(!metaName.compare("TITLE"))
                {
                    metaData.insert(Qmmp::TITLE, metaPair[1]);
                }
                else if(!metaName.compare("ARTIST"))
                {
                    metaData.insert(Qmmp::ARTIST, metaPair[1]);
                }
                else if(!metaName.compare("GAME"))
                {
                    metaData.insert(Qmmp::ALBUM, metaPair[1]);
                }
                else if(!metaName.compare("YEAR"))
                {
                    metaData.insert(Qmmp::YEAR, metaPair[1]);
                }
                else if(!metaName.compare("GENRE"))
                {
                    metaData.insert(Qmmp::GENRE, metaPair[1]);
                }
                else if(!metaName.compare("COMMENT"))
                {
                    metaData.insert(Qmmp::COMMENT, metaPair[1]);
                }
                else if(!metaName.compare("COPYRIGHT"))
                {
                    metaData.insert(Qmmp::ALBUMARTIST, metaPair[1]);
                }
                else if(!metaName.compare("S98BY"))
                {
                    metaData.insert(Qmmp::COMPOSER, metaPair[1]);
                }
                else if(!metaName.compare("SYSTEM"))
                {
                    metaData.insert(Qmmp::DISCNUMBER, metaPair[1]);
                }
            }
        }
        else
        {
            // some older format
            QString meta(buffer);
            if(meta.contains("Copyright"))
            {
                metaData.insert(Qmmp::ALBUMARTIST, meta);
            }
        }
    }
    return metaData;
}
