#include "ttkfileassocation.h"
#include "ttkqtobject.h"

#ifdef Q_OS_WIN
#include <QSettings>

void TTKFileAssocation::append(const QString &suffix)
{
    appendClass(suffix);
    appendSoftware(suffix);
    appendFileExts(suffix);
}

void TTKFileAssocation::remove(const QString &suffix)
{
    removeClass(suffix);
    removeSoftware(suffix);
    removeFileExts(suffix);
}

bool TTKFileAssocation::exist(const QString &suffix) const
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;

    QSettings reg1Setting("HKEY_CURRENT_USER\\Software\\Classes\\" + key, QSettings::NativeFormat);
    return reg1Setting.value("Default").toString() == mainName;
}

QStringList TTKFileAssocation::keys() const
{
    QStringList keys;
    QSettings reg1Setting("HKEY_CURRENT_USER\\Software\\Classes\\", QSettings::NativeFormat);

    for(const QString &key : reg1Setting.childGroups())
    {
        if(!key.isEmpty() && key[0] == '.')
        {
            keys.push_back(key.toLower().remove(0, 1));
        }
    }
    return keys;
}

void TTKFileAssocation::appendClass(const QString &suffix)
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;
    const QString &mainString = "HKEY_CURRENT_USER\\Software\\Classes\\" + key;

    QSettings reg1Setting(mainString, QSettings::NativeFormat);
    const QString &reg1Value = reg1Setting.value("Default").toString();
    if(reg1Value != mainName)
    {
        if(!reg1Value.isEmpty())
        {
            // do backup
            reg1Setting.setValue("TTKBackup", reg1Value);
        }
        reg1Setting.setValue("Default", mainName);
    }

    QSettings reg2Setting(mainString + "\\OpenWithProgids", QSettings::NativeFormat);
    reg2Setting.setValue(mainName, QString());
}

void TTKFileAssocation::appendSoftware(const QString &suffix)
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;
    const QString &mainString = "HKEY_CURRENT_USER\\Software\\Classes\\" + mainName;

    QString appPath = TTK_SERVICE_FULL;
    appPath.replace("/", "\\");

    QSettings reg1Setting(mainString, QSettings::NativeFormat);
    reg1Setting.setValue("Default", suffix.toUpper() + QObject::tr("File"));

    QSettings reg2Setting(mainString + "\\DefaultIcon", QSettings::NativeFormat);
    reg2Setting.setValue("Default", QString("%1,%2").arg(appPath).arg(1));

    QSettings reg3Setting(mainString + "\\Shell\\Open", QSettings::NativeFormat);
    reg3Setting.setValue("Default", QObject::tr("Use Module Play"));

    QSettings reg4Setting(mainString + "\\Shell\\Open\\Command", QSettings::NativeFormat);
    reg4Setting.setValue("Default", QString("\"%1\" -Open ").arg(appPath) + QString("\"%1\""));

    QSettings reg5Setting(mainString + "\\Shell\\PlayList", QSettings::NativeFormat);
    reg5Setting.setValue("Default", QObject::tr("Add To Module Playlist"));

    QSettings reg6Setting(mainString + "\\Shell\\PlayList\\Command", QSettings::NativeFormat);
    reg6Setting.setValue("Default", QString("\"%1\" -List ").arg(appPath) + QString("\"%1\""));
}

void TTKFileAssocation::appendFileExts(const QString &suffix)
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;
    const QString &mainString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\" + key;

    QSettings reg1Setting(mainString + "\\OpenWithProgids", QSettings::NativeFormat);
    reg1Setting.setValue(mainName, QString());

    QSettings reg2Setting(mainString + "\\UserChoice", QSettings::NativeFormat);
    const QString &reg2Value = reg2Setting.value("Progid").toString();
    if(reg2Value != mainName)
    {
        if(!reg2Value.isEmpty())
        {
            // do backup
            reg2Setting.setValue("TTKBackup", reg2Value);
        }
        reg2Setting.setValue("Progid", mainName);
    }
}

void TTKFileAssocation::removeClass(const QString &suffix)
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;
    const QString &mainString = "HKEY_CURRENT_USER\\Software\\Classes\\" + key;

    QSettings reg1Setting(mainString, QSettings::NativeFormat);
    const QString &reg1Value = reg1Setting.value("Default").toString();
    if(reg1Value == mainName)
    {
        reg1Setting.setValue("Default", reg1Setting.value("TTKBackup", QString()));
        reg1Setting.remove("TTKBackup");
    }

    reg1Setting.remove("OpenWithProgids");
}

void TTKFileAssocation::removeSoftware(const QString &suffix)
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;
    const QString &mainString = "HKEY_CURRENT_USER\\Software\\Classes\\";

    QSettings reg1Setting(mainString, QSettings::NativeFormat);
    reg1Setting.remove(mainName);
}

void TTKFileAssocation::removeFileExts(const QString &suffix)
{
    const QString &key = "." + suffix;
    const QString &mainName = TTK_APP_NAME + key;
    const QString &mainString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\" + key;

    QSettings reg1Setting(mainString, QSettings::NativeFormat);
    reg1Setting.remove("OpenWithProgids");

    bool empty = true;
    QSettings reg2Setting(mainString + "\\UserChoice", QSettings::NativeFormat);
    const QString &reg2Value = reg2Setting.value("Progid").toString();
    if(reg2Value == mainName)
    {
        const QString &backup = reg2Setting.value("TTKBackup").toString();
        if(!backup.isEmpty())
        {
            empty = false;
            reg2Setting.setValue("Progid", backup);
        }

        reg2Setting.remove("TTKBackup");
    }

    if(empty)
    {
        reg1Setting.remove("UserChoice");
    }
}
#endif
