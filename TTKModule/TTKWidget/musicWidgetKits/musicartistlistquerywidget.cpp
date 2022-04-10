#include "musicartistlistquerywidget.h"
#include "musicqueryplaylistrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musictinyuiobject.h"
#include "musicartistlistquerycategorypopwidget.h"
#include "musicpagequerywidget.h"
#include "musicrightareawidget.h"
#include "musicclickedgroup.h"

#include <qmath.h>

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  25
#define LINE_SPACING_SIZE  150

MusicArtistListQueryItemWidget::MusicArtistListQueryItemWidget(QWidget *parent)
    : MusicClickedLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet(MusicUIObject::MQSSColorStyle09);
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    connect(this, SIGNAL(clicked()), SLOT(currentItemClicked()));
}

void MusicArtistListQueryItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    setToolTip(item.m_name);
    setText(MusicUtils::Widget::elidedText(font(), toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
}

void MusicArtistListQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}



MusicArtistListQueryWidget::MusicArtistListQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_initialized = false;
    m_categoryChanged = false;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;
    m_pageQueryWidget = nullptr;
    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeArtistListRequest(this);
    connect(m_networkRequest, SIGNAL(createArtistListItem(MusicResultsItem)), SLOT(createArtistListItem(MusicResultsItem)));
}

MusicArtistListQueryWidget::~MusicArtistListQueryWidget()
{
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_networkRequest;
    delete m_pageQueryWidget;
}

void MusicArtistListQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::OtherQuery, QString());
}

void MusicArtistListQueryWidget::setSongNameById(const QString &id)
{
    setSongName(id);
}

void MusicArtistListQueryWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i].m_label);
        }

        const int lineNumber = width() / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistListQueryWidget::createArtistListItem(const MusicResultsItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QHBoxLayout *mainlayout = TTKStatic_cast(QHBoxLayout*, m_mainWindow->layout());
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
        containNumberLayout->setSpacing(15);
#endif
        MusicClickedGroup *clickedGroup = new MusicClickedGroup(this);
        connect(clickedGroup, SIGNAL(clicked(int)), SLOT(numberButtonClicked(int)));

        for(int i = -1; i < 27; ++i)
        {
            MusicClickedLabel *l = new MusicClickedLabel(QString(TTKStatic_cast(char, i + 65)), containNumberWidget);
            l->setStyleSheet(QString("QLabel::hover{%1} QLabel{%2}").arg(MusicUIObject::MQSSColorStyle08, MusicUIObject::MQSSColorStyle10));

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
        line->setStyleSheet(MusicUIObject::MQSSColorStyle06);

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
    connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SLOT(currentArtistListClicked(MusicResultsItem)));
    label->setMusicResultsItem(item);

    const int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicArtistListQueryWidget::currentArtistListClicked(const MusicResultsItem &item)
{
    MusicRightAreaWidget::instance()->musicArtistSearch(item.m_id);
}

void MusicArtistListQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_categoryId = category.m_key;
        m_songNameFull.clear();

        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        numberButtonClicked(DEFAULT_NORMAL_LEVEL);
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
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::OtherQuery, v);
}
