#include <QSettings>
#include <QApplication>
#include "metadatamanager.h"
#include "qmmpsettings.h"

QmmpSettings *QmmpSettings::m_instance = nullptr;

QmmpSettings::QmmpSettings(QObject *parent)
    : QObject(parent)
{
    if(m_instance)
        qFatal("QmmpSettings: only one instance is allowed");
    m_instance = this;

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    //replaygain settings
    settings.beginGroup("ReplayGain");
    m_rg_mode = (ReplayGainMode) settings.value("mode", REPLAYGAIN_DISABLED).toInt();
    m_rg_preamp = settings.value("preamp", 0.0).toDouble();
    m_rg_defaut_gain = settings.value("default_gain", 0.0).toDouble();
    m_rg_prevent_clipping = settings.value("prevent_clipping", true).toBool();
    settings.endGroup();
    //audio settings
    m_aud_software_volume = settings.value("Output/software_volume", true).toBool();
    m_aud_format = static_cast<Qmmp::AudioFormat>(settings.value("Output/format", Qmmp::PCM_S16LE).toInt());
    m_aud_dithering = settings.value("Output/dithering", true).toBool();
    m_volume_step = settings.value("Output/volume_step", 5).toInt();
    m_average_bitrate = settings.value("Output/average_bitrate", false).toBool();
    //cover settings
    settings.beginGroup("Cover");
    m_cover_inc = settings.value("include", (QStringList() << "*.jpg" << "*.png" << "*.webp")).toStringList();
    m_cover_exclude = settings.value("exclude", (QStringList() << "*back*")).toStringList();
    m_cover_depth = settings.value("depth", 0).toInt();
    m_cover_use_files = settings.value("use_files", true).toBool();
    settings.endGroup();
    //network settings
    m_proxy_enabled = settings.value("Proxy/use_proxy", false).toBool();
    m_proxy_auth = settings.value("Proxy/authentication", false).toBool();
    m_proxy_type = static_cast<ProxyType>(settings.value("Proxy/proxy_type", HTTP_PROXY).toInt());
    m_proxy_url = settings.value("Proxy/url").toUrl();
    //buffer
    m_buffer_size = settings.value("Output/buffer_size", 500).toInt();
    //file type determination
    m_determine_by_content = settings.value("Misc/determine_file_by_content", false).toBool();
}

QmmpSettings::~QmmpSettings()
{
    sync();
    m_instance = nullptr;
}

QmmpSettings::ReplayGainMode QmmpSettings::replayGainMode() const
{
    return m_rg_mode;
}

double QmmpSettings::replayGainPreamp() const
{
    return m_rg_preamp;
}

double QmmpSettings::replayGainDefaultGain() const
{
    return m_rg_defaut_gain;
}

bool QmmpSettings::replayGainPreventClipping() const
{
    return m_rg_prevent_clipping;
}

void QmmpSettings::setReplayGainSettings(ReplayGainMode mode, double preamp, double def_gain, bool clip)
{
    m_rg_mode = mode;
    m_rg_preamp = preamp;
    m_rg_defaut_gain = def_gain;
    m_rg_prevent_clipping = clip;
    saveSettings();
    emit replayGainSettingsChanged();
}

bool QmmpSettings::useSoftVolume() const
{
    return m_aud_software_volume;
}

Qmmp::AudioFormat QmmpSettings::outputFormat() const
{
    return m_aud_format;
}

bool QmmpSettings::useDithering() const
{
    return m_aud_dithering;
}

void QmmpSettings::setAudioSettings(bool soft_volume, Qmmp::AudioFormat format, bool use_dithering)
{
    m_aud_software_volume = soft_volume;
    m_aud_format = format;
    m_aud_dithering = use_dithering;
    saveSettings();
    emit audioSettingsChanged();
}

const QStringList &QmmpSettings::coverNameFilters(bool include) const
{
    return include ? m_cover_inc : m_cover_exclude;
}

int QmmpSettings::coverSearchDepth() const
{
    return m_cover_depth;
}

bool QmmpSettings::useCoverFiles() const
{
    return m_cover_use_files;
}

void QmmpSettings::setCoverSettings(QStringList inc, QStringList exc, int depth, bool use_files)
{
    m_cover_inc = inc;
    m_cover_exclude = exc;
    m_cover_depth = depth;
    m_cover_use_files = use_files;
    MetaDataManager::instance()->clearCoverCache();
    saveSettings();
    emit coverSettingsChanged();
}

bool QmmpSettings::isProxyEnabled() const
{
    return m_proxy_enabled;
}

bool QmmpSettings::useProxyAuth() const
{
    return m_proxy_auth;
}

const QUrl &QmmpSettings::proxy() const
{
    return m_proxy_url;
}

