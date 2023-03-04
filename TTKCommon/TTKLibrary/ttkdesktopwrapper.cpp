#include "ttkdesktopwrapper.h"
#include "ttkglobalhelper.h"

#include <QScreen>
#include <QApplication>
#if !TTK_QT_VERSION_CHECK(5,0,0)
#  include <QDesktopWidget>
#endif

TTKDesktopWrapper::TaskbarInfo TTKDesktopWrapper::screenTaskbar(int index)
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
    TaskbarInfo info;
    if(screenRect.left() != deskRect.left())
    {
        info.m_size = std::abs(screenRect.left() - deskRect.left());
        info.m_direction = TTKObject::Direction::Left;
    }
    else if(screenRect.right() != deskRect.right())
    {
        info.m_size = std::abs(screenRect.right() - deskRect.right());
        info.m_direction = TTKObject::Direction::Right;
    }
    else if(screenRect.top() != deskRect.top())
    {
        info.m_size = std::abs(screenRect.top() - deskRect.top());
        info.m_direction = TTKObject::Direction::Top;
    }
    else if(screenRect.bottom() != deskRect.bottom())
    {
        info.m_size = std::abs(screenRect.bottom() - deskRect.bottom());
        info.m_direction = TTKObject::Direction::Bottom;
    }
    else
    {
        info.m_size = 0;
        info.m_direction = TTKObject::Direction::No;
    }
    return info;
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
