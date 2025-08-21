#include "musicartistlistquerycategorypopwidget.h"
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

static constexpr int ITEM_MAX_COLUMN = 2;
static constexpr int ITEM_LABEL_HEIGHT = 20;

MusicArtistListQueryCategoryItem::MusicArtistListQueryCategoryItem(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet({});
}

void MusicArtistListQueryCategoryItem::setCategory(const MusicResultsCategory &category)
{
    m_category = category;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(category.m_category, this);
    label->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    label->setFixedSize(100, ITEM_LABEL_HEIGHT);
    layout->addWidget(label, 0, Qt::AlignTop);

    QWidget *item = new QWidget(this);
    QGridLayout *itemLayout = new QGridLayout(item);
    itemLayout->setContentsMargins(0, 0, 0, 0);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    for(int i = 0; i < m_category.m_items.count(); ++i)
    {
        TTKClickedLabel *label = new TTKClickedLabel(m_category.m_items[i].m_value, item);
        label->setStyleSheet(QString("QLabel::hover{ %1 }").arg(TTK::UI::ColorStyle07));
        label->setFixedSize(200, ITEM_LABEL_HEIGHT);

        clickedGroup->mapped(label);
        itemLayout->addWidget(label, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
    }

    item->setLayout(itemLayout);
    layout->addWidget(item, 0, Qt::AlignTop);
    setLayout(layout);
}

void MusicArtistListQueryCategoryItem::buttonClicked(int index)
{
    if(0 <= index && index < m_category.m_items.count())
    {
        Q_EMIT categoryChanged(m_category.m_items[index]);
    }
}


MusicArtistListQueryCategoryPopWidget::MusicArtistListQueryCategoryPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

void MusicArtistListQueryCategoryPopWidget::setCategory(const QString &server, QObject *obj)
{
    MusicResultsCategoryList categorys;
    MusicCategoryConfigManager manager(server);
    manager.fromFile(MusicCategoryConfigManager::Category::ArtistList);
    manager.readBuffer(categorys);

    QVBoxLayout *layout = new QVBoxLayout(m_containWidget);
    QWidget *containWidget = new QWidget(m_containWidget);
    containWidget->setStyleSheet(TTK::UI::BackgroundStyle10);
    QVBoxLayout *containLayout = new QVBoxLayout(containWidget);
    containWidget->setLayout(containLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, containWidget);
    layout->addWidget(scrollArea);

    for(const MusicResultsCategory &category : qAsConst(categorys))
    {
        MusicArtistListQueryCategoryItem *item = new MusicArtistListQueryCategoryItem(this);
        connect(item, SIGNAL(categoryChanged(MusicResultsCategoryItem)), obj, SLOT(categoryChanged(MusicResultsCategoryItem)));
        item->setCategory(category);
        containLayout->addWidget(item);
    }
    m_containWidget->setLayout(layout);
}

void MusicArtistListQueryCategoryPopWidget::closeMenu()
{
    m_menu->close();
}

void MusicArtistListQueryCategoryPopWidget::popupMenu()
{
    m_menu->exec(mapToGlobal(QPoint(0, 0)));
}

void MusicArtistListQueryCategoryPopWidget::initialize()
{
    setFixedSize(100, 30);
    setTranslucentBackground();
    setText(tr("All"));

    QString style = TTK::UI::BorderStyle02 + TTK::UI::BackgroundStyle10;
    setObjectName(MusicArtistListQueryCategoryPopWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), style));

    m_menu->setStyleSheet(TTK::UI::MenuStyle05);
    m_containWidget->setFixedSize(600, 370);
    m_containWidget->setObjectName("ContainWidget");
    m_containWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_containWidget->objectName(), style));
}
