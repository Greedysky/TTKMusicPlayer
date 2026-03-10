#include "musicresultscategorypopwidget.h"
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

struct Data
{
    QSize m_buttonSize;
    QSize m_containerSize;
    int m_column;
    int m_nameSize;
    int m_valueSize;
};

static Data CATEGORY_ITEMS[] = {
    {{100, 30}, {600, 370}, 6, 50, 75},   // PlayList
    {{200, 30}, {600, 370}, 2, 100, 200}, // TopList
    {{100, 30}, {600, 370}, 2, 100, 200}, // ArtistList
    {{150, 30}, {480, 220}, 2, 100, 200}, // MovieList
    {{100, 20}, {520, 250}, 5, 80, 75}    // SkinList
};
static constexpr int ITEM_LABEL_HEIGHT = 20;

MusicResultsCategoryPopItem::MusicResultsCategoryPopItem(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet({});
}

void MusicResultsCategoryPopItem::setCategory(MusicCategoryConfigManager::Category type, const MusicResultsCategory &category)
{
    m_category = category;
    const Data &data = CATEGORY_ITEMS[TTKStaticCast(int, type)];

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(category.m_category, this);
    label->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    label->setFixedSize(data.m_nameSize, ITEM_LABEL_HEIGHT);
    layout->addWidget(label, 0, Qt::AlignTop);

    QWidget *item = new QWidget(this);
    QGridLayout *itemLayout = new QGridLayout(item);
    itemLayout->setContentsMargins(0, 0, 0, 0);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    for(int i = 0; i < category.m_items.count(); ++i)
    {
        TTKClickedLabel *label = new TTKClickedLabel(category.m_items[i].m_value, item);
        label->setStyleSheet(QString("QLabel::hover{ %1 }").arg(TTK::UI::ColorStyle07));
        label->setFixedSize(data.m_valueSize, ITEM_LABEL_HEIGHT);

        clickedGroup->addWidget(label);
        itemLayout->addWidget(label, i / data.m_column, i % data.m_column, Qt::AlignLeft);
    }

    item->setLayout(itemLayout);
    layout->addWidget(item, 0, Qt::AlignTop);
    setLayout(layout);
}

void MusicResultsCategoryPopItem::buttonClicked(int index)
{
    if(0 <= index && index < m_category.m_items.count())
    {
        Q_EMIT categoryChanged(m_category.m_items[index]);
    }
}


MusicResultsCategoryPopWidget::MusicResultsCategoryPopWidget(MusicCategoryConfigManager::Category category, QWidget *parent)
    : MusicToolMenuWidget(parent),
      m_category(category)
{
    initialize();
}

void MusicResultsCategoryPopWidget::setCategory(const QString &server, QObject *obj)
{
    MusicResultsCategoryList categories;
    MusicCategoryConfigManager manager(server);
    if(manager.fromFile(m_category))
    {
        manager.readBuffer(categories);
    }

    QVBoxLayout *layout = new QVBoxLayout(m_containWidget);
    QWidget *containWidget = new QWidget(m_containWidget);
    containWidget->setStyleSheet(TTK::UI::BackgroundStyle10);
    QVBoxLayout *containLayout = new QVBoxLayout(containWidget);
    containWidget->setLayout(containLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, containWidget);
    layout->addWidget(scrollArea);

    for(const MusicResultsCategory &category : qAsConst(categories))
    {
        MusicResultsCategoryPopItem *item = new MusicResultsCategoryPopItem(this);
        connect(item, SIGNAL(categoryChanged(MusicResultsCategoryItem)), obj, SLOT(categoryChanged(MusicResultsCategoryItem)));
        item->setCategory(m_category, category);
        containLayout->addWidget(item);
    }
    m_containWidget->setLayout(layout);
}

void MusicResultsCategoryPopWidget::closeMenu()
{
    m_menu->close();
}

void MusicResultsCategoryPopWidget::popupMenu()
{
    m_menu->exec(mapToGlobal(QPoint(0, 0)));
}

void MusicResultsCategoryPopWidget::initialize()
{
    const Data &data = CATEGORY_ITEMS[TTKStaticCast(int, m_category)];

    setFixedSize(data.m_buttonSize);
    setTranslucentBackground();

    switch(m_category)
    {
        case MusicCategoryConfigManager::Category::TopList: setText(tr("Default")); break;
        case MusicCategoryConfigManager::Category::MovieList: setText(tr("Hot")); break;
        default: setText(tr("All")); break;
    }

    const QString &style = TTK::UI::BorderStyle02 + TTK::UI::BackgroundStyle10;
    setObjectName(MusicResultsCategoryPopWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), style));

    m_menu->setStyleSheet(TTK::UI::MenuStyle05);
    m_containWidget->setFixedSize(data.m_containerSize);
    m_containWidget->setObjectName("ContainWidget");
    m_containWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_containWidget->objectName(), style));
}
