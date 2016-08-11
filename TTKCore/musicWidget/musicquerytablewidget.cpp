#include "musicquerytablewidget.h"
#include "musicitemdelegate.h"
#include "musicdownloadqueryfactory.h"

#include <QPainter>
#include <QActionGroup>

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    m_downLoadManager = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(createSearchedItems(QString,QString,QString)),
                               SLOT(createSearchedItems(QString,QString,QString)));

    MusicUtils::UWidget::setTransparent(this, 255);
    setStyleSheet( styleSheet() + MusicUIObject::MTableWidgetStyle02);

    m_checkBoxDelegate = new MusicQueryTableDelegate(this);
    setItemDelegateForColumn(0, m_checkBoxDelegate);

    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionGroupClick(QAction*)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_downLoadManager;
    delete m_actionGroup;
    delete m_checkBoxDelegate;
}

QString MusicQueryTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQueryTableWidget::listCellClicked(int row, int column)
{
    if(column == 0)
    {
        QTableWidgetItem *it = item(row, 0);
        bool status = it->data(MUSIC_CHECK_ROLE).toBool();
        it->setData(MUSIC_CHECK_ROLE, !status);
    }
    else
    {
        if(m_previousClickRow != -1)
        {
            item(m_previousClickRow, 0)->setData(MUSIC_CHECK_ROLE, false);
        }
        m_previousClickRow = row;
        item(row, 0)->setData(MUSIC_CHECK_ROLE, true);
    }
}

void MusicQueryTableWidget::setSelectedAllItems(bool all)
{
    for(int i=0; i<rowCount(); ++i)
    {
        item(i, 0)->setData(MUSIC_CHECK_ROLE, all);
    }
}

void MusicQueryTableWidget::actionGroupClick(QAction *action)
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
        case 1: emit restartSearchQuery(songName); break;
        case 2: emit restartSearchQuery(artistName); break;
        case 3: emit restartSearchQuery(songName + "-" + artistName); break;
    }
}

void MusicQueryTableWidget::createContextMenu(QMenu &menu)
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

MusicObject::MIntList MusicQueryTableWidget::getSelectedItems() const
{
    MusicObject::MIntList list;
    for(int i=0; i<rowCount(); ++i)
    {
        if(item(i, 0)->data(MUSIC_CHECK_ROLE) == true)
        {
            list << i;
        }
    }
    return list;
}
