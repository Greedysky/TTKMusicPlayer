#include "musicsimilarfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"

MusicSimilarFoundTableWidget::MusicSimilarFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicSimilarFoundTableWidget::~MusicSimilarFoundTableWidget()
{
    clearAllItems();
}

void MusicSimilarFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicSimilarFoundTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    if(rowCount() >= 15)
    {
        return;
    }

    MusicQueryFoundTableWidget::createSearchedItem(songItem);
}



MusicSimilarFoundWidget::MusicSimilarFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_foundTableWidget = new MusicSimilarFoundTableWidget(this);
    m_foundTableWidget->hide();
}

void MusicSimilarFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_foundTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getSimilarSongThread(this));
    m_foundTableWidget->startSearchQuery(MusicUtils::String::songName(name));

    createLabels();
}

void MusicSimilarFoundWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicSimilarFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();
}

void MusicSimilarFoundWidget::queryAllFinished()
{

}

void MusicSimilarFoundWidget::createLabels()
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
    firstLabel->setText(tr("Like \"<font color=#158FE1> %1 </font>\" also like this").arg(m_songNameFull));
    grid->addWidget(firstLabel);
    //
    grid->addWidget(m_container);

    m_mainWindow->layout()->addWidget(function);
}
