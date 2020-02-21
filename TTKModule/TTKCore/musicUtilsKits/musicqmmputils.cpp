#include "musicqmmputils.h"
#include "musicobject.h"
#include "musicstringutils.h"
#include "musicsettingmanager.h"

#include <QSettings>
///qmmp incldue
#include "qmmp.h"
#include "visual.h"
#include "visualfactory.h"
#include "effect.h"
#include "effectfactory.h"

QString MusicUtils::QMMP::pluginPath(const QString &module, const QString &format)
{
    QString path = MusicObject::getAppDir();
#ifdef Q_OS_WIN
    path = path + QString("plugins/%1/%2.dll").arg(module).arg(format);
#elif defined Q_OS_UNIX
    path = path + QString("qmmp/%1/lib%2.so").arg(module).arg(format);
#endif
    return path;
}

void MusicUtils::QMMP::updateMidConfigFile()
{
    const QString &confPath = MAKE_CONFIG_DIR_FULL + QString("wildmidi.cfg");
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("conf_path", confPath);
    settings.endGroup();

    QFile file(confPath);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        if(file.open(QFile::WriteOnly))
        {
            data.remove(0, data.indexOf("\r\n"));
            data.insert(0, QString("dir %1freepats/").arg(MAKE_CONFIG_DIR_FULL));
            file.write(data);
        }
    }
    file.close();
}

void MusicUtils::QMMP::enabledVisualPlugin(const QString &name, bool enable)
{
    foreach(VisualFactory *v, Visual::factories())
    {
        if(v->properties().shortName == name)
        {
            Visual::setEnabled(v, enable);
            break;
        }
    }
}

void MusicUtils::QMMP::enabledEffectPlugin(const QString &name, bool enable)
{
    foreach(EffectFactory *factory, Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            Effect::setEnabled(factory, enable);
            break;
        }
    }
}

bool MusicUtils::QMMP::effectHasSetting(const QString &name)
{
    foreach(EffectFactory *factory, Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            return factory->properties().hasSettings;
        }
    }

    return false;
}

void MusicUtils::QMMP::showEffectSetting(const QString &name, QWidget *parent)
{
    foreach(EffectFactory *factory, Effect::factories())
    {
        if(factory->properties().shortName == name)
        {
            factory->showSettings(parent);
            break;
        }
    }
}

void MusicUtils::QMMP::updateRippleSpectrumConfigFile()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OuterEWave");

    QString colors = M_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString();
    settings.setValue("colors", colors.remove(";"));
    const double opacity = 1.0;
    settings.setValue("opacity", opacity);

    settings.endGroup();
}
