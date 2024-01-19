#include "musictoplistquerycategorypopwidget.h"
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

static constexpr int ITEM_MAX_COLUMN = 2;
static constexpr int ITEM_LABEL_WIDTH = 20;

MusicToplistQueryCategoryItem::MusicToplistQueryCategoryItem(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet({});
}

void MusicToplistQueryCategoryItem::setCategory(const MusicResultsCategory &category)
{
    m_category = category;

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(category.m_category, this);
    label->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    label->setFixedSize(100, ITEM_LABEL_WIDTH);
    layout->addWidget(label, 0, Qt::AlignTop);

    QWidget *item = new QWidget(this);
    QGridLayout *itemlayout = new QGridLayout(item);
    itemlayout->setContentsMargins(0, 0, 0, 0);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    for(int i = 0; i < m_category.m_items.count(); ++i)
    {
        TTKClickedLabel *l = new TTKClickedLabel(m_category.m_items[i].m_value, item);
        l->setStyleSheet(QString("QLabel::hover{%1}").arg(TTK::UI::ColorStyle07));
        l->setFixedSize(200, ITEM_LABEL_WIDTH);

        clickedGroup->mapped(l);
        itemlayout->addWidget(l, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
    }
    item->setLayout(itemlayout);

    layout->addWidget(item, 0, Qt::AlignTop);
    setLayout(layout);
}

void MusicToplistQueryCategoryItem::buttonClicked(int index)
{
    if(0 <= index && index < m_category.m_items.count())
    {
        Q_EMIT categoryChanged(m_category.m_items[index]);
    }
}


MusicToplistQueryCategoryPopWidget::MusicToplistQueryCategoryPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

void MusicToplistQueryCategoryPopWidget::setCategory(const QString &server, QObject *obj)
{
    MusicResultsCategoryList categorys;
    MusicCategoryConfigManager manager(server);
    manager.fromFile(MusicCategoryConfigManager::Category::TopList);
    manager.readBuffer(categorys);

    QVBoxLayout *layout = new QVBoxLayout(m_containWidget);
    QWidget *containWidget = new QWidget(m_containWidget);
    containWidget->setStyleSheet(TTK::UI::BackgroundStyle10);
    QVBoxLayout *containLayout = new QVBoxLayout(containWidget);
    containWidget->setLayout(containLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaFormat(scrollArea, containWidget);
    layout->addWidget(scrollArea);

    for(const MusicResultsCategory &category : qAsConst(categorys))
    {
        MusicToplistQueryCategoryItem *item = new MusicToplistQueryCategoryItem(this);
        connect(item, SIGNAL(categoryChanged(MusicResultsCategoryItem)), obj, SLOT(categoryChanged(MusicResultsCategoryItem)));
        item->setCategory(category);
        containLayout->addWidget(item);
    }
    m_containWidget->setLayout(layout);
}

void MusicToplistQueryCategoryPopWidget::closeMenu()
{
    m_menu->close();
}

void MusicToplistQueryCategoryPopWidget::popupMenu()
{
    m_menu->exec(mapToGlobal(QPoint(0, 0)));
}

void MusicToplistQueryCategoryPopWidget::initialize()
{
    setFixedSize(200, 30);
    setTranslucentBackground();
    setText(tr("All"));

    const QString &style = TTK::UI::BorderStyle03 + TTK::UI::BackgroundStyle10;
    setObjectName(className());
    setStyleSheet(QString("#%1{%2}").arg(className(), style));

    m_containWidget->setFixedSize(600, 370);
    m_containWidget->setObjectName("ContainWidget");
    m_containWidget->setStyleSheet(QString("#ContainWidget{%1}").arg(style));

    m_menu->setStyleSheet(TTK::UI::MenuStyle04);
}
