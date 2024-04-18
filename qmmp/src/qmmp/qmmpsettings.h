/***************************************************************************
 *   Copyright (C) 2010-2024 by Ilya Kotov                                 *
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

#ifndef QMMPSETTINGS_H
#define QMMPSETTINGS_H

#include <QUrl>
#include <QObject>
#include <QStringList>
#include "qmmp.h"
#include "eqsettings.h"

class QTimer;

/*! @brief The QmmpSettings class provides access to global settings.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT QmmpSettings : public QObject
{
    Q_OBJECT
public:
    /*!
     * This enum describes possible replaygain modes.
     */
    enum ReplayGainMode
    {
        REPLAYGAIN_TRACK = 0, /*!< Use track gain/peak */
        REPLAYGAIN_ALBUM,     /*!< Use album gain/peak */
        REPLAYGAIN_DISABLED   /*!< Disable ReplayGain */
    };

    /*!
     * This enum describes proxy types.
     */
    enum ProxyType
    {
        HTTP_PROXY = 0, /*!< HTTP proxy */
        SOCKS5_PROXY    /*!< SOCKS5 proxy */
    };
    /*!
     * Constructor.
     * @param parent Parent object.
     * This functions is for internal usage only, use QmmpSettings::instance() instead.
     */
    explicit QmmpSettings(QObject *parent = nullptr);
    /*!
     * Destructor.
     */
    virtual ~QmmpSettings();
    /*!
     * Returns current ReplayGain mode.
     */
    QmmpSettings::ReplayGainMode replayGainMode() const;
    /*!
     * Returns preamp in dB.
     */
    double replayGainPreamp() const;
    /*!
     * Returns default gain in dB.
     */
    double replayGainDefaultGain() const;
    /*!
     * Returns \b true if clipping prevention is enabled; otherwise returns \b false.
     */
    bool replayGainPreventClipping() const;
    /*!
     * Sets ReplayGains settings.
     * @param mode ReplayGain mode.
     * @param preamp Preamp in dB.
     * @param default_gain Default gain in dB.
     * @param clip Clipping prevention state.
     */
    void setReplayGainSettings(ReplayGainMode mode, double preamp, double default_gain, bool clip);
    /*!
     * Returns \b true if software volume is enabled; otherwise returns \b false.
     */
    bool useSoftVolume() const;
    /*!
     * Returns preferd output format.
     */
    Qmmp::AudioFormat outputFormat() const;
    /*!
     * Returns \b true if dithering is enabled; otherwise returns \b false.
     */
    bool useDithering() const;
    /*!
     * Sets audio settings.
     * @param soft_volume State of software volume.
     * @param format Preferd output format. Supported values: Qmmp::PCM_S16LE, Qmmp::PCM_S24LE, Qmmp::PCM_S32LE.
     * @param use_dithering Enable/Disable audio dithering.
     */
    void setAudioSettings(bool soft_volume, Qmmp::AudioFormat format, bool use_dithering);
    /*!
     * If \b include is \b true, this function returns include cover file name filters,
     * otherwise returns exclude filters.
     */
    const QStringList &coverNameFilters(bool include = true) const;
    /*!
     * Returns a depth of recursive cover file search.
     */
    int coverSearchDepth() const;
    /*!
     * Returns \b true if cover file search is enabled; otherwise returns \b false.
     */
    bool useCoverFiles() const;
    /*!
     * Sets cover search options.
     * @param inc Include cover name filters
     * @param exc Exclude cover name filters
     * @param depth Depth of recursive cover file search.
     * Recursive cover file search can be disabled by setting \b depth to \b 0.
     * @param use_files Use or not use files with covers.
     * This parameter doesn't take effect in embedded covers.
     */
    void setCoverSettings(QStringList inc, QStringList exc, int depth, bool use_files);
    /*!
     * Returns \b true if global proxy is enabled, otherwise returns \b false
     */
    bool isProxyEnabled() const;
    /*!
     * Returns \b true if global proxy authentication is enabled, otherwise returns \b false
     */
    bool useProxyAuth() const;
    /*!
     * Returns global proxy url.
     */
    const QUrl &proxy() const;
    /*!
     * Returls global proxy type.
     */
    ProxyType proxyType() const;
    /*!
     * Sets network settings.
     * @param use_proxy Enables or disables global proxy.
     * @param auth Enables or disables proxy authentication.
     * @param type Proxy type.
     * @param proxy Proxy url.
     */
    void setNetworkSettings(bool use_proxy, bool auth, ProxyType type, const QUrl &proxy);
    /*!
     * Returns equalizer settings.
     */
    const EqSettings &eqSettings() const;
    /*!
     * Changes equalizer settings to \b settings.
     */
    void setEqSettings(const EqSettings &settings);
    /*!
     * Reads equalizer settings. Call this function before equalizer usage.
     * @param bands Number of bands.
     */
    void readEqSettings(EqSettings::Bands bands = EqSettings::EQ_BANDS_10);
    /*!
     * Returns buffer size in milliseconds.
     */
    int bufferSize() const;
    /*!
     * Sets buffer size.
     * @param msec Buffer size in milliseconds.
     */
    void setBufferSize(int msec);
    /*!
     * Sets volume adjustment step.
     * \param step Volume adjustment step in percent.
     */
    void setVolumeStep(int step);
    /*!
     * Returns volume adjustment step.
     */
    int volumeStep() const;
    /*!
     * Enables/Disables average bitrate displaying.
     * @param enabled State of the average bitrate displaying
     * (\b true - enabled, \b false - disabled). This function emits
     * \b audioSettingsChanged() signal.
     */
    void setAverageBitrate(bool enabled);
    /*!
     * Returns \b true if average bitrate displaying is enabled, otherwise returns \b false
     */
    bool averageBitrate() const;
    /*!
     * Enables/Disables file type determination by content.
     * @param enabled State of the content based type determination.
     * (\b true - enabled, \b false - disabled).
     */
    void setDetermineFileTypeByContent(bool enabled);
    /*!
     * Return \b true if content based file type determination is enabled. Otherwise returns \b false.
     */
    bool determineFileTypeByContent() const;
    /*!
     * Returns a pointer to the QmmpSettings instance.
     */
    static QmmpSettings* instance();

