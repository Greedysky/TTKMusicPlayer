#include "musicqualitychoicepopwidget.h"
#include "musicuiobject.h"
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
    setStyleSheet(MusicUIObject::MTableWidgetStyle03 + MusicUIObject::MLineEditStyle01 + MusicUIObject::MTableWidgetStyle04);

    MusicCheckBoxDelegate *delegate = new MusicCheckBoxDelegate(this);
    delegate->setStyleSheet(MusicUIObject::MCheckBoxStyle02);
    setItemDelegateForColumn(2, delegate);
    m_previousClickRow = 1;

    createItems();
}

MusicQualityChoiceTableWidget::~MusicQualityChoiceTableWidget()
{
    clear();
}

void MusicQualityChoiceTableWidget::createItems()
{
    setRowCount(5);

    QTableWidgetItem *item = new QTableWidgetItem(tr("ST"));
    item->setTextColor(PREVIOUS_COLOR);
    item->setTextAlignment(Qt::AlignCenter);
    setItem(0, 0, item);

                      item = new QTableWidgetItem(tr("SD"));
    item->setTextColor(PREVIOUS_COLOR);
    item->setTextAlignment(Qt::AlignCenter);
    setItem(1, 0, item);

                      item = new QTableWidgetItem(tr("HQ"));
    item->setTextColor(PREVIOUS_COLOR);
    item->setTextAlignment(Qt::AlignCenter);
    setItem(2, 0, item);

                      item = new QTableWidgetItem(tr("SQ"));
    item->setTextColor(PREVIOUS_COLOR);
    item->setTextAlignment(Qt::AlignCenter);
    setItem(3, 0, item);

                      item = new QTableWidgetItem(tr("CD"));
    item->setTextColor(PREVIOUS_COLOR);
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

void MusicQualityChoiceTableWidget::listCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 0);
    if(it)
    {
       it->setTextColor(PREVIOUS_COLOR);
    }

    MusicAbstractTableWidget::listCellEntered(row, column);

    it = item(row, 0);
    if(it)
    {
       setRowColor(row, QColor(20, 20, 20, 200));
       it->setTextColor(HOVER_COLOR);
    }
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



MusicQualityChoicePopWidget::MusicQualityChoicePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    setToolTip(tr("Quality Choice"));
    setFixedSize(48, 20);

    initWidget();

    setStyleSheet(MusicUIObject::MToolButtonStyle05 + MusicUIObject::MKGBtnQuality + "QToolButton{ margin-left:-48px;}" );
}

void MusicQualityChoicePopWidget::initWidget()
{
    setTranslucentBackground();
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle04);

    QHBoxLayout *layout = new QHBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    MusicQualityChoiceTableWidget *table = new MusicQualityChoiceTableWidget(m_containWidget);
    connect(table, SIGNAL(cellClicked(int ,int)), SLOT(listCellClicked(int)));
    layout->addWidget(table);
    m_containWidget->setFixedSize(110, 150);

    m_containWidget->setLayout(layout);
}

void MusicQualityChoicePopWidget::listCellClicked(int row)
{
    m_menu->close();

    QString style;
    switch(row)
    {
        case 0:
            {
                style = "QToolButton{ margin-left:-0px; }";
                m_currentQuality = tr("ST");
                break;
            }
        case 1:
            {
                style = "QToolButton{ margin-left:-48px; }";
                m_currentQuality = tr("SD");
                break;
            }
        case 2:
            {
                style = "QToolButton{ margin-left:-96px; }";
                m_currentQuality = tr("HQ");
                break;
            }
        case 3:
            {
                style = "QToolButton{ margin-left:-144px; }";
                m_currentQuality = tr("SQ");
                break;
            }
        case 4:
            {
                style = "QToolButton{ margin-left:-192px; }";
                m_currentQuality = tr("CD");
                break;
            }
        default: break;
    }
    setStyleSheet( styleSheet() + style);

    MusicRightAreaWidget::instance()->researchQueryByQuality(m_currentQuality);
}
