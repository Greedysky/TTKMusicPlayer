#include "ttkfilterproxymodel.h"
#include "musicglobal.h"

TTKFilterProxyModel::TTKFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

void TTKFilterProxyModel::sourceModel(QAbstractItemModel *sourceModel)
{
    setSourceModel(sourceModel);
}

void TTKFilterProxyModel::sortColumn(int column, int order)
{
    if(order == -1)
    {
        order = Qt::AscendingOrder == sortOrder() ?
                Qt::DescendingOrder : Qt::AscendingOrder;
    }
    sort(column, MStatic_cast(Qt::SortOrder, order));
}
