#include "musicqualitychoicewidget.h"
#include "musicuiobject.h"
#include "musicitemdelegate.h"
#include "musicconnectionpool.h"
#include "musicrightareawidget.h"

#include <QBoxLayout>

MusicQualityChoiceTableWidget::MusicQualityChoiceTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 60);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 25);

    MusicCheckBoxDelegate *delegate = new MusicCheckBoxDelegate(this);
    setItemDelegateForColumn(2, delegate);
    m_previousClickRow = 1;

    createItems();
}

MusicQualityChoiceTableWidget::~MusicQualityChoiceTableWidget()
{
    clear();
}

QString MusicQualityChoiceTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQualityChoiceTableWidget::createItems()
{
    setRowCount(5);

    QTableWidgetItem *item = new QTableWidgetItem(tr("ST"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(0, 0, item);

                      item = new QTableWidgetItem(tr("SD"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(1, 0, item);

                      item = new QTableWidgetItem(tr("HD"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(2, 0, item);

                      item = new QTableWidgetItem(tr("SQ"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(3, 0, item);

                      item = new QTableWidgetItem(tr("CD"));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(4, 0, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_st_quality"));
    setItem(0, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_sd_quality"));
    setItem(1, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_hd_quality"));
    setItem(2, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_sq_quality"));
    setItem(3, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/lb_cd_quality"));
    setItem(4, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(0, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, true);
    setItem(1, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(2, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(3, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(4, 2, item);
}

void MusicQualityChoiceTableWidget::listCellClicked(int row, int)
{
    if(m_previousClickRow != -1)
    {
        item(m_previousClickRow, 2)->setData(MUSIC_CHECK_ROLE, false);
    }
    m_previousClickRow = row;
    item(row, 2)->setData(MUSIC_CHECK_ROLE, true);
}



MusicQualityChoiceWidget::MusicQualityChoiceWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    setText(tr("SD-text"));
    setToolTip(tr("Quality Choice"));
    setFixedSize(45, 20);

    initWidget();

    setStyleSheet(MusicUIObject::MToolButtonStyle09);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicRightAreaWidget::getClassName());
}

QString MusicQualityChoiceWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicQualityChoiceWidget::initWidget()
{
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QHBoxLayout *layout = new QHBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    MusicQualityChoiceTableWidget *table = new MusicQualityChoiceTableWidget(m_containWidget);
    connect(table, SIGNAL(cellClicked(int ,int)), SLOT(listCellClicked(int)));
    layout->addWidget(table);
    m_containWidget->setFixedSize(110, 150);

    m_containWidget->setLayout(layout);
}

void MusicQualityChoiceWidget::listCellClicked(int row)
{
    m_menu->close();
    switch(row)
    {
        case 0:
            {
                setText(tr("ST-text"));
                m_currentQuality = tr("ST");
                break;
            }
        case 1:
            {
                setText(tr("SD-text"));
                m_currentQuality = tr("SD");
                break;
            }
        case 2:
            {
                setText(tr("HD-text"));
                m_currentQuality = tr("HD");
                break;
            }
        case 3:
            {
                setText(tr("SQ-text"));
                m_currentQuality = tr("SQ");
                break;
            }
        case 4:
            {
                setText(tr("CD-text"));
                m_currentQuality = tr("CD");
                break;
            }
    }
    emit researchQueryByQuality(m_currentQuality);
}
