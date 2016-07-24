#include "musicsimilarfoundwidget.h"
#include "musicsourcedownloadthread.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdatadownloadthread.h"
#include "musiccryptographichash.h"
#include "musicnetworkthread.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musicutils.h"
#include "musicsongssummarizied.h"

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
    m_mainWindow->setStyleSheet("background:#FFFFFF");
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet("font-size:20px; font-weight:bold");
    QHBoxLayout *mLayout = new QHBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);

#ifndef USE_MULTIPLE_QUERY
    m_downloadThread = new MusicDownLoadQuerySingleThread(this);
#else
    m_downloadThread = new MusicDownLoadQueryMultipleThread(this);
#endif
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
    while(!m_likeDownloadDatas.isEmpty())
    {
        delete m_likeDownloadDatas.takeLast();
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

        foreach(MusicObject::MusicSongInfomation info, musicSongInfos)
        {
            DownloadData *data = new DownloadData;
            data->m_songName = info.m_songName;
            data->m_songArtist = info.m_singerName;
            data->m_time = info.m_timeLength;
            data->m_picUrl = info.m_smallPicUrl;
            if(!info.m_songAttrs.isEmpty())
            {
                MusicObject::MusicSongAttribute atrr = info.m_songAttrs.first();
                data->m_songUrl = atrr.m_url;
                data->m_format = atrr.m_format;
            }
            m_likeDownloadDatas << data;
        }

        createLabels();
    }
}

void MusicSimilarFoundWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
}

void MusicSimilarFoundWidget::createLabels()
{
    int index = 0;

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle05);
    QGridLayout *grid = new QGridLayout(function);
    grid->setSpacing(15);

    QLabel *firstLabel = new QLabel(m_mainWindow);
    firstLabel->setText(tr("Like \"<font color=blue> %1 </font>\" also like this").arg(m_songNameFull));
    grid->addWidget(firstLabel, index++, 0, 1, 7);
    ////////////////////////////////////////////////////////////////////////////
    QCheckBox *allCheckBox = new QCheckBox(tr("all"), m_mainWindow);
    QPushButton *playButton = new QPushButton(tr("play"), m_mainWindow);
    playButton->setFixedSize(55, 25);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *addButton = new QPushButton(tr("add"), m_mainWindow);
    addButton->setFixedSize(55, 25);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *downloadButton = new QPushButton(tr("download"), m_mainWindow);
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
            QCheckBox *box = new QCheckBox(m_mainWindow);
            m_checkBoxs << box;
            grid->addWidget(box, index, j*4);
            QLabel *songLabel = new QLabel(m_mainWindow);
            songLabel->setText(MusicUtils::UWidget::elidedText(font(), m_likeDownloadDatas[dIndex]->m_songName, Qt::ElideRight, 130));
            songLabel->setToolTip(m_likeDownloadDatas[dIndex]->m_songName);
            grid->addWidget(songLabel, index, j*4 + 1, 1, 2, Qt::AlignLeft);
            grid->addWidget(new QLabel(m_likeDownloadDatas[dIndex]->m_time, m_mainWindow), index, j*4 + 3);
        }
        index++;
    }
    ////////////////////////////////////////////////////////////////////////////
    QString artName = m_downloadThread->getSearchedText();
    QLabel *secondLabel = new QLabel(m_mainWindow);
    secondLabel->setText(tr("Other \"<font color=blue> %1 </font>\" things").arg(artName));
    grid->addWidget(secondLabel, index++, 0, 1, 7);
    ////////////////////////////////////////////////////////////////////////////
    QLabel *picLabel1 = new QLabel(m_mainWindow);
    picLabel1->setPixmap(QPixmap(":/image/lb_warning"));
    picLabel1->setFixedSize(100, 100);
    QLabel *picLabel2 = new QLabel(m_mainWindow);
    picLabel2->setPixmap(QPixmap(":/image/lb_warning"));
    picLabel2->setFixedSize(100, 100);
    QLabel *picLabel3 = new QLabel(m_mainWindow);
    picLabel3->setPixmap(QPixmap(":/image/lb_warning"));
    picLabel3->setFixedSize(100, 100);
    m_iconLabels << picLabel1 << picLabel2 << picLabel3;
    ////////////////////////////////////////////////////////////////////////////
    grid->addWidget(picLabel1, index, 0, 1, 2, Qt::AlignCenter);
    grid->addWidget(picLabel2, index, 3, 1, 2, Qt::AlignCenter);
    grid->addWidget(picLabel3, index++, 6, 1, 2, Qt::AlignCenter);
    QString artLimitString = MusicUtils::UWidget::elidedText(font(), artName, Qt::ElideRight, 90);
    grid->addWidget(new QLabel(artLimitString, m_mainWindow), index, 0, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel(artLimitString, m_mainWindow), index, 3, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel(artLimitString, m_mainWindow), index++, 6, 1, 2, Qt::AlignCenter);

    int downloadCounter = 0;
    foreach(DownloadData *data, m_likeDownloadDatas)
    {
        if(data->m_songArtist.contains(artName) && downloadCounter < 3)
        {
            downloadCounter++;
            MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
            connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            download->startToDownload(data->m_picUrl);
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
            m_iconLabels[i]->setPixmap(pix.scaled(100, 100));
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
        MusicDownloadMgmtWidget mgmt(this);
        DownloadData *data = m_likeDownloadDatas[index];
        mgmt.setSongName(data->m_songArtist + " - " + data->m_songName, MusicDownLoadQueryThreadAbstract::MusicQuery);
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
        DownloadData *data = m_likeDownloadDatas[ list[i] ];
        QString musicEnSong = MusicCryptographicHash().encrypt(data->m_songArtist + " - " + data->m_songName, DOWNLOAD_KEY);
        QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(data->m_format);

        QEventLoop loop(this);
        MusicDataDownloadThread *downSong = new MusicDataDownloadThread( data->m_songUrl, downloadName,
                                                                         MusicDownLoadThreadAbstract::Download_Music, this);
        connect(downSong, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        downSong->startToDownload();
        loop.exec();

        emit muiscSongToPlayListChanged(musicEnSong, data->m_time, data->m_format, play && (i == list.count() - 1));
    }
}
