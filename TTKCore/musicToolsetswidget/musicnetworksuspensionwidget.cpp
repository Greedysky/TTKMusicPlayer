#include "musicnetworksuspensionwidget.h"
#include "musicnetworktestthread.h"
#include "musicuiobject.h"

#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>

MusicNetworkSuspensionWidget::MusicNetworkSuspensionWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent), m_thread(nullptr)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    if(desktopWidget && desktopWidget->screen())
    {
        QSize s = desktopWidget->screen()->size();
        move(s.width() - 160, s.height() - 100);
    }
    else
    {
        move(10, 10);
    }
    resize(130, 25);

    drawWindowShadow(false);
    setStyleSheet("background:rgba(0,0,0,50);");
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    m_thread = new MusicNetworkTestThread(this);
    connect(m_thread, SIGNAL(networkData(ulong,ulong)), SLOT(networkData(ulong,ulong)));
//    m_thread->start();

}

MusicNetworkSuspensionWidget::~MusicNetworkSuspensionWidget()
{
    m_thread->stopAndQuitThread();
    delete m_thread;
}

void MusicNetworkSuspensionWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/quit"), tr("quit"), this, SLOT(close()));
    rightClickMenu.exec(QCursor::pos());
}

void MusicNetworkSuspensionWidget::networkData(ulong upload, ulong download)
{
//    qDebug() << upload << download;
}
