/***************************************************************************
 *   Copyright (C) 2009-2026 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#include <QStringList>
#include <QList>
#include <QMap>
#include <QIODevice>
#include "trackinfo.h"
#include "abstractengine.h"
#include "metadatamodel.h"

class AbstractEngine;

/*! @brief Structure to store custom audio engine properies.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
struct QMMP_EXPORT EngineProperties
{
    QString name;             /*!< Input plugin full name */
    QString shortName;        /*!< Input plugin short name for internal usage */
    QStringList filters;      /*!< File filters (example: "*.mp3,*.ogg") */
    QString description;      /*!< File filter description */
    QStringList contentTypes; /*!< Supported content types */
    QStringList protocols;    /*!< Supported protocols. Should be empty if plugin uses stream input. */
    bool hasSettings = false; /*!< Should be \b true if plugin has settings dialog, and \b false otherwise */
};

/*! @brief Engine plugin interface.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT EngineFactory
{
public:
    /*!
    * Object destructor.
    */
    virtual ~EngineFactory() = default;

    /*!
     * Returns \b true if plugin supports \b source, otherwise returns \b false
     */
    virtual bool supports(const QString &source) const = 0;
    /*!
     * Returns general plugin properties.
     */
    virtual EngineProperties properties() const = 0;
    /*!
     * Creates engine object.
     * @param parent Parent object File path
     */
    virtual AbstractEngine *create(QObject *parent = nullptr) = 0;
    /*!
     * Extracts metadata and audio information from file \b path and returns a list of FileInfo items.
     * One file may contain several playlist items (for example: cda disk or flac with embedded cue).
     * @param fileName File path.
     * @param ignoredPaths Pointer to a list of the files which should be ignored by the recursive search.
     * @param parts parts of metadata which should be extracted from file.
     * (useful to exclude cue data files from playlist).
     */
    virtual QList<TrackInfo*> createPlayList(const QString &fileName, TrackInfo::Parts parts, QStringList *ignoredPaths) = 0;
    /*!
     * Creats metadata object, which provides full access to file tags.
     * @param path File path.
     * @param readOnly Open file in read-only mode (\b true - enabled, \b false - disable).
     * @return MetaDataModel pointer.
     */
    virtual MetaDataModel* createMetaDataModel(const QString &path, bool readOnly) = 0;

};

Q_DECLARE_INTERFACE(EngineFactory, "EngineFactory/1.0")

#endif
