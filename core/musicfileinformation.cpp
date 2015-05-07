#include "musicfileinformation.h"
#include <QDebug>

MusicFileInformation::MusicFileInformation()
{
    m_player = CreateZPlay();
    Q_ASSERT(m_player);
    /// check if we have version 1.90 and above ( 1.90 if PF (pattent free) version,
    ///2.00 version is full version
    if(m_player->GetVersion() < 190)
    {
        qDebug()<<"Error: Need library version 2.00 and above !";
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
        qDebug()<<"Error: "<<m_player->GetError();
        return false;
    }
    m_player->GetStreamInfoW(&m_pInfo);
    if(!m_player->LoadID3ExW(&m_tagInfo, 1))
    {
        qDebug()<<"No ID3 data\r\n\r\n";
    }
    return true;
}

QString MusicFileInformation::getArtist() const
{
    return QString::fromWCharArray(m_tagInfo.Artist);
}

QString MusicFileInformation::getTitle() const
{
    return QString::fromWCharArray(m_tagInfo.Title);
}

QString MusicFileInformation::getAlbum() const
{
    return QString::fromWCharArray(m_tagInfo.Album);
}

QString MusicFileInformation::getComment() const
{
    return QString::fromWCharArray(m_tagInfo.Comment);
}

QString MusicFileInformation::getYear() const
{
    return QString::fromWCharArray(m_tagInfo.Year);
}

QString MusicFileInformation::getTrackNum() const
{
    return QString::fromWCharArray(m_tagInfo.TrackNum);
}

QString MusicFileInformation::getGenre() const
{
    return QString::fromWCharArray(m_tagInfo.Genre);
}

QString MusicFileInformation::getAlbumArtist() const
{
    return QString::fromWCharArray(m_tagInfo.AlbumArtist);
}

QString MusicFileInformation::getComposer() const
{
    return QString::fromWCharArray(m_tagInfo.Composer);
}

QString MusicFileInformation::getOriginalArtist() const
{
    return QString::fromWCharArray(m_tagInfo.OriginalArtist);
}

QString MusicFileInformation::getCopyright() const
{
    return QString::fromWCharArray(m_tagInfo.Copyright);
}

QString MusicFileInformation::getURL() const
{
    return QString::fromWCharArray(m_tagInfo.URL);
}

QString MusicFileInformation::getEncoder() const
{
    return QString::fromWCharArray(m_tagInfo.Encoder);
}

QString MusicFileInformation::getPublisher() const
{
    return QString::fromWCharArray(m_tagInfo.Publisher);
}

uint MusicFileInformation::getBPM() const
{
    return m_tagInfo.BPM;
}

TID3PictureW MusicFileInformation::getPicture() const
{
    return m_tagInfo.Picture;
}


int MusicFileInformation::getSamplingRate() const
{
    return m_pInfo.SamplingRate;
}

int MusicFileInformation::getChannelNumber() const
{
    return m_pInfo.ChannelNumber;
}

int MusicFileInformation::getVBR() const
{
    return m_pInfo.VBR;
}

QString MusicFileInformation::getVBRString() const
{
    return getVBR() ? "VBR" : "CBR";
}

int MusicFileInformation::getBitrate() const
{
    return m_pInfo.Bitrate;
}

TStreamTime MusicFileInformation::getLength() const
{
    return m_pInfo.Length;
}

QString MusicFileInformation::getLengthString() const
{
    return QString("%1:%2:%3")
//           .arg(QString::number(m_pInfo.Length.hms.hour).rightJustified(2,'0'))
           .arg(QString::number(m_pInfo.Length.hms.minute).rightJustified(2,'0'))
           .arg(QString::number(m_pInfo.Length.hms.second).rightJustified(2,'0'))
           .arg(QString::number(m_pInfo.Length.hms.millisecond));
}

QString MusicFileInformation::getDescription() const
{
    return QString::fromWCharArray(m_pInfo.Description);
}
