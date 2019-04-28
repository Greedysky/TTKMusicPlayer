#include "musicfunctiontoolboxwidget.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"

#include <QPainter>
#include <QScrollBar>
#include <QScrollArea>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

#define DRAG_FORMAT     "Swap Item"

MusicFunctionToolBoxTopWidget::MusicFunctionToolBoxTopWidget(int index, const QString &text, QWidget *parent)
    : QWidget(parent)
{
    m_index = index;
    m_isDrawTopState = false;
    m_isDrawMoveState = false;
    m_isBlockMoveExpand = false;

    setAcceptDrops(true);
    setFixedHeight(40);

    QHBoxLayout *topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(3, 0, 0, 0);
    topLayout->setSpacing(0);
    m_labelIcon = new QLabel(this);
    m_labelIcon->setPixmap(QPixmap(":/tiny/lb_arrow_up_normal"));
    m_labelText = new QLabel(this);
    m_labelText->setStyleSheet(MusicUIObject::MColorStyle10);
    m_labelText->setText(text);
    MusicUtils::Widget::setLabelFontStyle(m_labelText, MusicObject::FT_Bold);

    topLayout->addWidget(m_labelIcon);
    topLayout->addWidget(m_labelText);
    topLayout->addStretch(1);

    setLayout(topLayout);
}

MusicFunctionToolBoxTopWidget::~MusicFunctionToolBoxTopWidget()
{
    delete m_labelIcon;
    delete m_labelText;
}

void MusicFunctionToolBoxTopWidget::setItemExpand(bool expand)
{
    m_labelIcon->setPixmap(QPixmap(expand ? ":/tiny/lb_arrow_down_normal" : ":/tiny/lb_arrow_up_normal"));
}

bool MusicFunctionToolBoxTopWidget::isItemExpand() const
{
    return m_labelIcon->pixmap()->cacheKey() == QPixmap(":/tiny/lb_arrow_down_normal").cacheKey();
}

void MusicFunctionToolBoxTopWidget::setTitle(const QString &text)
{
    m_labelText->setText(QFontMetrics(m_labelText->font()).elidedText(text, Qt::ElideRight, RENAME_WIDTH - 10));
    m_labelText->setToolTip(text);
}

QString MusicFunctionToolBoxTopWidget::getTitle(bool suffix)
{
    QString text = m_labelText->toolTip().trimmed();
    if(!suffix)
    {
        const int index = text.lastIndexOf("[");
        m_suffixString = text.right(text.count() - index);
        text = text.left( index );
    }
    return text;
}

bool MusicFunctionToolBoxTopWidget::isItemEnable() const
{
    return false;
}

void MusicFunctionToolBoxTopWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    m_isDrawTopState = false;
    m_isDrawMoveState = false;
    update();
}

void MusicFunctionToolBoxTopWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat(DRAG_FORMAT) && isItemEnable())
    {
        m_isDrawMoveState = true;
        m_isDrawTopState = event->pos().y() < height()/2;
        update();
    }
}

void MusicFunctionToolBoxTopWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(DRAG_FORMAT))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MusicFunctionToolBoxTopWidget::dropEvent(QDropEvent *event)
{
    m_isDrawMoveState = false;
    update();

    if(event->mimeData()->hasFormat(DRAG_FORMAT) && isItemEnable())
    {
        emit swapDragItemIndex(event->mimeData()->data(DRAG_FORMAT).toInt(), m_index);
    }
}

void MusicFunctionToolBoxTopWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        emit mousePressAt(m_index);
        m_pressPosAt = event->pos();
    }
}

void MusicFunctionToolBoxTopWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    QRect itemRect(m_pressPosAt.x() - 2, m_pressPosAt.y() - 2, m_pressPosAt.x() + 2, m_pressPosAt.y() + 2);
    if(!itemRect.contains(event->pos()) && isItemEnable())
    {
        if(!m_isBlockMoveExpand && isItemExpand())
        {
            emit mousePressAt(m_index);
        }

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(DRAG_FORMAT, QByteArray::number(m_index));
        mimeData->setText( getTitle(true) );

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setHotSpot(QPoint(0, height()/2));
#ifdef TTK_GREATER_NEW
        drag->setPixmap( grab(rect()) );
#else
        drag->setPixmap( QPixmap::grabWidget(this, rect()) );
#endif
        drag->exec(Qt::MoveAction);
    }
}

void MusicFunctionToolBoxTopWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 0.1, Qt::SolidLine));
    painter.drawLine(0, height(), width(), height());

    if(m_isDrawMoveState)
    {
        painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 1, Qt::SolidLine));
        if(m_isDrawTopState)
        {
            painter.drawLine(0, 0, width(), 0);
        }
        else
        {
            painter.drawLine(0, height() - 1, width(), height() - 1);
        }
    }
}



MusicFunctionToolBoxWidgetItem::MusicFunctionToolBoxWidgetItem(QWidget *parent)
    : QWidget(parent)
{
    m_topWidget = nullptr;
    m_layout = nullptr;
}

MusicFunctionToolBoxWidgetItem::MusicFunctionToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : MusicFunctionToolBoxWidgetItem(parent)
{
    Q_UNUSED(index);
    Q_UNUSED(text);
}

MusicFunctionToolBoxWidgetItem::~MusicFunctionToolBoxWidgetItem()
{
    delete m_topWidget;
    delete m_layout;
}

QWidget *MusicFunctionToolBoxWidgetItem::item(int index)
{
    if(index < 0 || index >= m_itemList.count())
    {
        return nullptr;
    }
    return m_itemList[index];
}

void MusicFunctionToolBoxWidgetItem::addItem(QWidget *item)
{
    m_itemList.append(item);
    m_layout->addWidget(item);
}

void MusicFunctionToolBoxWidgetItem::removeItem(QWidget *item)
{
    m_itemList.removeAll(item);
    m_layout->removeWidget(item);
}

void MusicFunctionToolBoxWidgetItem::setTitle(const QString &text)
{
    m_topWidget->setTitle(text);
}

QString MusicFunctionToolBoxWidgetItem::getTitle() const
{
    return m_topWidget->getTitle();
}

void MusicFunctionToolBoxWidgetItem::setItemExpand(bool expand)
{
    m_topWidget->setItemExpand(expand);
    foreach(QWidget *w, m_itemList)
    {
        w->setVisible(expand);
    }
}

bool MusicFunctionToolBoxWidgetItem::itemExpand() const
{
    if(!m_itemList.isEmpty())
    {
        return m_itemList.first()->isVisible();
    }
    return false;
}

int MusicFunctionToolBoxWidgetItem::count() const
{
    return m_itemList.count();
}

void MusicFunctionToolBoxWidgetItem::init()
{
    connect(m_topWidget, SIGNAL(mousePressAt(int)), parent(), SLOT(mousePressAt(int)));
    connect(m_topWidget, SIGNAL(swapDragItemIndex(int,int)), SIGNAL(swapDragItemIndex(int,int)));

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_topWidget);
    setLayout(m_layout);
}

void MusicFunctionToolBoxWidgetItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFunctionToolBoxWidgetItem::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}



MusicNormalToolBoxWidgetItem::MusicNormalToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : MusicFunctionToolBoxWidgetItem(parent)
{
    m_topWidget = new MusicFunctionToolBoxTopWidget(index, text, this);

    init();
}



MusicFunctionToolBoxWidget::MusicFunctionToolBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    m_currentIndex = -1;
    m_itemIndexRaise = 0;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_contentsWidget = new QWidget(this);
    m_contentsWidget->setObjectName("contentsWidget");
    m_contentsWidget->setStyleSheet("#contentsWidget{background-color:transparent;}");

    m_layout = new QVBoxLayout(m_contentsWidget);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    m_layout->setSpacing(0);
    m_contentsWidget->setLayout(m_layout);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setFrameShadow(QFrame::Plain);
    m_scrollArea->setAlignment(Qt::AlignLeft);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidget(m_contentsWidget);

    setTransparent(0);

    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);
}

MusicFunctionToolBoxWidget::~MusicFunctionToolBoxWidget()
{
    while(!m_itemList.isEmpty())
    {
        delete m_itemList.takeLast().m_widgetItem;
    }
    delete m_layout;
    delete m_scrollArea;
}

