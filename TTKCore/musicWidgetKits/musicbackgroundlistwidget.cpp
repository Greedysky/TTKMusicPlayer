#include "musicbackgroundlistwidget.h"
#include "musicmessagebox.h"

#include <QPainter>
#include <QMouseEvent>

#define ITEM_COUNT      4

MusicBackgroundListItem::MusicBackgroundListItem(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(137, 100);
    setCursor(Qt::PointingHandCursor);

    m_printMask = false;
    m_closeMask = false;
    m_isSelected = false;
    m_closeSet = false;
}

QString MusicBackgroundListItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundListItem::select(bool select)
{
    m_isSelected = select;
    update();
}

void MusicBackgroundListItem::closeSet(bool set)
{
    m_closeSet = set;
    update();
}

void MusicBackgroundListItem::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);

    if(m_closeSet && QRect(width() - 14 - 6, 6, 14, 14).contains(event->pos()))
    {
        emit closeClicked(this);
    }
    else
    {
        emit itemClicked(this);
    }
}

void MusicBackgroundListItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_printMask = false;
    m_closeMask = false;
    update();
}

void MusicBackgroundListItem::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_printMask = true;
    m_closeMask = true;
    update();
}

void MusicBackgroundListItem::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if(m_isSelected)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 17, height() - 17, 17, 17, QPixmap(":/tiny/lb_selected"));
    }

    if(m_printMask)
    {
        QPainter painter(this);
        painter.setBrush(QColor(0, 0, 0, 155));
        painter.drawRect(rect());

        QFont f = painter.font();
        f.setPixelSize(13);
        painter.setFont(f);
        QFontMetrics metric(painter.font());

        painter.setPen(Qt::white);
        painter.drawText((width() - metric.width(m_name))/2, 32, m_name);
        painter.drawText((width() - metric.width(tr("Used By 88888")))/2, 50, tr("Used By 88888"));
        painter.drawText((width() - metric.width(tr("Au: Greedysky")))/2, 68, tr("Au: Greedysky"));
    }

    if(m_closeSet && m_closeMask)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 14 - 6, 6, 14, 14, QPixmap(":/functions/btn_close_hover"));
    }
}


MusicBackgroundListWidget::MusicBackgroundListWidget(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(7, 7, 7, 7);
    setLayout(m_layout);

    m_currentItem = nullptr;
}

MusicBackgroundListWidget::~MusicBackgroundListWidget()
{
    clearAllItems();
}

QString MusicBackgroundListWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundListWidget::setCurrentItemName(const QString &name)
{
    //Set the current theme index
    foreach(MusicBackgroundListItem *item, m_items)
    {
        if(item->getFileName() == name)
        {
            item->select(true);
            m_currentItem = item;
            break;
        }
    }
}

void MusicBackgroundListWidget::clearSelectState()
{
    foreach(MusicBackgroundListItem *item, m_items)
    {
        item->select(false);
    }
}

void MusicBackgroundListWidget::clearAllItems()
{
    while(!m_items.isEmpty())
    {
        delete m_items.takeLast();
    }
}

void MusicBackgroundListWidget::createItem(const QString &name, const QString &path, bool state)
{
    MusicBackgroundListItem *item = new MusicBackgroundListItem(this);
    item->closeSet(state);
    item->setFileName(name);
    item->setFilePath(path);
    item->setPixmap( QPixmap(path).scaled(item->size()) );
    connect(item, SIGNAL(itemClicked(MusicBackgroundListItem*)), SLOT(itemHasClicked(MusicBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(MusicBackgroundListItem*)), SLOT(itemCloseClicked(MusicBackgroundListItem*)));
    m_layout->addWidget(item, m_items.count()/ITEM_COUNT, m_items.count()%ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

bool MusicBackgroundListWidget::contains(const QString &name) const
{
    foreach(MusicBackgroundListItem *item, m_items)
    {
        if(item->getFileName() == name)
        {
            return true;
        }
    }

    return false;
}

int MusicBackgroundListWidget::find(MusicBackgroundListItem *item) const
{
    for(int i=0; i<m_items.count(); ++i)
    {
        if(m_items[i] == item)
        {
            return i;
        }
    }

    return -1;
}

void MusicBackgroundListWidget::updateLastedItem()
{
    if(!m_items.isEmpty())
    {
        itemHasClicked(m_items.last());
    }
}

void MusicBackgroundListWidget::itemCloseClicked(MusicBackgroundListItem *item)
{
    if(m_items.count() == 1)
    {
        MusicMessageBox message;
        message.setText(tr("Last One Item Can not be deleted!"));
        message.exec();
        return;
    }

    m_layout->removeWidget(item);
    int index = find(item);
    int cIndex = find(m_currentItem);
    QFile::remove(item->getFilePath());
    m_items.takeAt(index)->deleteLater();

    if(index == cIndex)
    {
        m_currentItem = nullptr;
        if(!m_items.isEmpty())
        {
            itemHasClicked( m_items[index == 0 ? 0 : index - 1] );
        }
    }

    for(int i=index; i<m_items.count(); ++i)
    {
        m_layout->addWidget(m_items[i], i/ITEM_COUNT, i%ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    }
}

void MusicBackgroundListWidget::itemHasClicked(MusicBackgroundListItem *item)
{
    if(m_currentItem)
    {
        m_currentItem->select(false);
    }

    m_currentItem = item;
    m_currentItem->select(true);
    emit itemClicked(item->getFileName());
}
