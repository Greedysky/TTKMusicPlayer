#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

#include "musicabstractxml.h"

class MUSIC_CORE_EXPORT MusicUserConfigManager : public MusicAbstractXml
{
public:
    explicit MusicUserConfigManager(QObject *parent = 0);

    //UserXMLConfig
    inline bool readUserXMLConfig(){ return readConfig(USERPATH); }
    void writeUserXMLConfig(const MStringsListMap &par);
    void readUserConfig(MStringsListMap &name);

};

#endif // MUSICXMLCONFIGMANAGER_H