void MusicFunctionToolBoxWidget::addItem(QWidget *item, const QString &text)
{
    const int count = m_layout->count();
    if(count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }

    //hide before widget
    for(int i=0; i<m_itemList.count(); ++i)
    {
        m_itemList[i].m_widgetItem->setItemExpand(false);
    }

    // Add item and make sure it stretches the remaining space.
    MusicToolBoxWidgetItem widgetItem;
    widgetItem.m_widgetItem = createItem(item, text);
    widgetItem.m_itemIndex = m_itemIndexRaise++;
    m_itemList.append(widgetItem);

    m_currentIndex = m_itemList.count() - 1;

    m_layout->addWidget(widgetItem.m_widgetItem);
    m_layout->addStretch(5);
}

void MusicFunctionToolBoxWidget::removeItem(QWidget *item)
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicFunctionToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                m_layout->removeWidget(item);
                m_itemList.takeAt(i).m_widgetItem->deleteLater();
                m_currentIndex = 0;
                return;
            }
        }
    }
}

void MusicFunctionToolBoxWidget::swapItem(int before, int after)
{
    const MusicToolBoxWidgetItem &widgetItem = m_itemList.takeAt(before);
    m_itemList.insert(after, widgetItem);

    m_layout->removeWidget(widgetItem.m_widgetItem);
    const int count = m_layout->count();
    if(count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }
    m_layout->insertWidget(after, widgetItem.m_widgetItem);
    m_layout->addStretch(5);
}

void MusicFunctionToolBoxWidget::setTitle(QWidget *item, const QString &text)
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicFunctionToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                it->setTitle(text);
                return;
            }
        }
    }
}

QString MusicFunctionToolBoxWidget::getTitle(QWidget *item) const
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicFunctionToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                return it->getTitle();
            }
        }
    }
    return QString();
}

void MusicFunctionToolBoxWidget::resizeScrollIndex(int index) const
{
    QScrollBar *bar = m_scrollArea->verticalScrollBar();
    if(bar)
    {
        bar->setSliderPosition(index);
    }
}

int MusicFunctionToolBoxWidget::currentIndex() const
{
    return m_currentIndex;
}

int MusicFunctionToolBoxWidget::count() const
{
    return m_itemList.count();
}

void MusicFunctionToolBoxWidget::setCurrentIndex(int index)
{
    m_currentIndex = index;
    for(int i=0; i<m_itemList.count(); ++i)
    {
        m_itemList[i].m_widgetItem->setItemExpand( i == index );
    }
}

void MusicFunctionToolBoxWidget::mousePressAt(int index)
{
    m_currentIndex = foundMappingIndex(index);
    for(int i=0; i<m_itemList.count(); ++i)
    {
        const bool hide = (i == m_currentIndex) ? !m_itemList[i].m_widgetItem->itemExpand() : false;
        m_itemList[i].m_widgetItem->setItemExpand(hide);
    }
}

void MusicFunctionToolBoxWidget::setTransparent(int alpha)
{
    const QString &alphaStr = QString("background:rgba(255, 255, 255, %1)").arg(alpha);
    QWidget *view = m_scrollArea->viewport();
    view->setObjectName("viewport");
    view->setStyleSheet(QString("#viewport{%1}").arg(alphaStr));

    m_scrollArea->verticalScrollBar()->setStyleSheet(" \
            QScrollBar{ background:transparent; width:8px; padding-top:0px; padding-bottom:0px;} \
            QScrollBar::handle:vertical{ border-radius:4px; background:#CFCFCF; min-height: 30px;} \
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB;} \
            QScrollBar::handle:vertical:hover{ background:#BBBBBB;} \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none;} \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none;}");
}

MusicFunctionToolBoxWidgetItem *MusicFunctionToolBoxWidget::createItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicFunctionToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addItem(item);
    it->setItemExpand(true);
    return it;
}

void MusicFunctionToolBoxWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFunctionToolBoxWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

int MusicFunctionToolBoxWidget::foundMappingIndex(int index)
{
    int id = -1;
    for(int i=0; i<m_itemList.count(); ++i)
    {
        if(m_itemList[i].m_itemIndex == index)
        {
            id = i;
            break;
        }
    }
    return id;
}
