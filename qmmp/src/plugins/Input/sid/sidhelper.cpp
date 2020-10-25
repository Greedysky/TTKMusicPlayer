#include <sidplayfp/SidDatabase.h>
#include "sidhelper.h"

SIDHelper::SIDHelper(SidDatabase *db)
    : m_db(db)
{

}

SIDHelper::~SIDHelper()
{
    if(m_tune)
        delete m_tune;
    m_tune = nullptr;
}

SidTune *SIDHelper::load(const QString &url)
{
    if(m_tune)
    {
        delete m_tune;
        m_tune = nullptr;
    }
    QString path = url;
    int track = 1;
    if(url.contains("://"))
    {
        path.remove("sid://");
#ifdef QMMP_GREATER_NEW
        path.remove(QRegularExpression("#\\d+$"));
#else
        path.remove(QRegExp("#\\d+$"));
#endif
        track = url.section("#", -1).toInt();
    }
    m_tune = new SidTune(qPrintable(path));
    m_tune->selectSong(track - 1);
    m_path = path;
    return m_tune;
}

QList<TrackInfo*> SIDHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> list;
    if(!m_tune || !m_tune->getInfo())
        return list;
    int count = m_tune->getInfo()->songs();

    char md5[SidTune::MD5_LENGTH+1];
    m_tune->createMD5(md5);

    for(int i = 1; i <= count; ++i)
    {
        m_tune->selectSong(i+1);
        TrackInfo *info = new TrackInfo();

        if(parts & TrackInfo::MetaData)
        {
            const SidTuneInfo *tune_info = m_tune->getInfo();
            info->setValue(Qmmp::TITLE, tune_info->infoString(0));
            info->setValue(Qmmp::ARTIST, tune_info->infoString(1));
            info->setValue(Qmmp::COMMENT, tune_info->commentString(0));
            info->setValue(Qmmp::TRACK, i);
        }
        int length = m_db->length(md5, i);
        if(length > -1)
            info->setDuration(length * 1000);

        info->setPath("sid://" + m_path + QString("#%1").arg(i));
        list << info;
    }
    return list;
}
