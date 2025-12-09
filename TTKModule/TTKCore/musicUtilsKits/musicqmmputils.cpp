#include "musicqmmputils.h"
#include "musicsettingmanager.h"

#include <QDialog>
#include <QSettings>
#include <qmmp/qmmp.h>
#include <qmmp/visual.h>
#include <qmmp/visualfactory.h>
#include <qmmp/effect.h>
#include <qmmp/effectfactory.h>

QString TTK::TTKQmmp::pluginPath(const QString &module, const QString &format)
{
    QString path = Qmmp::pluginPath();
#ifdef Q_OS_WIN
    path += QString("/%1/%2.dll").arg(module, format);
#elif defined Q_OS_LINUX
    path += QString("/%1/lib%2.so").arg(module, format);
#elif defined Q_OS_MAC
    path += QString("/%1/lib%2.dylib").arg(module, format);
#endif
    return path;
}

bool TTK::TTKQmmp::isVisualEnabled(const QString &name)
{
    for(VisualFactory *factory : Visual::factories())
    {
        if(factory->properties().shortName == name)
        {
            return Visual::isEnabled(factory);
        }
    }
    return false;
}

void TTK::TTKQmmp::setVisualEnabled(const QString &name, bool enabled)
{
    for(VisualFactory *factory : Visual::factories())
    {
        if(factory->properties().shortName == name)
        {
            Visual::setEnabled(factory, enabled);
            break;
        }
    }
}

bool TTK::TTKQmmp::isEffectEnabled(const QString &name)
{
    for(EffectFactory *factory : Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            return Effect::isEnabled(factory);
        }
    }
    return false;
}

void TTK::TTKQmmp::setEffectEnabled(const QString &name, bool enabled)
{
    for(EffectFactory *factory : Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            Effect::setEnabled(factory, enabled);
            break;
        }
    }
}

void TTK::TTKQmmp::setEffectsEnabled(bool enabled)
{
    for(EffectFactory *factory : Effect::factories())
    {
        Effect::setEnabled(factory, enabled);
    }
}

MusicPluginPropertyList TTK::TTKQmmp::effectModules()
{
    MusicPluginPropertyList properties;
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
        property.m_hasSettings = factory->properties().hasSettings;
        properties << property;
    }
    return properties;
}

void TTK::TTKQmmp::showEffectSetting(const QString &name)
{
    for(EffectFactory *factory : Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            QDialog *dialog = factory->createSettings(nullptr);
            if(dialog)
            {
                dialog->exec();
                dialog->deleteLater();
            }
            break;
        }
    }
}

void TTK::TTKQmmp::updateBaseConfig()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Http");
    settings.setValue("buffer_path", CACHE_DIR_FULL);
    settings.endGroup();

    settings.beginGroup("Archive");
    settings.setValue("unpack_path", ARCHIVE_DIR_FULL);
    settings.endGroup();
}

void TTK::TTKQmmp::updateBlurConfig()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OuterBlurWave");
    QString colors = G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString();
    settings.setValue("colors", colors.remove(";"));
    const double opacity = (G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumTransparent).toInt() + 50) / 150.0;
    settings.setValue("opacity", opacity);
    settings.endGroup();
}

void TTK::TTKQmmp::updateVoiceConfig(bool update, int &value)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Muffler");
    if(update)
    {
        settings.setValue("ratio", value);
    }
    else
    {
        value = settings.value("ratio", 100).toInt();
    }
    settings.endGroup();
}
