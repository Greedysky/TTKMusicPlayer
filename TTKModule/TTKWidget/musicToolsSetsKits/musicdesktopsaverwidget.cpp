#include "musicdesktopsaverwidget.h"
#include "musicsettingmanager.h"
#include "musicnumberdefine.h"

#include <QEvent>
#include <QTimer>
#include <QApplication>

MusicDesktopSaverWidget::MusicDesktopSaverWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    m_isRunning = false;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));

    setStyleSheet("background:blue");
    hide();

    qApp->installEventFilter(this);
}

MusicDesktopSaverWidget::~MusicDesktopSaverWidget()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    delete m_timer;
}

void MusicDesktopSaverWidget::applySettingParameter()
{
    const QString &time = M_SETTING_PTR->value(MusicSettingManager::OtherDesktopSaverTime).toString();
    const int value = time.toInt();
    if(value != 0)
    {
        m_timer->setInterval(value * MT_M2MS);
    }
}

void MusicDesktopSaverWidget::timeout()
{
    m_isRunning = true;
    setParent(nullptr);
    showFullScreen();
}

bool MusicDesktopSaverWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()== QEvent::MouseButtonPress || event->type()== QEvent::MouseButtonRelease ||
       event->type()== QEvent::MouseButtonDblClick || event->type()== QEvent::MouseMove ||
       event->type()== QEvent::KeyPress || event->type()== QEvent::KeyRelease)
    {
        if(m_isRunning)
        {
            m_isRunning = false;
            hide();
        }
        m_timer->stop();
        m_timer->start();
    }
    return QObject::eventFilter(watched, event);
}
