#include "musicrecommendfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"

MusicRecommendFoundTableWidget::MusicRecommendFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicRecommendFoundTableWidget::~MusicRecommendFoundTableWidget()
{
    clearAllItems();
}

void MusicRecommendFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicRecommendFoundTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    if(rowCount() >= 50)
    {
        return;
    }

    MusicQueryFoundTableWidget::createSearchedItem(songItem);
}



MusicRecommendFoundWidget::MusicRecommendFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_foundTableWidget = new MusicRecommendFoundTableWidget(this);
    m_foundTableWidget->hide();
}

void MusicRecommendFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_foundTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getRecommendThread(this));
    m_foundTableWidget->startSearchQuery(MusicUtils::String::songName(name));

    createLabels();
}

void MusicRecommendFoundWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicRecommendFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();
}

void MusicRecommendFoundWidget::queryAllFinished()
{

}

void MusicRecommendFoundWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle03);
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
