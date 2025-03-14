#include "ttkdesktopscreen.h"
#include "ttklibrary.h"
#include <QDebug>
#include <QScreen>
#include <QApplication>
#ifdef Q_OS_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <qt_windows.h>
#endif
#if !TTK_QT_VERSION_CHECK(5,0,0)
#  include <QDesktopWidget>
#endif
#ifdef Q_OS_UNIX
#  include <QRegExp>
#  include <X11/Xlib.h>
#endif

TTKDesktopScreen::TaskbarInfo TTKDesktopScreen::screenTaskbar(int index)
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

QRect TTKDesktopScreen::availableGeometry(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    Q_UNUSED(index);
    QScreen *screen = QApplication::primaryScreen();
    return screen ? screen->availableGeometry() : QRect();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget ? widget->availableGeometry(index) : QRect();
#endif
}

QRect TTKDesktopScreen::screenGeometry(int index)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QList<QScreen*> &screens = QApplication::screens();
    return (index < 0 || index >= screens.count()) ? QRect() : screens[index]->geometry();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget ? widget->screenGeometry(index) : QRect();
#endif
}

QRect TTKDesktopScreen::geometry()
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    const QList<QScreen*> &screens = QApplication::screens();
    QRegion virtualGeometry;
    for(QScreen *screen : screens)
    {
        virtualGeometry += screen->geometry();
    }
    return virtualGeometry.boundingRect();
#else
    QDesktopWidget *widget = QApplication::desktop();
    return widget ? widget->geometry() : QRect();
#endif
}

int TTKDesktopScreen::screenIndex()
{
    int index = 0;
#if TTK_QT_VERSION_CHECK(5,0,0)
    const int count = QApplication::screens().count();
#else
    const int count = QApplication::desktop()->screenCount();
#endif
    if(count > 1)
    {
        const QPoint &pos = QCursor::pos();
        for(int i = 0; i < count; ++i)
        {
#if TTK_QT_VERSION_CHECK(5,0,0)
            if(QApplication::screens().at(i)->geometry().contains(pos))
#else
            if(QApplication::desktop()->screenGeometry(i).contains(pos))
#endif
            {
                index = i;
                break;
            }
        }
    }
    return index;
}

QPixmap TTKDesktopScreen::grabWidget(QWidget *widget, const QRect &rect)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return widget->grab(rect);
#else
    return QPixmap::grabWidget(widget, rect);
#endif
}

QPixmap TTKDesktopScreen::grabWindow(int x, int y, int w, int h)
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

static constexpr int DEFAULT_DPI = 96;

static QSize generateDPIValue()
{
    const QSize defaultSize(DEFAULT_DPI, DEFAULT_DPI);
#ifdef Q_OS_WIN
#  if TTK_QT_VERSION_CHECK(5,0,0)
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);

    HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);
    if(!GetMonitorInfo(monitor, &miex))
    {
        return defaultSize;
    }

    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    if(!EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm))
    {
        return defaultSize;
    }

    QSize size;
    size.setWidth(dm.dmPelsWidth * DEFAULT_DPI * 1.0 / (miex.rcMonitor.right - miex.rcMonitor.left));
    size.setHeight(dm.dmPelsHeight * DEFAULT_DPI * 1.0 / (miex.rcMonitor.bottom - miex.rcMonitor.top));
    return size;
#  endif
#elif defined Q_OS_UNIX
    Display *dp = XOpenDisplay(nullptr);
    if(!dp)
    {
        return defaultSize;
    }

    double dpi = DEFAULT_DPI;
    const char *resource = XResourceManagerString(dp);
    if(resource)
    {
        const QString data(resource);
        const QRegExp regx("Xft.dpi:\t(\\d+)");
        if(regx.indexIn(data) != -1)
        {
            dpi = regx.cap(1).toDouble();
        }
    }

    const int screen = 0; /* Screen number */
    const double x = (DisplayWidth(dp, screen) * 25.4) / DisplayWidthMM(dp, screen);
    const double y = (DisplayHeight(dp, screen) * 25.4) / DisplayHeightMM(dp, screen);

    QSize size;
    if(x < dpi && x != DEFAULT_DPI && TTKStaticCast(int, x) % DEFAULT_DPI != 0 &&
       y < dpi && y != DEFAULT_DPI && TTKStaticCast(int, y) % DEFAULT_DPI != 0)
    {
        size.setWidth(x + 0.5);
        size.setHeight(y + 0.5);
    }
    else
    {
        size.setWidth(dpi + 0.5);
        size.setHeight(dpi + 0.5);
    }

    XCloseDisplay(dp);
    return size;
#endif
    return defaultSize;
}

int TTKDesktopScreen::dotsPerInchX()
{
    const QSize dpi(generateDPIValue());
    return dpi.width();
}

int TTKDesktopScreen::dotsPerInchY()
{
    const QSize dpi(generateDPIValue());
    return dpi.height();
}

int TTKDesktopScreen::dotsPerInch()
{
    const QSize dpi(generateDPIValue());
    return (dpi.width() + dpi.height()) / 2;
}
