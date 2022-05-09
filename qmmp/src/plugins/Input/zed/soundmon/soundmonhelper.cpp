#include "soundmonhelper.h"

SoundMonHelper::SoundMonHelper(const QString &path)
    : m_path(path)
{

}

SoundMonHelper::~SoundMonHelper()
{
    deinit();
}

void SoundMonHelper::deinit()
{
    if(m_input)
    {
        delete m_input;
    }
}

bool SoundMonHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("SoundMonHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray &module = file.readAll();
    file.close();

    m_input = new Player((BYTE*)module.constData(), size);
    if(!m_input->Load())
    {
        qWarning("SoundMonHelper: Load error");
        return false;
    }
    return true;
}

qint64 SoundMonHelper::read(unsigned char *data, qint64)
{
    return m_input->Run() ? m_input->Render(data) : 0;
}
