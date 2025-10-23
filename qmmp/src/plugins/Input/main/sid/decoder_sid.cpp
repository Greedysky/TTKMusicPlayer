#include <sidplayfp/sidplayfp.h>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/sidbuilder.h>
#include <sidplayfp/builders/residfp.h>
#include <sidplayfp/builders/resid.h>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/SidDatabase.h>
#include <qmmp/trackinfo.h>
#include "decoder_sid.h"

#include <QSettings>
#include <QFileInfo>

DecoderSID::DecoderSID(SidDatabase *db, const QString &path)
    : Decoder(),
      m_path(path),
      m_player(new sidplayfp()),
      m_db(db),
      m_tune(nullptr)
{

}

DecoderSID::~DecoderSID()
{
    delete m_player;
}

bool DecoderSID::initialize()
{
    m_length_in_bytes = 0;
    m_read_bytes = 0;

    int track = -1;
    const QString &filePath = TrackInfo::pathFromUrl(m_path, &track);

    m_tune.load(QmmpPrintable(filePath));
    if(!m_tune.getInfo())
    {
        qWarning("DecoderSID: unable to load tune, error: %s", m_tune.statusString());
        return false;
    }

    int count = m_tune.getInfo()->songs();
    if(track == 0)
        track = count;

    if(track > count || track < 1)
    {
        qWarning("DecoderSID: track number is out of range");
        return false;
    }

    m_tune.selectSong(track);
    if(!m_tune.getStatus())
    {
        qWarning("DecoderSID: error: %s", m_tune.statusString());
        return false;
    }

    //send metadata for pseudo-protocol
    const SidTuneInfo *tune_info = m_tune.getInfo();
    QMap<Qmmp::MetaData, QString> metadata;
    metadata.insert(Qmmp::TITLE,  tune_info->infoString(0));
    metadata.insert(Qmmp::ARTIST, tune_info->infoString(1));
    metadata.insert(Qmmp::COMMENT, tune_info->commentString(0));
    metadata.insert(Qmmp::TRACK, QString("%1").arg(track));
    addMetaData(metadata);

    //read settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");

    if(settings.value("use_hvsc", false).toBool())
    {
        char md5[SidTune::MD5_LENGTH + 1];
        m_tune.createMD5(md5);
        m_length = m_db->length(md5, track) * 1000;
    }

    if(m_length <= 0)
    {
        m_length = settings.value("use_length", false).toBool() ? (settings.value("song_length", 180).toInt() * 1000) : (QFileInfo(filePath).size() * 8.0 / bitrate());
    }

    qDebug("DecoderSID: song length: %ld", m_length);

    sidbuilder *rs = nullptr;
    if(settings.value("engine", "residfp").toString() == "residfp")
    {
        rs = new ReSIDfpBuilder("ReSIDfp builder");
        qDebug("DecoderSID: using ReSIDfp emulation");
    }
    else
    {
        rs = new ReSIDBuilder("ReSID builder");
        qDebug("DecoderSID: using ReSID emulation");
    }
    rs->create(m_player->info().maxsids());

    SidConfig cfg = m_player->config();
    cfg.frequency    = settings.value("sample_rate", 48000).toInt();
    cfg.samplingMethod = (SidConfig::sampling_method_t)settings.value("resampling_method", SidConfig::INTERPOLATE).toInt();
    cfg.playback     = SidConfig::STEREO;
    cfg.sidEmulation = rs;
    cfg.fastSampling = settings.value("fast_resampling", false).toBool();
    settings.endGroup();

    if(!m_player->config(cfg))
    {
        qWarning("DecoderSID: unable to load config, error: %s", m_player->error());
        return false;
    }

    if(!m_player->load(&m_tune))
    {
        qWarning("DecoderSID: unable to load tune, error: %s", m_player->error());
        return false;
    }

#if (LIBSIDPLAYFP_VERSION_MAJ > 2) || ((LIBSIDPLAYFP_VERSION_MAJ == 2) && (LIBSIDPLAYFP_VERSION_MIN >= 15))
    m_player->initMixer(true);
#endif

    configure(cfg.frequency, 2, Qmmp::PCM_S16LE);
    m_length_in_bytes = audioParameters().sampleRate() * audioParameters().frameSize() * m_length;
    qDebug("DecoderSID: initialize success");
    return true;
}

qint64 DecoderSID::totalTime() const
{
    return m_length;
}

void DecoderSID::seek(qint64 time)
{
    Q_UNUSED(time);
}

int DecoderSID::bitrate() const
{
    return 8;
}

qint64 DecoderSID::read(unsigned char *data, qint64 maxSize)
{
    maxSize = qMin(maxSize, qMax(m_length_in_bytes - m_read_bytes, qint64(0)));
    maxSize -= maxSize % 4;
    if(maxSize <= 0)
        return 0;

#if (LIBSIDPLAYFP_VERSION_MAJ > 2) || ((LIBSIDPLAYFP_VERSION_MAJ == 2) && (LIBSIDPLAYFP_VERSION_MIN >= 15))
    const int cycles = maxSize * 1000000 / m_player->config().frequency; //ms
    const int samples = m_player->play(cycles);
    const int decoded_bytes = m_player->mix((short *)data, samples) * 2;
#else
    const int decoded_bytes = m_player->play((short *)data, maxSize / 2) * 2;
#endif
    m_read_bytes += decoded_bytes;
    return decoded_bytes;
}
