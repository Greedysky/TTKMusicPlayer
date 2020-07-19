#include "musicrecommendquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"

MusicRecommendQueryTableWidget::MusicRecommendQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicRecommendQueryTableWidget::~MusicRecommendQueryTableWidget()
{
    clearAllItems();
}

void MusicRecommendQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicRecommendQueryTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    if(rowCount() >= 50)
    {
        return;
    }

    MusicItemQueryTableWidget::createSearchedItem(songItem);
}



MusicRecommendQueryWidget::MusicRecommendQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_queryTableWidget = new MusicRecommendQueryTableWidget(this);
    m_queryTableWidget->hide();
}

void MusicRecommendQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_queryTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getRecommendRequest(this));
    m_queryTableWidget->startSearchQuery(MusicUtils::String::songName(name));

    createLabels();
}

void MusicRecommendQueryWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicRecommendQueryWidget::resizeWindow()
{
    m_queryTableWidget->resizeWindow();
}

void MusicRecommendQueryWidget::queryAllFinished()
{

}

void MusicRecommendQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01 + MusicUIObject::MQSSPushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> Recommend Music</font>"));
    grid->addWidget(firstLabel);
    QLabel *iconLabel = new QLabel(function);
    iconLabel->setPixmap(QPixmap(":/image/lb_recmd_daily"));
    grid->addWidget(iconLabel);
    //
    grid->addWidget(m_container);

    m_mainWindow->layout()->addWidget(function);
}
