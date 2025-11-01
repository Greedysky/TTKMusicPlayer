#include "gmehelper.h"
#include "decoder_gme.h"

DecoderGME::DecoderGME(const QString &path)
    : Decoder(),
      m_path(path)
{

}

bool DecoderGME::initialize()
{
    m_emu = m_helper.load(m_path);
    if(!m_emu)
        return false;

    const int count = gme_track_count(m_emu);
    int track = m_path.section("#", -1).toInt();
    if(track > count || track < 0)
    {
        qWarning("DecoderGME: track number is out of range");
        gme_delete(m_emu);
        m_emu = nullptr;
        return false;
    }

    if(track == 0)
        track = count;

    gme_start_track(m_emu, track - 1);
    gme_info_t *track_info;
    if(!gme_track_info(m_emu, &track_info, track - 1))
    {
        if(track_info->length <= 0)
            track_info->length = track_info->intro_length + track_info->loop_length * 2;
    }

    if(!track_info)
        return false;

    if(track_info->length <= 0)
        track_info->length = (long) (2.5 * 60 * 1000); //track_info->play_length;

    if(m_helper.fadeLength())
    {
        if(track_info->length < m_helper.fadeLength())
            track_info->length += m_helper.fadeLength();
        gme_set_fade(m_emu, track_info->length - m_helper.fadeLength());
    }

    QMap<Qmmp::MetaData, QString> metadata;
    metadata.insert(Qmmp::ALBUM, track_info->game);
    metadata.insert(Qmmp::TITLE, track_info->song);
    metadata.insert(Qmmp::ARTIST, track_info->author);
    metadata.insert(Qmmp::COMMENT, track_info->comment);
    metadata.insert(Qmmp::TRACK, QString("%1").arg(track));

    addMetaData(metadata);
    m_totalTime = track_info->length;

    gme_free_info(track_info);
    configure(44100, 2, Qmmp::PCM_S16LE);
    qDebug("DecoderGME: initialize success");
    return true;
}

qint64 DecoderGME::totalTime() const
{
    return m_totalTime;
}

void DecoderGME::seek(qint64 time)
{
    gme_seek(m_emu, time);
}

int DecoderGME::bitrate() const
{
    return 8;
}

qint64 DecoderGME::read(unsigned char *data, qint64 maxSize)
{
    if(gme_track_ended(m_emu))
        return 0;

    if(m_totalTime && gme_tell(m_emu) > m_totalTime)
        return 0;

    if(gme_play(m_emu, maxSize / 2, (short*)data))
    {
        return 0;
    }
    return maxSize;
}
