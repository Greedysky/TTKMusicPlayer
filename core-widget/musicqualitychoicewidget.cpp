#include "musicqualitychoicewidget.h"
#include "musicuiobject.h"
#include "musicitemdelegate.h"
#include "musicconnectionpool.h"

#include <QMenu>
#include <QWidgetAction>

MusicQualityChoiceTableWidget::MusicQualityChoiceTableWidget(QWidget *parent)
    :MusicAbstractTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 60);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 25);

    MusicCheckBoxDelegate *delegate = new MusicCheckBoxDelegate(this);
    setItemDelegateForColumn(2, delegate);
    m_previousClickRow = 1;

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
    item->setIcon(QIcon(":/quality/hdQuality"));
    setItem(1, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/quality/sdQuality"));
    setItem(2, 1, item);

                      item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(0, 2, item);

                      item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, true);
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
    setText(tr("HD-text"));
    setToolTip(tr("Quality Choice"));
    setFixedSize(45, 20);
    initWidget();
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(MusicUIObject::MToolButtonStyle09);
    setEnabled(false);

    M_Connection->setValue("MusicQualityChoiceTableWidget", this);
    M_Connection->connect("MusicSongSearchOnlineTableWidget",
                          "MusicQualityChoiceTableWidget");
}

MusicQualityChoiceWidget::~MusicQualityChoiceWidget()
{
    delete m_menu;
}

void MusicQualityChoiceWidget::initWidget()
{
    m_menu = new QMenu(this);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle02);
    QWidgetAction *actionWidget = new QWidgetAction(m_menu);
    MusicQualityChoiceTableWidget *containWidget = new MusicQualityChoiceTableWidget(m_menu);
    connect(containWidget, SIGNAL(cellClicked(int ,int)), SLOT(listCellClicked(int)));

    actionWidget->setDefaultWidget(containWidget);
    m_menu->addAction(actionWidget);
    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);
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
                setText(tr("HD-text"));
                m_currentQuality = tr("HD");
                break;
            }
        case 2:
            {
                setText(tr("SD-text"));
                m_currentQuality = tr("SD");
                break;
            }
    }
    emit researchQueryByQuality();
}

void MusicQualityChoiceWidget::getQualityString(QString &string)
{
    setEnabled(true);
    string = m_currentQuality;
}
