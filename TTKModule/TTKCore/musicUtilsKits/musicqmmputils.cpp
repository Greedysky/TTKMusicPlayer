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
    QString path = TTK::applicationPath();
#ifdef Q_OS_WIN
    path = path + QString("plugins/%1/%2.dll").arg(module, format);
#elif defined Q_OS_UNIX
    path = path + QString("plugins/%1/lib%2.so").arg(module, format);
#endif
    return path;
}

void TTK::TTKQmmp::enabledEffectPlugin(bool enabled, const QString &name)
{
    for(EffectFactory *factory : Effect::factories())
    {
        if(name.isEmpty())
        {
            Effect::setEnabled(factory, enabled);
        }
        else if(factory->properties().shortName == name)
        {
            Effect::setEnabled(factory, enabled);
            break;
        }
    }
}

MusicPluginPropertyList TTK::TTKQmmp::effectPlugins()
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

void TTK::TTKQmmp::enabledVisualPlugin(const QString &name, bool enabled)
{
    for(VisualFactory *v : Visual::factories())
    {
        if(v->properties().shortName == name)
        {
            Visual::setEnabled(v, enabled);
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
    settings.setValue("unpack_path", ARCHIVE_DIR_DIR_FULL);
    settings.endGroup();

//    settings.beginGroup("Decoder");
//    QStringList names = settings.value("disabled_plugins").toStringList();
//    if(names.isEmpty())
//    {
//        settings.setValue("disabled_plugins", names << "ken");
//    }
//    settings.endGroup();
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
