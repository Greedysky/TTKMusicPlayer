#ifndef MUSICSYSCONFIGMANAGER_H
#define MUSICSYSCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QColor>
#include "musicabstractxml.h"

/*! @brief The class of the sys xml config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSysConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSysConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read config datas from xml file by given name.
     */
    inline bool readXMLConfig() { return readConfig(COFIGPATH_FULL); }
    /*!
     * Write datas into xml file.
     */
    void writeXMLConfig();

    /*!
     * Read System Last Play Index Config.
     */
    void readSystemLastPlayIndexConfig(QStringList &key) const;
    /*!
     * Read window widget Geometry Config.
     */
    QRect readWindowGeometry() const;
    /*!
     * Read Show Desktop Lrc Geometry Config.
     */
    QPoint readShowDLrcGeometry() const;
    /*!
     * Read Sys Load Config.
     */
    void readSysLoadConfig() const;

};

#endif // MUSICSYSCONFIGMANAGER_H
