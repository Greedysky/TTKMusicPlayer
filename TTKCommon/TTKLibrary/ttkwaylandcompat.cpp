#include "ttkwaylandcompat.h"

#include <QUrl>
#include <QFile>
#include <QDBusReply>
#include <QDBusInterface>
#include <QApplication>

#if defined Q_OS_UNIX || defined Q_CC_GNU
#  include <unistd.h>
#endif

TTKWaylandScreenshot::TTKWaylandScreenshot(QObject *parent)
    : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();

    QDBusInterface portal("org.freedesktop.portal.Desktop",
                          "/org/freedesktop/portal/desktop",
                          "org.freedesktop.portal.Screenshot",
                          connection);

    QVariantMap options;
    options["interactive"] = false;

    const QDBusReply<QDBusObjectPath> &reply = portal.call("Screenshot", QString(), options);
    if(!reply.isValid())
    {
        TTK_ERROR_STREAM("DBus call screenshot error");
        return;
    }

    connection.connect("org.freedesktop.portal.Desktop",
                       reply.value().path(),
                       "org.freedesktop.portal.Request",
                       "Response",
                       this,
                       SLOT(onResponse(uint,QVariantMap)));
}

QPixmap TTKWaylandScreenshot::grabWindow(int height, int x, int y, int w, int h) const
{
    int time = 0;

    while(m_pixmap.isNull() && time < 10 * 1000) // 10s
    {
        qApp->processEvents();

        time += 10;
        usleep(10 * 1000);
    }

    TTK_INFO_STREAM("Wayland grabWindow time cost: " << time);
    const qreal ratio = height * 1.0 / m_pixmap.height();
    const QPixmap &pixmap = m_pixmap.scaled(m_pixmap.size() * ratio, Qt::KeepAspectRatio);
    return pixmap.copy(x, y, w, h);
}

void TTKWaylandScreenshot::onResponse(uint response, const QVariantMap &results)
{
    if(response != 0)
    {
        TTK_ERROR_STREAM("DBus onResponse error");
        return;
    }

    const QString &path = results["uri"].toUrl().toLocalFile();
    m_pixmap.load(path);
    QFile::remove(path);
}
