#include "musicqualitychoicepopwidget.h"
#include "musicqualitywidgetuiobject.h"
#include "musicrightareawidget.h"

#include <QBoxLayout>

#define HOVER_COLOR     QColor(255, 255, 255)
#define PREVIOUS_COLOR  QColor(187, 187, 187)

MusicQualityChoiceTableWidget::MusicQualityChoiceTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 60);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 25);

    setStyleSheet(TTK::UI::TableWidgetStyle03 + TTK::UI::TableWidgetStyle04);

    TTK::Widget::setTransparent(this, 0);
#if defined Q_OS_UNIX && !TTK_QT_VERSION_CHECK(5,7,0) //Fix linux selection-background-color stylesheet bug
    TTK::Widget::setTransparent(this, QColor(50, 50, 50));
#endif
    TTKCheckBoxItemDelegate *delegate = new TTKCheckBoxItemDelegate(this);
    delegate->setStyleSheet(TTK::UI::CheckBoxStyle02);
    setItemDelegateForColumn(2, delegate);
    m_previousClickRow = 0;

    initialize();
}

MusicQualityChoiceTableWidget::~MusicQualityChoiceTableWidget()
{
    removeItems();
}

void MusicQualityChoiceTableWidget::initialize()
{
    setRowCount(4);

    QTableWidgetItem *item = new QTableWidgetItem(tr("SD"));
    item->setForeground(PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(0, 0, item);

                      item = new QTableWidgetItem(tr("HQ"));
    item->setForeground(PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(1, 0, item);

                      item = new QTableWidgetItem(tr("SQ"));
    item->setForeground(PREVIOUS_COLOR);
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(2, 0, item);

                      item = new QTableWidgetItem(tr("CD"));
    item->setForeground(PREVIOUS_COLOR);
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
    item->setData(TTK_CHECKED_ROLE, Qt::Checked);
    setItem(0, 2, item);

                      item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    setItem(1, 2, item);

                      item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    setItem(2, 2, item);

                      item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    setItem(3, 2, item);
}

void MusicQualityChoiceTableWidget::itemCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 0);
    if(it)
    {
        it->setForeground(PREVIOUS_COLOR);
    }

    MusicAbstractTableWidget::itemCellEntered(row, column);

    it = item(row, 0);
    if(it)
    {
        setRowColor(row, QColor(20, 20, 20, 200));
        it->setForeground(HOVER_COLOR);
    }
}

void MusicQualityChoiceTableWidget::itemCellClicked(int row, int column)
{
    MusicAbstractTableWidget::itemCellClicked(row, column);

    if(m_previousClickRow != -1)
    {
        item(m_previousClickRow, 2)->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    }

    m_previousClickRow = row;
    item(row, 2)->setData(TTK_CHECKED_ROLE, Qt::Checked);
}



MusicQualityChoicePopWidget::MusicQualityChoicePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    setToolTip(tr("Quality Choice"));
    setFixedSize(48, 20);

    initialize();

    setStyleSheet(TTK::UI::ToolButtonStyle05 + TTK::UI::BtnQuality + "QToolButton{ margin-left:-48px; }");
}

void MusicQualityChoicePopWidget::initialize()
{
    setTranslucentBackground();
    m_menu->setStyleSheet(TTK::UI::MenuStyle04);

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
    TTK::QueryQuality quality = TTK::QueryQuality::Standard;
    switch(row)
    {
        case 0:
        {
            style = "QToolButton{ margin-left:-0px; }";
            quality = TTK::QueryQuality::Standard;
            break;
        }
        case 1:
        {
            style = "QToolButton{ margin-left:-48px; }";
            quality = TTK::QueryQuality::High;
            break;
        }
        case 2:
        {
            style = "QToolButton{ margin-left:-96px; }";
            quality = TTK::QueryQuality::Super;
            break;
        }
        case 3:
        {
            style = "QToolButton{ margin-left:-144px; }";
            quality = TTK::QueryQuality::Lossless;
            break;
        }
        default: break;
    }

    setStyleSheet(styleSheet() + style);
    MusicRightAreaWidget::instance()->researchQueryByQuality(quality);
}
