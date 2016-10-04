#ifndef TTKMUSICCONFIGMANAGER_H
#define TTKMUSICCONFIGMANAGER_H

#include "musicabstractxml.h"

class TTKMusicConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit TTKMusicConfigManager(QObject *parent = 0);

    inline bool readXMLConfig(const QString &path) { return readConfig(path); }
    /*!
     * Read config datas from xml file by given name.
     */
    inline bool readMusicXMLConfig(const QString &path){ return readConfig(path); }
    /*!
     * Read music datas from xml file by given name.
     */
    void writeXMLConfig();
    /*!
     * Write datas into xml file.
     */
    void writeMusicSongsConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Write music datas into xml file.
     */

    void readMusicSongsConfig(MusicSongItems &musics);
    /*!
     * Read music datas into xml file.
     */
    inline int readMusicPlayModeConfig() const
    { return readXmlAttributeByTagNameValue("playMode").toInt(); }
    /*!
     * Read Music Play Mode Config.
     */

    void readSystemLastPlayIndexConfig(QStringList &key) const;
   /*!
    * Read System Last Play Index Config.
    */

protected:
    MusicSongs readMusicFilePath(const QDomNode &node) const;
    /*!
     * Read Music File Path.
     */

};

#endif // TTKMUSICCONFIGMANAGER_H
