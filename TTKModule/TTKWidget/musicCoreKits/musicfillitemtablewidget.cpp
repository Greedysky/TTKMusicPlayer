#include "musicfillitemtablewidget.h"
#include "musicitemdelegate.h"

MusicFillItemTableWidget::MusicFillItemTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    MusicUtils::Widget::setTransparent(this, 255);
    setStyleSheet( styleSheet() + MusicUIObject::MTableWidgetStyle02);

    m_checkBoxDelegate = new MusicQueryTableDelegate(this);
    setItemDelegateForColumn(0, m_checkBoxDelegate);
}

MusicFillItemTableWidget::~MusicFillItemTableWidget()
{
    clear();
    delete m_checkBoxDelegate;
}

MIntList MusicFillItemTableWidget::getSelectedItems() const
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

void MusicFillItemTableWidget::listCellClicked(int row, int column)
{
    if(column == 0)
    {
        QTableWidgetItem *it = item(row, 0);
        const bool status = it->data(MUSIC_CHECK_ROLE).toBool();
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

void MusicFillItemTableWidget::setSelectedAllItems(bool all)
{
    for(int i=0; i<rowCount(); ++i)
    {
        item(i, 0)->setData(MUSIC_CHECK_ROLE, all);
    }
    clearSelection();
}
