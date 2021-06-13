#include "ttkdesktopwrapper.h"

#include <QScreen>
#include <QApplication>
#if !TTK_QT_VERSION_CHECK(5,0,0)
#include <QDesktopWidget>
#endif

QRect TTKDesktopWrapper::screenGeometry(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QList<QScreen *> &screens = QApplication::screens();
    return (index < 0 || index >= screens.count()) ? QRect() : screens[index]->geometry();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget->screenGeometry(index);
#endif
}

QRect TTKDesktopWrapper::geometry()
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QList<QScreen *> &screens = QApplication::screens();
    QRegion virtualGeometry;
    for(int i = 0; i < screens.count(); ++i)
    {
        QScreen *screen = screens[i];
        virtualGeometry += screen->geometry();
    }
    return virtualGeometry.boundingRect();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget->geometry();
#endif
}

QPixmap TTKDesktopWrapper::grabWindow(int x, int y, int w, int h)
{
    QWidget widget(nullptr, Qt::Desktop);
    widget.setVisible(false);
    widget.setGeometry(geometry());
#if TTK_QT_VERSION_CHECK(5,0,0)
    return QApplication::primaryScreen()->grabWindow(widget.winId(), x, y, w, h);
#else
    return QPixmap::grabWindow(widget.winId(), x, y, w, h);
#endif
}
