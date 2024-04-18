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

#ifndef INPUTSOURCE_H
#define INPUTSOURCE_H

#include <QIODevice>
#include <QMap>
#include <QHash>
#include "qmmp.h"
#include "inputsourcefactory.h"

class QmmpPluginCache;

/*! @brief The InputSource class provides the base interface class of transports.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT InputSource : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     * @param path Input source path or url.
     * @param parent Parent object.
     */
    explicit InputSource(const QString &path, QObject *parent = nullptr);
    /*!
     * Returns QIODevice-based object for I/O operations.
     * Subclass shoud reimplement this function.
     */
    virtual QIODevice *ioDevice() const = 0;
    /*!
     * Prepares input data source for usage.
     * Subclass shoud reimplement this function.
     */
    virtual bool initialize() = 0;
    /*!
     * Returns \b true if transport is ready for usage; otherwise returns \b false.
     */
    virtual bool isReady() const = 0;
    /*!
     * Returns \b true if the transport is waiting more data; otherwise returns \b false.
     * Reader should wait until this function returns \b false.
     * Default implementation allways returns \b false.
     */
    virtual bool isWaiting() const;
    /*!
     * Returns content type of the input stream. Default implementation returns empty string.
     */
    virtual QString contentType() const;
    /*!
     * Stops long operation (reading, seeking) from another thead, if necessary. Default implementation
     * does nothing.
     */
    virtual void stop();
    /*!
     * Returns input source path or url.
     */
    const QString path() const;
    /*!
     * Returns start position in ms;
     */
    qint64 offset() const;
    /*!
     * Sets start position to \b offset ms.
     */
    void setOffset(qint64 offset);
    /*!
     * Informs input source object about new received metadata.
     * Call of this function is required for all non-local streams/files
     * @param metaData Metadata map.
     */
    void addMetaData(const QMap<Qmmp::MetaData, QString> &metaData);
    /*!
     * Returns \b true when new metadata has received, otherwise returns \b false.
     */
    bool hasMetaData() const;
    /*!
     * Takes metadata out of InputSource object and returns it.
     * Attention: hasMetaData() should return \b true before use of this fuction.
     */
    QMap<Qmmp::MetaData, QString> takeMetaData();
    /*!
     * Sets extra stream property.
     * \param key Property key.
     * \param value Property value.
     */
    void setProperty(Qmmp::TrackProperty key, const QVariant &value);
    /*!
     * Updates all extra stream properties.
     * \param properties New track properties.
     */
    void setProperties(const QMap<Qmmp::TrackProperty, QString> &properties);
    /*!
     * Returns stream properties
     */
    const QMap<Qmmp::TrackProperty, QString> &properties() const;
    /*!
     * Informs input source object about received stream information (for example icy data).
     * Call of this function is required for all non-local streams/files
     * @param info Stream information map.
     */
    void addStreamInfo(const QHash<QString, QString> &info);
    /*!
     * Returns \b true when stream information has received, otherwise returns \b false.
     */
    bool hasStreamInfo() const;
    /*!
     * Takes stream information out of InputSource object and returns it.
     * Attention: hasStreamInfo() should return \b true before use of this fuction.
     */
    QHash<QString, QString> takeStreamInfo();
    /*!
     * Creates InputSource object.
     * @param path Input source path or url.
     * @param parent Parent object.
     * Returns \b 0 if the given url is not supported.
     */
    static InputSource *create(const QString &path, QObject *parent = nullptr);
    /*!
     * Returns a list of transport factories.
     */
    static QList<InputSourceFactory *> factories();
    /*!
     * Returns a list of enabled transport factories.
     */
    static QList<InputSourceFactory *> enabledFactories();
    /*!
     * Returns plugin file path.
     * @param factory Transport plugin factory.
     */
    static QString file(const InputSourceFactory *factory);
    /*!
     * Returns a list of supported protocols.
     */
    static QStringList protocols();
    /*!
     * Returns a list of supported regular expressions for URL.
     */
    static QList<RegularExpression> regExps();
    /*!
     * Returns InputSourceFactory pointer which supports URL \b url or \b nullptr if \b URL is not supported.
     */
    static InputSourceFactory *findByUrl(const QString &url);
    /*!
     * Sets whether the input plugin is enabled.
     * @param factory Transport plugin factory.
     * @param enable Plugin enable state (\b true - enable, \b false - disable)
     */
    static void setEnabled(InputSourceFactory *factory, bool enable = true);
    /*!
     * Returns \b true if input plugin is enabled, otherwise returns \b false
     * @param factory Decoder plugin factory.
     */
    static bool isEnabled(const InputSourceFactory* factory);

signals:
    /*!
     * This signal is emitted when transport is ready for usage.
     */
    void ready();
    /*!
     * This signal is emitted after an error occurred.
     */
    void error();

private:
    QString m_path;
    qint64 m_offset = -1;
    QMap<Qmmp::MetaData, QString> m_metaData;
    QMap<Qmmp::TrackProperty, QString> m_properties;
    QHash<QString, QString> m_streamInfo;
    bool m_hasMetaData = false, m_hasStreamInfo = false;

    static void loadPlugins();
    static QList<QmmpPluginCache*> *m_cache;
    static QStringList m_disabledNames;

};

#endif
