#include "vgmstreamhelper.h"

#include <QSettings>

struct vfs_info
{
    FILE *file;
    int64_t offset;
    char *path;
};

static void vgm_seek(void *user_data, off_t offset)
{
    vfs_info *info = (vfs_info *)user_data;
    info->offset = stdio_seek(info->file, offset, SEEK_SET) == 0 ? offset : stdio_tell(info->file);
}

static int vgm_read(void *user_data, uint8_t *dst, int64_t offset, int length)
{
    vfs_info *info = (vfs_info *)user_data;
    if(info->offset != offset)
    {
        vgm_seek(info, offset);
    }

    const size_t read = stdio_read(info->file, dst, 1, length);
    if(read > 0)
    {
        info->offset += read;
    }
    return read;
}

static void vgm_close(struct libstreamfile_t *sf)
{
    if(!sf)
    {
        return;
    }

    vfs_info *info = (vfs_info *)sf->user_data;
    if(info && info->file)
    {
        stdio_close(info->file);
        info->file = nullptr;
    }

    free(info);
    free(sf);
}

static int64_t vgm_get_size(void *user_data)
{
    vfs_info *info = (vfs_info *)user_data;
    return stdio_length(info->file);
}

static off_t vgm_get_offset(void *user_data)
{
    vfs_info *info = (vfs_info *)user_data;
    return info->offset;
}

static const char *vgm_get_name(void *user_data)
{
    vfs_info *info = (vfs_info *)user_data;
    return info->path;
}

static libstreamfile_t *vfs_open(const char *const filename);
static libstreamfile_t *vgm_open(void *, const char *const filename)
{
    if(!filename)
    {
        return nullptr;
    }

    return vfs_open(filename);
}

static libstreamfile_t *vfs_open(const char *const filename)
{
    if(!filename)
    {
        return nullptr;
    }

    vfs_info *info = (vfs_info *)calloc(1, sizeof(vfs_info));
    info->file = stdio_open(filename);
    info->offset = 0;
    info->path = strdup(filename);

    if(!info->file)
    {
        /* Allow vgmstream's virtual files. */
        if(!libvgmstream_is_virtual_filename(filename))
        {
            qWarning("vfs_open: open file failed, %s", filename);
            free(info);
            return nullptr;
        }
    }

    libstreamfile_t *sf = (libstreamfile_t *)calloc(1, sizeof(libstreamfile_t));
    sf->read = vgm_read;
    sf->open = vgm_open;
    sf->close = vgm_close;
    sf->get_size = vgm_get_size;
    sf->get_name = vgm_get_name;
    sf->user_data = info;
    return sf;
}


VgmstreamHelper::VgmstreamHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

VgmstreamHelper::~VgmstreamHelper()
{
    deinit();
}

void VgmstreamHelper::deinit()
{
    if(m_info)
    {
        libvgmstream_free(m_info->stream);
        free(m_info);
    }
}

