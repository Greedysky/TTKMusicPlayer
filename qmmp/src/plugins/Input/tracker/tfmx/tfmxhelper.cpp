#include "tfmxhelper.h"

static const char* findFilenameStart(const char *path, int *offset)
{
    for(size_t i = strlen(path) - 1; i > 0; --i)
    {
        const char c = path[i];
        if(c == '/' || c == '\\')
        {
            *offset = (int)(i + 1);
            return &path[i + 1];
        }
    }

    *offset = 0;
    return path;
}

static const char* findExtension(const char *path, int *offset)
{
    for(size_t i = strlen(path) - 1; i > 0; --i)
    {
        if(path[i] == '.')
        {
            *offset = (int)(i + 1);
            return &path[i];
        }
    }

    *offset = 0;
    return path;
}

static bool loadMDAT(TfmxState *state, const char *mdatName, const char *smplName)
{
    TfmxData mdat = {0, 0, 0};
    TfmxData smpl = {0, 0, 0};

    // first load mdat to memory
    QFile mdatFile(mdatName);
    if(!mdatFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    const QByteArray &mdatBuffer = mdatFile.readAll();
    mdat.size = mdatBuffer.length();
    mdat.data = (U8*)mdatBuffer.data();

    // Load sample file to memory
    QFile smplFile(smplName);
    if(!smplFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    const QByteArray &smplBuffer = smplFile.readAll();
    smpl.size = smplBuffer.length();
    smpl.data = (U8*)smplBuffer.data();
    return LoadTFMXFile(state, &mdat, &smpl);
}

static bool loadTFM(TfmxState *state, const char *path)
{
    // first load mdat to memory
    QFile mdatFile(path);
    if(!mdatFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    TfmxData mdat = {0, 0, 0};
    const QByteArray &mdatBuffer = mdatFile.readAll();
    mdat.data = (U8*)mdatBuffer.data();

    if(mdat.size <= 0)
    {
        return false;
    }

    // validate that the size is valid
    if(mdat.size < sizeof(TFMXHeader) + 8 + (3 * 4))
    {
        qWarning("The file is too small (can't contain a full tfx file)");
        return false;
    }

    U8 *raw = mdat.data;
    if(strncmp("TFMX-MOD", (S8*)mdat.data, 8) != 0)
    {
        qWarning("The file doesn't have correct header data");
        return false;
    }

    mdat.data += 8;
    const U32 sOffset = *((U32*)mdat.data);
    mdat.data += 4;
    const U32 tOffset = *((U32*)mdat.data);
    mdat.data += 8;  // skipping res
    mdat.size = sOffset - 20 - sizeof(TFMXHeader);

    TfmxData smpl = {0, 0, 0};
    smpl.data = raw + sOffset;
    smpl.size = tOffset - sOffset;

    if(mdat.size > 0x10000)
    {
        mdat.size = 0x10000;
    }
    return LoadTFMXFile(state, &mdat, &smpl);
}

static int TFMXLoad(TfmxState *state, const char *path)
{
    int sOffset = 0;
    int eOffset = 0;
    bool status = false;

    const char *filename = findFilenameStart(path, &sOffset);
    const char *extension = findExtension(path, &eOffset);

    if(strncasecmp(filename, "mdat.", 5) == 0)
    {
        // first check if we have mdat.<filename>,smpl.<filename> which is the orignal Amiga naming
        char *smplName = strdup(path);
        // Case-preserving conversion of "mdat" to "smpl"
        smplName[sOffset + 0] ^= 'm' ^ 's';
        smplName[sOffset + 1] ^= 'd' ^ 'm';
        smplName[sOffset + 2] ^= 'a' ^ 'p';
        smplName[sOffset + 3] ^= 't' ^ 'l';

        status = loadMDAT(state, path, smplName);
        free(smplName);
    }
    else if(strncasecmp(extension, ".mdat", 5) == 0)
    {
        // check if we have <filename>.mdat., <filename>.smpl
        char *smplName = strdup(path);
        // Case-preserving conversion of "mdat" to "smpl"
        smplName[eOffset + 0] ^= 'm' ^ 's';
        smplName[eOffset + 1] ^= 'd' ^ 'm';
        smplName[eOffset + 2] ^= 'a' ^ 'p';
        smplName[eOffset + 3] ^= 't' ^ 'l';

        status = loadMDAT(state, path, smplName);
        free(smplName);
    }
    else if(strncasecmp(filename, "tfmx.", 5) == 0)
    {
        // first check if we have tfmx.<filename>,smpl.<filename> which is the orignal Amiga naming
        char *smplName = strdup(path);
        // Case-preserving conversion of "tfmx" to "smpl"
        smplName[sOffset + 0] ^= 't' ^ 's';
        smplName[sOffset + 1] ^= 'f' ^ 'm';
        smplName[sOffset + 2] ^= 'm' ^ 'p';
        smplName[sOffset + 2] ^= 'x' ^ 'l';

        status = loadMDAT(state, path, smplName);
        free(smplName);
    }
    else if(strncasecmp(extension, ".tfmx", 5) == 0)
    {
        // Check for <filename>.tfmx, <filename>.sam
        char *smplName = strdup(path);
        // Case-preserving conversion of "tfmx" to "smpl"
        smplName[eOffset + 0] ^= 't' ^ 's';
        smplName[eOffset + 1] ^= 'f' ^ 'm';
        smplName[eOffset + 2] ^= 'm' ^ 'p';
        smplName[eOffset + 2] ^= 'x' ^ 'l';

        status = loadMDAT(state, path, smplName);
        free(smplName);
    }
    else if(strncasecmp(extension, ".tfx", 4) == 0)
    {
        if(!status)
        {
            // Check for <filename>.tfx, <filename>.sam
            char *smplName = strdup(path);
            // Case-preserving conversion of "tfx" to "sam"
            smplName[eOffset + 0] ^= 't' ^ 's';
            smplName[eOffset + 1] ^= 'f' ^ 'a';
            smplName[eOffset + 2] ^= 'x' ^ 'm';

            status = loadMDAT(state, path, smplName);
            free(smplName);
        }

        if(!status)
        {
            // Check for <filename>.tfx, <filename>.smp
            char *smplName = strdup(path);
            // Case-preserving conversion of "tfx" to "smp"
            smplName[eOffset + 0] ^= 't' ^ 's';
            smplName[eOffset + 1] ^= 'f' ^ 'm';
            smplName[eOffset + 2] ^= 'x' ^ 'p';

            status = loadMDAT(state, path, smplName);
            free(smplName);
        }
    }
    else if(strncasecmp(extension, ".tfm", 3) == 0)
    {
        // Check for <filename>.tfm which is a single format
        status = loadTFM(state, path);
    }
    return status;
}


TFMXHelper::TFMXHelper(const QString &path)
    : m_path(path)
{
    m_state = (TfmxState*)calloc(sizeof(TfmxState), 1);
}

TFMXHelper::~TFMXHelper()
{
    deinit();
}

void TFMXHelper::deinit()
{
    if(m_state)
    {
        TFMXQuit(m_state);
        free(m_state);
    }
}

bool TFMXHelper::initialize()
{
    const QString &path = cleanPath();

    TfmxState_init(m_state);

    if(!TFMXLoad(m_state, QmmpPrintable(path)))
    {
        qWarning("TFMXHelper: Unable to open file");
        return false;
    }

    const int track = m_path.section("#", -1).toInt();
    const int count = TFMXGetSubSongs(m_state);
    if(track > count || track < 0)
    {
        qWarning("TFMXHelper: track number is out of range");
        return false;
    }

    TFMXSetSubSong(m_state, (track == 0 ? count : track) - 1);
    TFMXRewind(m_state);
    return true;
}

qint64 TFMXHelper::read(unsigned char *data, qint64)
{
    if(TFMXTryToMakeBlock(m_state) < 0)
    {
        return 0;
    }

    TFMXGetBlock(m_state, data);
    return TFMXGetBlockSize(m_state);
}

QList<TrackInfo*> TFMXHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_state)
    {
        return playlist;
    }

    QFileInfo fin(cleanPath());
    QString title = fin.baseName();
    if(!filters().contains("*." + fin.suffix(), Qt::CaseInsensitive))
    {
        title = fin.suffix();
    }

    for(int i = 1; i <= TFMXGetSubSongs(m_state); ++i)
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
            info->setValue(Qmmp::FORMAT_NAME, "Final Music System Tracker");
        }

        TFMXSetSubSong(m_state, i - 1);
        TFMXRewind(m_state);

        info->setPath("tfmx://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}

QString TFMXHelper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}

QStringList TFMXHelper::filters()
{
    const QStringList filters =
    {
        "*.tfm",
        // pair suffix section
        "*.tfmx", // (tfmx, smpl)
        "*.mdat", // (mdat, smpl))
        "*.tfx",  // (tfx, (smp, sam))
        // pair prefix section
        "tfmx.*", // (tfmx, smpl)
        "mdat.*"  // (mdat, smpl)
    };
    return filters;
}
