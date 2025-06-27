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
    if(rowCount() >= (SONG_PAGE_SIZE / 2))
    {
        return;
    }

    MusicItemQueryTableWidget::createResultItem(result);
}



MusicSimilarQueryWidget::MusicSimilarQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_queryTableWidget = new MusicSimilarQueryTableWidget(this);
    m_queryTableWidget->hide();
}

void MusicSimilarQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_queryTableWidget->setQueryInput(G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this));
    m_queryTableWidget->startToSearchByText(TTK::generateSongTitle(value));
    createLabels();
}

void MusicSimilarQueryWidget::resizeWidget()
{
    m_queryTableWidget->resizeSection();
}

void MusicSimilarQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("Like \"<font color=#158FE1> %1 </font>\" also like this").arg(m_value));
    grid->addWidget(firstLabel);
    //
    grid->addWidget(m_container);

    m_mainWindow->layout()->addWidget(function);
}
