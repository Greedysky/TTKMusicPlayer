#include "musicquerytablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicrightareawidget.h"

#include <QActionGroup>

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    m_downLoadManager = nullptr;
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_downLoadManager;
}

QString MusicQueryTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    delete m_downLoadManager;
    m_downLoadManager = query;
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(createSearchedItems(QString,QString,QString)),
                               SLOT(createSearchedItems(QString,QString,QString)));
}

void MusicQueryTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}



MusicQueryItemTableWidget::MusicQueryItemTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setQueryInput(M_DOWNLOAD_QUERY_PTR->getQueryThread(this));

    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionGroupClick(QAction*)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicQueryItemTableWidget::~MusicQueryItemTableWidget()
{
    delete m_actionGroup;
}

QString MusicQueryItemTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryItemTableWidget::actionGroupClick(QAction *action)
{
    int row = currentRow();
    if( row < 0)
    {
        return;
    }

    QString songName = (row != -1 && rowCount() > 0) ? item(row, 1)->toolTip() : QString();
    QString artistName = (row != -1 && rowCount() > 0) ? item(row, 2)->toolTip() : QString();

    switch( action->data().toInt() )
    {
        case 0: musicDownloadLocal(row); break;
        case 1: MusicRightAreaWidget::instance()->songResearchButtonSearched(songName); break;
        case 2: MusicRightAreaWidget::instance()->songResearchButtonSearched(artistName); break;
        case 3: MusicRightAreaWidget::instance()->songResearchButtonSearched(songName + "-" + artistName); break;
    }
}

void MusicQueryItemTableWidget::createContextMenu(QMenu &menu)
{
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    m_actionGroup->addAction(menu.addAction(tr("musicDownload")))->setData(0);

    menu.addSeparator();

    QString songName = currentRow() != -1 && rowCount() > 0 ?
                item(currentRow(), 1)->toolTip() : QString();
    QString artistName = currentRow() != -1 && rowCount() > 0 ?
                item(currentRow(), 2)->toolTip() : QString();
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(songName)))->setData(1);
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(artistName)))->setData(2);
    m_actionGroup->addAction(menu.addAction(tr("search '%1 - %2'").arg(songName).arg(artistName)))->setData(3);
}
