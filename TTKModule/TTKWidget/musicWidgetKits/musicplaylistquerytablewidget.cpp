#include "musicplaylistquerytablewidget.h"

MusicPlaylistQueryTableWidget::MusicPlaylistQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicPlaylistQueryTableWidget::~MusicPlaylistQueryTableWidget()
{
    removeItems();
}

void MusicPlaylistQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
}

void MusicPlaylistQueryTableWidget::setConnectClass(QObject *obj)
{
    if(obj->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), obj, SLOT(queryAllFinished()));
    }
}
