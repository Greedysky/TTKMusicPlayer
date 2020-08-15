extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <stdint.h>
#include "ffmpegmetadatamodel.h"

FFmpegMetaDataModel::FFmpegMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AVFormatContext *in = nullptr;
    QString filePath = path;

    if(path.startsWith("ffmpeg://"))
    {
        filePath.remove("ffmpeg://");
        filePath.remove(QRegExp("#\\d+$"));
    }

#ifdef Q_OS_WIN
    if(avformat_open_input(&in, filePath.toUtf8().constData(), nullptr, nullptr) < 0)
#else
    if(avformat_open_input(&in, filePath.toLocal8Bit().constData(), nullptr, nullptr) < 0)
#endif
        return;

    if(in)
    {
        avformat_find_stream_info(in, nullptr);
        av_read_play(in);

        AVCodecParameters *c = nullptr;

        for(uint idx = 0; idx < in->nb_streams; idx++)
        {
            c = in->streams[idx]->codecpar;

            if(c->codec_type == AVMEDIA_TYPE_VIDEO && c->codec_id == AV_CODEC_ID_MJPEG)
                break;
        }
        if(c)
        {
            AVPacket pkt;
            av_read_frame(in, &pkt);
            m_pixmap.loadFromData(QByteArray((const char*)pkt.data, pkt.size));
        }

        avformat_close_input(&in);
    }
}

FFmpegMetaDataModel::~FFmpegMetaDataModel()
{

}

QPixmap FFmpegMetaDataModel::cover() const
{
    return m_pixmap;
}
