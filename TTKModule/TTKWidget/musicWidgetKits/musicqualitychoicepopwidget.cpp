#include "musicqualitychoicepopwidget.h"
#include "musicqualitywidgetuiobject.h"
#include "musicitemdelegate.h"
#include "musicrightareawidget.h"

#include <QBoxLayout>

#define PREVIOUS_COLOR  QColor(187, 187, 187)
#define HOVER_COLOR     QColor(255, 255, 255)

MusicQualityChoiceTableWidget::MusicQualityChoiceTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 60);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 25);

    MusicUtils::Widget::setTransparent(this, 0);
    setStyleSheet(MusicUIObject::MQSSTableWidgetStyle03 + MusicUIObject::MQSSTableWidgetStyle04);

    MusicCheckBoxDelegate *delegate = new MusicCheckBoxDelegate(this);
    delegate->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle02);
    setItemDelegateForColumn(2, delegate);
    m_previousClickRow = 0;

    createItems();
}

MusicQualityChoiceTableWidget::~MusicQualityChoiceTableWidget()
{
    clear();
}

void MusicQualityChoiceTableWidget::createItems()
{
    setRowCount(4);

    QTableWidgetItem *item = new QTableWidgetItem(tr("SD"));
    QtItemSetForegroundColor(item, PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(0, 0, item);

                      item = new QTableWidgetItem(tr("HQ"));
    QtItemSetForegroundColor(item, PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(1, 0, item);

                      item = new QTableWidgetItem(tr("SQ"));
    QtItemSetForegroundColor(item, PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(2, 0, item);

                      item = new QTableWidgetItem(tr("CD"));
    QtItemSetForegroundColor(item, PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(3, 0, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_sd_quality"));
    setItem(0, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_hd_quality"));
    setItem(1, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_sq_quality"));
    setItem(2, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_cd_quality"));
    setItem(3, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Checked);
    setItem(0, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
    setItem(1, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
    setItem(2, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
    setItem(3, 2, item);
}

void MusicQualityChoiceTableWidget::itemCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 0);
    if(it)
    {
        QtItemSetForegroundColor(it, PREVIOUS_COLOR);
    }

    MusicAbstractTableWidget::itemCellEntered(row, column);

    it = item(row, 0);
    if(it)
    {
        setRowColor(row, QColor(20, 20, 20, 200));
        QtItemSetForegroundColor(it, HOVER_COLOR);
    }
}

void MusicQualityChoiceTableWidget::itemCellClicked(int row, int)
{
    if(m_previousClickRow != -1)
    {
        item(m_previousClickRow, 2)->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
    }

    m_previousClickRow = row;
    item(row, 2)->setData(MUSIC_CHECK_ROLE, Qt::Checked);
}



MusicQualityChoicePopWidget::MusicQualityChoicePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    setToolTip(tr("Quality Choice"));
    setFixedSize(48, 20);

    initialize();

    setStyleSheet(MusicUIObject::MQSSToolButtonStyle05 + MusicUIObject::MQSSBtnQuality + "QToolButton{ margin-left:-48px; }");
}

void MusicQualityChoicePopWidget::initialize()
{
    setTranslucentBackground();
    m_menu->setStyleSheet(MusicUIObject::MQSSMenuStyle04);

    QHBoxLayout *layout = new QHBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    MusicQualityChoiceTableWidget *table = new MusicQualityChoiceTableWidget(m_containWidget);
    connect(table, SIGNAL(cellClicked(int ,int)), SLOT(itemCellClicked(int)));
    layout->addWidget(table);

    m_containWidget->setFixedSize(110, 120);
    m_containWidget->setLayout(layout);
}

void MusicQualityChoicePopWidget::itemCellClicked(int row)
{
    m_menu->close();

    QString style;
    MusicObject::QueryQuality quality = MusicObject::QueryQuality::Standard;
    switch(row)
    {
        case 0:
        {
            style = "QToolButton{ margin-left:-0px; }";
            quality = MusicObject::QueryQuality::Standard;
            break;
        }
        case 1:
        {
            style = "QToolButton{ margin-left:-48px; }";
            quality = MusicObject::QueryQuality::High;
            break;
        }
        case 2:
        {
            style = "QToolButton{ margin-left:-96px; }";
            quality = MusicObject::QueryQuality::Super;
            break;
        }
        case 3:
        {
            style = "QToolButton{ margin-left:-144px; }";
            quality = MusicObject::QueryQuality::Lossless;
            break;
        }
        default: break;
    }

    setStyleSheet(styleSheet() + style);
    MusicRightAreaWidget::instance()->researchQueryByQuality(quality);
}
