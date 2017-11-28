/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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

#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include <QMutexLocker>
#include "decoder.h"
#include "decoderfactory.h"
#include "abstractengine.h"
#include "inputsource.h"
#include "qmmpsettings.h"
#include "metadatamanager.h"

#define COVER_CACHE_SIZE 10

MetaDataManager* MetaDataManager::m_instance = 0;

MetaDataManager::MetaDataManager() : m_mutex(QMutex::Recursive)
{
    if(m_instance)
        qFatal("MetaDataManager is already created");
    m_instance = this;
    m_settings = QmmpSettings::instance();
}

MetaDataManager::~MetaDataManager()
{
    clearCoverCache();
    m_instance = 0;
}

QList <FileInfo *> MetaDataManager::createPlayList(const QString &fileName, bool useMetaData, QStringList *ignoredPaths) const
{
    QList <FileInfo *> list;
    DecoderFactory *fact = 0;
    EngineFactory *efact = 0;
    QStringList dummyList;
    if(!ignoredPaths)
        ignoredPaths = &dummyList;

    if (!fileName.contains("://")) //local file
    {
        if(!QFile::exists(fileName))
            return list;
        else if((fact = Decoder::findByFilePath(fileName, m_settings->determineFileTypeByContent())))
            return fact->createPlayList(fileName, useMetaData, ignoredPaths);
        else if((efact = AbstractEngine::findByFilePath(fileName)))
            return efact->createPlayList(fileName, useMetaData, ignoredPaths);
        return list;
    }
    else
    {
        QString scheme = fileName.section("://",0,0);
        if(InputSource::protocols().contains(scheme))
        {
            list << new FileInfo(fileName);
            return list;
        }
        foreach(fact, Decoder::factories())
        {
            if(fact->properties().protocols.contains(scheme) && Decoder::isEnabled(fact))
                return fact->createPlayList(fileName, useMetaData, ignoredPaths);
        }
    }
    return list;
}

MetaDataModel* MetaDataManager::createMetaDataModel(const QString &path, QObject *parent) const
{
    DecoderFactory *fact = 0;
    EngineFactory *efact = 0;

    if (!path.contains("://")) //local file
    {
        if(!QFile::exists(path))
            return 0;
        else if((fact = Decoder::findByFilePath(path, m_settings->determineFileTypeByContent())))
            return fact->createMetaDataModel(path, parent);
        else if((efact = AbstractEngine::findByFilePath(path)))
            return efact->createMetaDataModel(path, parent);
        return 0;
    }
    else
    {
        QString scheme = path.section("://",0,0);
        MetaDataModel *model = 0;
        if((fact = Decoder::findByProtocol(scheme)))
        {
            return fact->createMetaDataModel(path, parent);
        }
        foreach(efact, AbstractEngine::enabledFactories())
        {
            if(efact->properties().protocols.contains(scheme))
                model = efact->createMetaDataModel(path, parent);
            if(model)
                return model;
        }
    }
    return 0;
}

QStringList MetaDataManager::filters() const
{
    QStringList filters;
    foreach(DecoderFactory *fact, Decoder::enabledFactories())
    {
        if (!fact->properties().filters.isEmpty())
            filters << fact->properties().description + " (" + fact->properties().filters.join(" ") + ")";
    }
    foreach(EngineFactory *fact, AbstractEngine::enabledFactories())
    {
        if (!fact->properties().filters.isEmpty())
            filters << fact->properties().description + " (" + fact->properties().filters.join(" ") + ")";
    }
    return filters;
}

QStringList MetaDataManager::nameFilters() const
{
    QStringList filters;
    foreach(DecoderFactory *fact, Decoder::enabledFactories())
    {
        if (Decoder::isEnabled(fact))
            filters << fact->properties().filters;
    }
    foreach(EngineFactory *fact, AbstractEngine::enabledFactories())
    {
        if (AbstractEngine::isEnabled(fact))
            filters << fact->properties().filters;
    }
    if(m_settings->determineFileTypeByContent())
        filters << "*";
    filters.removeDuplicates();
    return filters;
}

