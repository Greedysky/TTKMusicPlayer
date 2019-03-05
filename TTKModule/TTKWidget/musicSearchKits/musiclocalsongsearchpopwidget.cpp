#include "musiclocalsongsearchpopwidget.h"
#include "musiclocalsongsearchrecordconfigmanager.h"
#include "musictime.h"
#include "musicwidgetheaders.h"

#include <QPainter>

MusicLocalSongSearchPopTableWidget::MusicLocalSongSearchPopTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 215);
    headerview->resizeSection(1, 62);
    MusicUtils::Widget::setTransparent(this, 255);
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

void MusicLocalSongSearchPopTableWidget::createItems(int index, const QString &name, const QString &time)
{
    setRowHeight(index, ITEM_ROW_HEIGHT_M);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem(MusicUtils::Widget::elidedText(font(), "  " + name, Qt::ElideRight, headerview->sectionSize(0) - 20));
    item->setToolTip(name);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 0, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(index, 1, item);
}

void MusicLocalSongSearchPopTableWidget::listCellClicked(int row, int)
{
    emit setText( item(row, 0)->toolTip().trimmed() );

    QWidget *widget = MStatic_cast(QWidget*, parent());
    widget->lower();
    widget->hide();
}


MusicLocalSongSearchPopWidget::MusicLocalSongSearchPopWidget(QWidget *parent)
    : QWidget(parent)
{
    move(405, 45);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    m_popTableWidget = new MusicLocalSongSearchPopTableWidget(this);
    m_popTableWidget->setFixedWidth(285);
    m_clearButton = new QPushButton("   " + tr("clear"), this);
    m_clearButton->setCursor(Qt::PointingHandCursor);
    m_clearButton->setFixedHeight(35);
    m_clearButton->setStyleSheet(MusicUIObject::MCustomStyle01 + MusicUIObject::MFontStyle02 + MusicUIObject::MColorStyle03 + MusicUIObject::MBorderStyle01 + MusicUIObject::MBackgroundStyle17);
#ifdef Q_OS_UNIX
    m_clearButton->setFocusPolicy(Qt::NoFocus);
#endif

    QFrame *frame = new QFrame(this);
    frame->setFixedHeight(1);
    frame->setStyleSheet(MusicUIObject::MBackgroundStyle03);
    frame->setFrameShape(QFrame::HLine);

    layout->addWidget(m_popTableWidget);
    layout->addWidget(frame);
    layout->addWidget(m_clearButton);
    setLayout(layout);

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
    m_clearButton->show();
    m_popTableWidget->clearAllItems();

    MusicLocalSongSearchRecordConfigManager search(this);
    if(!search.readConfig())
    {
        return;
    }

    MusicSearchRecords records;
    search.readSearchData( records );

    const int count = records.count();
    resize(m_popTableWidget->width() + 2, count == 0 ? 0 : (count < 6 ? count*ITEM_ROW_HEIGHT_M + 45 : 7*ITEM_ROW_HEIGHT_M + 8) );

    m_popTableWidget->setRowCount( count );
    for(int i=0; i<count; ++i)
    {
        m_popTableWidget->createItems(i, records[i].m_name, utcTimeToLocal(records[i].m_time));
    }
}

void MusicLocalSongSearchPopWidget::createSuggestItems(const QStringList &names)
{
    m_clearButton->hide();
    m_popTableWidget->clearAllItems();

    const int count = names.count();
    resize(m_popTableWidget->width() + 2, count == 0 ? 0 : (count < 6 ? count*ITEM_ROW_HEIGHT_M + 8 : 6*ITEM_ROW_HEIGHT_M + 8) );

    m_popTableWidget->setRowCount( count );
    for(int i=0; i<count; ++i)
    {
        m_popTableWidget->createItems(i, names[i], QString());
    }
}

QString MusicLocalSongSearchPopWidget::utcTimeToLocal(const QString &time) const
{
    const qint64 t = (MusicTime::timeStamp() - time.toLongLong()) / MT_S2MS;
    return MusicTime::normalTime2Label(t);
}

void MusicLocalSongSearchPopWidget::clearButtonClicked()
{
    MusicLocalSongSearchRecordConfigManager search(this);
    if(!search.readConfig())
    {
        return;
    }
    search.writeSearchData( MusicSearchRecords() );
    close();
}

void MusicLocalSongSearchPopWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::gray);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void MusicLocalSongSearchPopWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    lower();
    hide();
}
