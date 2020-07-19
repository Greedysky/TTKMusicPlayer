#include "musicsimilarquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"

MusicSimilarQueryTableWidget::MusicSimilarQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicSimilarQueryTableWidget::~MusicSimilarQueryTableWidget()
{
    clearAllItems();
}

void MusicSimilarQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicSimilarQueryTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    if(rowCount() >= 15)
    {
        return;
    }

    MusicItemQueryTableWidget::createSearchedItem(songItem);
}



MusicSimilarQueryWidget::MusicSimilarQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_queryTableWidget = new MusicSimilarQueryTableWidget(this);
    m_queryTableWidget->hide();
}

void MusicSimilarQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_queryTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getSimilarSongRequest(this));
    m_queryTableWidget->startSearchQuery(MusicUtils::String::songName(name));

    createLabels();
}

void MusicSimilarQueryWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicSimilarQueryWidget::resizeWindow()
{
    m_queryTableWidget->resizeWindow();
}

void MusicSimilarQueryWidget::queryAllFinished()
{

}

void MusicSimilarQueryWidget::createLabels()
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
    firstLabel->setText(tr("Like \"<font color=#158FE1> %1 </font>\" also like this").arg(m_songNameFull));
    grid->addWidget(firstLabel);
    //
    grid->addWidget(m_container);

    m_mainWindow->layout()->addWidget(function);
}
