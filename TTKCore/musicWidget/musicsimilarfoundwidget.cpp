#include "musicsimilarfoundwidget.h"
#include "musicsourcedownloadthread.h"
#include "musicdownloadwidget.h"
#include "musicdatadownloadthread.h"
#include "musiccryptographichash.h"
#include "musicsongssummarizied.h"
#include "musicdownloadqueryfactory.h"
#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musicutils.h"

#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QEventLoop>

MusicSimilarFoundWidget::MusicSimilarFoundWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    m_mainWindow = new QWidget(this);
    m_mainWindow->setStyleSheet(MusicUIObject::MBackgroundStyle17);
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MCustomStyle04 + MusicUIObject::MCustomStyle01);
    QHBoxLayout *mLayout = new QHBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);

    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummarizied::getClassName());
}

MusicSimilarFoundWidget::~MusicSimilarFoundWidget()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
    while(!m_checkBoxs.isEmpty())
    {
        delete m_checkBoxs.takeLast();
    }
    while(!m_iconLabels.isEmpty())
    {
        delete m_iconLabels.takeLast();
    }
    delete m_downloadThread;
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
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name.split("-").front().trimmed());
}

void MusicSimilarFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noSimilar"));
    }
    else
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_likeDownloadDatas = musicSongInfos;
        createLabels();
    }
}

void MusicSimilarFoundWidget::contextMenuEvent(QContextMenuEvent *event)
{
   Q_UNUSED(event);
}

void MusicSimilarFoundWidget::createLabels()
{
    int index = 0;

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle03);
    QGridLayout *grid = new QGridLayout(function);
    grid->setSpacing(15);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("Like \"<font color=#169AF3> %1 </font>\" also like this").arg(m_songNameFull));
    grid->addWidget(firstLabel, index++, 0, 1, 7);
    ////////////////////////////////////////////////////////////////////////////
    QCheckBox *allCheckBox = new QCheckBox("  " + tr("all"), function);
    QPushButton *playButton = new QPushButton(tr("play"), function);
    playButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
    playButton->setIconSize(QSize(14, 14));
    playButton->setFixedSize(55, 25);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *addButton = new QPushButton(tr("add"), function);
    addButton->setFixedSize(55, 25);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("download"), function);
    downloadButton->setFixedSize(55, 25);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    grid->addWidget(allCheckBox, index, 0);
    grid->addWidget(playButton, index, 5);
    grid->addWidget(addButton, index, 6);
    grid->addWidget(downloadButton, index++, 7);
    connect(allCheckBox, SIGNAL(clicked(bool)), SLOT(selectAllItems(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    ////////////////////////////////////////////////////////////////////////////
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<2; j++)
        {
            int dIndex = 2*i + j;
            if( dIndex >= m_likeDownloadDatas.count())
            {
                break;
            }
            QCheckBox *box = new QCheckBox(function);
            m_checkBoxs << box;
            grid->addWidget(box, index, j*4);
            box->setText("  " + MusicUtils::UWidget::elidedText(font(), m_likeDownloadDatas[dIndex].m_songName, Qt::ElideRight, 200));
            box->setToolTip(m_likeDownloadDatas[dIndex].m_songName);
            grid->addWidget(new QLabel(m_likeDownloadDatas[dIndex].m_timeLength, function), index, j*4 + 3);
        }
        index++;
    }
    ////////////////////////////////////////////////////////////////////////////
    QString artName = m_downloadThread->getSearchedText();
    QLabel *secondLabel = new QLabel(function);
    secondLabel->setText(tr("Other \"<font color=#169AF3> %1 </font>\" things").arg(artName));
    grid->addWidget(secondLabel, index++, 0, 1, 7);
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
    grid->addWidget(picLabel1, index, 0, 1, 2, Qt::AlignCenter);
    grid->addWidget(picLabel2, index, 3, 1, 2, Qt::AlignCenter);
    grid->addWidget(picLabel3, index++, 6, 1, 2, Qt::AlignCenter);
    QString artLimitString = MusicUtils::UWidget::elidedText(font(), artName, Qt::ElideRight, 90);
    grid->addWidget(new QLabel(artLimitString, function), index, 0, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel(artLimitString, function), index, 3, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel(artLimitString, function), index++, 6, 1, 2, Qt::AlignCenter);

    int downloadCounter = 0;
    foreach(const MusicObject::MusicSongInfomation &data, m_likeDownloadDatas)
    {
        if(data.m_singerName.contains(artName) && downloadCounter < 3)
        {
            downloadCounter++;
            MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
            connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            download->startToDownload(data.m_smallPicUrl);
        }
    }

    m_mainWindow->layout()->addWidget(function);
}

MusicObject::MIntList MusicSimilarFoundWidget::foundCheckedItem()
{
    MusicObject::MIntList list;
    for(int i=0; i<m_checkBoxs.count(); ++i)
    {
        if(m_checkBoxs[i]->isChecked())
        {
            list << i;
        }
    }
    return list;
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

void MusicSimilarFoundWidget::selectAllItems(bool all)
{
    foreach(QCheckBox *v, m_checkBoxs)
    {
        v->setChecked(all);
    }
}

void MusicSimilarFoundWidget::playButtonClicked()
{
    downloadDataFrom(true);
}

void MusicSimilarFoundWidget::downloadButtonClicked()
{
    foreach(int index, foundCheckedItem())
    {
        MusicObject::MusicSongInfomation downloadInfo = m_likeDownloadDatas[ index ];
        MusicDownloadWidget *download = new MusicDownloadWidget(this);
        download->setSongName(downloadInfo, MusicDownLoadQueryThreadAbstract::MusicQuery);
        download->show();
    }
}

void MusicSimilarFoundWidget::addButtonClicked()
{
    downloadDataFrom(false);
}

void MusicSimilarFoundWidget::downloadDataFrom(bool play)
{
    MusicObject::MIntList list = foundCheckedItem();
    for(int i=0; i<list.count(); ++i)
    {
        if(!M_NETWORK_PTR->isOnline())
        {
            continue;
        }

        MusicObject::MusicSongInfomation downloadInfo = m_likeDownloadDatas[ list[i] ];
        MusicObject::MusicSongAttributes attrs = downloadInfo.m_songAttrs;
        MusicObject::MusicSongAttribute attr;
        if(!attrs.isEmpty())
        {
            attr = attrs.first();
        }
        QString musicEnSong = MusicCryptographicHash().encrypt(downloadInfo.m_singerName + " - " + downloadInfo.m_songName, DOWNLOAD_KEY);
        QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(attr.m_format);

        QEventLoop loop(this);
        MusicDataDownloadThread *downSong = new MusicDataDownloadThread( attr.m_url, downloadName,
                                                                         MusicDownLoadThreadAbstract::Download_Music, this);
        connect(downSong, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        downSong->startToDownload();
        loop.exec();

        emit muiscSongToPlayListChanged(musicEnSong, downloadInfo.m_timeLength, attr.m_format, play && (i == list.count() - 1));
    }
}
