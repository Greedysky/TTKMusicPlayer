#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

#include <QObject>
#include "../core/musiclibexportglobal.h"
#include "../core/musicobject.h"

class QFile;
class QDomDocument;

class MUSIC_EXPORT MusicUserConfigManager : public QObject
{
public:
    explicit MusicUserConfigManager(QObject *parent = 0);
    ~MusicUserConfigManager();

    //UserXMLConfig
    inline bool readUserXMLConfig(){ return readConfig(USERPATH); }
    void writeUserXMLConfig(const QMap<QString, QStringList> &par);
    void readUserConfig(QMap<QString, QStringList> &);

protected:
    bool readConfig(const QString& type);
    QFile* m_file;
    QDomDocument* m_ddom;

};

#endif // MUSICXMLCONFIGMANAGER_H
