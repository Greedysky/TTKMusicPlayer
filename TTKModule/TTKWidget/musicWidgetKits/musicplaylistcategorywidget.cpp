#include "musicplaylistcategorywidget.h"
#include "musiccategoryconfigmanager.h"
#include "musicabstractqueryrequest.h"
#include "musicuiobject.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

#include <QBoxLayout>

static constexpr int ITEM_MAX_COLUMN = 6;
static constexpr int ITEM_LABEL_HEIGHT = 20;
static constexpr int LINE_SPACING_SIZE = 75;

MusicPlaylistCategoryItem::MusicPlaylistCategoryItem(MusicResultsCategory *category, const QString &tag, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 0, 20, 0);

    QWidget *leftLabel = new QWidget(this);
    QVBoxLayout *leftLabelLayout = new QVBoxLayout(leftLabel);
    leftLabel->setFixedWidth(100);
    leftLabel->setLayout(leftLabelLayout);
    leftLabel->setStyleSheet("background:red");
    layout->addWidget(leftLabel);

    QLabel *iconLabel = new QLabel(leftLabel);
    iconLabel->setFixedSize(38, 38);
    iconLabel->setPixmap(QPixmap(":/playlist/" + tag));
    leftLabelLayout->addWidget(iconLabel, 0, Qt::AlignCenter);

    QLabel *label = new QLabel(category->m_category, leftLabel);
    label->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    leftLabelLayout->addWidget(label, 0, Qt::AlignCenter);

    QWidget *item = new QWidget(this);
    m_gridLayout = new QGridLayout(item);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
//    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    for(int i = 0; i < category->m_items.count(); ++i)
    {
        TTKClickedLabel *label = new TTKClickedLabel(category->m_items[i].m_value, item);
        label->setStyleSheet(QString("QLabel::hover{ %1 }").arg(TTK::UI::ColorStyle07));
        label->setFixedSize(LINE_SPACING_SIZE, ITEM_LABEL_HEIGHT);

        clickedGroup->mapped(label);

        m_items << label;
        m_gridLayout->addWidget(label, i / ITEM_MAX_COLUMN, i % ITEM_MAX_COLUMN, Qt::AlignLeft);
    }

    item->setLayout(m_gridLayout);
    layout->addWidget(item);
}

void MusicPlaylistCategoryItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    for(QWidget *item : qAsConst(m_items))
    {
        m_gridLayout->removeWidget(item);
    }

    const int lineNumber = width() / LINE_SPACING_SIZE;
    for(int i = 0; i < m_items.count(); ++i)
    {
        m_gridLayout->addWidget(m_items[i], i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}


MusicPlaylistCategoryWidget::MusicPlaylistCategoryWidget(QWidget *parent)
    : QFrame(parent)
{
    initialize();
}

void MusicPlaylistCategoryWidget::initialize()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    setStyleSheet(TTK::UI::BackgroundStyle10);

    QStringList tags;
    QString server = QUERY_WY_INTERFACE;

    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
    {
    case MusicAbstractQueryRequest::QueryServer::WY:
        server = QUERY_WY_INTERFACE;
        tags << "h" << "b" << "a" << "g" << "i";
        break;
    case MusicAbstractQueryRequest::QueryServer::KW:
        server = QUERY_KW_INTERFACE;
        tags << "i" << "g" << "a" << "d" << "b" << "h";
        break;
    case MusicAbstractQueryRequest::QueryServer::KG:
        server = QUERY_KG_INTERFACE;
        tags << "a" << "i" << "h" << "b" << "g" << "d" << "c";
        break;
    default: break;
    }

    MusicResultsCategoryList categorys;
    MusicCategoryConfigManager manager(server);
    manager.fromFile(MusicCategoryConfigManager::Category::PlayList);
    manager.readBuffer(categorys);

    if(tags.count() != categorys.count() - 1)
    {
        TTK_ERROR_STREAM("Playlist tags is not the same from category");
        return;
    }

    for(int i = 0; i < categorys.count() - 1; ++i)
    {
        MusicPlaylistCategoryItem *item = new MusicPlaylistCategoryItem(&categorys[i + 1], tags[i], this);
        layout->addWidget(item);

        QFrame *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);
        layout->addWidget(line);
    }
}
