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
    if(!file.open(QIODevice::ReadOnly))
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

    const QByteArray &buffer = file.readAll();
    file.close();

    if(!m_input->OpenFromBuffer((unsigned char *)buffer.constData(), buffer.length(), &m_info))
    {
        qWarning("S98Helper: unable to open file");
        return false;
    }

    char raw[1024];
    m_input->getRawFileInfo((unsigned char*)raw, 255, 0);
    if(strlen(raw) != 0)
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
        const int valid = !strncmp(raw, pfx, strlen(pfx));
        if(valid || m_info.dwIsV3)
        {
            const QString text(raw + (valid ? strlen(pfx) : 0));
            for(const QString &v : text.split(char(0xa), QtSkipEmptyParts))
            {
                const QStringList &parts = v.split('=', QtSkipEmptyParts);
                if(parts.count() < 2)
                {
                    continue;
                }

                m_metaData.insert(parts.first().toLower(), parts.last());
            }
        }
        else
        {
            // some older format
            const QString text(raw);
            const int index = text.indexOf("Copyright");	// some contain this..
            if(index != -1)
            {
                m_metaData.insert("title", text.left(index));
                m_metaData.insert("comment", text.mid(index));
            }
        }
    }
    return true;
}

qint64 S98Helper::read(unsigned char *data, qint64 maxSize)
{
    if(m_input->GetPosition() > totalTime())
    {
        return 0;
    }

    const int size = channels() * depth() / 8;
    return m_input->Write((short *)data, maxSize / size) * size;
}
