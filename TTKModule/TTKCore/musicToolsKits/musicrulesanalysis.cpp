#include "musicrulesanalysis.h"

QStringList MusicRulesAnalysis::rules()
{
    static QStringList rules{QObject::tr("Title"), QObject::tr("Artist"), QObject::tr("Album")};
    return rules;
}

QString MusicRulesAnalysis::rule(int index)
{
    switch(index)
    {
        case 0: return "%title";
        case 1: return "%artist";
        case 2: return "%album";
        default: return {};
    }
}

QString MusicRulesAnalysis::parse(const TTK::MusicSongInformation &info, const QString &value)
{
    qDebug() << value;
    return info.m_singerName + " - " + info.m_songName;
}
