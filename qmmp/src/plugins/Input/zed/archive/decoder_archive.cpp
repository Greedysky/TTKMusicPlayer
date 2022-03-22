#include "decoder_archive.h"

DecoderArchive::DecoderArchive(const QString &path)
    : Decoder()
{
    Q_UNUSED(path);
}

DecoderArchive::~DecoderArchive()
{

}

bool DecoderArchive::initialize()
{
    qDebug("DecoderArchive: initialize failed");
    return false;
}

qint64 DecoderArchive::totalTime() const
{
    return 0;
}

int DecoderArchive::bitrate() const
{
    return 0;
}

qint64 DecoderArchive::read(unsigned char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return 0;
}

void DecoderArchive::seek(qint64 time)
{
    Q_UNUSED(time);
}
