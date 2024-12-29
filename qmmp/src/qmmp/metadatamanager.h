/***************************************************************************
 *   Copyright (C) 2009-2025 by Ilya Kotov                                 *
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

#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QList>
#include <QStringList>
#include <QPixmap>
#include <QDir>
#include <QMutex>
#include "trackinfo.h"
#include "metadatamodel.h"

class DecoderFactory;
class EngineFactory;
class InputSourceFactory;
class QmmpSettings;


/*! @brief The MetaDataManager class is the base class for metadata access.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT MetaDataManager
{
public:
    /*!
     * Extracts metadata and audio information from file \b path and returns a list of FileInfo items.
     * One file may contain several playlist items (for example: cda disk or flac with embedded cue)
     * @param path Local file path or URL.
     * @param parts parts of metadata which should be extracted from file.
     * @param ignoredPaths Pointer to a list of the files which should be ignored by the recursive search
     * (useful to exclude cue data files from playlist)
     */
    QList<TrackInfo*> createPlayList(const QString &path, TrackInfo::Parts parts = TrackInfo::AllParts, QStringList *ignoredPaths = nullptr) const;
    /*!
     * Creats metadata object, which provides full access to file tags.
     * @param url File path or URL.
     * @param readOnly Open file in read-only mode (\b true - enabled, \b false - disable).
     * @return MetaDataModel pointer or null pointer.
     */
    MetaDataModel* createMetaDataModel(const QString &url, bool readOnly) const;
    /*!
     * Returns a list of file name filters with description, i.e. "MPEG Files (*.mp3 *.mpg)"
     */
    QStringList filters() const;
    /*!
     * Returns a list of file name filters, i.e. "*.mp3 *.mpg".
     */
    QStringList nameFilters() const;
    /*!
     * Returns a list of the suported protocols
     */
    QStringList protocols() const;
    /*!
     * Returns a list of supported regular expressions for URL.
     */
    QList<RegularExpression> regExps() const;
    /*!
     * Returns \b true if \b file is supported and exists, otherwise returns \b false
     */
    bool supports(const QString &file) const;
    /*!
     * Returns the cover pixmap for the given file \b url,
     * or returns an empty pixmap if cover is not available.
     * IMPORTANT: to avoid infinite recursion, do not use this function inside \b MetaDataModel reimplementation.
     */
    QPixmap getCover(const QString &url) const;
    /*!
     * Returns the cover file path for the given file \b url, or returns
     * an empty string if the cover file is not available. This function does not work
     * with embedded covers.
     * IMPORTANT: to avoid infinite recursion, do not use this function inside \b MetaDataModel reimplementation.
     */
    QString getCoverPath(const QString &url) const;
    /*!
     * Returns the cover file path for the given local audio file, or returns
     * an empty string if the cover file is not available.
     * Unlike \b getCover and \b getCoverPath this function provides simple file search (without cache) and
     * should be used inside \b MetaDataModel reimplementation.
     * @param fileName Path of the local audio file.
     */
    QString findCoverFile(const QString &fileName) const;
    /*!
     * Clears cover path cache.
     */
    void clearCoverCache();
    /*!
     * Prepares object for usage by another thread to avoid warnings about parent from the different thread
     */
    void prepareForAnotherThread();
    /*!
     * Returns a pointer to the MetaDataManager instance.
     */
    static MetaDataManager* instance();

private:
    MetaDataManager();
    ~MetaDataManager();

    static void destroy();

    struct CoverCacheItem
    {
        QString url;
        QString coverPath;
        QPixmap coverPixmap;
    };

    QFileInfoList findCoverFiles(QDir dir, int depth) const;
    CoverCacheItem *createCoverCacheItem(const QString &url) const;
    mutable QList<CoverCacheItem *> m_cover_cache;
    QmmpSettings *m_settings = nullptr;
    mutable QMutex m_mutex;

    static MetaDataManager* m_instance;

};

#endif
