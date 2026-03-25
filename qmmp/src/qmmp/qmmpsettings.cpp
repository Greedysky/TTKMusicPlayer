#include <QSettings>
#include "metadatamanager.h"
#include "qmmpsettings.h"

class QmmpSettingsPrivate
{
public:
    QmmpSettingsPrivate(QmmpSettings *q)
    {
        if(instance)
            qFatal("QmmpSettings: only one instance is allowed!");

        instance = q;

        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        //replaygain settings
        settings.beginGroup("ReplayGain");
        replayGainMode = (QmmpSettings::ReplayGainMode) settings.value("mode", QmmpSettings::REPLAYGAIN_DISABLED).toInt();
        replayGainPreamp = settings.value("preamp", 0.0).toDouble();
        replayGainDefautGain = settings.value("default_gain", 0.0).toDouble();
        replayGainPreventClipping = settings.value("prevent_clipping", true).toBool();
        settings.endGroup();
        //audio settings
        audioSoftwareVolume = settings.value("Output/software_volume", true).toBool();
        audioFormat = static_cast<Qmmp::AudioFormat>(settings.value("Output/format", Qmmp::PCM_S16).toInt());
        audioDithering = settings.value("Output/dithering", true).toBool();
        volumeStep = settings.value("Output/volume_step", 10).toInt();
        averageBitrate = settings.value("Output/average_bitrate", false).toBool();
        //cover settings
        settings.beginGroup("Cover");
        coverInclude = settings.value("include", QStringList({"*.jpg", "*.png", "*.webp"})).toStringList();
        coverExclude = settings.value("exclude", QStringList({"*back*"})).toStringList();
        coverDepth = settings.value("depth", 0).toInt();
        coverUseFiles = settings.value("use_files", true).toBool();
        settings.endGroup();
        //network settings
        proxyEnabled = settings.value("Proxy/use_proxy", false).toBool();
        proxyAuth = settings.value("Proxy/authentication", false).toBool();
        proxyType = static_cast<QmmpSettings::ProxyType>(settings.value("Proxy/proxy_type", QmmpSettings::HTTP_PROXY).toInt());
        proxyUrl = settings.value("Proxy/url").toUrl();
        //buffer
        bufferSize = settings.value("Output/buffer_size", 500).toInt();
        //file type determination
        determineByContent = settings.value("Misc/determine_file_by_content", false).toBool();
    }

    ~QmmpSettingsPrivate()
    {
        instance = nullptr;
    }

    void saveSettings()
    {
        saveSettingsRequest = true;
        QMetaObject::invokeMethod(instance, "sync", Qt::QueuedConnection);
    }

    //replaygain settings
    QmmpSettings::ReplayGainMode replayGainMode;
    double replayGainPreamp;
    double replayGainDefautGain;
    bool replayGainPreventClipping;
    //audio settings
    bool audioSoftwareVolume;
    bool audioDithering;
    Qmmp::AudioFormat audioFormat;
    int volumeStep;
    bool averageBitrate;
    //cover settings
    QStringList coverInclude;
    QStringList coverExclude;
    int coverDepth;
    bool coverUseFiles;
    //network settings
    bool proxyEnabled;
    bool proxyAuth;
    QUrl proxyUrl;
    QmmpSettings::ProxyType proxyType;
    //equalizer settings
    EqSettings equalizerSettings;
    //buffer size
    int bufferSize;
    //file type determination
    bool determineByContent;
    //protect from multiple calls
    bool saveSettingsRequest = false;

    static QmmpSettings* instance;

};

QmmpSettings *QmmpSettingsPrivate::instance = nullptr;


QmmpSettings::QmmpSettings(QObject *parent)
    : QObject(parent),
      d(new QmmpSettingsPrivate(this))
{

}

QmmpSettings::~QmmpSettings()
{
    sync();
    delete d;
}

QmmpSettings::ReplayGainMode QmmpSettings::replayGainMode() const
{
    return d->replayGainMode;
}

double QmmpSettings::replayGainPreamp() const
{
    return d->replayGainPreamp;
}

double QmmpSettings::replayGainDefaultGain() const
{
    return d->replayGainDefautGain;
}

bool QmmpSettings::replayGainPreventClipping() const
{
    return d->replayGainPreventClipping;
}

void QmmpSettings::setReplayGainSettings(ReplayGainMode mode, double preamp, double def_gain, bool clip)
{
    d->replayGainMode = mode;
    d->replayGainPreamp = preamp;
    d->replayGainDefautGain = def_gain;
    d->replayGainPreventClipping = clip;
    d->saveSettings();
    emit replayGainSettingsChanged();
}

bool QmmpSettings::useSoftVolume() const
{
    return d->audioSoftwareVolume;
}

Qmmp::AudioFormat QmmpSettings::outputFormat() const
{
    return d->audioFormat;
}

bool QmmpSettings::useDithering() const
{
    return d->audioDithering;
}

void QmmpSettings::setAudioSettings(bool soft_volume, Qmmp::AudioFormat format, bool use_dithering)
{
    d->audioSoftwareVolume = soft_volume;
    d->audioFormat = format;
    d->audioDithering = use_dithering;
    d->saveSettings();
    emit audioSettingsChanged();
}

const QStringList &QmmpSettings::coverNameFilters(bool include) const
{
    return include ? d->coverInclude : d->coverExclude;
}

int QmmpSettings::coverSearchDepth() const
{
    return d->coverDepth;
}

bool QmmpSettings::useCoverFiles() const
{
    return d->coverUseFiles;
}

