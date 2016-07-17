#include "musicsongstoolboxwidget.h"
#include "musicuiobject.h"

#include <QScrollArea>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>

MusicSongsToolBoxTopWidget::MusicSongsToolBoxTopWidget(int index, QWidget *parent)
    : QWidget(parent)
{
    m_index = index;
}

QString MusicSongsToolBoxTopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolBoxTopWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        emit mousePressAt(m_index);
    }
}

void MusicSongsToolBoxTopWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("addNewItem"), parent(), SIGNAL(addNewItem()));
    menu.addSeparator();

    bool disable = !(m_index == 0 || m_index == 1 || m_index == 2);
    menu.addAction(tr("deleteItem"), parent(), SIGNAL(deleteItem()))->setEnabled(disable);
    menu.addAction(tr("changItemName"), parent(), SIGNAL(changItemName()))->setEnabled(disable);
    menu.exec(QCursor::pos());
}

//void MusicSongsToolBoxTopWidget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255, 20));
//    painter.end();

//    QWidget::paintEvent(event);
//}

MusicSongsToolBoxWidgetItem::MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : QWidget(parent)
{
    MusicSongsToolBoxTopWidget *topWidget = new MusicSongsToolBoxTopWidget(index, this);
    connect(topWidget, SIGNAL(mousePressAt(int)), parent, SLOT(mousePressAt(int)));

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(3, 0, 0, 0);
    topLayout->setSpacing(0);
    m_labelIcon = new QLabel(this);
    m_labelIcon->setPixmap(QPixmap(":/image/arrowup"));
    m_labelText = new QLabel(this);
    m_labelText->setText(text);
    topLayout->addWidget(m_labelIcon);
    topLayout->addWidget(m_labelText);
    topLayout->addStretch(5);
    topWidget->setLayout(topLayout);
    topWidget->setFixedHeight(30);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(topWidget);
    setLayout(m_layout);
}

MusicSongsToolBoxWidgetItem::~MusicSongsToolBoxWidgetItem()
{
    delete m_labelIcon;
    delete m_labelText;
    delete m_layout;
}

QString MusicSongsToolBoxWidgetItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolBoxWidgetItem::addItem(QWidget *item)
{
    m_itemList.append(item);
    m_layout->addWidget(item);
}

void MusicSongsToolBoxWidgetItem::removeItem(QWidget *item)
{
    m_itemList.removeAll(item);
    m_layout->removeWidget(item);
}

void MusicSongsToolBoxWidgetItem::setTitle(const QString &text)
{
    m_labelText->setText(text);
}

QString MusicSongsToolBoxWidgetItem::getTitle() const
{
    return m_labelText->text().trimmed();
}

void MusicSongsToolBoxWidgetItem::setItemHide(bool hide)
{
    m_labelIcon->setPixmap(QPixmap(hide ? ":/image/arrowdown" : ":/image/arrowup"));
    foreach(QWidget *w, m_itemList)
    {
        w->setVisible(hide);
    }
}

bool MusicSongsToolBoxWidgetItem::itemHide() const
{
    if(!m_itemList.isEmpty())
    {
        return m_itemList.first()->isVisible();
    }
    return false;
}

void MusicSongsToolBoxWidgetItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicSongsToolBoxWidgetItem::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}


MusicSongsToolBoxWidget::MusicSongsToolBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    m_currentIndex = -1;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *contentsWidget = new QWidget(this);
    m_layout = new QVBoxLayout(contentsWidget);
    m_layout->setContentsMargins(0, 0, 0 ,0);
    m_layout->setSpacing(0);
    contentsWidget->setLayout(m_layout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(contentsWidget);
//    scrollArea->viewport()->setStyleSheet("border: none;");

    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

MusicSongsToolBoxWidget::~MusicSongsToolBoxWidget()
{
    delete m_layout;
}

QString MusicSongsToolBoxWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolBoxWidget::setCurrentIndex(int index)
{
    m_currentIndex = index;
    for(int i=0; i<m_itemList.count(); ++i)
    {
        m_itemList[i]->setItemHide( i== index );
    }
}

void MusicSongsToolBoxWidget::mousePressAt(int index)
{
    m_currentIndex = index;
    for(int i=0; i<m_itemList.count(); ++i)
    {
        bool hide = (i== index) ? !m_itemList[i]->itemHide() : false;
        m_itemList[i]->setItemHide(hide);
    }
}

int MusicSongsToolBoxWidget::currentIndex() const
{
    return m_currentIndex;
}

int MusicSongsToolBoxWidget::count() const
{
    return m_itemList.count();
}

void MusicSongsToolBoxWidget::addItem(QWidget *item, const QString &text)
{
    int count = m_layout->count();
    if(count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }

    //hide before widget
    for(int i=0; i<m_itemList.count(); ++i)
    {
        m_itemList[i]->setItemHide(false);
    }

    // Add item and make sure it stretches the remaining space.
    MusicSongsToolBoxWidgetItem *it = new MusicSongsToolBoxWidgetItem(m_itemList.count(), text, this);
    it->addItem(item);
    m_itemList.append(it);
    m_layout->addWidget(it);
    m_layout->addStretch(5);
}

void MusicSongsToolBoxWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicSongsToolBoxWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