QmmpSettings::ProxyType QmmpSettings::proxyType() const
{
    return m_proxy_type;
}

void QmmpSettings::setNetworkSettings(bool use_proxy, bool auth, ProxyType type, const QUrl &proxy)
{
    m_proxy_enabled = use_proxy;
    m_proxy_auth = auth;
    m_proxy_type = type;
    m_proxy_url = proxy;
    saveSettings();
    emit networkSettingsChanged();
}

const EqSettings &QmmpSettings::eqSettings() const
{
    return m_eq_settings;
}

void QmmpSettings::setEqSettings(const EqSettings &settings)
{
    m_eq_settings = settings;
    saveSettings();
    emit eqSettingsChanged();
}

void QmmpSettings::readEqSettings(EqSettings::Bands bands)
{
    m_eq_settings = EqSettings(bands);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup(QString("Equalizer_%1").arg(bands));
    for(int i = 0; i < bands; ++i)
        m_eq_settings.setGain(i, settings.value("band_"+ QString("%1").arg(i), 0).toDouble());
    m_eq_settings.setPreamp(settings.value("preamp", 0).toDouble());
    m_eq_settings.setEnabled(settings.value("enabled", false).toBool());
    settings.endGroup();
    m_eq_settings.setTwoPasses(settings.value("Equalizer/two_passes", true).toBool());
    emit eqSettingsChanged();
}

int QmmpSettings:: bufferSize() const
{
    return m_buffer_size;
}

void QmmpSettings::setBufferSize(int msec)
{
    m_buffer_size = msec;
}

void QmmpSettings::setVolumeStep(int step)
{
    m_volume_step = qBound(1, step, 20);
}

int QmmpSettings::volumeStep() const
{
    return m_volume_step;
}

void QmmpSettings::setAverageBitrate(bool enabled)
{
    m_average_bitrate = enabled;
    saveSettings();
    emit audioSettingsChanged();
}

bool QmmpSettings::averageBitrate() const
{
    return m_average_bitrate;
}

void QmmpSettings::sync()
{
    if(m_saveSettings)
    {
        return;
    }

    qDebug("%s", Q_FUNC_INFO);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    //replaygain settings
    settings.beginGroup("ReplayGain");
    settings.setValue("mode", m_rg_mode);
    settings.setValue("preamp", m_rg_preamp);
    settings.setValue("default_gain", m_rg_defaut_gain);
    settings.setValue("prevent_clipping", m_rg_prevent_clipping);
    settings.endGroup();
    //audio settings
    settings.setValue("Output/software_volume", m_aud_software_volume);
    settings.setValue("Output/format", m_aud_format);
    settings.setValue("Output/dithering", m_aud_dithering);
    settings.setValue("Output/volume_step", m_volume_step);
    settings.setValue("Output/average_bitrate", m_average_bitrate);
    //cover settings
    settings.beginGroup("Cover");
    settings.setValue("include", m_cover_inc);
    settings.setValue("exclude", m_cover_exclude);
    settings.setValue("depth", m_cover_depth);
    settings.setValue("use_files", m_cover_use_files);
    settings.endGroup();
    //network settings
    settings.setValue("Proxy/use_proxy", m_proxy_enabled);
    settings.setValue("Proxy/authentication", m_proxy_auth);
    settings.setValue("Proxy/url", m_proxy_url);
    settings.setValue("Proxy/proxy_type", m_proxy_type);
    //equalizer settings
    settings.beginGroup(QString("Equalizer_%1").arg(m_eq_settings.bands()));
    for(int i = 0; i < m_eq_settings.bands(); ++i)
        settings.setValue("band_" + QString("%1").arg(i), m_eq_settings.gain(i));
    settings.setValue("preamp", m_eq_settings.preamp());
    settings.setValue("enabled", m_eq_settings.isEnabled());
    settings.endGroup();
    settings.setValue("Equalizer/two_passes", m_eq_settings.twoPasses());
    //buffer size
    settings.setValue("Output/buffer_size", m_buffer_size);
    //file type determination
    settings.setValue("Misc/determine_file_by_content", m_determine_by_content);
    m_saveSettings = false;  //protect from multiple calls
}

void QmmpSettings::saveSettings()
{
    m_saveSettings = true;
    QMetaObject::invokeMethod(this, "sync", Qt::QueuedConnection);
}

QmmpSettings* QmmpSettings::instance()
{
    if(!m_instance)
        return new QmmpSettings(qApp);
    return m_instance;
}

void QmmpSettings::setDetermineFileTypeByContent(bool enabled)
{
    m_determine_by_content = enabled;
}

bool QmmpSettings::determineFileTypeByContent() const
{
    return m_determine_by_content;
}
