#include "musicquerytablewidget.h"
#include "musicitemdelegate.h"

#include <QPainter>
#include <QActionGroup>

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    m_downLoadManager = new MusicDownLoadQueryThread(this);
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(creatSearchedItems(QString,QString,QString)),
                               SLOT(creatSearchedItems(QString,QString,QString)));

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
    QString songName = row != -1 && rowCount() > 0 ? item(row, 1)->text() : QString();
    QString artistName = row != -1 && rowCount() > 0 ? item(row, 2)->text() : QString();

    switch( findActionGroup(action) )
    {
        case 0: musicDownloadLocal(row); break;
        case 1: emit restartSearchQuery(songName); break;
        case 2: emit restartSearchQuery(artistName); break;
        case 3: emit restartSearchQuery(songName + "-" + artistName); break;
    }
}

int MusicQueryTableWidget::findActionGroup(QAction *action)
{
    int key = -1;
    QList<QAction*> actions = m_actionGroup->actions();
    for(int i=0; i<actions.count(); ++i)
    {
        if(actions[i] == action)
        {
            key = i;
            break;
        }
    }
    return key;
}

void MusicQueryTableWidget::createContextMenu(QMenu &menu)
{
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    m_actionGroup->addAction(menu.addAction(tr("musicDownload")));

    menu.addSeparator();

    QString songName = currentRow() != -1 && rowCount() > 0 ?
                item(currentRow(), 1)->text() : QString();
    QString artistName = currentRow() != -1 && rowCount() > 0 ?
                item(currentRow(), 2)->text() : QString();
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(songName)));
    m_actionGroup->addAction(menu.addAction(tr("search '%1'").arg(artistName)));
    m_actionGroup->addAction(menu.addAction(tr("search '%1 - %2'").arg(songName).arg(artistName)));
}

void MusicQueryTableWidget::paintEvent(QPaintEvent *event)
{
    MusicAbstractTableWidget::paintEvent(event);
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 0.1, Qt::SolidLine));
    for(int i=0; i<rowCount(); ++i)
    {
        painter.drawLine(10, rowHeight(0)*(i + 1),
                         width() - 15, rowHeight(0)*(i + 1));
    }
}

MIntList MusicQueryTableWidget::getSelectedItems() const
{
    MIntList list;
    for(int i=0; i<rowCount(); ++i)
    {
        if(item(i, 0)->data(MUSIC_CHECK_ROLE) == true)
        {
            list << i;
        }
    }
    return list;
}
