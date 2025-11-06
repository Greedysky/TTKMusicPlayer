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
        m_tags.clear();
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
        m_tags.insert("modulemd5", info->modulemd5);
        m_tags.insert("modulepath", info->modulefname);
        m_tags.insert("formatname", info->formatname);
        m_tags.insert("modulename", info->modulename);
        m_tags.insert("playername", info->playername);
        m_tags.insert("format", info->detectioninfo.ext);
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
            info->setValue(Qmmp::FORMAT_NAME, "Unix Amiga Delitracker Emulator");
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
    QStringList filters;
    filters << "*.aam" << "*.amc" << "*.aon" << "*.aon8" << "*.aps" << "*.ash" << "*.ast" << "*.avp";
    filters << "*.bd" << "*.bds" << "*.bsi" << "*.bss" << "*.bye";
    filters << "*.cm" << "*.core" << "*.cust";
    filters << "*.dh" << "*.dl" << "*.dlm1" << "*.dlm2" << "*.dln" << "*.dm" << "*.dm2" << "*.dmu" << "*.doda" << "*.dsc" << "*.dsr" << "*.dss" << "*.dw" << "*.dz";
    filters << "*.ea" << "*.ems" << "*.emsv6" << "*.ex";
    filters << "*.fp" << "*.fred" << "*.fw";
    filters << "*.glue" << "*.gmc" << "*.gray";
    filters << "*.hd" << "*.hip" << "*.hip7" << "*.hipc" << "*.hot";
    filters << "*.ims" << "*.is" << "*.is20";
    filters << "*.jam" << "*.jcb" << "*.jcbo" << "*.jd" << "*.jmf" << "*.jo" << "*.jpo" << "*.jt";
    filters << "*.kh" << "*.kim" << "*.kris";
    filters << "*.lion" << "*.lme";
    filters << "*.ma" << "*.mc" << "*.mcmd" << "*.mco" << "*.md" << "*.mii" << "*.mk2" << "*.mkii" << "*.ml" << "*.mm8" << "*.mmdc" << "*.mok" << "*.mon" << "*.mosh" << "*.mso" << "*.mug" << "*.mug2" << "*.mw";
    filters << "*.ntp";
    filters << "*.pap" << "*.pn" << "*.ps" << "*.psa" << "*.psf" << "*.pt" << "*.ptm" << "*.puma" << "*.pvp";
    filters << "*.rh" << "*.rho" << "*.riff" << "*.rmc";
    filters << "*.s7g" << "*.sa" << "*.sas"  << "*.sb" << "*.sc" << "*.scn" << "*.scr" << "*.sct" << "*.scumm" << "*.sdr" << "*.sg" << "*.sid1" << "*.sid2" << "*.sm" << "*.sm3" << "*.smn" << "*.smpro" << "*.sng" << "*.snk" << "*.soc" << "*.sog" << "*.spl" << "*.sqt" << "*.ss" << "*.sun" << "*.syn" << "*.synmod";
    filters << "*.tcb" << "*.tf" << "*.tfmx" << "*.thx" << "*.tits" << "*.tme" << "*.tro" << "*.tronic" << "*.tw";
    filters << "*.ufo";
    filters << "*.vss";
    filters << "*.wb";
    // dir section
    filters << "*.smus"; // (Instruments)
    filters << "*.snx";  // (Instruments)
    filters << "*.tiny"; // (Instruments)
    // pair suffix section
    filters << "*.adsc"; // (adsc, as)
    filters << "*.dat";  // (dat, ssd)
    filters << "*.dum";  // (dum, ins)
    filters << "*.osp";  // (osp, smp)
    // pair prefix section
    filters << "dns.*";  // (dns, smp)
    filters << "jpn.*";  // (jpn, smp)
    filters << "jpnd.*"; // (jpnd, jpns)
    filters << "max.*";  // (thm, smp)
    filters << "mcr.*";  // (mcr, mcs)
    filters << "mfp.*";  // (mfp, smp)
    filters << "npp.*";  // (npp, smp)
    filters << "pat.*";  // (pat, smp, WantedTeam.bin)
    filters << "qpa.*";  // (qpa, smp)
    filters << "qts.*";  // (qts, smp)
    filters << "sjs.*";  // (sjs, smp)
    filters << "thm.*";  // (thm, smp)
    filters << "tmk.*";  // (tmk, smp)
    filters << "tpu.*";  // (tpu, smp)
    filters << "uds.*";  // (uds, smp)
    return filters;
}
