#include "musicplaylistcategorywidget.h"
#include "musiccategoryconfigmanager.h"
#include "musicabstractqueryrequest.h"
#include "musicrightareawidget.h"
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

struct CategoryProperty
{
    enum
    {
        New,
        Hot
    };

    struct Property
    {
        int m_index;
        int m_type;
    };

    QString m_tag;
    MusicResultsCategory m_category;
    QList<Property> m_property;
};
TTK_DECLARE_LIST(CategoryProperty);


static constexpr int ITEM_MAX_COLUMN = 6;
static constexpr int ITEM_LABEL_HEIGHT = 20;
static constexpr int LINE_SPACING_SIZE = 75;

using CategorySingleItem = TTKClickedLabel;

class CategoryMultiItem : public QWidget
{
public:
    CategoryMultiItem(const QString &text, const QString &icon, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);

        m_item = new CategorySingleItem(text, this);
        layout->addWidget(m_item);

        QLabel *ico = new QLabel(this);
        ico->setPixmap(QPixmap(icon));
        layout->addWidget(ico);
    }

    CategorySingleItem *m_item;
};


MusicPlaylistCategoryItem::MusicPlaylistCategoryItem(CategoryProperty *category, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 0, 20, 0);

    QWidget *leftLabel = new QWidget(this);
    QVBoxLayout *leftLabelLayout = new QVBoxLayout(leftLabel);
    leftLabelLayout->addStretch();
    leftLabel->setFixedWidth(100);
    leftLabel->setLayout(leftLabelLayout);
    layout->addWidget(leftLabel);

    QLabel *iconLabel = new QLabel(leftLabel);
    iconLabel->setFixedSize(38, 38);
    iconLabel->setPixmap(QPixmap(":/playlist/" + category->m_tag));
    leftLabelLayout->addWidget(iconLabel, 0, Qt::AlignCenter);

    QLabel *textLabel = new QLabel(category->m_category.m_category, leftLabel);
    textLabel->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    leftLabelLayout->addWidget(textLabel, 0, Qt::AlignCenter);
    leftLabelLayout->addStretch();

    QWidget *item = new QWidget(this);
    m_gridLayout = new QGridLayout(item);
    m_gridLayout->setContentsMargins(0, 10, 0, 10);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    const MusicResultsCategoryItemList &items = category->m_category.m_items;

    for(int i = 0; i < items.count(); ++i)
    {
        QWidget *label = nullptr;

        for(const CategoryProperty::Property &property : qAsConst(category->m_property))
        {
            if(property.m_index != i)
            {
                continue;
            }

            const QString &icon = property.m_type == CategoryProperty::New ? "lb_new" : "lb_hot";
            label = new CategoryMultiItem(items[i].m_value, ":/playlist/" + icon, item);
            clickedGroup->mapped(TTKStaticCast(CategoryMultiItem*, label)->m_item);
        }

        if(!label)
        {
            label = new CategorySingleItem(items[i].m_value, item);
            clickedGroup->mapped(label);
        }

        label->setProperty("key", items[i].m_key);
        label->setProperty("value", items[i].m_value);
        label->setStyleSheet(QString("QLabel::hover{ %1 }").arg(TTK::UI::ColorStyle07));
        label->setFixedSize(LINE_SPACING_SIZE, ITEM_LABEL_HEIGHT);

        m_items << label;
        m_gridLayout->addWidget(label, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
    }

    item->setLayout(m_gridLayout);
    layout->addWidget(item);
}

MusicPlaylistCategoryItem::~MusicPlaylistCategoryItem()
{
    qDeleteAll(m_items);
    delete m_gridLayout;
}

