#include "musicabstracttablewidget.h"

MusicAbstractTableWidget::MusicAbstractTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFont(QFont("Helvetica"));
    setColumnCount(3);
    setRowCount(0);
    setShowGrid(false);//Does not display the grid

    QHeaderView *headerview = horizontalHeader();
    headerview->setVisible(false);
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 247);
    headerview->resizeSection(2, 45);
    verticalHeader()->setVisible(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);  //Open the capture mouse function
    setStyleSheet(MusicUIObject::MTableWidgetStyle01 + MusicUIObject::MScrollBarStyle01 + MusicUIObject::MLineEditStyle01);

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

    connect(this, SIGNAL(cellEntered(int,int)), SLOT(listCellEntered(int,int)));
    connect(this, SIGNAL(cellClicked(int,int)), SLOT(listCellClicked(int,int)));
}

MusicAbstractTableWidget::~MusicAbstractTableWidget()
{

}

void MusicAbstractTableWidget::clear()
{
    clearContents();
    setRowCount(0);
}

void MusicAbstractTableWidget::listCellEntered(int row, int column)
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
            it->setBackgroundColor(color);
        }
    }
}

MIntList MusicAbstractTableWidget::getMultiIndexSet()
{
    MIntSet rows;
    foreach(const QModelIndex& index, selectedIndexes())
    {
        rows.insert(index.row());
    }

    MIntList rowsList = rows.toList();
    qSort(rowsList);

    return rowsList;
}
