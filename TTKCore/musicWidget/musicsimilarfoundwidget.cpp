#include "musicsimilarfoundwidget.h"
#include "musicsongssummarizied.h"
#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"

#include "musicsourcedownloadthread.h"

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>

MusicSimilarFoundTableWidget::MusicSimilarFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 460);
    headerview->resizeSection(2, 60);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummarizied::getClassName());
}

MusicSimilarFoundTableWidget::~MusicSimilarFoundTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
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

void MusicSimilarFoundTableWidget::createSearchedItems(const QString &songname, const QString &artistname,
                                                       const QString &time)
{
    if(rowCount() >= 15)
    {
        return;
    }
    MusicQueryFoundTableWidget::createSearchedItems(songname, artistname, time);
}

void MusicSimilarFoundTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryFoundTableWidget::resizeEvent(event);
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.9 + 460);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.1 + 60);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    }
}



MusicSimilarFoundWidget::MusicSimilarFoundWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    m_mainWindow = new QWidget(this);
    m_mainWindow->setObjectName("MainWindow");
    m_mainWindow->setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MBackgroundStyle17));
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MFontStyle05 + MusicUIObject::MFontStyle01);
    QHBoxLayout *mLayout = new QHBoxLayout(m_mainWindow);
    mLayout->setContentsMargins(2, 2, 2, 2);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);

    m_similarTableWidget = new MusicSimilarFoundTableWidget(this);
}

MusicSimilarFoundWidget::~MusicSimilarFoundWidget()
{
    while(!m_iconLabels.isEmpty())
    {
        delete m_iconLabels.takeLast();
    }
    delete m_similarTableWidget;
    delete m_statusLabel;
    delete m_mainWindow;
}

QString MusicSimilarFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSimilarFoundWidget::setSongName(const QString &name)
{
    m_songNameFull = name;
    m_similarTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getQueryThread(this));
    m_similarTableWidget->startSearchQuery(MusicUtils::Core::songName(name));
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

void MusicSimilarFoundWidget::contextMenuEvent(QContextMenuEvent *event)
{
   Q_UNUSED(event);
}

void MusicSimilarFoundWidget::createLabels()
{
    layout()->removeWidget(m_mainWindow);
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
    QString artName = MusicUtils::Core::artistName(m_songNameFull);
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
    m_iconLabels << picLabel1 << picLabel2 << picLabel3;
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
            urlHasChanged(data.m_smallPicUrl);
        }
    }

    m_mainWindow->layout()->addWidget(function);
}

void MusicSimilarFoundWidget::downLoadFinished(const QByteArray &data)
{
    for(int i=0; i<m_iconLabels.count(); ++i)
    {
        if(m_iconLabels[i]->pixmap()->cacheKey() == QPixmap(":/image/lb_warning").cacheKey())
        {
            QPixmap pix;
            pix.loadFromData(data);
            m_iconLabels[i]->setPixmap(pix.scaled(m_iconLabels[i]->size()));
            return;
        }
    }
}

void MusicSimilarFoundWidget::urlHasChanged(const QString &imageUrl)
{
    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
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
