#include <QObject>
#include <QSettings>
#include <QDir>
#include <QSettings>
#include "decoder_xmp.h"

DecoderXmp *DecoderXmp::m_instance = nullptr;

DecoderXmp::DecoderXmp(const QString &path)
    : Decoder(nullptr),
      m_path(path)
{
    m_instance = this;
}

DecoderXmp::~DecoderXmp()
{
    if(m_instance == this)
        m_instance = nullptr;
    deinit();
}

DecoderXmp *DecoderXmp::instance()
{
    return m_instance;
}

bool DecoderXmp::initialize()
{
    m_ctx = xmp_create_context();

    int err = xmp_load_module(m_ctx, m_path.toLocal8Bit().data());
    if(err != 0)
    {
        qWarning("DecoderXmp: unable to load module file, error = %d", err);
        xmp_free_context(m_ctx);
        m_ctx = nullptr;
        return false;
    }

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    m_totalTime = mi.seq_data[0].duration;

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_srate = settings.value("Xmp/sample_rate", 44100).toInt();

    xmp_start_player(m_ctx, m_srate, 0);
    readSettings();

    configure(m_srate, 2, Qmmp::PCM_S16LE);
    return true;
}

qint64 DecoderXmp::totalTime() const
{
    return m_totalTime;
}

int DecoderXmp::bitrate() const
{
    return 8;
}

qint64 DecoderXmp::read(unsigned char *audio, qint64 maxSize)
{
    int c = xmp_play_buffer(m_ctx, audio, maxSize, 1);

    if(c == 0)
        return maxSize;
    else if(c == -XMP_END)
        return 0;

    return -1;
}

void DecoderXmp::seek(qint64 pos)
{
    xmp_seek_time(m_ctx, pos);
}

void DecoderXmp::deinit()
{
    if(m_ctx)
    {
        xmp_end_player(m_ctx);
        xmp_release_module(m_ctx);
        xmp_free_context(m_ctx);
        m_ctx = nullptr;
    }
}

void DecoderXmp::readSettings()
{
    if(m_ctx)
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        settings.beginGroup("Xmp");
        xmp_set_player(m_ctx, XMP_PLAYER_AMP, settings.value("amp_factor", 1).toInt());
        xmp_set_player(m_ctx, XMP_PLAYER_MIX, settings.value("stereo_mix", 70).toInt());
        xmp_set_player(m_ctx, XMP_PLAYER_INTERP, settings.value("interpolation", XMP_INTERP_LINEAR).toInt());
        int flags = 0;
        if(settings.value("lowpass", false).toBool())
            flags |= XMP_DSP_LOWPASS;
        xmp_set_player(m_ctx, XMP_PLAYER_DSP, flags);
        flags = 0;
        if(settings.value("vblank", false).toBool())
            flags |= XMP_FLAGS_VBLANK;
        if(settings.value("fx9bug", false).toBool())
            flags |= XMP_FLAGS_FX9BUG;
        xmp_set_player(m_ctx, XMP_PLAYER_FLAGS, flags);
        settings.endGroup();
    }
}