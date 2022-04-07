#include "musicwebmvradioquerycategorypopwidget.h"
#include "musicwidgetutils.h"
#include "musicclickedlabel.h"
#include "musicwidgetheaders.h"
#include "musicclickedgroup.h"

#define ITEM_MAX_COLUMN     2
#define ITEM_LABEL_WIDTH    20

MusicWebMVRadioQueryCategoryItem::MusicWebMVRadioQueryCategoryItem(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(QString());
}

void MusicWebMVRadioQueryCategoryItem::setCategory(const MusicResultsCategory &category)
{
    m_category = category;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(category.m_category, this);
    label->setStyleSheet(MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSFontStyle03);
    label->setFixedSize(100, ITEM_LABEL_WIDTH);
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
        l->setFixedSize(200, ITEM_LABEL_WIDTH);

        itemlayout->addWidget(l, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
        clickedGroup->mapped(l);
    }
    item->setLayout(itemlayout);

    layout->addWidget(item, 0, Qt::AlignTop);
    setLayout(layout);
}

void MusicWebMVRadioQueryCategoryItem::buttonClicked(int index)
{
    if(0 <= index && index < m_category.m_items.count())
    {
        Q_EMIT categoryChanged(m_category.m_items[index]);
    }
}



MusicWebMVRadioQueryCategoryPopWidget::MusicWebMVRadioQueryCategoryPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

void MusicWebMVRadioQueryCategoryPopWidget::setCategory(const QString &server, QObject *obj)
{
    MusicResultsCategoryList categorys;
    MusicCategoryConfigManager manager;
    manager.fromFile(MusicCategoryConfigManager::MovieList);
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
        MusicWebMVRadioQueryCategoryItem *item = new MusicWebMVRadioQueryCategoryItem(this);
        connect(item, SIGNAL(categoryChanged(MusicResultsCategoryItem)), obj, SLOT(categoryChanged(MusicResultsCategoryItem)));
        item->setCategory(category);
        containLayout->addWidget(item);
    }
    m_containWidget->setLayout(layout);
}

void MusicWebMVRadioQueryCategoryPopWidget::closeMenu()
{
    m_menu->close();
}

void MusicWebMVRadioQueryCategoryPopWidget::popupMenu()
{
    m_menu->exec(mapToGlobal(QPoint(0, 0)));
}

void MusicWebMVRadioQueryCategoryPopWidget::initWidget()
{
    setFixedSize(150, 30);
    setTranslucentBackground();
    setText(tr("All"));

    const QString &style = MusicUIObject::MQSSBorderStyle04 + MusicUIObject::MQSSBackgroundStyle17;
    setObjectName("mianWidget");
    setStyleSheet(QString("#mianWidget{%1}").arg(style));

    m_containWidget->setFixedSize(480, 200);
    m_containWidget->setObjectName("containWidget");
    m_containWidget->setStyleSheet(QString("#containWidget{%1}").arg(style));

    m_menu->setStyleSheet(MusicUIObject::MQSSMenuStyle05);
}