bool VgmstreamHelper::initialize()
{
    libstreamfile_t *sf = vfs_open(QmmpPrintable(cleanPath()));
    if(!sf)
    {
        qWarning("VgmstreamHelper: Unable to open file");
        return false;
    }

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);

    libvgmstream_config_t cfg;
    memset(&cfg, 0, sizeof(libvgmstream_config_t));
    cfg.allow_play_forever = settings.value("Vgmstream/loop_forever", false).toBool();
    cfg.play_forever = settings.value("Vgmstream/loop_forever", false).toBool();
    cfg.loop_count = settings.value("Vgmstream/loop_count", 2).toInt();
    cfg.fade_time = settings.value("Vgmstream/fade_time", 10.0).toDouble();
    cfg.fade_delay = settings.value("Vgmstream/fade_delay", 10.0).toDouble();
    cfg.force_loop = settings.value("Vgmstream/force_loop", false).toBool();

    const int track = m_path.section("#", -1).toInt() - 1;
    m_info->stream = libvgmstream_create(sf, track < 0 ? 0 : track, &cfg);
    libstreamfile_close(sf);

    if(!m_info->stream)
    {
        qWarning("VgmstreamHelper: File is not a valid format");
        return false;
    }

    m_info->totalsamples = m_info->stream->format->play_samples;

    {
        libvgmstream_title_t cfg;
        memset(&cfg, 0, sizeof(libvgmstream_title_t));
        cfg.remove_extension = 1;
        cfg.remove_archive = 1;
        cfg.filename = QmmpPrintable(cleanPath());

        char title[1024] = {0};
        libvgmstream_get_title(m_info->stream, &cfg, title, sizeof(title));
        m_metaData[Qmmp::TITLE] = title;
    }

    const QString &path = QFileInfo(cleanPath()).absoluteFilePath() + "/!tags.m3u";
    if(settings.value("Vgmstream/use_tagfile", true).toBool() && QFile::exists(path))
    {
        libstreamfile_t *sf = vfs_open(QmmpPrintable(path));
        libvgmstream_tags_t *tags = libvgmstream_tags_init(sf);
        libvgmstream_tags_find(tags, QmmpPrintable(cleanPath()));

        while(libvgmstream_tags_next_tag(tags))
        {
             const char *key = tags->key;
             const char *value = tags->val;

             if(strcasecmp(key, "TITLE") == 0)
             {
                 m_metaData[Qmmp::TITLE] = value;
             }
             else if(strcasecmp(key, "ARTIST") == 0)
             {
                 m_metaData[Qmmp::ARTIST] = value;
             }
             else if(strcasecmp(key, "ALBUMARTIST") == 0)
             {
                 m_metaData[Qmmp::ALBUMARTIST] = value;
             }
             else if(strcasecmp(key, "ALBUM") == 0)
             {
                 m_metaData[Qmmp::ALBUM] = value;
             }
             else if(strcasecmp(key, "COMMENT") == 0)
             {
                 m_metaData[Qmmp::COMMENT] = value;
             }
             else if(strcasecmp(key, "GENRE") == 0)
             {
                 m_metaData[Qmmp::GENRE] = value;
             }
             else if(strcasecmp(key, "COMPOSER") == 0)
             {
                 m_metaData[Qmmp::COMPOSER] = value;
             }
             else if(strcasecmp(key, "YEAR") == 0)
             {
                 m_metaData[Qmmp::YEAR] = value;
             }
             else if(strcasecmp(key, "TRACK") == 0)
             {
                 m_metaData[Qmmp::TRACK] = value;
             }
         }

         libvgmstream_tags_free(tags);
         libstreamfile_close(sf);
    }
    return m_info->stream;
}

void VgmstreamHelper::seek(qint64 time)
{
    m_info->position = time * sampleRate() / 1000;
    libvgmstream_seek(m_info->stream, m_info->position);
}

qint64 VgmstreamHelper::read(unsigned char *data, qint64 maxSize)
{
    if(m_info->stream->decoder->done)
    {
        return 0;
    }

    const int sample_size = channels() * sizeof(int16_t);
    int32_t sample_count = maxSize / sample_size;

    const int result = libvgmstream_fill(m_info->stream, data, sample_count);
    if(result < 0)
    {
        return 0;
    }

    sample_count = m_info->stream->decoder->buf_samples;
    m_info->position += sample_count;
    return sample_count * sample_size;
}

QList<TrackInfo*> VgmstreamHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_info)
    {
        return playlist;
    }

    int songs = m_info->stream->format->subsong_count;
    if(songs < 1)
    {
        songs = 1;
    }
    else if(songs > 32767)
    {
        songs = 32767;
    }

    for(int i = 1; i <= songs; ++i)
    {
        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TRACK, i);
            info->setValue(Qmmp::TITLE, m_metaData[Qmmp::TITLE]);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, "Video Game Music");
        }

        info->setPath("vgm://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}

QString VgmstreamHelper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}

