#include "musicplaylistquerytablewidget.h"

MusicPlaylistQueryTableWidget::MusicPlaylistQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicPlaylistQueryTableWidget::~MusicPlaylistQueryTableWidget()
{
    clearAllItems();
}

void MusicPlaylistQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
}

void MusicPlaylistQueryTableWidget::setConnectObject(QObject *obj)
{
    if(obj->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), obj, SLOT(queryAllFinished()));
    }
}
