#include "musicplaylistquerywidget.h"
#include "musicqueryitemwidget.h"
#include "musicpagequerywidget.h"
#include "musicqueryplaylistrequest.h"
#include "musicplaylistqueryinfowidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicplaylistquerycategorypopwidget.h"

MusicPlaylistQueryWidget::MusicPlaylistQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_categoryChanged(false),
      m_gridLayout(nullptr),
      m_pageQueryWidget(nullptr),
      m_infoWidget(nullptr),
      m_categoryButton(nullptr)
{
    m_container->show();
    layout()->removeWidget(m_mainWidget);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWidget);

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makePlaylistRequest(this);
    connect(m_networkRequest, SIGNAL(createPlaylistItem(MusicResultDataItem)), SLOT(createPlaylistItem(MusicResultDataItem)));
}

MusicPlaylistQueryWidget::~MusicPlaylistQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_pageQueryWidget;
}

void MusicPlaylistQueryWidget::setCurrentCategory(const MusicResultsCategoryItem &category)
{
    MusicAbstractItemQueryWidget::setCurrentValue(category.m_value);
    m_networkRequest->startToSearchByID(category.m_key);
}

void MusicPlaylistQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearchByID({});
}

void MusicPlaylistQueryWidget::setCurrentKey(const QString &id)
{
    MusicResultDataItem item;
    item.m_id = id;
    currentItemClicked(item, true);
}

void MusicPlaylistQueryWidget::resizeGeometry()
{
    if(m_infoWidget)
    {
        m_infoWidget->resizeGeometry();
    }

    if(m_resizeWidgets.isEmpty() || !m_gridLayout)
    {
        return;
    }

    for(const Data &widget : qAsConst(m_resizeWidgets))
    {
        m_gridLayout->removeWidget(widget.m_label);
    }

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}

void MusicPlaylistQueryWidget::createPlaylistItem(const MusicResultDataItem &item)
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
        QHBoxLayout *containTopLayout = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(10, 0, 10, 0);

        m_categoryButton = new MusicPlaylistFoundCategoryPopWidget(m_mainWidget);
        m_categoryButton->setCategory(m_networkRequest->queryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);

        const QStringList titles{tr("Recommend"), tr("Top"), tr("Hot"), tr("New")};
        for(const QString &data : qAsConst(titles))
        {
            QFrame *hline = new QFrame(containTopWidget);
            hline->setFrameShape(QFrame::VLine);
            hline->setStyleSheet(TTK::UI::ColorStyle12);
            containTopLayout->addWidget(new QLabel(data, containTopWidget));
            containTopLayout->addWidget(hline);
        }
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWidget);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);

        QWidget *containWidget = new QWidget(m_mainWidget);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(20);
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

    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setShowTime(false);
    label->setShowCount(true);
    label->setResultDataItem(item, G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);
    m_resizeWidgets.append({label, label->font()});
}

void MusicPlaylistQueryWidget::currentItemClicked(const MusicResultDataItem &item, bool single)
{
    delete m_infoWidget;
    m_infoWidget = new MusicPlaylistQueryInfoWidget(this);

    MusicQueryPlaylistRequest *req = TTKObjectCast(MusicQueryPlaylistRequest*, G_DOWNLOAD_QUERY_PTR->makePlaylistRequest(this));
    MusicResultDataItem info(item);
    if(info.isEmpty())
    {
        req->startToQueryInfo(info);
    }

    m_infoWidget->setQueryInput(req);
    m_infoWidget->setResultDataItem(info, single ? nullptr : this);
    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicPlaylistQueryWidget::backToPlaylistMenu()
{
    m_container->setCurrentIndex(0);
}

void MusicPlaylistQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_value.clear();
        m_categoryChanged = true;
        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        removeItems(m_gridLayout);
        m_networkRequest->startToSearchByID(category.m_key);
    }
}

void MusicPlaylistQueryWidget::buttonClicked(int index)
{
    removeItems(m_gridLayout);
    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}
