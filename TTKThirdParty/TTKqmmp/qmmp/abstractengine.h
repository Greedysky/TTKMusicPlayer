/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
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

#ifndef ABSTRACTENGINE_H
#define ABSTRACTENGINE_H

#include <QMutex>
#include <QThread>
#include <QStringList>
#include "enginefactory.h"
#include "qmmpsettings.h"

class QIODevice;
class InputSource;
class QmmpPluginCache;
class EngineFactory;

/*! @brief The AbstractEngine class provides the base interface class of audio audio engines.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT AbstractEngine : public QThread
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     * @param parent Parent object.
     */
    explicit AbstractEngine(QObject *parent = nullptr);
    /*!
     * Destructor.
     */
    virtual ~AbstractEngine();
    /*!
     * Prepares decoder for usage.
     * Subclass should reimplement this function.
     */
    virtual bool enqueue(InputSource *source) = 0;
    /*!
     * Starts playback. Returns \b true if playback has been started successful,
     * otherwise returns \b false.
     */
    virtual bool play() = 0;
    /*!
     * Requests a seek to the time \b time indicated, specified in milliseconds.
     */
    virtual void seek(qint64 time) = 0;
    /*!
     * Requests playback to stop
     */
    virtual void stop() = 0;
    /*!
     *  Pauses/resumes playback
     */
    virtual void pause() = 0;
    /*!
     * Creates Engine object.
     * @param s InputSource object.
     * @param parent Parent object.
     * Returns \b 0 if the given source is not supported.
     */
    static AbstractEngine *create(InputSource *s, QObject *parent = nullptr);
    /*!
     * Returns a list of engine factories.
     */
    static QList<EngineFactory*> factories();
    /*!
     * Returns a list of enabled engine factories.
     */
    static QList<EngineFactory*> enabledFactories();
    /*!
     * Returns a list of file name filters, i.e. "*.mp3 *.mpg".
     */
    static QStringList nameFilters();
    /*!
     * Returns a list of the supported mime types.
     */
    static QStringList contentTypes();
    /*!
     * Returns EngineFactory pointer which supports file \b path or 0 if file \b path is unsupported.
     */
    static EngineFactory *findByFilePath(const QString &path);
    /*!
     * Sets whether the engine is enabled.
     * @param factory Engine plugin factory.
     * @param enable Plugin enable state (\b true - enable, \b false - disable)
     */
    static void setEnabled(EngineFactory *factory, bool enable = true);
    /*!
     * Returns \b true if engine is enabled, otherwise returns \b false
     * This function uses \b QObject::objectName() function.
     * @param factory Engine plugin factory.
     */
    static bool isEnabled(const EngineFactory *factory);
    /*!
     * Returns \b true if engine is enabled, otherwise returns \b false
     * @param engine Engine object.
     */
    static bool isEnabled(const AbstractEngine *engine);
    /*!
     * Returns plugin file path.
     * @param factory Engine plugin factory.
     */
    static QString file(const EngineFactory *factory);
    /*!
     * Returns a list of supported protocols (including meta-protocols).
     * This fuction ignores disabled engines.
     */
    static QStringList protocols();

protected:
    /*!
     * Returns mutex pointer.
     */
    QMutex *mutex();

private:
    QMutex m_mutex;
    static void loadPlugins();
    static QList<QmmpPluginCache*> *m_cache;
    static QStringList m_disabledNames;

};


#endif
