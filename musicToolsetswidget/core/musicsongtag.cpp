#include "musicsongtag.h"

MusicSongTag::MusicSongTag()
{
    m_player = CreateZPlay();
    Q_ASSERT(m_player);
    /// check if we have version 1.90 and above ( 1.90 if PF (pattent free) version,
    ///2.00 version is full version
    if(m_player->GetVersion() < 190)
    {
        M_LOGGERS("Error: Need library version 2.00 and above !");
        m_player->Release();
        Q_ASSERT(false);
    }
}

MusicSongTag::~MusicSongTag()
{
    m_player->Close();
    m_player->Release();
}

bool MusicSongTag::readFile(const QString &file)
{
    if(m_player->OpenFileW(file.toStdWString().c_str(), sfAutodetect) == 0)
    {
        M_LOGGER << "Error: " << m_player->GetError() << LOG_END;
        return false;
    }

    m_player->GetStreamInfoW(&m_pInfo);
    if(!m_player->LoadID3ExW(&m_tagInfo, 1))
    {
        M_LOGGER << "No ID3 data\r\n\r\n" << LOG_END;
    }
    return true;
}
