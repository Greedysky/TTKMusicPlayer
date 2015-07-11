#include "musiclocalsongsearchpopwidget.h"
#include "musiclocalsongsearchrecordobject.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDateTime>
#include <QFontMetrics>

MusicLocalSongSearchPopTableWidget::MusicLocalSongSearchPopTableWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,210);
    headerview->resizeSection(1,62);
    setTransparent(255);
}

MusicLocalSongSearchPopTableWidget::~MusicLocalSongSearchPopTableWidget()
{
    clearAllItems();
}

void MusicLocalSongSearchPopTableWidget::clearAllItems()
{
    clearContents();
    setColumnCount(2);
    setRowCount(0);
}

void MusicLocalSongSearchPopTableWidget::createItems(int index, const QString& name,
                                                     const QString &time)
{
    setRowHeight(index, 20);

    QTableWidgetItem *item0 = new QTableWidgetItem(name);
    item0->setTextColor(QColor(50,50,50));
    item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(time);
    item1->setTextColor(QColor(50,50,50));
    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item1);
}


MusicLocalSongSearchPopWidget::MusicLocalSongSearchPopWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_popTableWidget = new MusicLocalSongSearchPopTableWidget(this);
    m_clearButton = new QPushButton(tr("clear"), this);
    layout->addWidget(m_popTableWidget);
    layout->addWidget(m_clearButton);
    setLayout(layout);

    m_clearButton->setStyleSheet("text-align:left; font-weight:bold; font-size:15px;"
                                 "color:red; height:20px; border:none;"
                                 "background-color:white;");
    connect(m_clearButton,SIGNAL(clicked()),SLOT(clearButtonClicked()));
    connect(m_popTableWidget,SIGNAL(cellDoubleClicked(int,int)),
                             SLOT(cellDoubleClicked(int,int)));
}

MusicLocalSongSearchPopWidget::~MusicLocalSongSearchPopWidget()
{
    delete m_popTableWidget;
    delete m_clearButton;
}

void MusicLocalSongSearchPopWidget::createItems()
{
    m_popTableWidget->clearAllItems();
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig()) return;

    QStringList names, times;
    search.readSearchConfig(names, times);
    int count = names.count();
    setGeometry(484, 35, 285, count < 5 ? (20 + count*20) : 120 );

    m_popTableWidget->setRowCount( count );
    for(int i=0; i<count; ++i)
    {
        m_popTableWidget->createItems(i, names[i], utcTimeToLocal(times[i]));
    }
}

QString MusicLocalSongSearchPopWidget::utcTimeToLocal(const QString& time)
{
    qint64 t = (QDateTime::currentMSecsSinceEpoch() - time.toLongLong()) / 1000;
    if(t < 60)
        return QString::number(t) + tr("ss");
    else if(60 <= t && t < 60*60)
        return QString::number(t/60) + tr("mm");
    else if(60*60 <= t && t < 60*60*24)
        return QString::number(t/60/60) + tr("hh");
    else
        return QString::number(t/60/60/24) + tr("day");
}

void MusicLocalSongSearchPopWidget::clearButtonClicked()
{
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig()) return;
    search.writeSearchConfig(QStringList(), QStringList());
}

void MusicLocalSongSearchPopWidget::cellDoubleClicked(int row,int)
{
    emit setText( m_popTableWidget->item(row, 0)->text() );
    lower();
    hide();
}
