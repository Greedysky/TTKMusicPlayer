#include "musicsongsearchpopwidget.h"
#include "musicsongsearchrecordconfigmanager.h"
#include "musicwidgetheaders.h"

#define MAX_ITEM_COUNT  7

MusicSongSearchPopTableWidget::MusicSongSearchPopTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 215);
    headerview->resizeSection(1, 62);

    TTK::Widget::setTransparent(this, 255);
}

MusicSongSearchPopTableWidget::~MusicSongSearchPopTableWidget()
{
    removeItems();
}

void MusicSongSearchPopTableWidget::selectRow(bool up)
{
    const int row = rowCount();
    if(row <= 0)
    {
        return;
    }

    int index = currentRow();
    if(up)
    {
        --index;
        if(index < 0)
        {
            index = 0;
        }
    }
    else
    {
        ++index;
        if(index >= row)
        {
            index = row - 1;
        }
    }

    MusicAbstractTableWidget::selectRow(index);
    Q_EMIT setText(item(index, 0)->toolTip().trimmed(), false);
}

void MusicSongSearchPopTableWidget::addCellItem(int index, const QString &name, const QString &time)
{
    setRowHeight(index, TTK_ITEM_SIZE_M);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem(TTK::Widget::elidedText(font(), "  " + name, Qt::ElideRight, headerview->sectionSize(0) - 20));
    item->setToolTip(name);
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 0, item);

                      item = new QTableWidgetItem(time);
    item->setForeground(QColor(TTK::UI::Color03));
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(index, 1, item);
}

void MusicSongSearchPopTableWidget::itemCellClicked(int row, int column)
{
    MusicAbstractTableWidget::itemCellClicked(row, column);
    Q_EMIT setText(item(row, 0)->toolTip().trimmed(), true);
}

void MusicSongSearchPopTableWidget::removeItems()
{
    MusicAbstractTableWidget::removeItems();
    setColumnCount(2);
}


MusicSongSearchPopWidget::MusicSongSearchPopWidget(QWidget *parent)
    : QWidget(parent)
{
    move(405, 45);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    m_popTableWidget = new MusicSongSearchPopTableWidget(this);
    m_popTableWidget->setFixedWidth(285);
    m_clearButton = new QPushButton("   " + tr("Clear History"), this);
    m_clearButton->setCursor(Qt::PointingHandCursor);
    m_clearButton->setFixedHeight(35);
    m_clearButton->setStyleSheet(TTK::UI::CustomStyle01 + TTK::UI::FontStyle02 + TTK::UI::ColorStyle03 + TTK::UI::BorderStyle01 + TTK::UI::BackgroundStyle10);
#ifdef Q_OS_UNIX
    m_clearButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_frame = new QFrame(this);
    m_frame->setFixedHeight(1);
    m_frame->setStyleSheet(TTK::UI::ColorStyle05);
    m_frame->setFrameShape(QFrame::HLine);

    layout->addWidget(m_popTableWidget);
    layout->addWidget(m_frame);
    layout->addWidget(m_clearButton);
    setLayout(layout);

    connect(m_clearButton, SIGNAL(clicked()), SLOT(clearButtonClicked()));
}

MusicSongSearchPopWidget::~MusicSongSearchPopWidget()
{
    delete m_popTableWidget;
    delete m_clearButton;
}

void MusicSongSearchPopWidget::initialize(QWidget *parent)
{
    if(parent)
    {
        connect(m_popTableWidget, SIGNAL(setText(QString,bool)), parent, SLOT(selectedTextChanged(QString,bool)), Qt::UniqueConnection);
    }

    setControlEnabled(true);
    m_popTableWidget->removeItems();

    MusicSongSearchRecordConfigManager manager(this);
    if(!manager.fromFile(SEARCH_PATH_FULL))
    {
        return;
    }

    MusicSearchRecordList records;
    manager.readBuffer(records);

    const int count = records.count();
    resize(m_popTableWidget->width() + 2, count == 0 ? 0 : (count < MAX_ITEM_COUNT ? count * TTK_ITEM_SIZE_M + 45 : (MAX_ITEM_COUNT + 1) * TTK_ITEM_SIZE_M + 8));

    m_popTableWidget->setRowCount(count);
    for(int i = 0; i < count; ++i)
    {
        m_popTableWidget->addCellItem(i, records[i].m_name, utcTimeToLocal(records[i].m_timestamp));
    }
}

void MusicSongSearchPopWidget::selectRow(bool up)
{
    m_popTableWidget->selectRow(up);
}

void MusicSongSearchPopWidget::setControlEnabled(bool enabled)
{
    if(enabled)
    {
        m_frame->show();
        m_clearButton->show();
    }
    else
    {

        m_frame->hide();
        m_clearButton->hide();
    }
}

void MusicSongSearchPopWidget::createSuggestItems(const QStringList &names)
{
    setControlEnabled(false);
    m_popTableWidget->removeItems();

    const int count = names.count();
    resize(m_popTableWidget->width() + 2, count == 0 ? 0 : (count * TTK_ITEM_SIZE_M + 8));

    m_popTableWidget->setRowCount(count);
    for(int i = 0; i < count; ++i)
    {
        m_popTableWidget->addCellItem(i, names[i], {});
    }
}

QString MusicSongSearchPopWidget::utcTimeToLocal(const QString &time) const
{
    const qint64 t = (TTKTime::currentTimestamp() - time.toLongLong()) / MT_S2MS;
    if(t < MT_M2S)
    {
        return QString::number(t) + tr("ss");
    }
    else if(MT_M2S <= t && t < MT_H2S)
    {
        return QString::number(t / MT_M2S) + tr("mm");
    }
    else if(MT_H2S <= t && t < MT_D2S)
    {
        return QString::number(t / MT_H2S) + tr("hh");
    }
    else
    {
        return QString::number(t / MT_D2S) + tr("day");
    }
}

void MusicSongSearchPopWidget::clearButtonClicked()
{
    MusicSongSearchRecordConfigManager manager(this);
    if(!manager.load(SEARCH_PATH_FULL))
    {
        return;
    }

    manager.writeBuffer(MusicSearchRecordList());
    close();
}

void MusicSongSearchPopWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::gray);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
