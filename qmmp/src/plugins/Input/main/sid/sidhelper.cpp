#include "sidhelper.h"

#include <QFileInfo>
#include <QSettings>

SIDHelper::SIDHelper(SidDatabase *db)
    : m_db(db)
{

}

SIDHelper::~SIDHelper()
{
    delete m_tune;
    m_tune = nullptr;
}

SidTune *SIDHelper::load(const QString &path)
{
    if(m_tune)
    {
        delete m_tune;
        m_tune = nullptr;
    }

    int track = 1;
    m_path = path.contains("://") ? TrackInfo::pathFromUrl(path, &track) : path;

    m_tune = new SidTune(QmmpPrintable(m_path));
    m_tune->selectSong(track - 1);
    return m_tune;
}

TrackInfoList SIDHelper::createPlayList(TrackInfo::Parts parts)
{
    TrackInfoList playlist;
    if(!m_tune || !m_tune->getInfo())
    {
        return playlist;
    }

    const int count = m_tune->getInfo()->songs();
    char md5[SidTune::MD5_LENGTH + 1];
    m_tune->createMD5(md5);

    for(int i = 1; i <= count; ++i)
    {
        m_tune->selectSong(i + 1);
        TrackInfo raw, *info = &raw;

        bool hasMetaData = false;
        if(parts & TrackInfo::MetaData)
        {
            hasMetaData = true;
            const SidTuneInfo *tag = m_tune->getInfo();
            info->setValue(Qmmp::TITLE, tag->infoString(0));
            info->setValue(Qmmp::ARTIST, tag->infoString(1));
            info->setValue(Qmmp::COMMENT, tag->commentString(0));
            info->setValue(Qmmp::FORMAT_NAME, QString::fromLatin1(tag->formatString()));
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
            settings.beginGroup("SID");
            const int sample_rate = settings.value("sample_rate", 48000).toInt();
            settings.endGroup();

            info->setValue(Qmmp::BITRATE, 8);
            info->setValue(Qmmp::SAMPLERATE, sample_rate);
            info->setValue(Qmmp::CHANNELS, 2);
            info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
            if(!hasMetaData)
            {
                info->setValue(Qmmp::FORMAT_NAME, "SID");
            }

            int length = m_db->length(md5, i) * 1000;
            if(length <= 0)
            {
                length = QFileInfo(m_path).size() * 8.0 / 8;
            }
            info->setDuration(length);
        }

        info->setPath("sid://" + m_path + QString("#%1").arg(i));
        playlist << raw;
    }

    return playlist;
}
