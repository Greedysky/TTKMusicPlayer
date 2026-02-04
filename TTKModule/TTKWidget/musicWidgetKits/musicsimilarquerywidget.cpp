#include "musicsimilarquerywidget.h"
#include "musicdownloadqueryfactory.h"

MusicSimilarQueryTableWidget::MusicSimilarQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicSimilarQueryTableWidget::~MusicSimilarQueryTableWidget()
{
    removeItems();
}

void MusicSimilarQueryTableWidget::createResultItem(const MusicResultInfoItem &result)
{
    if(rowCount() >= (TTK_PAGE_SIZE_30 / 2))
    {
        return;
    }

    MusicItemQueryTableWidget::createResultItem(result);
}



MusicSimilarQueryWidget::MusicSimilarQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_tableWidget = new MusicSimilarQueryTableWidget(this);
    m_tableWidget->hide();
}

void MusicSimilarQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_tableWidget->setQueryInput(G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this));
    m_tableWidget->startToSearchByValue(TTK::generateSongTitle(value));
    createLabels();
}

void MusicSimilarQueryWidget::resizeGeometry()
{
    m_tableWidget->resizeGeometry();
}

void MusicSimilarQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    createFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWidget);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWidget);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("Like \"<font color=#158FE1> %1 </font>\" also like this").arg(m_value));
    grid->addWidget(firstLabel);
    //
    grid->addWidget(m_container);

    m_mainWidget->layout()->addWidget(function);
}
