#include "musicfillitemtablewidget.h"
#include "musicitemdelegate.h"

MusicFillItemTableWidget::MusicFillItemTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    MusicUtils::Widget::setTransparent(this, 255);
    setStyleSheet( styleSheet() + MusicUIObject::MQSSTableWidgetStyle02);

    m_checkBoxDelegate = new MusicCheckBoxDelegate(this);
    m_checkBoxDelegate->setFillBackground(true);
    setItemDelegateForColumn(0, m_checkBoxDelegate);
}

MusicFillItemTableWidget::~MusicFillItemTableWidget()
{
    clear();
    delete m_checkBoxDelegate;
}

TTKIntList MusicFillItemTableWidget::getSelectedItems() const
{
    TTKIntList list;
    for(int i=0; i<rowCount(); ++i)
    {
        if(item(i, 0)->data(MUSIC_CHECK_ROLE) == Qt::Checked)
        {
            list << i;
        }
    }
    return list;
}

void MusicFillItemTableWidget::itemCellClicked(int row, int column)
{
    if(column == 0)
    {
        QTableWidgetItem *it = item(row, 0);
        const Qt::CheckState status = TTKStatic_cast(Qt::CheckState, it->data(MUSIC_CHECK_ROLE).toInt());
        it->setData(MUSIC_CHECK_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }
    else
    {
        if(m_previousClickRow != -1)
        {
            item(m_previousClickRow, 0)->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
        }
        m_previousClickRow = row;
        item(row, 0)->setData(MUSIC_CHECK_ROLE, Qt::Checked);
    }
}

void MusicFillItemTableWidget::setSelectedAllItems(bool check)
{
    for(int i=0; i<rowCount(); ++i)
    {
        item(i, 0)->setData(MUSIC_CHECK_ROLE, check ? Qt::Checked : Qt::Unchecked);
    }
    clearSelection();
}
