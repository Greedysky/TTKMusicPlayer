#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

#include "../core/musicxmlabstract.h"

class MUSIC_EXPORT MusicUserConfigManager : public MusicXmlAbstract
{
public:
    explicit MusicUserConfigManager(QObject *parent = 0);

    //UserXMLConfig
    inline bool readUserXMLConfig(){ return readConfig(USERPATH); }
    void writeUserXMLConfig(const QMap<QString, QStringList> &par);
    void readUserConfig(QMap<QString, QStringList> &);

};

#endif // MUSICXMLCONFIGMANAGER_H
