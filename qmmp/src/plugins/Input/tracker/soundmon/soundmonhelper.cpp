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
    delete m_input;
}

bool SoundMonHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("SoundMonHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = new Player((BYTE*)buffer.constData(), buffer.length());
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
