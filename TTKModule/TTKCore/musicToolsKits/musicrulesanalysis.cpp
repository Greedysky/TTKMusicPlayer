#include "musicrulesanalysis.h"
#include "ttktime.h"

static constexpr const char *RULE_TITLE = "%title";
static constexpr const char *RULE_ARTIST = "%artist";
static constexpr const char *RULE_ALBUM = "%album";
static constexpr const char *RULE_DURAION = "%duration";
static constexpr const char *RULE_TRACK = "%track";
static constexpr const char *RULE_YEAR = "%year";

QStringList MusicRulesAnalysis::rules()
{
    static QStringList rules{QObject::tr("Title"), QObject::tr("Artist"), QObject::tr("Album"), QObject::tr("Duration"), QObject::tr("Track"), QObject::tr("Year")};
    return rules;
}

QString MusicRulesAnalysis::rule(int index)
{
    switch(index)
    {
        case 0: return RULE_TITLE;
        case 1: return RULE_ARTIST;
        case 2: return RULE_ALBUM;
        case 3: return RULE_DURAION;
        case 4: return RULE_TRACK;
        case 5: return RULE_YEAR;
        default: return {};
    }
}

QString MusicRulesAnalysis::parse(const TTK::MusicSongInformation &info, const QString &value)
{
    QString v(value);
    v.replace(RULE_TITLE, info.m_songName);
    v.replace(RULE_ARTIST, info.m_artistName);
    v.replace(RULE_ALBUM, info.m_albumName);
    v.replace(RULE_DURAION, QString::number(TTKTime::formatDuration(info.m_duration) / TTK_DN_S2MS));
    v.replace(RULE_TRACK, info.m_trackNumber);
    v.replace(RULE_YEAR, info.m_year);
    return v;
}
