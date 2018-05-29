#include "musicplaylistfoundtablewidget.h"

MusicPlaylistFoundTableWidget::MusicPlaylistFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicPlaylistFoundTableWidget::~MusicPlaylistFoundTableWidget()
{
    clearAllItems();
}

void MusicPlaylistFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
}

void MusicPlaylistFoundTableWidget::setConnectObject(QObject *obj)
{
    if(obj->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), obj, SLOT(queryAllFinished()));
    }
}
