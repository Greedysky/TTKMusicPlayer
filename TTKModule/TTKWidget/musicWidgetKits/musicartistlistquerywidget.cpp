#include "musicartistlistquerywidget.h"
#include "musicartistlistquerycategorypopwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicpagequerywidget.h"
#include "musicrightareawidget.h"
#include "ttkclickedgroup.h"
#include "musicqueryitemwidget.h"

MusicArtistListQueryWidget::MusicArtistListQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_categoryChanged(false),
      m_gridLayout(nullptr),
      m_pageQueryWidget(nullptr),
      m_categoryButton(nullptr)
{
    m_container->show();
    layout()->removeWidget(m_mainWidget);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWidget);

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeArtistListRequest(this);
    connect(m_networkRequest, SIGNAL(createArtistListItem(MusicResultDataItem)), SLOT(createArtistListItem(MusicResultDataItem)));
}

MusicArtistListQueryWidget::~MusicArtistListQueryWidget()
{
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_pageQueryWidget;
}

void MusicArtistListQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearch({});
}

void MusicArtistListQueryWidget::setCurrentKey(const QString &id)
{
    setCurrentValue(id);
}

void MusicArtistListQueryWidget::resizeGeometry()
{
    if(m_resizeWidgets.isEmpty() || !m_gridLayout)
    {
        return;
    }

    for(const Data &widget : qAsConst(m_resizeWidgets))
    {
        m_gridLayout->removeWidget(widget.m_label);
    }

    const int lineSize = MusicTextQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
    }
}

void MusicArtistListQueryWidget::createArtistListItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWidget);
        QScrollArea *scrollArea = new QScrollArea(this);
        TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QVBoxLayout *mainLayout = TTKObjectCast(QVBoxLayout*, m_mainWidget->layout());
        QWidget *containTopWidget = new QWidget(m_mainWidget);
        QVBoxLayout *containTopLayout = new QVBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);

        m_categoryButton = new MusicArtistListQueryCategoryPopWidget(m_mainWidget);
        m_categoryButton->setCategory(m_networkRequest->queryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        //
        QWidget *containNumberWidget = new QWidget(containTopWidget);
        QHBoxLayout *containNumberLayout = new QHBoxLayout(containNumberWidget);
#ifdef Q_OS_WIN
        containNumberLayout->setSpacing(10);
#endif
        TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
        connect(clickedGroup, SIGNAL(clicked(int)), SLOT(numberButtonClicked(int)));

        for(int i = -1; i < 27; ++i)
        {
            TTKClickedLabel *label = new TTKClickedLabel(QString(TTKStaticCast(char, i + 65)), containNumberWidget);
            label->setStyleSheet(QString("QLabel::hover{ %1 } QLabel{ %2 }").arg(TTK::UI::ColorStyle07, TTK::UI::ColorStyle08));

            if(i == -1)
            {
                label->setText(tr("Hot"));
            }
            else if(i == 26)
            {
                label->setText(tr("#"));
            }

            clickedGroup->addWidget(label);
            containNumberLayout->addWidget(label);
        }

        containNumberWidget->setLayout(containNumberLayout);
        containTopLayout->addWidget(containNumberWidget);
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWidget);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);

        QWidget *containWidget = new QWidget(m_mainWidget);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(15);
        containWidget->setLayout(m_gridLayout);

        mainLayout->addWidget(containTopWidget);
        mainLayout->addWidget(line);
        mainLayout->addWidget(containWidget);

        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWidget);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        mainLayout->addWidget(m_pageQueryWidget->createPageWidget(m_mainWidget, m_networkRequest->pageTotalSize()));
        mainLayout->addStretch(1);
    }

    if(m_categoryChanged && m_pageQueryWidget)
    {
        m_categoryChanged = false;
        m_pageQueryWidget->reset(m_networkRequest->pageTotalSize());
    }

    MusicTextQueryItemWidget *label = new MusicTextQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    const int lineSize = MusicTextQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);
    m_resizeWidgets.append({label, label->font()});
}

void MusicArtistListQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    MusicRightAreaWidget::instance()->artistSearchByID(item.m_id);
}

void MusicArtistListQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_value.clear();
        m_categoryId = category.m_key;

        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        numberButtonClicked(TTK_NORMAL_LEVEL);
    }
}

void MusicArtistListQueryWidget::buttonClicked(int index)
{
    removeItems(m_gridLayout);
    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}

void MusicArtistListQueryWidget::numberButtonClicked(int index)
{
    removeItems(m_gridLayout);
    m_categoryChanged = true;
    m_networkRequest->startToSearch(QString("%1%2%3").arg(m_categoryId, TTK_SPLITER).arg(index));
}
