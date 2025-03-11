#include "musicbackgroundlistwidget.h"
#include "musicextractmanager.h"
#include "musictoastlabel.h"
#include "musicwidgetutils.h"

#include <QMouseEvent>

MusicBackgroundListItem::MusicBackgroundListItem(QWidget *parent)
    : QLabel(parent),
      m_isSelected(false),
      m_printMask(false),
      m_selectedEnabled(true),
      m_closeEnabled(false),
      m_showNameEnabled(true)
{
    setFixedSize(137, 100);
    setCursor(Qt::PointingHandCursor);
}

void MusicBackgroundListItem::updatePixmap()
{
    if(!m_path.isEmpty())
    {
        MusicBackgroundImage image;
        if(MusicExtractManager::outputSkin(&image, m_path))
        {
            updatePixmap(image);
        }
    }
}

void MusicBackgroundListItem::updatePixmap(const MusicBackgroundImage &image)
{
    m_imageInfo = image.m_item;
    setPixmap(image.m_pix.scaled(size()));
}

bool MusicBackgroundListItem::contains(const MusicSkinItem &item) const
{
    if(item.isValid() && m_imageInfo.isValid())
    {
        return item.m_name == m_imageInfo.m_name;
    }
    return false;
}

void MusicBackgroundListItem::setSelected(bool v)
{
    m_isSelected = v;
    update();
}

void MusicBackgroundListItem::setSelectEnabled(bool v)
{
    m_selectedEnabled = v;
    update();
}

void MusicBackgroundListItem::setCloseEnabled(bool v)
{
    m_closeEnabled = v;
    update();
}

void MusicBackgroundListItem::setShowNameEnabled(bool v)
{
    m_showNameEnabled = v;
    update();
}

void MusicBackgroundListItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(m_closeEnabled && QRect(width() - 16 - 6, 6, 16, 16).contains(event->pos()))
    {
        Q_EMIT closeClicked(this);
    }
    else
    {
        Q_EMIT itemClicked(this);
    }
}

void MusicBackgroundListItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_printMask = false;
    update();
}

void MusicBackgroundListItem::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);
    m_printMask = true;
    update();
}

void MusicBackgroundListItem::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if(m_selectedEnabled && m_isSelected)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 17, height() - 17, 17, 17, QPixmap(":/tiny/lb_selected"));
    }

    if(m_printMask)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 0, 155));

        QFont font = painter.font();
        font.setPixelSize(13);
        painter.setFont(font);

        painter.setPen(Qt::white);
        if(m_showNameEnabled)
        {
            painter.drawText((width() - TTK::Widget::fontTextWidth(painter.font(), m_name)) / 2, 32, m_name);
        }

        QString v = QString::number(m_imageInfo.m_useCount);
        painter.drawText((width() - TTK::Widget::fontTextWidth(painter.font(), v)) / 2, 50, v);
                v = m_imageInfo.m_name;
        painter.drawText((width() - TTK::Widget::fontTextWidth(painter.font(), v)) / 2, 68, v);

        if(m_closeEnabled)
        {
            QPainter painter(this);
            painter.drawPixmap(width() - 18 - 4, 4, 18, 18, QPixmap(":/functions/btn_close_hover"));
        }
    }
}


MusicBackgroundListWidget::MusicBackgroundListWidget(QWidget *parent)
    : QWidget(parent),
      m_type(CachedModule),
      m_currentItem(nullptr)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_gridLayout->setContentsMargins(7, 0, 7, 0);
    setLayout(m_gridLayout);
}

MusicBackgroundListWidget::~MusicBackgroundListWidget()
{
    clearItems();
    delete m_gridLayout;
}

void MusicBackgroundListWidget::setCurrentItemName(const QString &name)
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            item->setSelected(true);
            m_currentItem = item;
            break;
        }
    }
}

void MusicBackgroundListWidget::clearState()
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        item->setSelected(false);
    }
}

void MusicBackgroundListWidget::clearItems()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_currentItem = nullptr;
}

void MusicBackgroundListWidget::addCellItem(const QString &icon, bool state)
{
    MusicBackgroundListItem *item = new MusicBackgroundListItem(this);
    item->setCloseEnabled(state);
    item->setPixmap(QPixmap(icon).scaled(item->size()));

    connect(item, SIGNAL(itemClicked(MusicBackgroundListItem*)), SLOT(currentItemClicked(MusicBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(MusicBackgroundListItem*)), SLOT(itemCloseClicked(MusicBackgroundListItem*)));
    m_gridLayout->addWidget(item, m_items.count() / MIN_ITEM_COUNT, m_items.count() % MIN_ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicBackgroundListWidget::addCellItem(const QString &name, const QString &path, bool state)
{
    MusicBackgroundListItem *item = new MusicBackgroundListItem(this);
    item->setCloseEnabled(state);
    item->setFileName(name);
    item->setFilePath(path);
    item->updatePixmap();

    connect(item, SIGNAL(itemClicked(MusicBackgroundListItem*)), SLOT(currentItemClicked(MusicBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(MusicBackgroundListItem*)), SLOT(itemCloseClicked(MusicBackgroundListItem*)));
    m_gridLayout->addWidget(item, m_items.count() / MIN_ITEM_COUNT, m_items.count() % MIN_ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicBackgroundListWidget::updateItem(const MusicBackgroundImage &image, const QString &path)
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName().isEmpty())
        {
            item->setShowNameEnabled(false);
            item->setSelectEnabled(false);
            item->setFileName(path);
            item->updatePixmap(image);
            break;
        }
    }
}

bool MusicBackgroundListWidget::contains(const QString &name) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            return true;
        }
    }

    return false;
}

bool MusicBackgroundListWidget::contains(const MusicBackgroundImage &image) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->contains(image.m_item))
        {
            return true;
        }
    }

    return false;
}

int MusicBackgroundListWidget::find(MusicBackgroundListItem *item) const
{
    for(int i = 0; i < m_items.count(); ++i)
    {
        if(m_items[i] == item)
        {
            return i;
        }
    }

    return -1;
}

MusicBackgroundListItem* MusicBackgroundListWidget::find(const QString &name) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            return item;
        }
    }

    return nullptr;
}

MusicBackgroundListItem* MusicBackgroundListWidget::find(const MusicBackgroundImage &image) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->contains(image.m_item))
        {
            return item;
        }
    }

    return nullptr;
}

void MusicBackgroundListWidget::updateLastItem()
{
    if(!m_items.isEmpty())
    {
        currentItemClicked(m_items.back());
    }
}

void MusicBackgroundListWidget::itemCloseClicked(MusicBackgroundListItem *item)
{
    if(m_items.count() == 1)
    {
        MusicToastLabel::popup(tr("Last one item can not be deleted"));
        return;
    }

    const int index = find(item);
    const int cIndex = find(m_currentItem);
    QFile::remove(item->filePath());
    m_gridLayout->removeWidget(item);
    m_items.takeAt(index)->deleteLater();

    if(index == cIndex)
    {
        m_currentItem = nullptr;
        if(!m_items.isEmpty())
        {
            currentItemClicked(m_items[index == 0 ? 0 : index - 1]);
        }
    }

    for(int i = index; i < m_items.count(); ++i)
    {
        m_gridLayout->addWidget(m_items[i], i / MIN_ITEM_COUNT, i % MIN_ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    }
}

void MusicBackgroundListWidget::currentItemClicked(MusicBackgroundListItem *item)
{
    if(m_currentItem)
    {
        m_currentItem->setSelected(false);
    }

    m_currentItem = item;
    m_currentItem->setSelected(true);
    Q_EMIT itemClicked(m_type, item->fileName());
}
