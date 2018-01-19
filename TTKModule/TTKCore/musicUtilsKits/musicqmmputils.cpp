#include "musicqmmputils.h"
#include "musicobject.h"

#include <QSettings>
///qmmp incldue
#include "qmmp.h"

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

void MusicUtils::QMMP::midTransferFile()
{
    QString conf_path = MAKE_CONFIG_DIR_FULL + QString("wildmidi.cfg");
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("conf_path", conf_path);
    settings.endGroup();

    QFile file(conf_path);
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
