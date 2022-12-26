#include "musicitemdelegate.h"
#include "musicitemrenameedit.h"

MusicLineEditItemDelegate::MusicLineEditItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

QWidget *MusicLineEditItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QString text;
    const QAbstractItemModel *model = index.model();
    if(model)
    {
        text = model->data(index, Qt::DisplayRole).toString();
    }

    return new MusicItemRenameEidt(text, parent);
}
