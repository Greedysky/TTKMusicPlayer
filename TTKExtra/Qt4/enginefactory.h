/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef EMGINEFACTORY_H
#define EMGINEFACTORY_H

class QObject;
class QString;
class QIODevice;
class QWidget;
class QTranslator;
class QStringList;
class MetaDataModel;
class FileInfo;
class AbstractEngine;

/*! @brief Helper class to store custom audio engine properies.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class EngineProperties
{
public:
    /*!
     * Constructor
     */
    EngineProperties()
    {
        hasAbout = false;
        hasSettings = false;
    }
    QString name;          /*!< Input plugin full name */
    QString shortName;     /*!< Input plugin short name for internal usage */
    QStringList filters;   /*!< File filters (example: "*.mp3,*.ogg") */
    QString description;   /*!< File filter description */
    QStringList contentTypes; /*!< Supported content types */
    QStringList protocols; /*!< Supported protocols. Should be empty if plugin uses stream input. */
    bool hasAbout;         /*!< Should be \b true if plugin has about dialog, otherwise returns \b false */
    bool hasSettings;   /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
};
/*! @brief Engine plugin interface.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class EngineFactory
{
public:
    /*!
    * Object destructor.
    */
    virtual ~EngineFactory() {}
    /*!
     * Returns \b true if plugin supports \b source, otherwise returns \b false
     */
    virtual bool supports(const QString &source) const = 0;
    /*!
     * Returns general plugin properties.
     */
    virtual const EngineProperties properties() const = 0;
    /*!
     * Creates engine object.
     * @param parent Parent object File path
     */
    virtual AbstractEngine *create(QObject *parent = 0) = 0;
    /*!
     * Extracts metadata and audio information from file \b path and returns a list of FileInfo items.
     * One file may contain several playlist items (for example: cda disk or flac with embedded cue)
     * @param fileName File path.
     * @param useMetaData Metadata usage (\b true - use, \b - do not use)
     * @param ignoredPaths Pointer to a list of the files which should be ignored by the recursive search
     * (useful to exclude cue data files from playlist)
     */
    virtual QList<FileInfo *> createPlayList(const QString &fileName, bool useMetaData, QStringList *ignoredPaths) = 0;
    /*!
     * Creats metadata object, which provides full access to file tags.
     * @param path File path.
     * @param parent Parent object.
     * @return MetaDataModel pointer.
     */
    virtual MetaDataModel* createMetaDataModel(const QString &path, QObject *parent = 0) = 0;

};

Q_DECLARE_INTERFACE(EngineFactory, "EngineFactory/1.0")

#endif