QStringList MetaDataManager::protocols() const
{
    QStringList p;
    p << InputSource::protocols();
    p << Decoder::protocols();
    p << AbstractEngine::protocols();
    p.removeDuplicates();
    return p;
}

bool MetaDataManager::supports(const QString &fileName) const
{
    DecoderFactory *fact = 0;
    EngineFactory *efact = 0;
    if (!fileName.contains("://")) //local file
    {
        if (!QFile::exists(fileName))
            return false;
        if((fact = Decoder::findByFilePath(fileName)))
            return true;
        else if((efact = AbstractEngine::findByFilePath(fileName)))
            return true;
        return false;
    }
    return false;
}

QPixmap MetaDataManager::getCover(const QString &url) const
{
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_cover_cache.size(); ++i)
    {
        if(m_cover_cache[i]->url == url)
            return m_cover_cache[i]->coverPixmap;
    }

    m_cover_cache << createCoverCacheItem(url);

    while(m_cover_cache.size() > COVER_CACHE_SIZE)
        delete m_cover_cache.takeFirst();

    return m_cover_cache.last()->coverPixmap;
}

QString MetaDataManager::getCoverPath(const QString &url) const
{
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_cover_cache.size(); ++i)
    {
        if(m_cover_cache[i]->url == url)
            return m_cover_cache[i]->coverPath;
    }

    m_cover_cache << createCoverCacheItem(url);

    while(m_cover_cache.size() > COVER_CACHE_SIZE)
        delete m_cover_cache.takeFirst();

    return m_cover_cache.last()->coverPath;
}

QString MetaDataManager::findCoverFile(const QString &fileName) const
{
    if(!m_settings->useCoverFiles())
        return QString();

    if(!QFile::exists(fileName))
    {
        return QString();
    }

    QFileInfoList l = findCoverFiles(QFileInfo(fileName).absoluteDir(), m_settings->coverSearchDepth());
    return l.isEmpty() ? QString() : l.at(0).filePath();
}

QFileInfoList MetaDataManager::findCoverFiles(QDir dir, int depth) const
{
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    QFileInfoList file_list = dir.entryInfoList(m_settings->coverNameFilters());
    foreach(QFileInfo i, file_list)
    {
        foreach(QString pattern, m_settings->coverNameFilters(false))
        {
            if(QRegExp (pattern, Qt::CaseInsensitive, QRegExp::Wildcard).exactMatch(i.fileName()))
            {
                file_list.removeAll(i);
                break;
            }
        }
    }
    if(!depth || !file_list.isEmpty())
        return file_list;
    depth--;
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    QFileInfoList dir_info_list = dir.entryInfoList();
    foreach(QFileInfo i, dir_info_list)
    {
        file_list << findCoverFiles(QDir(i.absoluteFilePath()), depth);
    }
    return file_list;
}

MetaDataManager::CoverCacheItem *MetaDataManager::createCoverCacheItem(const QString &url) const
{
    CoverCacheItem *item = new CoverCacheItem;
    item->url = url;
    MetaDataModel *model = createMetaDataModel(url);
    if(model)
    {
        item->coverPath = model->coverPath();
        item->coverPixmap = model->cover();
        delete model;
    }

    if(!m_settings->useCoverFiles())
        return item;

    if(!url.contains("://") && item->coverPath.isEmpty())
        item->coverPath = findCoverFile(url);

    if(!item->coverPath.isEmpty() && item->coverPixmap.isNull())
        item->coverPixmap = QPixmap(item->coverPath);

    if(item->coverPixmap.width() > 1024 || item->coverPixmap.height() > 1024)
        item->coverPixmap = item->coverPixmap.scaled(1024, 1024, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return item;
}

void MetaDataManager::clearCoverCache()
{
    qDeleteAll(m_cover_cache);
    m_cover_cache.clear();
}

void MetaDataManager::prepareForAnotherThread()
{
    protocols(); //this hack should load all required plugins
}

MetaDataManager *MetaDataManager::instance()
{
    if(!m_instance)
        new MetaDataManager();
    return m_instance;
}

void MetaDataManager::destroy()
{
    if(m_instance)
        delete m_instance;
}
