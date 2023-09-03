#include "musicfillitemtablewidget.h"

MusicFillItemTableWidget::MusicFillItemTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setStyleSheet(styleSheet() + TTK::UI::TableWidgetStyle02);

    TTK::Widget::setTransparent(this, 255);
#if defined Q_OS_UNIX && !TTK_QT_VERSION_CHECK(5,7,0) //Fix linux selection-background-color stylesheet bug
    TTK::Widget::setTransparent(this, QColor(220, 220, 220));
#endif
    m_checkBoxDelegate = new TTKCheckBoxItemDelegate(this);
    m_checkBoxDelegate->setStyleSheet(TTK::UI::CheckBoxStyle01);
    setItemDelegateForColumn(0, m_checkBoxDelegate);
}

MusicFillItemTableWidget::~MusicFillItemTableWidget()
{
    removeItems();
    delete m_checkBoxDelegate;
}

TTKIntList MusicFillItemTableWidget::checkedIndexList() const
{
    TTKIntList list;
    for(int i = 0; i < rowCount(); ++i)
    {
        const QTableWidgetItem *it = item(i, 0);
        if(it && it->data(TTK_CHECKED_ROLE) == Qt::Checked)
        {
            list << i;
        }
    }
    return list;
}

TTKIntList MusicFillItemTableWidget::uncheckedIndexList() const
{
    TTKIntList list;
    for(int i = 0; i < rowCount(); ++i)
    {
        const QTableWidgetItem *it = item(i, 0);
        if(it && it->data(TTK_CHECKED_ROLE) == Qt::Unchecked)
        {
            list << i;
        }
    }
    return list;
}

void MusicFillItemTableWidget::itemCellClicked(int row, int column)
{
    QTableWidgetItem *it = nullptr;
    if(column == 0)
    {
        it = item(row, 0);
        const Qt::CheckState status = TTKStaticCast(Qt::CheckState, it->data(TTK_CHECKED_ROLE).toInt());
        it->setData(TTK_CHECKED_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }
    else
    {
        if(m_previousClickRow != -1 && (it = item(m_previousClickRow, 0)))
        {
            it->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        }

        m_previousClickRow = row;
        item(row, 0)->setData(TTK_CHECKED_ROLE, Qt::Checked);
    }
}

void MusicFillItemTableWidget::checkedItemsState(bool checked)
{
    for(int i = 0; i < rowCount(); ++i)
    {
        item(i, 0)->setData(TTK_CHECKED_ROLE, checked ? Qt::Checked : Qt::Unchecked);
    }

    clearSelection();
}

void MusicFillItemTableWidget::checkedItemsStatus(bool checked)
{
    for(int i = 0; i < rowCount(); ++i)
    {
        item(i, 0)->setData(TTK_CHECKED_ROLE, checked ? Qt::Checked : Qt::Unchecked);
    }

    if(!checked)
    {
        clearSelection();
        setCurrentIndex(QModelIndex());
    }
    else
    {
        selectAll();
    }
}
