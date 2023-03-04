#include "ttkdesktopwrapper.h"
#include "ttkglobalhelper.h"

#include <QScreen>
#include <QApplication>
#if !TTK_QT_VERSION_CHECK(5,0,0)
#  include <QDesktopWidget>
#endif

int TTKDesktopWrapper::screenTaskbarSize(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    QScreen *screen = QApplication::primaryScreen();
    const QRect &screenRect = screenGeometry(index);
    const QRect &deskRect = screen->availableGeometry();
#else
    QDesktopWidget *widget = QApplication::desktop();
    const QRect &screenRect = screenGeometry(index);
    const QRect &deskRect = widget->availableGeometry();
#endif
    if(screenRect.bottom() != deskRect.bottom())
    {
        return std::abs(screenRect.bottom() - deskRect.bottom());
    }
    else if(screenRect.top() != deskRect.top())
    {
        return std::abs(screenRect.top() - deskRect.top());
    }
    else if(screenRect.left() != deskRect.left())
    {
        return std::abs(screenRect.left() - deskRect.left());
    }
    else if(screenRect.right() != deskRect.right())
    {
        return std::abs(screenRect.right() - deskRect.right());
    }
    else
    {
        return 0;
    }
}

TTKObject::Direction TTKDesktopWrapper::screenTaskbarDirection(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    QScreen *screen = QApplication::primaryScreen();
    const QRect &screenRect = screenGeometry(index);
    const QRect &deskRect = screen->availableGeometry();
#else
    QDesktopWidget *widget = QApplication::desktop();
    const QRect &screenRect = screenGeometry(index);
    const QRect &deskRect = widget->availableGeometry();
#endif
    if(screenRect.bottom() != deskRect.bottom())
    {
        return TTKObject::Direction::Bottom;
    }
    else if(screenRect.top() != deskRect.top())
    {
        return TTKObject::Direction::Top;
    }
    else if(screenRect.left() != deskRect.left())
    {
        return TTKObject::Direction::Left;
    }
    else if(screenRect.right() != deskRect.right())
    {
        return TTKObject::Direction::Right;
    }
    else
    {
        return TTKObject::Direction::No;
    }
}

QRect TTKDesktopWrapper::screenGeometry(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QList<QScreen*> &screens = QApplication::screens();
    return (index < 0 || index >= screens.count()) ? QRect() : screens[index]->geometry();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget->screenGeometry(index);
#endif
}

QRect TTKDesktopWrapper::geometry()
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QList<QScreen*> &screens = QApplication::screens();
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

QPixmap TTKDesktopWrapper::grabWidget(QWidget *widget, const QRect &rect)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return widget->grab(rect);
#else
    return QPixmap::grabWidget(widget, rect);
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
