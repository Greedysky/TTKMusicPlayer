#include "musicfileinformation.h"

MusicFileInformation::MusicFileInformation()
{
    m_player = CreateZPlay();
    Q_ASSERT(m_player);
    /// check if we have version 1.90 and above ( 1.90 if PF (pattent free) version,
    ///2.00 version is full version
    if(m_player->GetVersion() < 190)
    {
        M_LOOGER << "Error: Need library version 2.00 and above !";
        m_player->Release();
        Q_ASSERT(false);
    }
}

MusicFileInformation::~MusicFileInformation()
{
    m_player->Close();
    m_player->Release();
}

bool MusicFileInformation::readFile(const QString &file)
{
    if(m_player->OpenFileW(file.toStdWString().c_str(), sfAutodetect) == 0)
    {
        M_LOOGER << "Error: " << m_player->GetError();
        return false;
    }
    m_player->GetStreamInfoW(&m_pInfo);
    if(!m_player->LoadID3ExW(&m_tagInfo, 1))
    {
        M_LOOGER << "No ID3 data\r\n\r\n";
    }
    return true;
}
