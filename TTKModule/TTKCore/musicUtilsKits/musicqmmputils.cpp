#include "musicqmmputils.h"
#include "musicsettingmanager.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <qmmp/visual.h>
#include <qmmp/visualfactory.h>
#include <qmmp/effect.h>
#include <qmmp/effectfactory.h>

QString MusicUtils::QMMP::pluginPath(const QString &module, const QString &format)
{
    QString path = MusicObject::applicationPath();
#ifdef Q_OS_WIN
    path = path + QString("plugins/%1/%2.dll").arg(module, format);
#elif defined Q_OS_UNIX
    path = path + QString("plugins/%1/lib%2.so").arg(module, format);
#endif
    return path;
}

void MusicUtils::QMMP::updateQmmpConfigFile()
{
    const QString &confPath = MAKE_CONFIG_DIR_FULL + "wildmidi.cfg";
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("conf_path", confPath);
    settings.endGroup();

    settings.beginGroup("Network");
    settings.setValue("buffer_path", CACHE_DIR_FULL);
    settings.endGroup();

    QFile file(confPath);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        if(file.open(QFile::WriteOnly))
        {
            data.remove(0, data.indexOf("\r\n"));
            data.insert(0, QString("dir %1freepats/").arg(MAKE_CONFIG_DIR_FULL).toUtf8());
            file.write(data);
        }
    }
    file.close();
}

void MusicUtils::QMMP::enabledEffectPlugin(bool enable)
{
    for(EffectFactory *factory : Effect::factories())
    {
        Effect::setEnabled(factory, enable);
    }
}

void MusicUtils::QMMP::enabledEffectPlugin(const QString &name, bool enable)
{
    for(EffectFactory *factory : Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            Effect::setEnabled(factory, enable);
            break;
        }
    }
}

MusicPluginPropertys MusicUtils::QMMP::effectPlugins()
{
    MusicPluginPropertys properties;
    for(EffectFactory *factory : Effect::factories())
    {
#ifdef Q_OS_WIN
        if(factory->properties().shortName == "ladspa")
        {
            continue;
        }
#endif
        MusicPluginProperty property;
        property.m_type = factory->properties().shortName;
        property.m_name = factory->properties().name;
        property.m_setting = factory->properties().hasSettings;
        properties << property;
    }
    return properties;
}

void MusicUtils::QMMP::showEffectSetting(const QString &name, QWidget *parent)
{
    for(EffectFactory *factory : Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            factory->showSettings(parent);
            break;
        }
    }
}

void MusicUtils::QMMP::enabledVisualPlugin(const QString &name, bool enable)
{
    for(VisualFactory *v : Visual::factories())
    {
        if(v->properties().shortName == name)
        {
            Visual::setEnabled(v, enable);
            break;
        }
    }
}

void MusicUtils::QMMP::updateRippleSpectrumConfigFile()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OuterBlurWave");

    QString colors = G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString();
    settings.setValue("colors", colors.remove(";"));
    const double opacity = 1.0;
    settings.setValue("opacity", opacity);
    settings.endGroup();
}