void QmmpSettings::setCoverSettings(QStringList inc, QStringList exc, int depth, bool use_files)
{
    d->coverInclude = inc;
    d->coverExclude = exc;
    d->coverDepth = depth;
    d->coverUseFiles = use_files;
    MetaDataManager::instance()->clearCoverCache();
    d->saveSettings();
    emit coverSettingsChanged();
}

bool QmmpSettings::isProxyEnabled() const
{
    return d->proxyEnabled;
}

bool QmmpSettings::useProxyAuth() const
{
    return d->proxyAuth;
}

const QUrl &QmmpSettings::proxy() const
{
    return d->proxyUrl;
}

QmmpSettings::ProxyType QmmpSettings::proxyType() const
{
    return d->proxyType;
}

void QmmpSettings::setNetworkSettings(bool use_proxy, bool auth, ProxyType type, const QUrl &proxy)
{
    d->proxyEnabled = use_proxy;
    d->proxyAuth = auth;
    d->proxyType = type;
    d->proxyUrl = proxy;
    if(type == HTTP_PROXY)
        d->proxyUrl.setScheme("http");
    else if(type == SOCKS5_PROXY)
        d->proxyUrl.setScheme("socks5");
    d->saveSettings();
    emit networkSettingsChanged();
}

const EqSettings &QmmpSettings::eqSettings() const
{
    return d->equalizerSettings;
}

void QmmpSettings::setEqSettings(const EqSettings &settings)
{
    d->equalizerSettings = settings;
    d->saveSettings();
    emit eqSettingsChanged();
}

void QmmpSettings::readEqSettings(EqSettings::Bands bands)
{
    d->equalizerSettings = EqSettings(bands);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup(QString("Equalizer_%1").arg(bands));
    for(int i = 0; i < bands; ++i)
        d->equalizerSettings.setGain(i, settings.value("band_"+ QString("%1").arg(i), 0).toDouble());
    d->equalizerSettings.setPreamp(settings.value("preamp", 0).toDouble());
    d->equalizerSettings.setEnabled(settings.value("enabled", false).toBool());
    settings.endGroup();
    d->equalizerSettings.setTwoPasses(settings.value("Equalizer/two_passes", true).toBool());
    emit eqSettingsChanged();
}

int QmmpSettings:: bufferSize() const
{
    return d->bufferSize;
}

void QmmpSettings::setBufferSize(int msec)
{
    d->bufferSize = msec;
}

void QmmpSettings::setVolumeStep(int step)
{
    d->volumeStep = qBound(1, step, 20);
}

int QmmpSettings::volumeStep() const
{
    return d->volumeStep;
}

void QmmpSettings::setAverageBitrate(bool enabled)
{
    d->averageBitrate = enabled;
    d->saveSettings();
    emit audioSettingsChanged();
}

bool QmmpSettings::averageBitrate() const
{
    return d->averageBitrate;
}

void QmmpSettings::sync()
{
    if(d->saveSettingsRequest)
    {
        return;
    }

    qDebug("%s", Q_FUNC_INFO);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    //replaygain settings
    settings.beginGroup("ReplayGain");
    settings.setValue("mode", d->replayGainMode);
    settings.setValue("preamp", d->replayGainPreamp);
    settings.setValue("default_gain", d->replayGainDefautGain);
    settings.setValue("prevent_clipping", d->replayGainPreventClipping);
    settings.endGroup();
    //audio settings
    settings.setValue("Output/software_volume", d->audioSoftwareVolume);
    settings.setValue("Output/format", d->audioFormat);
    settings.setValue("Output/dithering", d->audioDithering);
    settings.setValue("Output/volume_step", d->volumeStep);
    settings.setValue("Output/average_bitrate", d->averageBitrate);
    //cover settings
    settings.beginGroup("Cover");
    settings.setValue("include", d->coverInclude);
    settings.setValue("exclude", d->coverExclude);
    settings.setValue("depth", d->coverDepth);
    settings.setValue("use_files", d->coverUseFiles);
    settings.endGroup();
    //network settings
    settings.setValue("Proxy/use_proxy", d->proxyEnabled);
    settings.setValue("Proxy/authentication", d->proxyAuth);
    settings.setValue("Proxy/url", d->proxyUrl);
    settings.setValue("Proxy/proxy_type", d->proxyType);
    //equalizer settings
    settings.beginGroup(QString("Equalizer_%1").arg(d->equalizerSettings.bands()));
    for(int i = 0; i < d->equalizerSettings.bands(); ++i)
        settings.setValue("band_" + QString("%1").arg(i), d->equalizerSettings.gain(i));
    settings.setValue("preamp", d->equalizerSettings.preamp());
    settings.setValue("enabled", d->equalizerSettings.isEnabled());
    settings.endGroup();
    settings.setValue("Equalizer/two_passes", d->equalizerSettings.twoPasses());
    //buffer size
    settings.setValue("Output/buffer_size", d->bufferSize);
    //file type determination
    settings.setValue("Misc/determine_file_by_content", d->determineByContent);
    d->saveSettingsRequest = false;  //protect from multiple calls
}

QmmpSettings* QmmpSettings::instance()
{
    if(!QmmpSettingsPrivate::instance)
        return new QmmpSettings(qApp);
    return QmmpSettingsPrivate::instance;
}

void QmmpSettings::setDetermineFileTypeByContent(bool enabled)
{
    d->determineByContent = enabled;
}

bool QmmpSettings::determineFileTypeByContent() const
{
    return d->determineByContent;
}
