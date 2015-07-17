#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

#include "../core/musicxmlabstract.h"

class MUSIC_EXPORT MusicUserConfigManager : public MusicXmlAbstract
{
public:
    explicit MusicUserConfigManager(QObject *parent = 0);

    //UserXMLConfig
    inline bool readUserXMLConfig(){ return readConfig(USERPATH); }
    void writeUserXMLConfig(const MStringsListMap &par);
    void readUserConfig(MStringsListMap &);

};

#endif // MUSICXMLCONFIGMANAGER_H
