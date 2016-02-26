#include "musicnetworksuspensionwidget.h"
#include "musicnetworktestthread.h"
#include "musicuiobject.h"
#include "musicutils.h"

#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>
#include <QActionGroup>
#include <QPainter>

MusicNetworkSuspensionWidget::MusicNetworkSuspensionWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent), m_thread(nullptr)
{
    resize(200, 25);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    if(desktopWidget && desktopWidget->screen())
    {
        QSize s = desktopWidget->screen()->size();
        move(s.width() - width() - 60, s.height() - height() - 60);
    }
    else
    {
        move(10, 10);
    }
    m_upload = m_download = "0B/s";

    drawWindowShadow(false);
    setAttribute(Qt::WA_DeleteOnClose);

    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionTriggered(QAction*)));

    m_thread = new MusicNetworkTestThread(this);
    connect(m_thread, SIGNAL(networkData(ulong,ulong)), SLOT(networkData(ulong,ulong)));
    m_thread->start();

}

MusicNetworkSuspensionWidget::~MusicNetworkSuspensionWidget()
{
    delete m_actionGroup;
    delete m_thread;
}

void MusicNetworkSuspensionWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractMoveWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    QMenu settingMenu(tr("setting"), this);
    settingMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addMenu(&settingMenu);

    QStringList list = m_thread->getNewtworkNames();
    QStringList available = m_thread->getAvailableNewtworkNames();
    foreach(QString var, list)
    {
        QAction *action = settingMenu.addAction(var);
        m_actionGroup->addAction(action);
        if(available.contains(var))
        {
            action->setIcon(QIcon(":/share/selected"));
        }
    }

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/quit"), tr("quit"), this, SLOT(close()));
    rightClickMenu.exec(QCursor::pos());
}

void MusicNetworkSuspensionWidget::paintEvent(QPaintEvent *event)
{
    MusicAbstractMoveWidget::paintEvent(event);

    QPainter painter(this);
    ///redraw background color
    painter.fillRect(rect(), QBrush(QColor(0, 0, 0)));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.setBrush(QBrush(QColor(0, 255, 0)));
    QPainterPath path1;
    path1.moveTo(5, 20);
    path1.lineTo(15, 5);
    path1.lineTo(25, 20);
    painter.drawPath(path1);

    painter.setBrush(QBrush(QColor(255, 0, 0)));
    QPainterPath path2;
    path2.moveTo(100, 5);
    path2.lineTo(100 + 10, 20);
    path2.lineTo(100 + 20, 5);
    painter.drawPath(path2);

    painter.setPen(QPen(Qt::white, 2));
    QFontMetrics metr(painter.font());
    int upWidth = metr.width(m_upload);
    painter.drawText(20 + 10, 0, upWidth, height(), Qt::AlignCenter, m_upload);

    QFontMetrics metr2(painter.font());
    int upWidth2 = metr2.width(m_download);
    painter.drawText(100 + 20 + 5, 0, upWidth2, height(), Qt::AlignCenter, m_download);
}

void MusicNetworkSuspensionWidget::setAvailableNewtworkNames(const QStringList &names)
{
    m_thread->setAvailableNewtworkNames(names);
}

void MusicNetworkSuspensionWidget::networkData(ulong upload, ulong download)
{
    m_upload = MusicUtils::speed2Label(upload);
    m_download = MusicUtils::speed2Label(download);
    update();
}

void MusicNetworkSuspensionWidget::actionTriggered(QAction *action)
{
    if(action->icon().isNull())
    {
        action->setIcon(QIcon(":/share/selected"));
    }
    else
    {
        action->setIcon(QIcon());
    }

    QStringList selected;
    QList<QAction*> actions = m_actionGroup->actions();
    foreach(QAction *action, actions)
    {
#if defined Q_OS_WIN
        if(!action->icon().isNull())
        {
            selected << action->text();
        }
    }
#elif defined Q_OS_UNIX
        action->setIcon(QIcon());
    }
    action->setIcon(QIcon(":/share/selected"));
    selected << action->text();
#endif
    m_thread->setAvailableNewtworkNames(selected);
}
