#include "musicwebdjradioquerywidget.h"
#include "musicdjradioprogramcategoryrequest.h"
#include "musicqueryitemwidget.h"
#include "musicpagequerywidget.h"
#include "musicwebdjradioinfowidget.h"

MusicWebDJRadioQueryWidget::MusicWebDJRadioQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWidget);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWidget);

    m_initialized = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_pageQueryWidget = nullptr;

    m_networkRequest = new MusicDJRadioProgramCategoryRequest(this);
    connect(m_networkRequest, SIGNAL(createProgramItem(MusicResultDataItem)), SLOT(createProgramItem(MusicResultDataItem)));
}

MusicWebDJRadioQueryWidget::~MusicWebDJRadioQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_pageQueryWidget;
}

void MusicWebDJRadioQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearchByID(value);
}

void MusicWebDJRadioQueryWidget::setCurrentKey(const QString &id)
{
    MusicAbstractItemQueryWidget::setCurrentValue(id);
    MusicResultDataItem item;
    item.m_id = id;
    currentItemClicked(item);
}

void MusicWebDJRadioQueryWidget::resizeGeometry()
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

void MusicWebDJRadioQueryWidget::createProgramItem(const MusicResultDataItem &item)
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

        QPushButton *backButton = new QPushButton(tr("Back"), containTopWidget);
        backButton->setFixedSize(90, 30);
        backButton->setStyleSheet(TTK::UI::PushButtonStyle03);
        backButton->setCursor(QCursor(Qt::PointingHandCursor));
        connect(backButton, SIGNAL(clicked()), this, SIGNAL(backToMainMenu()));
        containTopLayout->addWidget(backButton);
        containTopLayout->addStretch(1);
#ifdef Q_OS_UNIX
        backButton->setFocusPolicy(Qt::NoFocus);
#endif
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

    if(m_pageQueryWidget)
    {
        m_pageQueryWidget->reset(m_networkRequest->pageTotalSize());
    }

    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setShowTime(false);
    label->setShowCount(false);
    label->setResultDataItem(item, new MusicCoverSourceRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);
    m_resizeWidgets.append({label, label->font()});
}

void MusicWebDJRadioQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebDJRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setCurrentValue(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicWebDJRadioQueryWidget::backToMainMenuClicked()
{
    if(!m_initialized)
    {
        Q_EMIT backToMainMenu();
        return;
    }
    m_container->setCurrentIndex(0);
}

void MusicWebDJRadioQueryWidget::buttonClicked(int index)
{
    removeItems(m_gridLayout);
    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}
