#include "musicdownloadxminterface.h"
#include "musicnumberutils.h"

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["filePath"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["fileSize"].toInt());
    attr.m_format = key["format"].toString();
    attr.m_bitrate = bitrate;
    info->m_songAttrs.append(attr);
}

int MusicDownLoadXMInterface::map2NormalBitrate(int bitrate)
{
    if(bitrate > 0 && bitrate < 128)
        return MB_32;
    else if(bitrate > 128 && bitrate < 192)
        return MB_128;
    else if(bitrate > 192 && bitrate < 320)
        return MB_320;
    else if(bitrate > 320)
        return MB_500;
    else
        return bitrate;
}
