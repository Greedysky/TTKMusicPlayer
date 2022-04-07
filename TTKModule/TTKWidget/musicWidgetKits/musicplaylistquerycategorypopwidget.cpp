#include "musicplaylistquerycategorypopwidget.h"
#include "musicwidgetutils.h"
#include "musicclickedlabel.h"
#include "musicwidgetheaders.h"
#include "musicclickedgroup.h"

#define ITEM_MAX_COLUMN     6
#define ITEM_LABEL_WIDTH    20

MusicPlaylistQueryCategoryItem::MusicPlaylistQueryCategoryItem(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(QString());
}

void MusicPlaylistQueryCategoryItem::setCategory(const MusicResultsCategory &category)
{
    m_category = category;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(category.m_category, this);
    label->setStyleSheet(MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSFontStyle03);
    label->setFixedSize(50, ITEM_LABEL_WIDTH);
    layout->addWidget(label, 0, Qt::AlignTop);

    QWidget *item = new QWidget(this);
    QGridLayout *itemlayout = new QGridLayout(item);
    itemlayout->setContentsMargins(0, 0, 0, 0);

    MusicClickedGroup *clickedGroup = new MusicClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    for(int i = 0; i < m_category.m_items.count(); ++i)
    {
        MusicClickedLabel *l = new MusicClickedLabel(m_category.m_items[i].m_value, item);
        l->setStyleSheet(QString("QLabel::hover{%1}").arg(MusicUIObject::MQSSColorStyle08));
        l->setFixedSize(75, ITEM_LABEL_WIDTH);

        clickedGroup->mapped(l);
        itemlayout->addWidget(l, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
    }
    item->setLayout(itemlayout);

    layout->addWidget(item, 0, Qt::AlignTop);
    setLayout(layout);
}

void MusicPlaylistQueryCategoryItem::buttonClicked(int index)
{
    if(0 <= index && index < m_category.m_items.count())
    {
        Q_EMIT categoryChanged(m_category.m_items[index]);
    }
}



MusicPlaylistFoundCategoryPopWidget::MusicPlaylistFoundCategoryPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

void MusicPlaylistFoundCategoryPopWidget::setCategory(const QString &server, QObject *obj)
{
    MusicResultsCategoryList categorys;
    MusicCategoryConfigManager manager;
    manager.fromFile(MusicCategoryConfigManager::Playlist);
    manager.readBuffer(categorys, server);

    QVBoxLayout *layout = new QVBoxLayout(m_containWidget);
    QWidget *containWidget = new QWidget(m_containWidget);
    containWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle17);
    QVBoxLayout *containLayout = new QVBoxLayout(containWidget);
    containWidget->setLayout(containLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, containWidget);
    layout->addWidget(scrollArea);

    for(const MusicResultsCategory &category : qAsConst(categorys))
    {
        MusicPlaylistQueryCategoryItem *item = new MusicPlaylistQueryCategoryItem(this);
        connect(item, SIGNAL(categoryChanged(MusicResultsCategoryItem)), obj, SLOT(categoryChanged(MusicResultsCategoryItem)));
        item->setCategory(category);
        containLayout->addWidget(item);
    }
    m_containWidget->setLayout(layout);
}

void MusicPlaylistFoundCategoryPopWidget::closeMenu()
{
    m_menu->close();
}

void MusicPlaylistFoundCategoryPopWidget::popupMenu()
{
    m_menu->exec(mapToGlobal(QPoint(0, 0)));
}

void MusicPlaylistFoundCategoryPopWidget::initialize()
{
    setFixedSize(100, 30);
    setTranslucentBackground();
    setText(tr("All"));

    const QString &style = MusicUIObject::MQSSBorderStyle04 + MusicUIObject::MQSSBackgroundStyle17;
    setObjectName("mianWidget");
    setStyleSheet(QString("#mianWidget{%1}").arg(style));

    m_containWidget->setFixedSize(600, 370);
    m_containWidget->setObjectName("containWidget");
    m_containWidget->setStyleSheet(QString("#containWidget{%1}").arg(style));

    m_menu->setStyleSheet(MusicUIObject::MQSSMenuStyle05);
}
