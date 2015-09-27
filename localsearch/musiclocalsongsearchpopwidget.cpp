#include "musiclocalsongsearchpopwidget.h"
#include "musiclocalsongsearchrecordobject.h"
#include "musictime.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDateTime>
#include <QFontMetrics>

MusicLocalSongSearchPopTableWidget::MusicLocalSongSearchPopTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,215);
    headerview->resizeSection(1,62);
    setTransparent(255);
}

MusicLocalSongSearchPopTableWidget::~MusicLocalSongSearchPopTableWidget()
{
    clearAllItems();
}

void MusicLocalSongSearchPopTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(2);
}

void MusicLocalSongSearchPopTableWidget::createItems(int index, const QString &name,
                                                     const QString &time)
{
    setRowHeight(index, 20);

    QTableWidgetItem *item0 = new QTableWidgetItem(name);
    item0->setTextColor(QColor(50,50,50));
    item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem(time);
    item1->setTextColor(QColor(50,50,50));
    item1->setTextAlignment(Qt::AlignCenter);
    setItem(index, 1, item1);
}

void MusicLocalSongSearchPopTableWidget::listCellClicked(int row, int)
{
    emit setText( item(row, 0)->text() );
    QWidget *widget = static_cast<QWidget*>(parent());
    widget->lower();
    widget->hide();
}


MusicLocalSongSearchPopWidget::MusicLocalSongSearchPopWidget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(504, 55, 0, 0);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_popTableWidget = new MusicLocalSongSearchPopTableWidget(this);
    m_clearButton = new QPushButton(tr("clear"), this);
    layout->addWidget(m_popTableWidget);
    layout->addWidget(m_clearButton);
    setLayout(layout);

    m_clearButton->setStyleSheet(MusicUIObject::MCustomStyle07);
    connect(m_clearButton, SIGNAL(clicked()), SLOT(clearButtonClicked()));
    connect(m_popTableWidget, SIGNAL(setText(QString)), SIGNAL(setText(QString)));
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
    if(!search.readSearchXMLConfig())
    {
        return;
    }

    MusicSearchRecord record;
    search.readSearchConfig( record );
    int count = record.m_names.count();
    resize(285, count == 0 ? 40 : (count < 5 ? (20 + count*20) : 120) );

    m_popTableWidget->setRowCount( count );
    for(int i=0; i<count; ++i)
    {
        m_popTableWidget->createItems(i, record.m_names[i],
                                      utcTimeToLocal(record.m_times[i]));
    }
}

QString MusicLocalSongSearchPopWidget::utcTimeToLocal(const QString &time) const
{
    qint64 t = (QDateTime::currentMSecsSinceEpoch() - time.toLongLong()) / 1000;
    return MusicTime::normalTime2Label(t);
}

void MusicLocalSongSearchPopWidget::clearButtonClicked()
{
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig())
    {
        return;
    }
    search.writeSearchConfig( MusicSearchRecord() );
}

void MusicLocalSongSearchPopWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    lower();
    hide();
}
