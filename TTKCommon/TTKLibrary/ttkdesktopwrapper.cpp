#include "ttkdesktopwrapper.h"
#include "ttklibrary.h"

#include <QScreen>
#include <QApplication>
#if !TTK_QT_VERSION_CHECK(5,0,0)
#  include <QDesktopWidget>
#endif

TTKDesktopWrapper::TaskbarInfo TTKDesktopWrapper::screenTaskbar(int index)
{
    const QRect &sr = screenGeometry(index);
    const QRect &dr = availableGeometry(index);

    TaskbarInfo info;
    if(sr.left() != dr.left())
    {
        info.m_size = std::abs(sr.left() - dr.left());
        info.m_direction = TTK::Direction::Left;
    }
    else if(sr.right() != dr.right())
    {
        info.m_size = std::abs(sr.right() - dr.right());
        info.m_direction = TTK::Direction::Right;
    }
    else if(sr.top() != dr.top())
    {
        info.m_size = std::abs(sr.top() - dr.top());
        info.m_direction = TTK::Direction::Top;
    }
    else if(sr.bottom() != dr.bottom())
    {
        info.m_size = std::abs(sr.bottom() - dr.bottom());
        info.m_direction = TTK::Direction::Bottom;
    }
    else
    {
        info.m_size = 0;
        info.m_direction = TTK::Direction::No;
    }
    return info;
}

QRect TTKDesktopWrapper::availableGeometry(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    Q_UNUSED(index);
    QScreen *screen = QApplication::primaryScreen();
    return screen->availableGeometry();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget->availableGeometry(index);
#endif
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
