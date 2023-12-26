#include "musicartistlistquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictinyuiobject.h"
#include "musicartistlistquerycategorypopwidget.h"
#include "musicpagequerywidget.h"
#include "musicrightareawidget.h"
#include "ttkclickedgroup.h"

#include <qmath.h>

static constexpr int WIDTH_LABEL_SIZE = 150;
static constexpr int HEIGHT_LABEL_SIZE = 25;
static constexpr int LINE_SPACING_SIZE = 150;

MusicArtistListQueryItemWidget::MusicArtistListQueryItemWidget(QWidget *parent)
    : TTKClickedLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet(TTK::UI::ColorStyle02);
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    connect(this, SIGNAL(clicked()), SLOT(currentItemClicked()));
}

void MusicArtistListQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    setToolTip(item.m_name);
    setText(TTK::Widget::elidedText(font(), toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
}

void MusicArtistListQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}



MusicArtistListQueryWidget::MusicArtistListQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_categoryChanged(false),
      m_gridLayout(nullptr),
      m_pageQueryWidget(nullptr),
      m_categoryButton(nullptr)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

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

void MusicArtistListQueryWidget::setCurrentID(const QString &id)
{
    setCurrentValue(id);
}

void MusicArtistListQueryWidget::resizeWidget()
{
    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(const TTKResizeWidget &widget : qAsConst(m_resizeWidgets))
        {
            m_gridLayout->removeWidget(widget.m_label);
        }

        const int lineNumber = QUERY_WIDGET_WIDTH / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistListQueryWidget::createArtistListItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        TTK::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QVBoxLayout *mainlayout = TTKObjectCast(QVBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QVBoxLayout *containTopLayout  = new QVBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicArtistListQueryCategoryPopWidget(m_mainWindow);
        m_categoryButton->setCategory(m_networkRequest->queryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        //
        QWidget *containNumberWidget = new QWidget(containTopWidget);
        QHBoxLayout *containNumberLayout  = new QHBoxLayout(containNumberWidget);
#ifdef Q_OS_WIN
        containNumberLayout->setSpacing(10);
#endif
        TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
        connect(clickedGroup, SIGNAL(clicked(int)), SLOT(numberButtonClicked(int)));

        for(int i = -1; i < 27; ++i)
        {
            TTKClickedLabel *l = new TTKClickedLabel(QString(TTKStaticCast(char, i + 65)), containNumberWidget);
            l->setStyleSheet(QString("QLabel::hover{%1} QLabel{%2}").arg(TTK::UI::ColorStyle07, TTK::UI::ColorStyle08));

            if(i == -1)
            {
                l->setText(tr("Hot"));
            }
            else if(i == 26)
            {
                l->setText(tr("#"));
            }

            clickedGroup->mapped(l);
            containNumberLayout->addWidget(l);
        }
        containNumberWidget->setLayout(containNumberLayout);
        containTopLayout->addWidget(containNumberWidget);
        //
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(15);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);

        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWindow);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
        mainlayout->addWidget(m_pageQueryWidget->createPageWidget(m_mainWindow, pageTotal));
        mainlayout->addStretch(1);
    }

    if(m_categoryChanged && m_pageQueryWidget)
    {
        m_categoryChanged = false;
        const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
        m_pageQueryWidget->reset(pageTotal);
    }

    MusicArtistListQueryItemWidget *label = new MusicArtistListQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentArtistListClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    const int lineNumber = QUERY_WIDGET_WIDTH / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicArtistListQueryWidget::currentArtistListClicked(const MusicResultDataItem &item)
{
    MusicRightAreaWidget::instance()->artistSearchBy(item.m_id);
}

void MusicArtistListQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_categoryId = category.m_key;
        m_value.clear();

        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        numberButtonClicked(TTK_NORMAL_LEVEL);
    }
}

void MusicArtistListQueryWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast().m_label;
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
    m_pageQueryWidget->page(index, pageTotal);
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}

void MusicArtistListQueryWidget::numberButtonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast().m_label;
        m_gridLayout->removeWidget(w);
        delete w;
    }
    m_categoryChanged = true;

    const QString &v = QString("%1%2%3").arg(m_categoryId, TTK_SPLITER).arg(index);
    m_networkRequest->startToSearch(v);
}
