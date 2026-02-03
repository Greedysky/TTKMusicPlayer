#include "musicplaylistquerycategorypopwidget.h"
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

static constexpr int ITEM_MAX_COLUMN = 6;
static constexpr int ITEM_LABEL_HEIGHT = 20;

MusicPlaylistQueryCategoryItem::MusicPlaylistQueryCategoryItem(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet({});
}

void MusicPlaylistQueryCategoryItem::setCategory(const MusicResultsCategory &category)
{
    m_category = category;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(category.m_category, this);
    label->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    label->setFixedSize(50, ITEM_LABEL_HEIGHT);
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
        label->setFixedSize(75, ITEM_LABEL_HEIGHT);

        clickedGroup->addWidget(label);
        itemLayout->addWidget(label, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
    }

    item->setLayout(itemLayout);
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
    MusicCategoryConfigManager manager(server);
    if(manager.fromFile(MusicCategoryConfigManager::Category::PlayList))
    {
        manager.readBuffer(categorys);
    }

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

    const QString &style = TTK::UI::BorderStyle02 + TTK::UI::BackgroundStyle10;
    setObjectName(MusicPlaylistFoundCategoryPopWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), style));

    m_menu->setStyleSheet(TTK::UI::MenuStyle05);
    m_containWidget->setFixedSize(600, 370);
    m_containWidget->setObjectName("ContainWidget");
    m_containWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_containWidget->objectName(), style));
}
