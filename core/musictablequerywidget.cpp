#include "musictablequerywidget.h"

MusicTableQueryWidget::MusicTableQueryWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    m_downLoadManager = new MusicDownLoadQueryThread(this);
    connect(m_downLoadManager,SIGNAL(clearAllItems()),this,SLOT(clearAllItems()));
    connect(m_downLoadManager,SIGNAL(showDownLoadInfoFor(DownLoadType)),
            this,SIGNAL(showDownLoadInfoFor(DownLoadType)));
    connect(m_downLoadManager,SIGNAL(showDownLoadInfoFinished(QString)),
            this,SIGNAL(showDownLoadInfoFinished(QString)));
    connect(m_downLoadManager,SIGNAL(creatSearchedItems(QString,QString,QString)),
            this,SLOT(creatSearchedItems(QString,QString,QString)));

    connect(this,SIGNAL(cellEntered(int,int)),SLOT(listCellEntered(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(itemDoubleClicked(int,int)));
}

void MusicTableQueryWidget::contextMenuEvent(QContextMenuEvent *)
{
//    QTableWidget::contextMenuEvent(event);
}
