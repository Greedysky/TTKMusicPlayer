#include "musicitemdelegate.h"
#include "musicuiobject.h"

#include <QPainter>
#include <QCheckBox>


MusicCheckBoxDelegate::MusicCheckBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_checkBox  = new QCheckBox;
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
}

MusicCheckBoxDelegate::~MusicCheckBoxDelegate()
{
    delete m_checkBox;
}

QSize MusicCheckBoxDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicCheckBoxDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    painter->save();
    int minSize = qMin(option.rect.width(), option.rect.height());
    m_checkBox->resize(minSize, minSize);
    m_checkBox->setChecked( index.data(Qt::DisplayRole).toBool() );
    painter->translate((option.rect.width() - 16)/2, 0); // top left
    m_checkBox->render(painter, option.rect.topLeft(), QRegion(),
                       QWidget::DrawChildren);
    painter->restore();
}


MusicQueryTableDelegate::MusicQueryTableDelegate(QObject *parent)
    : MusicCheckBoxDelegate(parent)
{

}

MusicQueryTableDelegate::~MusicQueryTableDelegate()
{

}


void MusicQueryTableDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    painter->fillRect(option.rect,
                      static_cast<Qt::GlobalColor>(index.data(Qt::UserRole + 1).toInt()));
    MusicCheckBoxDelegate::paint(painter, option, index);
}
