#include "musicbackgroundlistwidget.h"

#include <QPainter>

MusicBackgroundListItem::MusicBackgroundListItem(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(137, 100);
    setCursor(Qt::PointingHandCursor);

    m_printMask = false;
    m_isSelected = false;
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

void MusicBackgroundListItem::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit itemClicked(this);
}

void MusicBackgroundListItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_printMask = false;
    update();
}

void MusicBackgroundListItem::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_printMask = true;
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

void MusicBackgroundListWidget::clearAllItems()
{
    while(!m_items.isEmpty())
    {
        delete m_items.takeLast();
    }
}

void MusicBackgroundListWidget::createItem(const QString &name, const QString &path)
{
    MusicBackgroundListItem *item = new MusicBackgroundListItem(this);
    item->setFileName(name);
    item->setPixmap( QPixmap(path).scaled(item->size()) );
    connect(item, SIGNAL(itemClicked(MusicBackgroundListItem*)),
                  SLOT(itemHasClicked(MusicBackgroundListItem*)));
    m_layout->addWidget(item, m_items.count()/4, m_items.count()%4);
    m_items << item;
}

void MusicBackgroundListWidget::updateLastedItem()
{
    if(!m_items.isEmpty())
    {
        itemHasClicked(m_items.last());
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
