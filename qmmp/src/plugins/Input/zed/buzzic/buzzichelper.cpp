#include "buzzichelper.h"

BuzzicHelper::BuzzicHelper(const QString &path)
    : m_path(path)
{

}

BuzzicHelper::~BuzzicHelper()
{
    deinit();
}

void BuzzicHelper::deinit()
{
    if(m_input)
    {
        Buzzic2Release(m_input);
    }
}

bool BuzzicHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("BuzzicHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = Buzzic2Load((unsigned char *)buffer.constData(), buffer.length());
    if(!m_input)
    {
        qWarning("BuzzicHelper: failed to Buzzic2Load");
        return false;
    }
    return true;
}

qint64 BuzzicHelper::read(unsigned char *data, qint64 maxSize)
{
    const int offset = sizeof(float) * channels();
    return Buzzic2Render(m_input, (StereoSample*)data, maxSize / offset) * offset;
}

QString BuzzicHelper::instruments() const
{
    QString name;

    for(uint32_t i = 0; i < instrumentCount(); ++i)
    {
        const char *v = Buzzic2IntrumentName(m_input, i);
        if(v)
        {
            name += v;
            name += "\n";
        }
    }
    return name;
}

uint32_t BuzzicHelper::instrumentCount() const
{
    return Buzzic2NumIntruments(m_input);
}