QStringList VgmstreamHelper::filters()
{
    const QStringList filters =
    {
        //"", /* vgmstream can play extensionless files too, but plugins must accept them manually */
        "*.208",
        "*.2dx",
        "*.2dx9",
        "*.3do",
        "*.3ds",
        "*.4", //for Game.com audio
        "*.8", //txth/reserved [Gungage (PS1)]
        "*.800",
        "*.9tav",

        "*.a3c", //txth/reserved [Puyo Puyo 20th Anniversary (PSP)]
        //"aac", //common
        "*.aa3", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
        "*.aaf",
        "*.aax",
        "*.abc", //txth/reserved [Find My Own Way (PS2) tech demo]
        "*.abk",
        //"ac3", //common, FFmpeg/not parsed (AC3)
        "*.acb",
        "*.acm",
        "*.acx",
        "*.ad", //txth/reserved [Xenosaga Freaks (PS2)]
        "*.adc", //txth/reserved [Tomb Raider The Last Revelation (DC), Tomb Raider Chronicles (DC)]
        "*.adm",
        "*.adp",
        "*.adpcm",
        "*.adpcmx",
        "*.ads",
        "*.adw",
        "*.adx",
        "*.afc",
        "*.afs2",
        "*.agsc",
        "*.ahx",
        "*.ahv",
        "*.ai",
        //"aif", //common
        "*.aifc", //common?
        //"aiff", //common
        "*.aix",
        "*.akb",
        "*.al", //txth/raw [Dominions 3 - The Awakening (PC)]
        "*.al2", //txth/raw [Conquest of Elysium 3 (PC)]
        "*.amb",
        "*.ams", //txth/reserved [Super Dragon Ball Z (PS2) ELF names]
        "*.amx",
        "*.an2",
        "*.ao",
        "*.ap",
        "*.apc",
        "*.apm",
        "*.as4",
        "*.asbin",
        "*.asd",
        "*.asf",
        "*.asr",
        "*.ast",
        "*.at3",
        "*.at9",
        "*.atsl",
        "*.atsl3",
        "*.atsl4",
        "*.atslx",
        "*.atx",
        "*.aud",
        "*.audio", //txth/reserved [Grimm Echoes (Android)]
        "*.audio_data",
        "*.audiopkg",
        "*.aus",
        "*.awa", //txth/reserved [Missing Parts Side A (PS2)]
        "*.awb",
        "*.awc",
        "*.awd",
        "*.awx",

        "*.b1s", //txth/reserved [7 Wonders of the Ancient World (PS2)]
        "*.baa",
        "*.baf",
        "*.baka",
        "*.bank",
        "*.bar",
        "*.bcstm",
        "*.bcwav",
        "*.bcv", //txth/reserved [The Bigs (PSP)]
        "*.bfstm",
        "*.bfwav",
        "*.bg00",
        "*.bgm",
        "*.bgw",
        "*.bigrp",
        "*.bik",
        "*.bika", //fake extension for .bik (to be removed)
        "*.bik2",
        "*.binka",
        //"bin", //common
        "*.bk2",
        "*.bkh",
        "*.bkr",  //txth/reserved [P.N.03 (GC), Viewtiful Joe (GC)]
        "*.blk",
        "*.bmdx", //fake extension (to be removed?)
        "*.bms",
        "*.bnk",
        "*.bnm",
        "*.bns",
        "*.bnsf",
        "*.bo2",
        "*.brstm",
        "*.brstmspm",
        "*.brwav",
        "*.brwsd", //fake extension for RWSD (non-format)
        "*.bsnd",
        "*.btsnd",
        "*.bvg",
        "*.bwav",
        "*.bx",

        "*.cads",
        "*.caf",
        "*.cat",
        "*.cbd2",
        "*.cbx",
        "*.cd",
        "*.cfn", //fake extension for CAF (renamed, to be removed?)
        "*.chd", //txth/reserved [Donkey Konga (GC), Star Fox Assault (GC)]
        "*.chk",
        "*.ckb",
        "*.ckd",
        "*.cks",
        "*.cnk",
        "*.cpk",
        "*.cps",
        "*.crd",
        "*.csa", //txth/reserved [LEGO Racers 2 (PS2)]
        "*.csb",
        "*.csmp",
        "*.cvs", //txth/reserved [Aladdin in Nasira's Revenge (PS1)]
        "*.cwav",
        "*.cxb",
        "*.cxk", // ,acb+cpk/reserved [Dariusburst: Another Chronicle (AC)]
        "*.cxs",

        "*.d2", //txth/reserved [Dodonpachi Dai-Ou-Jou (PS2)]
        "*.da",
        //"dat", //common
        "*.data",
        "*.dax",
        "*.dbm",
        "*.dct",
        "*.dcs",
        "*.ddsp",
        "*.de2",
        "*.dec",
        "*.dic",
        "*.diva",
        "*.dmsg", //fake extension/header id for .sgt (to be removed)
        "*.drm", //txth/reserved [Ben 10 (HyperScan), IWL (HyperScan)]
        "*.ds2", //txth/reserved [Star Wars Bounty Hunter (GC)]
        "*.dsb",
        "*.dsf",
        "*.dsp",
        "*.dspw",
        "*.dtk",
        "*.dty",
        "*.dvi",
        "*.dyx", //txth/reserved [Shrek 4 (iOS)]

        "*.e4x",
        "*.eam",
        "*.eas",
        "*.eda", //txth/reserved [Project Eden (PS2)]
        "*.emff", //fake extension for .mul (to be removed)
        "*.enm",
        "*.eno",
        "*.ens",
        "*.esf",
        "*.exa",
        "*.ezw",

        "*.fag",
        "*.fda",
        "*.filp",
        "*.fish",
        //"flac", //common
        "*.flx",
        "*.fsb",
        "*.fsv",
        "*.fwav",
        "*.fwse",

        "*.g1l",
        "*.gbts",
        "*.gca",
        "*.gcm",
        "*.gcub",
        "*.gcw",
        "*.ged",
        "*.genh",
        "*.gin",
        "*.gmd",  //txth/semi [High Voltage games: Charlie and the Chocolate Factory (GC), Zathura (GC)]
        "*.gms",
        "*.grn",
        "*.gsf",
        "*.gsp",
        "*.gtd",
        "*.gwb",
        "*.gwm",

        "*.h4m",
        "*.hab",
        "*.hbd",
        "*.hca",
        "*.hd",
        "*.hd2",
        "*.hd3",
        "*.hdr",
        "*.hdt",
        "*.his",
        "*.hps",
        "*.hsf",
        "*.hvqm",
        "*.hwx", //txth/reserved [Star Wars Episode III (Xbox)]
        "*.hx2",
        "*.hx3",
        "*.hxc",
        "*.hxd",
        "*.hxg",
        "*.hxx",
        "*.hwas",
        "*.hwb",
        "*.hwd",

        "*.iab",
        "*.iadp",
        "*.idmsf",
        "*.idsp",
        "*.idvi", //fake extension/header id for .pcm (renamed, to be removed)
        "*.idwav",
        "*.idx",
        "*.idxma",
        "*.ifs",
        "*.ikm",
        "*.ild",
        "*.ilf", //txth/reserved [Madden NFL 98 (PS1)]
        "*.ilv", //txth/reserved [Star Wars Episode III (PS2)]
        "*.ima",
        "*.imc",
        "*.imf",
        "*.imx",
        "*.int",
        "*.is14",
        "*.isb",
        "*.isd",
        "*.isws",
        "*.itl",
        "*.ivaud",
        "*.ivag",
        "*.ivb",
        "*.ivs", //txth/reserved [Burnout 2 (PS2)]
        "*.ixa",

        "*.joe",
        "*.jstm",

        "*.k2sb",
        "*.ka1a",
        "*.kat",
        "*.kces",
        "*.kcey", //fake extension/header id for .pcm (renamed, to be removed)
        "*.km9",
        "*.kma",  //txth/reserved [Dynasty Warriors 7: Empires (PS3)]
        "*.kmx",
        "*.kovs", //fake extension/header id for .kvs
        "*.kno",
        "*.kns",
        "*.koe",
        "*.kraw",
        "*.ktac",
        "*.ktsl2asbin",
        "*.ktss", //fake extension/header id for .kns
        "*.kvs",
        "*.kwa",

        "*.l",
        "*.l00", //txth/reserved [Disney's Dinosaur (PS2)]
        "*.laac", //fake extension for .aac (tri-Ace)
        "*.ladpcm", //not fake
        "*.laif", //fake extension for .aif (various)
        "*.laiff", //fake extension for .aiff
        "*.laifc", //fake extension for .aifc
        "*.lac3", //fake extension for .ac3, FFmpeg/not parsed
        "*.lasf", //fake extension for .asf (various)
        "*.lbin", //fake extension for .bin (various)
        "*.ldat", //fake extension for .dat
        "*.ldt",
        "*.lep",
        "*.lflac", //fake extension for .flac, FFmpeg/not parsed
        "*.lin",
        "*.lm0",
        "*.lm1",
        "*.lm2",
        "*.lm3",
        "*.lm4",
        "*.lm5",
        "*.lm6",
        "*.lm7",
        "*.lmp2", //fake extension for .mp2, FFmpeg/not parsed
        "*.lmp3", //fake extension for .mp3, FFmpeg/not parsed
        "*.lmp4", //fake extension for .mp4
        "*.lmpc", //fake extension for .mpc, FFmpeg/not parsed
        "*.logg", //fake extension for .ogg
        "*.lopus", //fake extension for .opus, used by LOPU too
        "*.lp",
        "*.lpcm",
        "*.lpk",
        "*.lps",
        "*.lrmh",
        "*.lse",
        "*.lsf",
        "*.lstm", //fake extension for .stm
        "*.lwav", //fake extension for .wav
        "*.lwd",
        "*.lwma", //fake extension for .wma, FFmpeg/not parsed

        "*.mab",
        "*.mad",
        "*.map",
        "*.mc3",
        "*.mca",
        "*.mcadpcm",
        "*.mcg",
        "*.mds",
        "*.mdsp",
        "*.med",
        "*.mhk",
        "*.mjb",
        "*.mi4", //fake extension for .mib (renamed, to be removed)
        "*.mib",
        "*.mic",
        "*.mio",
        "*.mogg",
        //"m4a", //common
        //"m4v", //common
        "*.mon",
        //"mov", //common
        "*.move",
        //"mp+", //common [Moonshine Runners (PC)]
        //"mp2", //common
        //"mp3", //common
        //"mp4", //common
        //"mpc", //common
        "*.mpdsp",
        "*.mpds",
        "*.mpf",
        "*.mps", //txth/reserved [Scandal (PS2)]
        "*.ms",
        "*.msa",
        "*.msb",
        "*.msd",
        "*.mse",
        "*.msf",
        "*.mss",
        "*.msv",
        "*.msvp", //fake extension/header id for .msv
        "*.msx",
        "*.mta2",
        "*.mtaf",
        "*.mtt", //txth/reserved [Splinter Cell: Pandora Tomorrow (PS2)]
        "*.mul",
        "*.mups",
        "*.mus",
        "*.musc",
        "*.musx",
        "*.mvb", //txth/reserved [Porsche Challenge (PS1)]
        "*.mwa", //txth/reserved [Fatal Frame (Xbox)]
        "*.mwv",
        "*.mxst",
        "*.myspd",

        "*.n64",
        "*.naac",
        "*.nds",
        "*.ndp", //fake extension/header id for .nds
        "*.nlsd",
        "*.no",
        "*.nop",
        "*.nps",
        "*.npsf", //fake extension/header id for .nps (in bigfiles)
        "*.nsa",
        "*.nsopus",
        "*.nfx",
        "*.nub",
        "*.nub2",
        "*.nusnub",
        "*.nus3audio",
        "*.nus3bank",
        "*.nwa",
        "*.nwav",
        "*.nxa",
        "*.nxopus",

        "*.oga",
        //"ogg", //common
        "*.ogg_",
        "*.ogl",
        "*.ogs",
        "*.ogv",
        "*.oma", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
        "*.omu",
        "*.oor",
        "*.opu",
        //"opus", //common
        "*.opusnx",
        "*.opusx",
        "*.oto", //txth/reserved [Vampire Savior (SAT)]
        "*.ovb", //txth/semi [namCollection: Tekken (PS2), Tekken 5: Tekken 1-3 (PS2)]

        "*.p04", //txth/reserved [Psychic Force 2012 (DC), Skies of Arcadia (DC)]
        "*.p08", //txth/reserved [SoulCalibur (DC)]
        "*.p16", //txth/reserved [Astal (SAT)]
        "*.p1d", //txth/reserved [Farming Simulator 18 (3DS)]
        "*.p2a", //txth/reserved [Thunderhawk Operation Phoenix (PS2)]
        "*.p2bt",
        "*.p3d",
        "*.paf",
        "*.past",
        "*.patch3audio",
        "*.pcm",
        "*.pdt",
        "*.phd",
        "*.pk",
        "*.pona",
        "*.pos",
        "*.ps3",
        "*.psb",
        "*.psf",
        "*.psh", //fake extension for .vsv (to be removed)
        "*.psn",
        "*.pth",
        "*.pwb",

        "*.qwv", //txth/reserved [Bishi Bashi Champ Online (AC)]

        "*.r",
        "*.rac", //txth/reserved [Manhunt (Xbox)]
        "*.rad",
        "*.rak",
        "*.ras",
        "*.raw", //txth/reserved [Madden NHL 97 (PC)-pcm8u]
        "*.rda", //FFmpeg/reserved [Rhythm Destruction (PC)]
        "*.res", //txth/reserved [Spider-Man: Web of Shadows (PSP)]
        "*.rkv",
        "*.rof",
        "*.rpgmvo",
        "*.rrds",
        "*.rsd",
        "*.rsf",
        "*.rsm",
        "*.rsnd", //txth/reserved [Birushana: Ichijuu no Kaze (Switch)]
        "*.rsp",
        "*.rstm", //fake extension/header id for .rstm (in bigfiles)
        "*.rvw", //txth/reserved [Half-Minute Hero (PC)]
        "*.rvws",
        "*.rwar",
        "*.rwav",
        "*.rws",
        "*.rwsd",
        "*.rwx",
        "*.rxx", //txth/reserved [Full Auto (X360)]

        "*.s14",
        "*.s3s", //txth/reserved [DT Racer (PS2)]
        "*.s3v", //Sound Voltex (AC)
        "*.sab",
        "*.sad",
        "*.saf",
        "*.sag",
        "*.sam", //txth/reserved [Lost Kingdoms 2 (GC)]
        "*.sap",
        "*.sb0",
        "*.sb1",
        "*.sb2",
        "*.sb3",
        "*.sb4",
        "*.sb5",
        "*.sb6",
        "*.sb7",
        "*.sbk",
        "*.sbin",
        "*.sbr",
        "*.sbv",
        "*.sig",
        "*.slb", //txth/reserved [Vingt-et-un Systems PS2 games (Last Escort, etc)]
        "*.sm0",
        "*.sm1",
        "*.sm2",
        "*.sm3",
        "*.sm4",
        "*.sm5",
        "*.sm6",
        "*.sm7",
        "*.sc",
        "*.scd",
        "*.sch",
        "*.sd9",
        "*.sdd",
        "*.sdl",
        "*.sdp", //txth/reserved [Metal Gear Arcade (AC)]
        "*.sdf",
        "*.sdt",
        "*.se",
        "*.se3", //txth/reserved (.nub container) [Tales of Vesperia (X360/PS3), Tales of Graces f (PS3)]
        "*.seb",
        "*.sed",
        "*.seg",
        "*.sem", //txth/reserved [Oretachi Game Center Zoku: Sonic Wings (PS2)]
        "*.sf0",
        "*.sfl",
        "*.sfs",
        "*.sfx",
        "*.sgb",
        "*.sgd",
        "*.sgt",
        "*.shaa",
        "*.shsa",
        "*.skx",
        "*.slb", //txth/reserved [THE Nekomura no Hitobito (PS2)]
        "*.sli",
        "*.smh",
        "*.smk",
        "*.smp",
        "*.smv",
        "*.sn0",
        "*.snb",
        "*.snd",
        "*.snds", //fake extension for .wav (renamed, to be removed)
        "*.sng",
        "*.sngw",
        "*.snr",
        "*.sns",
        "*.snu",
        "*.snz", //txth/reserved [Killzone HD (PS3)]
        "*.sod",
        "*.son",
        "*.spc",
        "*.sph",
        "*.spt",
        "*.spm",
        "*.sps",
        "*.spsd",
        "*.spsis14",
        "*.spsis22",
        "*.spw",
        "*.srcd",
        "*.sre",
        "*.srsa",
        "*.ss2",
        "*.ssd", //txth/reserved [Zack & Wiki (Wii)]
        "*.ssf",
        "*.ssm",
        "*.ssp",
        "*.sspr",
        "*.sss",
        "*.ster",
        "*.sth",
        "*.stm",
        "*.str",
        "*.stream",
        "*.strm",
        "*.sts",
        "*.stv", //txth/reserved [Socio Art Logic PS2 games (Zero no Tsukaima games, Cambrian QTS, Shirogane no Soleil, etc)]
        "*.sts_cp3",
        "*.stx",
        "*.svag",
        "*.svs",
        "*.svg",
        "*.swag",
        "*.swar",
        "*.swav",
        "*.swd",
        "*.switch", //txth/reserved (.m4a-x.switch) [Ikinari Maou (Switch)]
        "*.switch_audio",
        "*.sx",
        "*.sxd",
        "*.sxd2",
        "*.sxd3",
        "*.szd",
        "*.szd1",
        "*.szd3",

        "*.tad",
        "*.tgq",
        "*.tgv",
        "*.thp",
        "*.tmx",
        "*.tra",
        "*.trk",
        "*.trs", //txth/semi [Kamiwaza (PS2), Shinobido (PS2)]
        "*.tsdse3", //txth/reserved [Tales of Xillia (PS3)-nub container]
        "*.tsdse4", //txth/reserved [Tales of Xillia (PS3)-nub container]
        "*.tun",
        "*.txth",
        "*.txtp",

        "*.u0",
        "*.ue4opus",
        "*.ueba",
        "*.ulw", //txth/raw [Burnout (GC)]
        "*.um3",
        "*.utk",
        "*.uv",

        "*.v",
        "*.v0",
        //"v1", //dual channel with v0
        "*.va3",
        "*.vab",
        "*.vag",
        "*.vai",
        "*.vam", //txth/reserved [Rocket Power: Beach Bandits (PS2)]
        "*.vas",
        "*.vb", //txth/reserved [Tantei Jinguji Saburo: Mikan no Rupo (PS1)]
        "*.vbk",
        "*.vbx", //txth/reserved [THE Taxi 2 (PS2)]
        "*.vca", //txth/reserved [Pac-Man World (PS1)]
        "*.vcb", //txth/reserved [Pac-Man World (PS1)]
        "*.vds",
        "*.vdm",
        "*.vgi", //txth/reserved [Time Crisis II (PS2)]
        "*.vgm", //txth/reserved [Maximo (PS2)]
        "*.vgs",
        "*.vgv",
        "*.vh",
        "*.vid",
        "*.vig",
        "*.vis",
        "*.vm4", //txth/reserved [Elder Gate (PS1)]
        "*.vms",
        "*.vmu", //txth/reserved [Red Faction (PS2)]
        "*.voi",
        "*.vp6",
        "*.vpk",
        "*.vs",
        "*.vsf",
        "*.vsv",
        "*.vxn",

        "*.w",
        "*.waa",
        "*.wac",
        "*.wad",
        "*.waf",
        "*.wam",
        "*.was",
        //"wav", //common
        "*.wavc",
        "*.wave",
        "*.wavebatch",
        "*.wavm",
        "*.wavx", //txth/reserved [LEGO Star Wars (Xbox)]
        "*.wax",
        "*.way",
        "*.wb",
        "*.wb2",
        "*.wbd",
        "*.wbk",
        "*.wd",
        "*.wem",
        "*.wiive", //txth/semi [Rubik World (Wii)]
        "*.wic", //txth/reserved [Road Rash (SAT)-videos]
        "*.wip", //txth/reserved [Colin McRae DiRT (PC)]
        "*.wlv", //txth/reserved [ToeJam & Earl III: Mission to Earth (DC)]
        "*.wp2",
        "*.wpd",
        "*.wsd",
        "*.wsi",
        "*.wst", //txth/reserved [3jigen Shoujo o Hogo Shimashita (PC)]
        "*.wua",
        "*.wv2",
        "*.wv6",
        "*.wvd", //txth/reserved [Donkey Kong Barrel Blast (Wii)]
        "*.wve",
        "*.wvs",
        "*.wvx",
        "*.wxd",
        "*.wxv",

        "*.x",
        "*.x360audio", //fake extension for Unreal Engine 3 XMA (real extension unknown)
        "*.xa",
        "*.xa2",
        "*.xa30",
        "*.xai",
        "*.xag", //txth/reserved [Tamsoft's PS2 games]
        "*.xau",
        "*.xav",
        "*.xb", //txth/reserved [Scooby-Doo! Unmasked (Xbox)]
        "*.xbw",
        "*.xhd",
        "*.xen",
        "*.xma",
        "*.xma2",
        "*.xms",
        "*.xmu",
        "*.xmv",
        "*.xnb",
        "*.xsh",
        "*.xsf",
        "*.xst",
        "*.xse",
        "*.xsew",
        "*.xss",
        "*.xvag",
        "*.xwav", //fake extension for .wav (renamed, to be removed)
        "*.xwb",
        "*.xmd",
        "*.xopus",
        "*.xps",
        "*.xwc",
        "*.xwm",
        "*.xwma",
        "*.xws",
        "*.xwv",

        "*.ydsp",
        "*.ymf",

        "*.zic",
        "*.zsd",
        "*.zsm",
        "*.zss",
        "*.zwv",

        "*.vgmstream" /* fake extension, catch-all for FFmpeg/txth/etc */

        //, NULL //end mark
    };
    return filters;
}
