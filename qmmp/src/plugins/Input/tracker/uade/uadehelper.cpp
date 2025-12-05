#include "uadehelper.h"

UADEHelper::UADEHelper(const QString &path)
    : m_path(path)
{

}

UADEHelper::~UADEHelper()
{
    deinit();
}

void UADEHelper::deinit()
{
    if(m_state)
    {
        m_metaData.clear();
        uade_stop(m_state);
        uade_cleanup_state(m_state);
    }
}

bool UADEHelper::initialize()
{
    struct uade_config* config = uade_new_config();
    uade_config_set_option(config, UC_ONE_SUBSONG, nullptr);
    uade_config_set_option(config, UC_IGNORE_PLAYER_CHECK, nullptr);
    uade_config_set_option(config, UC_FREQUENCY, "44100");
    uade_config_set_option(config, UC_BASE_DIR, qPrintable(Qmmp::ttkPluginPath() + "/config/uade"));

    try
    {
        m_state = uade_new_state(config);
        free(config);
    }
    catch(...)
    {
        free(config);
        return false;
    }

    if(!m_state)
    {
        qWarning("UADEHelper: Unable to create uade state");
        return false;
    }

    m_track = m_path.section("#", -1).toInt() - 1;
    if(m_track < 0)
    {
        m_track = 0;
    }

    const QString &path = cleanPath();
    if(uade_play(QmmpPrintable(path), m_track, m_state) != 1)
    {
        qWarning("UADEHelper: Unable to open file, %s", qPrintable(path));
        return false;
    }

    const struct uade_song_info *info = uade_get_song_info(m_state);
    if(info)
    {
        m_metaData.insert("modulemd5", info->modulemd5);
        m_metaData.insert("modulepath", info->modulefname);
        m_metaData.insert("formatname", info->formatname);
        m_metaData.insert("modulename", info->modulename);
        m_metaData.insert("playername", info->playername);
        m_metaData.insert("format", info->detectioninfo.ext);
    }
    return true;
}

void UADEHelper::seek(qint64 time)
{
    uade_seek(UADE_SEEK_SONG_RELATIVE, time / 1000.0, m_track, m_state);
}

qint64 UADEHelper::totalTime()
{
    const struct uade_song_info *info = uade_get_song_info(m_state);
    if(!info)
    {
        return 0;
    }

    const int time = info->duration;
    const int max = info->subsongs.max - info->subsongs.min + 1;
    if(max <= 0)
    {
        return 0;
    }

    return (time <= 0 ? (info->modulebytes / 8 / 10 / max) : time) * 1000;
}

qint64 UADEHelper::read(unsigned char *data, qint64 maxSize)
{
    return uade_read(data, maxSize, m_state);
}

QList<TrackInfo*> UADEHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    const struct uade_song_info *tag = uade_get_song_info(m_state);
    if(!tag)
    {
        return playlist;
    }

    QFileInfo fin(cleanPath());
    QString title = fin.baseName();
    if(!filters().contains("*." + fin.suffix(), Qt::CaseInsensitive))
    {
        title = fin.suffix();
    }

    int min = tag->subsongs.min;
    int max = tag->subsongs.max;
    if(min == 0)
    {
        min += 1;
        max += 1;
    }

    for(int i = min; i <= max; ++i)
    {
        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, title);
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, "UADE");
        }

        info->setPath("uade://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}

QString UADEHelper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}

QStringList UADEHelper::filters()
{
    const QStringList filters =
    {
        "*.aam", "*.ac1d", "*.amc", "*.aon", "*.aon8", "*.aps", "*.ash", "*.ast", "*.avp",
        "*.bd", "*.bds", "*.bsi", "*.bss", "*.bye",
        "*.cm", "*.core", "*.cust",
        "*.dh", "*.di", "*.dl", "*.dlm1", "*.dlm2", "*.dln", "*.dm", "*.dm2", "*.dmu", "*.doda", "*.dsc", "*.dsr", "*.dss", "*.dw", "*.dz",
        "*.ea", "*.ems", "*.emsv6", "*.ex",
        "*.fc-m", "*.fp", "*.fred", "*.fw",
        "*.glue", "*.gmc", "*.gray",
        "*.hd", "*.hip", "*.hip7", "*.hipc", "*.hot",
        "*.ims", "*.is", "*.is20",
        "*.jam", "*.jcb", "*.jcbo", "*.jd", "*.jmf", "*.jo", "*.jpo", "*.js", "*.jt",
        "*.kh", "*.kim", "*.kris",
        "*.lion", "*.lme",
        "*.ma", "*.mc", "*.mcmd", "*.mco", "*.md", "*.mdst", "*.mii", "*.mk2", "*.mkii", "*.ml", "*.mm8", "*.mmdc", "*.mok", "*.mon", "*.mosh", "*.mso", "*.mtp2", "*.mug", "*.mug2", "*.mw",
        "*.ntp", "*.np2", "*.np3",
        "*.one",
        "*.p4x", "*.p5x", "*.p6x", "*.pap", "*.pn", "*.pp21", "*.pp30", "*.pr1", "*.prom", "*.prun", "*.pru2", "*.ps", "*.psa", "*.psf", "*.pt", "*.ptm", "*.puma", "*.pvp",
        "*.rh", "*.rho", "*.riff", "*.rmc",
        "*.s7g", "*.sa", "*.sa_old", "*.sas", "*.sb", "*.sc", "*.scn", "*.scr", "*.sct", "*.scumm", "*.sdr", "*.sg", "*.sid1", "*.sid2", "*.skyt", "*.sm", "*.sm3", "*.smn", "*.smpro", "*.sng", "*.snk", "*.soc", "*.sog", "*.spl", "*.sqt", "*.ss", "*.sun", "*.syn", "*.synmod",
        "*.tcb", "*.tf", "*.tfmx", "*.thx", "*.tit", "*.tits", "*.tme", "*.tp2", "*.tp3", "*.tro", "*.tronic", "*.tw", "*.two",
        "*.ufo", "*.unic",
        "*.vss",
        "*.wb",
        "*.xann",
        "*.ymst",
        // dir section
        "*.cin",  // (Instruments)
        "*.smus", // (Instruments)
        "*.snx",  // (Instruments)
        "*.tiny", // (Instruments)
        // pair suffix section
        "*.adsc", // (adsc, as)
        "*.dat",  // (dat, ssd)
        "*.dum",  // (dum, ins)
        "*.osp",  // (osp, smp)
        // pair prefix section
        "alp.*",  // (alp, smp)
        "dns.*",  // (dns, smp)
        "jpn.*",  // (jpn, smp)
        "jpnd.*", // (jpnd, jpns)
        "max.*",  // (max, smp)
        "mcr.*",  // (mcr, mcs)
        "mfp.*",  // (mfp, smp)
        "midi.*", // (midi, smpl)
        "npp.*",  // (npp, smp)
        "pat.*",  // (pat, smp, WantedTeam.bin)
        "qpa.*",  // (qpa, smp)
        "qts.*",  // (qts, smp)
        "rjp.*",  // (rjp, smp)
        "sjs.*",  // (sjs, smp)
        "thm.*",  // (thm, smp)
        "tmk.*",  // (tmk, smp)
        "tpu.*",  // (tpu, smp)
        "uds.*"   // (uds, smp)
    };
    return filters;
}
