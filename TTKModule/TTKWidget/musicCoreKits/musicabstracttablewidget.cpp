#include "musicabstracttablewidget.h"

MusicAbstractTableWidget::MusicAbstractTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setColumnCount(3);
    setRowCount(0);
    setShowGrid(false);//Does not display the grid

    QHeaderView *headerview = horizontalHeader();
    headerview->setMinimumSectionSize(0);
    headerview->setVisible(false);
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 247);
    headerview->resizeSection(2, 45);
    verticalHeader()->setVisible(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);  //Open the capture mouse function
    setStyleSheet(MusicUIObject::MQSSTableWidgetStyle01 + MusicUIObject::MQSSScrollBarStyle01 + MusicUIObject::MQSSLineEditStyle01);

    QFont f = font();
    f.setBold(false);
    setFont(f);

    //Set the color of selected row
    setFrameShape(QFrame::NoFrame);//Set No Border
    setEditTriggers(QTableWidget::NoEditTriggers);//No edit
    setSelectionBehavior(QTableWidget::SelectRows);
    //Multi-line election
    setSelectionMode(QAbstractItemView::SingleSelection);
    setFocusPolicy(Qt::NoFocus);

    MusicUtils::Widget::setTransparent(this, 50);
    m_previousColorRow = -1;
    m_previousClickRow = -1;
    m_defaultBkColor = QColor(255, 255, 255, 0);

    connect(this, SIGNAL(cellEntered(int,int)), SLOT(itemCellEntered(int,int)));
    connect(this, SIGNAL(cellClicked(int,int)), SLOT(itemCellClicked(int,int)));
}

MusicAbstractTableWidget::~MusicAbstractTableWidget()
{

}

void MusicAbstractTableWidget::clear()
{
    clearContents();
    setRowCount(0);
}

TTKIntList MusicAbstractTableWidget::getMultiSelectedIndexs() const
{
    TTKIntSet rows;
    for(const QModelIndex& index : selectedIndexes())
    {
        rows.insert(index.row());
    }

    TTKIntList rowsList = rows.values();
    std::sort(rowsList.begin(), rowsList.end());
    return rowsList;
}

void MusicAbstractTableWidget::itemCellEntered(int row, int column)
{
    if(item(m_previousColorRow, 0))
    {
       setRowColor(m_previousColorRow, m_defaultBkColor);
    }

    if(item(row, column))
    {
       setRowColor(row, QColor(20, 20, 20, 20));
    }

    m_previousColorRow = row;
}

void MusicAbstractTableWidget::setRowColor(int row, const QColor &color) const
{
    for(int col=0; col<columnCount(); col++)
    {
        QTableWidgetItem *it = item(row, col);
        if(it)
        {
#if TTK_QT_VERSION_CHECK(5,13,0)
            it->setBackground(color);
#else
            it->setBackgroundColor(color);
#endif
        }
    }
}
