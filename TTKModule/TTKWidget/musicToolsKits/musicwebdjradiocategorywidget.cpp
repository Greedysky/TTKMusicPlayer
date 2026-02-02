#include "musicwebdjradiocategorywidget.h"
#include "musicdjradiocategoryrequest.h"
#include "musicqueryitemwidget.h"

MusicWebDJRadioCategoryWidget::MusicWebDJRadioCategoryWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget *mainWindow = new QWidget(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, mainWindow);
    layout->addWidget(scrollArea);

    m_gridLayout = new QGridLayout(mainWindow);
    m_gridLayout->setVerticalSpacing(35);
    mainWindow->setLayout(m_gridLayout);

    m_networkRequest = new MusicDJRadioCategoryRequest(this);
    connect(m_networkRequest, SIGNAL(downloadDataChanged(QString)), SLOT(createCategoryItems()));
}

MusicWebDJRadioCategoryWidget::~MusicWebDJRadioCategoryWidget()
{
    delete m_gridLayout;
    delete m_networkRequest;
}

void MusicWebDJRadioCategoryWidget::initialize()
{
    m_networkRequest->startToRequest();
}

void MusicWebDJRadioCategoryWidget::resizeGeometry()
{
    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    for(QWidget *widget : qAsConst(m_resizeWidgets))
    {
        m_gridLayout->removeWidget(widget);
    }

    const int lineSize = MusicLabelQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i], i / lineNumber, i % lineNumber, Qt::AlignCenter);
    }
}

void MusicWebDJRadioCategoryWidget::createCategoryItems()
{
    for(const MusicResultDataItem &item : m_networkRequest->items())
    {
        MusicLabelQueryItemWidget *label = new MusicLabelQueryItemWidget(this);
        connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SIGNAL(currentItemClicked(MusicResultDataItem)));
        label->setResultDataItem(item);

        const int lineSize = MusicLabelQueryItemWidget::LINE_SPACING_SIZE;
        const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
        m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);
        m_resizeWidgets << label;
    }
}
