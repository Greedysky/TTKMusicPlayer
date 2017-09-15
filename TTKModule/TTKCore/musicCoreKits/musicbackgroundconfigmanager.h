#ifndef MUSICBACKGROUNDCONFIGMANAGER_H
#define MUSICBACKGROUNDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QPixmap>
#include "musicabstractxml.h"

/*! @brief The class of the skin config item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicSkinConfigItem
{
    QString m_name;
    int m_useCount;

    MusicSkinConfigItem()
    {
        m_name = "Greedysky";
        m_useCount = 8888;
    }

    bool isValid() const
    {
        return !(m_name.isEmpty() && m_useCount == 0);
    }

}MusicSkinConfigItem;
MUSIC_DECLARE_LISTS(MusicSkinConfigItem)

/*! @brief The class of the skin backgroud image item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicBackgroundImage
{
    QPixmap m_pix;
    MusicSkinConfigItem m_item;

    bool isValid() const
    {
        return !m_pix.isNull() && m_item.isValid();
    }

}MusicBackgroundImage;
MUSIC_DECLARE_LISTS(MusicBackgroundImage)


/*! @brief The class of the skin XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSkinConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSkinConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Write datas into xml file.
     */
    void writeSkinXMLConfig(const MusicSkinConfigItem &item, const QString &path);
    /*!
     * Read datas into xml file.
     */
    void readSkinXMLConfig(MusicSkinConfigItem &item);

};

#endif // MUSICBACKGROUNDCONFIGMANAGER_H
