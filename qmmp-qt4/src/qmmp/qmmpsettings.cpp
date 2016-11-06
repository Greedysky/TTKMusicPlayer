/***************************************************************************
 *   Copyright (C) 2010-2013 by Ilya Kotov                                 *
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

#include <QSettings>
#include <QApplication>
#include <QTimer>
#include "metadatamanager.h"
#include "qmmp.h"
#include "qmmpsettings.h"

QmmpSettings *QmmpSettings::m_instance = 0;

QmmpSettings::QmmpSettings(QObject *parent) : QObject(parent)
{
    if(m_instance)
        qFatal("QmmpSettings: only one instance is allowed");
    m_instance = this;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    //replaygain settings
    settings.beginGroup("ReplayGain");
    m_rg_mode = (ReplayGainMode) settings.value("mode", REPLAYGAIN_DISABLED).toInt();
    m_rg_preamp = settings.value("preamp", 0.0).toDouble();
    m_rg_defaut_gain = settings.value("default_gain", 0.0).toDouble();
    m_rg_prevent_clipping = settings.value("prevent_clipping", true).toBool();
    settings.endGroup();
    //audio settings
    m_aud_software_volume = settings.value("Output/software_volume", false).toBool();
    m_aud_format = (Qmmp::AudioFormat) settings.value("Output/format", Qmmp::PCM_S16LE).toInt();
    m_aud_dithering = settings.value("Output/dithering", true).toBool();
    m_volume_step = settings.value("Output/volume_step", 5).toInt();
    //cover settings
    settings.beginGroup("Cover");
    m_cover_inc = settings.value("include", (QStringList() << "*.jpg" << "*.png")).toStringList();
    m_cover_exclude = settings.value("exclude", (QStringList() << "*back*")).toStringList();
    m_cover_depth = settings.value("depth", 0).toInt();
    m_cover_use_files = settings.value("use_files", true).toBool();
    settings.endGroup();
    //network settings
    m_proxy_enabled = settings.value("Proxy/use_proxy", false).toBool();
    m_proxy_auth = settings.value("Proxy/authentication", false).toBool();
    m_proxy_url = settings.value("Proxy/url").toUrl();
    //buffer
    m_buffer_size = settings.value("Output/buffer_size", 500).toInt();
    //file type determination
    m_determine_by_content = settings.value("Misc/determine_file_by_content", false).toBool();
    //timer
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(5000);
    connect(m_timer, SIGNAL(timeout()), SLOT(sync()));
}

QmmpSettings::~QmmpSettings()
{
    sync();
    m_instance = 0;
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
    m_timer->start();
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
    m_timer->start();
    emit audioSettingsChanged();
}

QStringList QmmpSettings::coverNameFilters(bool include) const
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
    MetaDataManager::instance()->clearCoverChache();
    m_timer->start();
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

QUrl QmmpSettings::proxy() const
{
    return  m_proxy_url;
}

void QmmpSettings::setNetworkSettings(bool use_proxy, bool auth, const QUrl &proxy)
{
    m_proxy_enabled = use_proxy;
    m_proxy_auth = auth;
    m_proxy_url = proxy;
    m_timer->start();
    emit networkSettingsChanged();
}

EqSettings QmmpSettings::eqSettings() const
{
    return m_eq_settings;
}

void QmmpSettings::setEqSettings(const EqSettings &settings)
{
    m_eq_settings = settings;
    m_timer->start();
    emit eqSettingsChanged();
}

void QmmpSettings::readEqSettings(int bands)
{
    m_eq_settings = EqSettings(bands);
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup(QString("Equalizer_%1").arg(bands));
    for (int i = 0; i < bands; ++i)
        m_eq_settings.setGain(i, settings.value("band_"+ QString("%1").arg(i), 0).toDouble());
    m_eq_settings.setPreamp(settings.value("preamp", 0).toDouble());
    m_eq_settings.setEnabled(settings.value("enabled", false).toBool());
    settings.endGroup();
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

void QmmpSettings::sync()
{
    qDebug("%s", Q_FUNC_INFO);
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
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
    //equalizer settings
    settings.beginGroup(QString("Equalizer_%1").arg(m_eq_settings.bands()));
    for (int i = 0; i < m_eq_settings.bands(); ++i)
        settings.setValue("band_"+ QString("%1").arg(i), m_eq_settings.gain(i));
    settings.setValue("preamp", m_eq_settings.preamp());
    settings.setValue("enabled", m_eq_settings.isEnabled());
    settings.endGroup();
    //buffer size
    settings.setValue("Output/buffer_size", m_buffer_size);
    //file type determination
    settings.setValue("Misc/determine_file_by_content", m_determine_by_content);
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
