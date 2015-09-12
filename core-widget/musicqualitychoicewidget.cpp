#include "musicqualitychoicewidget.h"
#include "musicuiobject.h"

#include <QMenu>
#include <QWidgetAction>
#include <QPainter>
#include <QCheckBox>

MusicQualityRadioDelegate::MusicQualityRadioDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_checkBox  = new QCheckBox;
    m_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
}

MusicQualityRadioDelegate::~MusicQualityRadioDelegate()
{
    delete m_checkBox;
}

QSize MusicQualityRadioDelegate::sizeHint(const QStyleOptionViewItem &option,
                                          const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void MusicQualityRadioDelegate::paint(QPainter *painter,
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



MusicQualityChoiceTableWidget::MusicQualityChoiceTableWidget(QWidget *parent)
    :MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 60);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 25);

    MusicQualityRadioDelegate *delegate = new MusicQualityRadioDelegate(this);
    setItemDelegateForColumn(2, delegate);
    m_previousClickRow = 0;

    createItems();
    setFixedSize(110, 90);
}

MusicQualityChoiceTableWidget::~MusicQualityChoiceTableWidget()
{
    clear();
}

void MusicQualityChoiceTableWidget::createItems()
{
    setRowCount(3);

    QTableWidgetItem *item = new QTableWidgetItem(tr("ST"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(0, 0, item);

                      item = new QTableWidgetItem(tr("HD"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(1, 0, item);

                      item = new QTableWidgetItem(tr("SD"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(2, 0, item);

                      item = new QTableWidgetItem;
    setItem(0, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/image/hdQuality"));
    setItem(1, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/image/sdQuality"));
    setItem(2, 1, item);

                      item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, true);
    setItem(0, 2, item);

                      item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(1, 2, item);

                      item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(2, 2, item);

}

void MusicQualityChoiceTableWidget::listCellClicked(int row, int)
{
    if(m_previousClickRow != -1)
    {
        item(m_previousClickRow, 2)->setData(Qt::DisplayRole, false);
    }
    m_previousClickRow = row;
    item(row, 2)->setData(Qt::DisplayRole, true);
}



MusicQualityChoiceWidget::MusicQualityChoiceWidget(QWidget *parent)
    : QToolButton(parent)
{
    setFixedSize(45, 20);
    initWidget();
}

MusicQualityChoiceWidget::~MusicQualityChoiceWidget()
{

}

void MusicQualityChoiceWidget::initWidget()
{
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet(MusicUIObject::MMenuStyle01);
    QWidgetAction *actionWidget = new QWidgetAction(menu);
    MusicQualityChoiceTableWidget *containWidget = new MusicQualityChoiceTableWidget(menu);

    actionWidget->setDefaultWidget(containWidget);
    menu->addAction(actionWidget);
    setMenu(menu);
    setPopupMode(QToolButton::InstantPopup);
}
