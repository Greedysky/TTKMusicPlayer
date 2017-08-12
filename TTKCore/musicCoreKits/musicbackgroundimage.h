#ifndef MUSICBACKGROUNDIMAGE_H
#define MUSICBACKGROUNDIMAGE_H

#include <QPixmap>
#include "musicabstractxml.h"

typedef struct MUSIC_CORE_EXPORT MusicBackgroundImage
{
    QPixmap m_pix;
    QString m_name;
    int m_useCount;

    MusicBackgroundImage()
    {
        m_useCount = -1;
    }

    bool isValid() const
    {
        return m_pix.isNull() && m_name.isEmpty() && m_useCount == -1;
    }

}MusicBackgroundImage;
TTK_DECLARE_LISTS(MusicBackgroundImage)

typedef struct MUSIC_CORE_EXPORT MusicSkinConfigItem
{
    QString m_name;
    int m_useCount;

    MusicSkinConfigItem()
    {
        m_useCount = -1;
    }

    bool isValid() const
    {
        return m_name.isEmpty() && m_useCount == -1;
    }

}MusicSkinConfigItem;
TTK_DECLARE_LISTS(MusicSkinConfigItem)


/*! @brief The class of the skin XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSkinConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicSkinConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void writeMusicSongsConfig(const MusicSkinConfigItem &item, const QString &path);
    /*!
     * Write music datas into xml file.
     */
    void readMusicSongsConfig(MusicSkinConfigItem &item);
    /*!
     * Read music datas into xml file.
     */

};


/*! @brief The class of the background image core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicBackgroundImageCore
{
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */

    bool outputSkin(MusicBackgroundImage &image, const QString &path);
    /*!
     * Transfer file to image data.
     */
    bool inputSkin(const MusicBackgroundImage &image, const QString &path);
    /*!
     * Transfer image data to file.
     */

};

#endif // MUSICBACKGROUNDIMAGE_H
