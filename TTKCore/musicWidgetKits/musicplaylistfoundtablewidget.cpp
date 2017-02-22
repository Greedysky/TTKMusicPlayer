#include "musicplaylistfoundtablewidget.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicsongssummariziedwidget.h"

MusicPlaylistFoundTableWidget::MusicPlaylistFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 436);
    headerview->resizeSection(2, 47);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);
    headerview->resizeSection(6, 26);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicPlaylistFoundTableWidget::~MusicPlaylistFoundTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllItems();
}

QString MusicPlaylistFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
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

void MusicPlaylistFoundTableWidget::resizeWindow()
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.9 + 436);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.1 + 47);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));
    }
}

void MusicPlaylistFoundTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryFoundTableWidget::resizeEvent(event);
    resizeWindow();
}
