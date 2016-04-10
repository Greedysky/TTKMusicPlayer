#include "musicconnecttransfertablewidget.h"

MusicConnectTransferTableWidget::MusicConnectTransferTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::setTransparent(this, 255);

    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 283);
    headerview->resizeSection(2, 40);

    m_checkBoxDelegate = new MusicQueryTableDelegate(this);
    setItemDelegateForColumn(0, m_checkBoxDelegate);

}

MusicConnectTransferTableWidget::~MusicConnectTransferTableWidget()
{
    clear();
    delete m_checkBoxDelegate;
}

void MusicConnectTransferTableWidget::listCellClicked(int row, int col)
{
    if(col == 0)
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

MIntList MusicConnectTransferTableWidget::getSelectedItems() const
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
