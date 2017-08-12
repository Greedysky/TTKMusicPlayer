#ifndef MUSICBACKGROUNDIMAGE_H
#define MUSICBACKGROUNDIMAGE_H

#include <QPixmap>
#include "musicabstractxml.h"

typedef struct MUSIC_CORE_EXPORT MusicSkinConfigItem
{
    QString m_name;
    int m_useCount;

    MusicSkinConfigItem()
    {
        m_useCount = 0;
    }

    bool isValid() const
    {
        return m_name.isEmpty() && m_useCount == 0;
    }

}MusicSkinConfigItem;
TTK_DECLARE_LISTS(MusicSkinConfigItem)


typedef struct MUSIC_CORE_EXPORT MusicBackgroundImage
{
    QPixmap m_pix;
    MusicSkinConfigItem m_item;

    bool isValid() const
    {
        return m_pix.isNull() && m_item.isValid();
    }

}MusicBackgroundImage;
TTK_DECLARE_LISTS(MusicBackgroundImage)


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

    void writeSkinXMLConfig(const MusicSkinConfigItem &item, const QString &path);
    /*!
     * Write datas into xml file.
     */
    void readSkinXMLConfig(MusicSkinConfigItem &item);
    /*!
     * Read datas into xml file.
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
    static bool outputSkin(MusicBackgroundImage &image, const QString &path);
    /*!
     * Transfer file to image data.
     */
    static bool inputSkin(const MusicBackgroundImage &image, const QString &path);
    /*!
     * Transfer image data to file.
     */

};

#endif // MUSICBACKGROUNDIMAGE_H
