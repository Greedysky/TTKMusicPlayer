#include "musictablequerywidget.h"

#include <QPainter>

MusicTableQueryWidget::MusicTableQueryWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    m_downLoadManager = new MusicDownLoadQueryThread(this);
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(showDownLoadInfoFor(DownLoadType)),
                               SIGNAL(showDownLoadInfoFor(DownLoadType)));
    connect(m_downLoadManager, SIGNAL(showDownLoadInfoFinished(QString)),
                               SIGNAL(showDownLoadInfoFinished(QString)));
    connect(m_downLoadManager, SIGNAL(creatSearchedItems(QString,QString,QString)),
                               SLOT(creatSearchedItems(QString,QString,QString)));

    connect(this, SIGNAL(cellEntered(int,int)), SLOT(listCellEntered(int,int)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

void MusicTableQueryWidget::contextMenuEvent(QContextMenuEvent *)
{

}

void MusicTableQueryWidget::paintEvent(QPaintEvent *event)
{
    MusicTableWidgetAbstract::paintEvent(event);
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QBrush(QColor(0,0,0)), 0.1, Qt::SolidLine));
    for(int i=0; i<rowCount(); ++i)
    {
        painter.drawLine(10, rowHeight(0)*(i + 1),
                         width() - 15, rowHeight(0)*(i + 1));
    }
}