void MusicPlaylistCategoryItem::resizeWindow()
{
    for(QWidget *item : qAsConst(m_items))
    {
        m_gridLayout->removeWidget(item);
    }

    const int lineNumber = (G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - LEFT_SIDE_WIDTH_MIN - 350) / LINE_SPACING_SIZE;
    for(int i = 0; i < m_items.count(); ++i)
    {
        m_gridLayout->addWidget(m_items[i], i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}

void MusicPlaylistCategoryItem::buttonClicked(int index)
{
    if(index < 0 || index >= m_items.count())
    {
        return;
    }

    const QString &key = m_items[index]->property("key").toString();
    const QString &value = m_items[index]->property("value").toString();
    MusicRightAreaWidget::instance()->showPlaylistCategoryFound(key, value);
}

void MusicPlaylistCategoryItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}


MusicPlaylistCategoryWidget::MusicPlaylistCategoryWidget(QWidget *parent)
    : QFrame(parent)
{
    initialize();
}

MusicPlaylistCategoryWidget::~MusicPlaylistCategoryWidget()
{
    qDeleteAll(m_items);
}

void MusicPlaylistCategoryWidget::resizeWidget()
{
    for(MusicPlaylistCategoryItem *item : qAsConst(m_items))
    {
        item->resizeWindow();
    }
}

void MusicPlaylistCategoryWidget::initialize()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    setStyleSheet(TTK::UI::BackgroundStyle10);

    QWidget *containWidget = new QWidget(this);
    QVBoxLayout *containLayout = new QVBoxLayout(containWidget);
    containWidget->setLayout(containLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaFormat(scrollArea, containWidget);
    layout->addWidget(scrollArea);

    CategoryPropertyList items;
    QString server = QUERY_WY_INTERFACE;

    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
    {
    case MusicAbstractQueryRequest::QueryServer::WY:
    {
        server = QUERY_WY_INTERFACE;
        items.append({"h", {}, {{0, CategoryProperty::Hot}, {3, CategoryProperty::New}, {4, CategoryProperty::New}}});
        items.append({"b", {}, {{0, CategoryProperty::Hot}, {5, CategoryProperty::New}, {9, CategoryProperty::New}, {15, CategoryProperty::New}, {21, CategoryProperty::Hot}}});
        items.append({"a", {}, {{2, CategoryProperty::New}, {7, CategoryProperty::New}, {8, CategoryProperty::New}}});
        items.append({"g", {}, {{1, CategoryProperty::Hot}, {3, CategoryProperty::New}, {9, CategoryProperty::New}}});
        items.append({"i", {}, {{0, CategoryProperty::New}, {1, CategoryProperty::Hot}, {8, CategoryProperty::New}, {15, CategoryProperty::New}}});
        break;
    }
    case MusicAbstractQueryRequest::QueryServer::KW:
    {
        server = QUERY_KW_INTERFACE;
        items.append({"i", {}, {{0, CategoryProperty::Hot}, {3, CategoryProperty::New}, {8, CategoryProperty::New}, {13, CategoryProperty::New}}});
        items.append({"g", {}, {{1, CategoryProperty::Hot}, {7, CategoryProperty::New}}});
        items.append({"a", {}, {{0, CategoryProperty::New}, {1, CategoryProperty::Hot}, {8, CategoryProperty::New}, {15, CategoryProperty::New}}});
        items.append({"d", {}, {{2, CategoryProperty::Hot}}});
        items.append({"b", {}, {{2, CategoryProperty::New}, {7, CategoryProperty::New}, {15, CategoryProperty::Hot}, {16, CategoryProperty::New}}});
        items.append({"h", {}, {{0, CategoryProperty::Hot}, {5, CategoryProperty::New}}});
        break;
    }
    case MusicAbstractQueryRequest::QueryServer::KG:
    {
        server = QUERY_KG_INTERFACE;
        items.append({"a", {}, {{0, CategoryProperty::Hot}, {5, CategoryProperty::New}, {9, CategoryProperty::New}, {15, CategoryProperty::New}, {21, CategoryProperty::Hot}}});
        items.append({"i", {}, {{3, CategoryProperty::New}, {5, CategoryProperty::New}, {11, CategoryProperty::Hot}, {13, CategoryProperty::New}}});
        items.append({"h", {}, {{0, CategoryProperty::Hot}, {6, CategoryProperty::New}}});
        items.append({"b", {}, {{0, CategoryProperty::Hot}, {4, CategoryProperty::New}, {10, CategoryProperty::New}, {16, CategoryProperty::New}}});
        items.append({"g", {}, {{1, CategoryProperty::Hot}, {3, CategoryProperty::New}, {12, CategoryProperty::New}}});
        items.append({"d", {}, {{0, CategoryProperty::Hot}}});
        items.append({"c", {}, {{2, CategoryProperty::New}, {5, CategoryProperty::New}, {9, CategoryProperty::Hot}}});
        break;
    }
    default: break;
    }

    MusicResultsCategoryList categorys;
    MusicCategoryConfigManager manager(server);
    manager.fromFile(MusicCategoryConfigManager::Category::PlayList);
    manager.readBuffer(categorys);

    if(items.count() != categorys.count() - 1)
    {
        TTK_ERROR_STREAM("Playlist tags is not the same from category");
        return;
    }

    for(int i = 0; i < categorys.count() - 1; ++i)
    {
        items[i].m_category = categorys[i + 1];

        MusicPlaylistCategoryItem *item = new MusicPlaylistCategoryItem(&items[i], this);
        containLayout->addWidget(item);
        m_items << item;

        QFrame *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);
        containLayout->addWidget(line);
    }
}