signals:
    /*!
     * Emitted when ReplayGain settings are changed.
     */
    void replayGainSettingsChanged();
    /*!
     * Emitted when audio settings are changed.
     */
    void audioSettingsChanged();
    /*!
     * Emitted when cover settings are changed.
     */
    void coverSettingsChanged();
    /*!
     * Emitted when network settings are changed.
     */
    void networkSettingsChanged();
    /*!
     * Emitted when equalizer settings are changed.
     */
    void eqSettingsChanged();

private slots:
    /*!
     * Writes all unsaved settings to configuration file
     */
    void sync();

private:
    void saveSettings();

    //replaygain settings
    QmmpSettings::ReplayGainMode m_rg_mode;
    double m_rg_preamp;
    double m_rg_defaut_gain;
    bool m_rg_prevent_clipping;
    //audio settings
    bool m_aud_software_volume;
    bool m_aud_dithering;
    Qmmp::AudioFormat m_aud_format;
    int m_volume_step;
    bool m_average_bitrate;
    //cover settings
    QStringList m_cover_inc;
    QStringList m_cover_exclude;
    int m_cover_depth;
    bool m_cover_use_files;
    //network settings
    bool m_proxy_enabled;
    bool m_proxy_auth;
    QUrl m_proxy_url;
    ProxyType m_proxy_type;
    //equalizer settings
    EqSettings m_eq_settings;
    //buffer size
    int m_buffer_size;
    //file type determination
    bool m_determine_by_content;
    //protect from multiple calls
    bool m_saveSettings = false;

    static QmmpSettings* m_instance;

};

#endif
