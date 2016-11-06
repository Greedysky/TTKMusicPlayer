// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//

#ifndef DECODER_H
#define DECODER_H

#include <QStringList>
#include <QList>
#include <QMap>
#include <QIODevice>
#include "fileinfo.h"
#include "qmmp.h"
#include "audioparameters.h"
#include "channelmap.h"
#include "decoderfactory.h"

class QmmpPluginCache;

/*! @brief The Decoder class provides the base interface class of audio decoders.
 * @author Brad Hughes <bhughes@trolltech.com>
 * @author Ilya Kotov <forkotov@hotmail.ru>
 */
class Q_DECL_EXPORT Decoder
{
public:
    /*!
     * Object contsructor.
     * @param input QIODevice-based input source.
     */
    Decoder(QIODevice *input = 0);
    /*!
     * Destructor.
     */
    virtual ~Decoder();
    /*!
     * Prepares decoder for usage.
     * Subclass should reimplement this function.
     */
    virtual bool initialize() = 0;
    /*!
     * Returns the total time in milliseconds.
     * Subclass should reimplement this function.
     */
    virtual qint64 totalTime() = 0;
    /*!
     * Requests a seek to the time \b time indicated, specified in milliseconds.
     * Subclass should reimplement this function.
     */
    virtual void seek(qint64 time) = 0;
    /*!
     * Reads up to \b maxSize bytes of decoded audio to \b data
     * Returns the number of bytes read, or -1 if an error occurred.
     * Subclass should reimplement this function.
     */
    virtual qint64 read(unsigned char *data, qint64 maxSize) = 0;
    /*!
     * Returns current bitrate (in kbps).
     * Subclass should reimplement this function.
     */
    virtual int bitrate() = 0;
    /*!
     * Tells decoder that it should play next track.
     * By default this function does nothing.
     * Reimplemet it if your decoder can play next track without stop/start cycle.
     * This may be useful for multitrack formats like cue or cda.
     */
    virtual void next();
    /*!
     * Returns url which decoder can play without stop/start cycle.
     * By default this function does nothing.
     * Reimplemet it if your decoder can play next track without stop/start cycle.
     */
    virtual const QString nextURL() const;
    /*!
     * Returns detected audio parameters.
     */
    AudioParameters audioParameters() const;
    /*!
     * Returns ReplayGain information.
     */
    QMap<Qmmp::ReplayGainKey, double> replayGainInfo() const;
    /*!
     * Sets ReplayGain information. Use this function before playback.
     * @param rg ReplayGain information
     */
    void setReplayGainInfo(const QMap<Qmmp::ReplayGainKey,double> &rg);
    /*!
     * Returns QIODevice-based input source assigned for this decoder.
     */
    QIODevice *input();
    /*!
     * Informs decoder about new received metadata.
     * Call of this function is required for all non-local streams/files
     * @param metaData Metadata map.
     */
    void addMetaData(const QMap<Qmmp::MetaData, QString> &metaData);
    /*!
     * Returns \b true when new metadata has received, otherwise returns \b false.
     */
    bool hasMetaData() const;
    /*!
     * Takes metadata out of decoder and returns it.
     * Attention: hasMetaData() should return \b true before use of this fuction.
     */
    QMap<Qmmp::MetaData, QString> takeMetaData();
    /*!
     * Returns DecoderFactory pointer which supports file \b path or 0 if file \b path is unsupported
     * @param path Full local file path.
     * @param useContent Content-based file type determination (\b true - enabled, \b false - disabled)
     */
    static DecoderFactory *findByPath(const QString &path, bool useContent = false);
    /*!
     * Returns DecoderFactory pointer which supports mime type \b mime or \b 0 if mime type \b mime is unsupported
     */
    static DecoderFactory *findByMime(const QString &mime);
    /*!
     * Returns DecoderFactory pointer which supports data provided by \b input
     * or \b 0 if data is unsupported.
     */
    static DecoderFactory *findByContent(QIODevice *input);
    /*!
     * Returns DecoderFactory pointer which supports protocol \b p or \b 0 if \b url is not supported.
     */
    static DecoderFactory *findByProtocol(const QString &p);
    /*!
     * Returns a list of decoder factories.
     */
    static QList<DecoderFactory*> factories();
    /*!
     * Returns a list of enabled decoder factories.
     */
    static QList<DecoderFactory*> enabledFactories();
    /*!
     * Returns plugin file path.
     * @param factory Decoder plugin factory.
     */
    static QString file(DecoderFactory *factory);
    /*!
     * Returns a list of supported protocols (including meta-protocols).
     * This fuction ignores disabled decoders.
     */
    static QStringList protocols();
    /*!
     * Sets whether the input plugin is enabled.
     * @param factory Decoder plugin factory.
     * @param enable Plugin enable state (\b true - enable, \b false - disable)
     */
    static void setEnabled(DecoderFactory* factory, bool enable = true);
    /*!
     * Returns \b true if input plugin is enabled, otherwise returns \b false
     * @param factory Decoder plugin factory.
     */
    static bool isEnabled(DecoderFactory* factory);

protected:
    /*!
     * Use this function inside initialize() reimplementation to tell other plugins about audio parameters.
     * @param srate Sample rate.
     * @param map Map of channels.
     * @param f Audio format.
     */
    void configure(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat f = Qmmp::PCM_S16LE);
    /*!
     * Use this function inside initialize() reimplementation to tell other plugins about audio parameters.
     * @param srate Sample rate.
     * @param channels Number of channels (internal channel sequence).
     * @param f Audio format.
     */
    void configure(quint32 srate, int channels, Qmmp::AudioFormat f = Qmmp::PCM_S16LE);

private:
    static void loadPlugins();
    static DecoderFactory *m_lastFactory;
    static QList<QmmpPluginCache*> *m_cache;
    static QStringList m_disabledNames;
    AudioParameters m_parameters;
    QIODevice *m_input;
    bool m_hasMetaData;
    QMap<Qmmp::MetaData, QString> m_metaData;
    QMap <Qmmp::ReplayGainKey, double> m_rg; //replay gain information
};

#endif // DECODER_H
