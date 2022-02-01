#include "bphelper.h"

BpHelper::BpHelper(const QString &path)
    : m_path(path)
{

}

BpHelper::~BpHelper()
{
    deinit();
}

void BpHelper::deinit()
{
    if(m_input)
    {
        delete m_input;
    }
}

bool BpHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("BpHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    m_input = new Player((BYTE*)module.constData(), size);
    if(!m_input->Load())
    {
        qWarning("BpHelper: Load error");
        return false;
    }

    m_bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

qint64 BpHelper::read(unsigned char *data, qint64)
{
    return m_input->Run() ? m_input->Render(data) : 0;
}
