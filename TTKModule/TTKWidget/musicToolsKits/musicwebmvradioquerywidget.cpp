#include "musicwebmvradioquerywidget.h"
#include "musicwebmvradioquerycategorypopwidget.h"
#include "musicwebmvradioinfowidget.h"
#include "musicmvradiocategoryrequest.h"
#include "musicqueryitemwidget.h"

MusicWebMVRadioQueryWidget::MusicWebMVRadioQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWidget);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWidget);

    m_initialized = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;

    m_networkRequest = new MusicMVRadioCategoryRequest(this);
    connect(m_networkRequest, SIGNAL(createMVRadioItem(MusicResultDataItem)), SLOT(createMVRadioCategoryItem(MusicResultDataItem)));
}

MusicWebMVRadioQueryWidget::~MusicWebMVRadioQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_categoryButton;
}

void MusicWebMVRadioQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearch({});
}

void MusicWebMVRadioQueryWidget::resizeGeometry()
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

    const int lineSize = MusicRectQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}

void MusicWebMVRadioQueryWidget::createMVRadioCategoryItem(const MusicResultDataItem &item)
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
        containTopLayout->setContentsMargins(10, 10, 0, 0);

        m_categoryButton = new MusicWebMVRadioQueryCategoryPopWidget(m_mainWidget);
        m_categoryButton->setCategory(MUSIC_MOVIE_RADIO, this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWidget);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);

        QWidget *containWidget = new QWidget(m_mainWidget);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainLayout->addWidget(containTopWidget);
        mainLayout->addWidget(line);
        mainLayout->addWidget(containWidget);
        mainLayout->addStretch(1);
    }

    MusicRectQueryItemWidget *label = new MusicRectQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    const int lineSize = MusicRectQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);
    m_resizeWidgets.append({label, label->font()});
}

void MusicWebMVRadioQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebMVRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setCurrentValue(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicWebMVRadioQueryWidget::backToMainMenuClicked()
{
    m_container->setCurrentIndex(0);
}

void MusicWebMVRadioQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_value.clear();
        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        removeItems(m_gridLayout);
        m_networkRequest->startToSearch(category.m_key);
    }
}
