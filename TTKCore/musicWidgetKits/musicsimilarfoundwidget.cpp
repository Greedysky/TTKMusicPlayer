#include "musicsimilarfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicdownloadsourcethread.h"

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>

MusicSimilarFoundTableWidget::MusicSimilarFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicSimilarFoundTableWidget::~MusicSimilarFoundTableWidget()
{
    clearAllItems();
}

QString MusicSimilarFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSimilarFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicSimilarFoundTableWidget::createSearchedItems(const MusicSearchedItem &songItem)
{
    if(rowCount() >= 15)
    {
        return;
    }
    MusicQueryFoundTableWidget::createSearchedItems(songItem);
}



MusicSimilarFoundWidget::MusicSimilarFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_similarTableWidget = new MusicSimilarFoundTableWidget(this);
}

MusicSimilarFoundWidget::~MusicSimilarFoundWidget()
{
    delete m_similarTableWidget;
}

QString MusicSimilarFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSimilarFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_similarTableWidget->hide();
    m_similarTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getQueryThread(this));
    m_similarTableWidget->startSearchQuery(MusicUtils::String::songName(name));
}

void MusicSimilarFoundWidget::resizeWindow()
{
    m_similarTableWidget->resizeWindow();
}

void MusicSimilarFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_similarTableWidget->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noSimilar"));
    }
    else
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        createLabels();
    }
}

void MusicSimilarFoundWidget::createLabels()
{
    layout()->removeWidget(m_mainWindow);
    m_similarTableWidget->show();

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    MusicObject::MusicSongInfomations musicSongInfos(m_similarTableWidget->getMusicSongInfos());
    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle03);
    QGridLayout *grid = new QGridLayout(function);
    grid->setSpacing(15);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("Like \"<font color=#169AF3> %1 </font>\" also like this").arg(m_songNameFull));
    grid->addWidget(firstLabel, 0, 0, 1, 7);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *middleFuncWidget = new QWidget(function);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(" " + tr("all"), middleFuncWidget);
    QPushButton *playButton = new QPushButton(tr("play"), middleFuncWidget);
    playButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
    playButton->setIconSize(QSize(14, 14));
    playButton->setFixedSize(55, 25);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *addButton = new QPushButton(tr("add"), middleFuncWidget);
    addButton->setFixedSize(55, 25);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *downloadButton = new QPushButton(tr("download"), middleFuncWidget);
    downloadButton->setFixedSize(55, 25);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    middleFuncLayout->addWidget(marginLabel);
    middleFuncLayout->addWidget(allCheckBox);
    middleFuncLayout->addStretch(1);
    middleFuncLayout->addWidget(playButton);
    middleFuncLayout->addWidget(addButton);
    middleFuncLayout->addWidget(downloadButton);
    connect(allCheckBox, SIGNAL(clicked(bool)), m_similarTableWidget, SLOT(setSelectedAllItems(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    grid->addWidget(middleFuncWidget, 1, 0, 1, 8);
    ////////////////////////////////////////////////////////////////////////////
    grid->addWidget(m_similarTableWidget, 2, 0, 1, 8);
    ////////////////////////////////////////////////////////////////////////////
    QString artName = MusicUtils::String::artistName(m_songNameFull);
    QLabel *secondLabel = new QLabel(function);
    secondLabel->setText(tr("Other \"<font color=#169AF3> %1 </font>\" things").arg(artName));
    grid->addWidget(secondLabel, 3, 0, 1, 7);
    ////////////////////////////////////////////////////////////////////////////
    QLabel *picLabel1 = new QLabel(function);
    picLabel1->setPixmap(QPixmap(":/image/lb_warning"));
    picLabel1->setFixedSize(100, 100);
    QLabel *picLabel2 = new QLabel(function);
    picLabel2->setPixmap(QPixmap(":/image/lb_warning"));
    picLabel2->setFixedSize(100, 100);
    QLabel *picLabel3 = new QLabel(function);
    picLabel3->setPixmap(QPixmap(":/image/lb_warning"));
    picLabel3->setFixedSize(100, 100);
    m_resizeWidget << picLabel1 << picLabel2 << picLabel3;
    ////////////////////////////////////////////////////////////////////////////
    grid->addWidget(picLabel1, 4, 0, 1, 2, Qt::AlignCenter);
    grid->addWidget(picLabel2, 4, 3, 1, 2, Qt::AlignCenter);
    grid->addWidget(picLabel3, 4, 6, 1, 2, Qt::AlignCenter);
    QString artLimitString = MusicUtils::Widget::elidedText(font(), artName, Qt::ElideRight, 90);
    grid->addWidget(new QLabel(artLimitString, function), 5, 0, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel(artLimitString, function), 5, 3, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel(artLimitString, function), 5, 6, 1, 2, Qt::AlignCenter);

    int downloadCounter = 0;
    foreach(const MusicObject::MusicSongInfomation &data, musicSongInfos)
    {
        if(data.m_singerName.contains(artName) && downloadCounter < 3)
        {
            downloadCounter++;
            downloadUrlChanged(data.m_smallPicUrl);
        }
    }

    m_mainWindow->layout()->addWidget(function);
}

void MusicSimilarFoundWidget::downLoadFinished(const QByteArray &data)
{
    for(int i=0; i<m_resizeWidget.count(); ++i)
    {
        QLabel *l = m_resizeWidget[i];
        if(l->pixmap()->cacheKey() == QPixmap(":/image/lb_warning").cacheKey())
        {
            QPixmap pix;
            pix.loadFromData(data);
            l->setPixmap(pix.scaled(l->size()));
            return;
        }
    }
}

void MusicSimilarFoundWidget::downloadUrlChanged(const QString &imageUrl)
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(imageUrl);
}

void MusicSimilarFoundWidget::playButtonClicked()
{
    m_similarTableWidget->downloadDataFrom(true);
}

void MusicSimilarFoundWidget::downloadButtonClicked()
{
    foreach(int index, m_similarTableWidget->getSelectedItems())
    {
        m_similarTableWidget->musicDownloadLocal(index);
    }
}

void MusicSimilarFoundWidget::addButtonClicked()
{
    m_similarTableWidget->downloadDataFrom(false);
}
