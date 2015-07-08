#include "musictablewidgetabstract.h"

MusicTableWidgetAbstract::MusicTableWidgetAbstract(QWidget *parent)
    : QTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFont(QFont("Helvetica"));
    setColumnCount(3);
    setRowCount(0);
    setShowGrid(false);//Does not display the grid
    QHeaderView *headerview = horizontalHeader();
    headerview->setVisible(false);
    headerview->resizeSection(0,40);
    headerview->resizeSection(1,252);
    headerview->resizeSection(2,26);
    verticalHeader()->setVisible(false);
    setMouseTracking(true);  //Open the capture mouse function
    setStyleSheet("QTableWidget{selection-background-color:pink;}" + \
                             MusicObject::MusicScrollBarStyle + \
                             MusicObject::MusicListEditStyle );
    //Set the color of selected row
    setFrameShape(QFrame::NoFrame);//Set No Border
    setEditTriggers(QTableWidget::NoEditTriggers);//No edit
    setSelectionBehavior(QTableWidget::SelectRows);
    //Multi-line election
    setSelectionMode(QAbstractItemView::SingleSelection);
    setFocusPolicy(Qt::NoFocus);

    setTransparent(80);

    m_previousColorRow = -1;
    m_defaultBkColor = QColor(255,255,255,0);
}

MusicTableWidgetAbstract::~MusicTableWidgetAbstract()
{

}

void MusicTableWidgetAbstract::setTransparent(int angle)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(255,255,255,angle)));
    setPalette(pal);
}

void MusicTableWidgetAbstract::listCellEntered(int row, int column)
{
    QTableWidgetItem *item = this->item(m_previousColorRow, 0);
    if(item != 0)
       setRowColor(m_previousColorRow, m_defaultBkColor);

    item = this->item(row, column);
    if(item != 0 && !item->isSelected() && !item->text().isEmpty())
       setRowColor(row, QColor(20,20,20,40));

    m_previousColorRow = row;
}

void MusicTableWidgetAbstract::listCellClicked(int , int)
{

}

void MusicTableWidgetAbstract::setRowColor(int row,const QColor& color)
{
    for(int col=0; col<columnCount(); col++)
    {
        QTableWidgetItem *item = this->item(row, col);
        item->setBackgroundColor(color);
    }
}
