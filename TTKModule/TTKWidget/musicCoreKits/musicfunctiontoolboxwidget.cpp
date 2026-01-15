#include "musicfunctiontoolboxwidget.h"
#include "musicwidgetutils.h"
#include "ttkdesktopscreen.h"

#include <QDrag>
#include <QMimeData>
#include <QScrollArea>
#include <QMouseEvent>

static constexpr const char *DRAG_FORMAT = "Swap Item";

MusicFunctionToolBoxTopWidget::MusicFunctionToolBoxTopWidget(int index, const QString &text, QWidget *parent)
    : QWidget(parent),
      m_index(index),
      m_isDrawTopState(false),
      m_isDrawMoveState(false),
      m_isBlockMoveExpand(false),
      m_isActive(false)
{
    setFixedHeight(40);
    setAcceptDrops(true);

    QHBoxLayout *topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(3, 0, 0, 0);
    topLayout->setSpacing(0);

    m_labelIcon = new QLabel(this);
    m_labelIcon->setPixmap(QPixmap(":/tiny/lb_arrow_up_normal"));

    m_labelText = new QLabel(this);
    m_labelText->setStyleSheet(TTK::UI::ColorStyle02);
    m_labelText->setText(text);
    TTK::Widget::setFontStyle(m_labelText, TTK::FontStyleMode::Bold);

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

bool MusicFunctionToolBoxTopWidget::isActive()
{
    m_isActive = geometry().contains(mapFromGlobal(QCursor::pos()));
    update();
    return m_isActive;
}

void MusicFunctionToolBoxTopWidget::setExpand(bool expand)
{
    m_labelIcon->setPixmap(QPixmap(expand ? ":/tiny/lb_arrow_down_normal" : ":/tiny/lb_arrow_up_normal"));
}

bool MusicFunctionToolBoxTopWidget::isExpand() const
{
    const QPixmap &pixmap = QtLablePixmap(m_labelIcon);
    return pixmap.cacheKey() == QPixmap(":/tiny/lb_arrow_down_normal").cacheKey();
}

void MusicFunctionToolBoxTopWidget::setTitle(const QString &text)
{
    m_labelText->setToolTip(text);
    const int index = text.lastIndexOf("[");

    if(index == -1)
    {
        m_labelText->setText(TTK::Widget::elidedText(m_labelText->font(), text, Qt::ElideRight, RENAME_WIDTH - 10));
    }
    else
    {
        m_labelText->setText(TTK::Widget::elidedTitleText(m_labelText->font(), text, RENAME_WIDTH));
    }
}

QString MusicFunctionToolBoxTopWidget::title(bool suffix)
{
    QString text = m_labelText->toolTip().trimmed();
    if(!suffix)
    {
        const int index = text.lastIndexOf("[");
        text = text.left(index);
    }
    return text;
}

bool MusicFunctionToolBoxTopWidget::isItemEnabled() const
{
    return false;
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

void MusicFunctionToolBoxTopWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat(DRAG_FORMAT) && isItemEnabled())
    {
        m_isDrawMoveState = true;
        m_isDrawTopState = QtPosition(event).y() < height() / 2;
        update();
    }
}

void MusicFunctionToolBoxTopWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    m_isDrawTopState = false;
    m_isDrawMoveState = false;
    update();
}

void MusicFunctionToolBoxTopWidget::dropEvent(QDropEvent *event)
{
    m_isDrawMoveState = false;
    update();

    if(event->mimeData()->hasFormat(DRAG_FORMAT) && isItemEnabled())
    {
        Q_EMIT swapDragItemIndex(event->mimeData()->data(DRAG_FORMAT).toInt(), m_index);
    }
}

void MusicFunctionToolBoxTopWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        Q_EMIT itemIndexChanged(m_index);
        m_pressedPos = event->pos();
    }
}

void MusicFunctionToolBoxTopWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QRect itemRect(m_pressedPos.x() - 2, m_pressedPos.y() - 2, m_pressedPos.x() + 2, m_pressedPos.y() + 2);
    if(!itemRect.contains(event->pos()) && isItemEnabled())
    {
        if(!m_isBlockMoveExpand && isExpand())
        {
            Q_EMIT itemIndexChanged(m_index);
        }

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(DRAG_FORMAT, QByteArray::number(m_index));
        mimeData->setText(title(true));

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setHotSpot(QPoint(0, height() / 2));
        drag->setPixmap(TTKDesktopScreen::grabWidget(this, rect()));
        drag->exec(Qt::MoveAction);
    }
}

void MusicFunctionToolBoxTopWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
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
    : QWidget(parent),
      m_layout(nullptr),
      m_item(nullptr),
      m_topWidget(nullptr)
{

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

QWidget *MusicFunctionToolBoxWidgetItem::item() const noexcept
{
    return m_item;
}

void MusicFunctionToolBoxWidgetItem::addCellItem(QWidget *item)
{
    m_item = item;
    m_layout->addWidget(item);
}

void MusicFunctionToolBoxWidgetItem::setTitle(const QString &text)
{
    m_topWidget->setTitle(text);
}

void MusicFunctionToolBoxWidgetItem::setExpand(bool expand)
{
    m_topWidget->setExpand(expand);
    if(m_item)
    {
        m_item->setVisible(expand);
    }
}

bool MusicFunctionToolBoxWidgetItem::isExpand() const
{
    return m_item ? m_item->isVisible() : false;
}

bool MusicFunctionToolBoxWidgetItem::isActive() const
{
    return m_topWidget->isActive();
}

void MusicFunctionToolBoxWidgetItem::initialize()
{
    connect(m_topWidget, SIGNAL(itemIndexChanged(int)), parent(), SLOT(itemIndexChanged(int)));
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



MusicNormalToolBoxWidgetItem::MusicNormalToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : MusicFunctionToolBoxWidgetItem(parent)
{
    m_topWidget = new MusicFunctionToolBoxTopWidget(index, text, this);
    initialize();
}



MusicFunctionToolBoxWidget::MusicFunctionToolBoxWidget(QWidget *parent)
    : QWidget(parent),
      m_currentIndex(-1),
      m_itemIndexRaise(0)
{
    setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_contentsWidget = new QWidget(this);
    m_contentsWidget->setObjectName("ContentsWidget");
    m_contentsWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_contentsWidget->objectName(), TTK::UI::BackgroundStyle01));

    m_layout = new QVBoxLayout(m_contentsWidget);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    m_layout->setSpacing(0);
    m_contentsWidget->setLayout(m_layout);

    m_scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(m_scrollArea, m_contentsWidget, false);

    setTransparent(0);

    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);
}

MusicFunctionToolBoxWidget::~MusicFunctionToolBoxWidget()
{
    while(!m_items.isEmpty())
    {
        delete m_items.takeLast().m_itemWidget;
    }
    delete m_layout;
    delete m_scrollArea;
}

void MusicFunctionToolBoxWidget::addCellItem(QWidget *item, const QString &text)
{
    const int count = m_layout->count();
    if(count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }

    //hide before widget
    for(int i = 0; i < m_items.count(); ++i)
    {
        m_items[i].m_itemWidget->setExpand(false);
    }

    // Add item and make sure it stretches the remaining space.
    MusicToolBoxWidgetItem widgetItem;
    widgetItem.m_itemWidget = initialItem(item, text);
    widgetItem.m_itemIndex = m_itemIndexRaise++;
    m_items.append(widgetItem);

    m_currentIndex = m_items.count() - 1;

    m_layout->addWidget(widgetItem.m_itemWidget);
    m_layout->addStretch(5);
}

void MusicFunctionToolBoxWidget::removeItem(QWidget *item)
{
    for(int i = 0; i < m_items.count(); ++i)
    {
        MusicFunctionToolBoxWidgetItem *it = m_items[i].m_itemWidget;
        if(it->item() == item)
        {
            m_layout->removeWidget(item);
            m_items.takeAt(i).m_itemWidget->deleteLater();
            m_currentIndex = 0;
            return;
        }
    }
}

void MusicFunctionToolBoxWidget::swapItem(int start, int end)
{
    const MusicToolBoxWidgetItem &widgetItem = m_items.takeAt(start);
    m_items.insert(end, widgetItem);

    m_layout->removeWidget(widgetItem.m_itemWidget);
    const int count = m_layout->count();
    if(count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }
    m_layout->insertWidget(end, widgetItem.m_itemWidget);
    m_layout->addStretch(5);
}

void MusicFunctionToolBoxWidget::setTitle(QWidget *item, const QString &text)
{
    for(int i = 0; i < m_items.count(); ++i)
    {
        MusicFunctionToolBoxWidgetItem *it = m_items[i].m_itemWidget;
        if(it->item() == item)
        {
            it->setTitle(text);
            return;
        }
    }
}

void MusicFunctionToolBoxWidget::resizeScrollIndex(int index) const
{
    QScrollBar *bar = m_scrollArea->verticalScrollBar();
    if(bar)
    {
        bar->setSliderPosition(index);
    }
}

int MusicFunctionToolBoxWidget::currentIndex() const noexcept
{
    return m_currentIndex;
}

int MusicFunctionToolBoxWidget::count() const noexcept
{
    return m_items.count();
}

void MusicFunctionToolBoxWidget::setCurrentIndex(int index)
{
    m_currentIndex = index;
    for(int i = 0; i < m_items.count(); ++i)
    {
        m_items[i].m_itemWidget->setExpand(i == index);
    }
}

void MusicFunctionToolBoxWidget::itemIndexChanged(int index)
{
    m_currentIndex = foundMappedIndex(index);
    for(int i = 0; i < m_items.count(); ++i)
    {
        const bool hide = (i == m_currentIndex) ? !m_items[i].m_itemWidget->isExpand() : false;
        m_items[i].m_itemWidget->setExpand(hide);
    }
}

void MusicFunctionToolBoxWidget::setTransparent(int alpha)
{
    const QString &alphaStr = QString("background:rgba(255, 255, 255, %1)").arg(alpha);
    QWidget *view = m_scrollArea->viewport();
    view->setObjectName("Viewport");
    view->setStyleSheet(QString("#%1{ %2 }").arg(view->objectName(), alphaStr));

    m_scrollArea->verticalScrollBar()->setStyleSheet(" \
            QScrollBar{ background:transparent; width:8px; padding-top:0px; padding-bottom:0px; } \
            QScrollBar::handle:vertical{ border-radius:4px; background:#CFCFCF; min-height: 30px; } \
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB; } \
            QScrollBar::handle:vertical:hover{ background:#BBBBBB; } \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none; } \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none; }");
}

void MusicFunctionToolBoxWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

int MusicFunctionToolBoxWidget::foundMappedIndex(int index)
{
    int id = -1;
    for(int i = 0; i < m_items.count(); ++i)
    {
        if(m_items[i].m_itemIndex == index)
        {
            id = i;
            break;
        }
    }
    return id;
}
