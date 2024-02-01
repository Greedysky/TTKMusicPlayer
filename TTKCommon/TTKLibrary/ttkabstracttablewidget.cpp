#include "ttkabstracttablewidget.h"

TTKAbstractTableWidget::TTKAbstractTableWidget(QWidget *parent)
    : QTableWidget(parent),
      m_previousColorRow(-1),
      m_previousClickRow(-1),
      m_backgroundColor(255, 255, 255, 0)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setColumnCount(3);
    setRowCount(0);

    QHeaderView *headerView = horizontalHeader();
    headerView->setMinimumSectionSize(0);
    headerView->setVisible(false);
    headerView->resizeSection(0, 20);
    headerView->resizeSection(1, 247);
    headerView->resizeSection(2, 45);
    verticalHeader()->setVisible(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);  //Open the capture mouse function

    QFont font = this->font();
    font.setBold(false);
    setFont(font);

    setShowGrid(false);//Does not display the grid
    setFrameShape(QFrame::NoFrame);//Set No Border
    setEditTriggers(QAbstractItemView::NoEditTriggers);//No edit
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setFocusPolicy(Qt::NoFocus);

    QPalette plt = palette();
    plt.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, 150)));
#if defined Q_OS_UNIX && !TTK_QT_VERSION_CHECK(5,7,0) //Fix linux selection-background-color stylesheet bug
    plt.setBrush(QPalette::Highlight, QColor(20, 20, 20, 20));
#endif
    setPalette(plt);

    connect(this, SIGNAL(cellEntered(int,int)), SLOT(itemCellEntered(int,int)));
    connect(this, SIGNAL(cellClicked(int,int)), SLOT(itemCellClicked(int,int)));
}

TTKAbstractTableWidget::~TTKAbstractTableWidget()
{

}

TTKIntList TTKAbstractTableWidget::multiSelectedIndexList() const
{
    TTKIntSet rows;
    for(const QModelIndex& index : selectedIndexes())
    {
        rows.insert(index.row());
    }

    TTKIntList indexs = rows.values();
    std::sort(indexs.begin(), indexs.end());
    return indexs;
}

void TTKAbstractTableWidget::itemCellEntered(int row, int column)
{
    if(item(m_previousColorRow, 0))
    {
       setRowColor(m_previousColorRow, m_backgroundColor);
    }

    if(item(row, column))
    {
       setRowColor(row, QColor(20, 20, 20, 20));
    }

    m_previousColorRow = row;
}

void TTKAbstractTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void TTKAbstractTableWidget::removeItems()
{
    clearContents();
    setRowCount(0);

    m_previousColorRow = -1;
    m_previousClickRow = -1;
    m_backgroundColor = Qt::transparent;
}

void TTKAbstractTableWidget::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    itemCellEntered(-1, -1);
}

void TTKAbstractTableWidget::setRowColor(int row, const QColor &color) const
{
    for(int i = 0; i < columnCount(); ++i)
    {
        QTableWidgetItem *it = item(row, i);
        if(it)
        {
            it->setBackground(color);
        }
    }
}
