#include "asaphelper.h"

AsapHelper::AsapHelper(const QString &path)
    : m_path(path)
{

}

AsapHelper::~AsapHelper()
{
    deinit();
}

void AsapHelper::deinit()
{
    if(m_input)
    {
        ASAP_Delete(m_input);
    }
}

bool AsapHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("AsapHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = ASAP_New();
    ASAP_DetectSilence(m_input, 5);

    if(!ASAP_Load(m_input, QmmpPrintable(m_path), (unsigned char *)buffer.constData(), buffer.length()))
    {
        qWarning("AsapHelper: ASAP_Load error");
        return false;
    }

    /*
    struct ASAPInfo
    {
        int channels;
        int covoxAddr;
        int defaultSong;
        int fastplay;
        int headerLen;
        int init;
        int music;
        cibool ntsc;
        int player;
        int songs;
        ASAPModuleType type;
        unsigned char songPos[32];
        char author[128];
        char date[128];
        int durations[32];
        char filename[128];
        cibool loops[32];
        char title[128];
    };*/
    ASAPInfo *info =(ASAPInfo *)ASAP_GetInfo(m_input);
    if(!ASAP_PlaySong(m_input, ASAPInfo_GetDefaultSong(info), 360000))
    {
        qWarning("AsapHelper: ASAP_PlaySong error");
        return false;
    }

    m_title = ASAPInfo_GetTitle(info);
    m_author = ASAPInfo_GetAuthor(info);
    m_year = QString::number(ASAPInfo_GetYear(info));
    m_length = ASAPInfo_GetDuration(info, ASAPInfo_GetDefaultSong(info));
    m_channels = ASAPInfo_GetChannels(info);
    m_subSongs = ASAPInfo_GetSongs(info);
    m_loop = ASAPInfo_GetLoop(info, ASAPInfo_GetDefaultSong(info));
    return true;
}

qint64 AsapHelper::read(unsigned char *data, qint64 maxSize)
{
    if(ASAP_GetPosition(m_input) >= totalTime())
    {
        return 0;
    }

    return ASAP_Generate(m_input, data, maxSize, ASAPSampleFormat_S16_L_E);
}
