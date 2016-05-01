#include "musicsimilarfoundwidget.h"
#include "musicuiobject.h"

#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>

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
    connect(m_downloadThread, SIGNAL(resolvedSuccess()), SLOT(queryAllFinished()));
}

MusicSimilarFoundWidget::~MusicSimilarFoundWidget()
{
    while(!m_likeDownloadDatas.isEmpty())
    {
        delete m_likeDownloadDatas.takeLast();
    }
    delete m_downloadThread;
    delete m_statusLabel;
    delete m_mainWindow;
}

void MusicSimilarFoundWidget::setSongName(const QString &name)
{
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
}

void MusicSimilarFoundWidget::queryAllFinished()
{
    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/share/noSimilar"));
    }
    else
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        foreach(MusicSongInfomation info, musicSongInfos)
        {
            DownloadData *data = new DownloadData;
            data->m_songName = info.m_songName;
            data->m_time = info.m_timeLength;
            if(!info.m_songAttrs.isEmpty())
            {
                MusicSongAttribute atrr = info.m_songAttrs.first();
                data->m_songUrl = atrr.m_url;
                data->m_format = atrr.m_format;
            }
            m_likeDownloadDatas <<data;
        }

        createLabels();
    }
}

void MusicSimilarFoundWidget::createLabels()
{
    QString songName = m_downloadThread->getSearchedText();
    int index = 0;

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle05);
    QGridLayout *grid = new QGridLayout(function);
    grid->setSpacing(15);

    QLabel *firstLabel = new QLabel(m_mainWindow);
    firstLabel->setText(QString("Like '<font color=blue> %1 </font>' also like this").arg(songName));
    grid->addWidget(firstLabel, index++, 0, 1, 5);
    ////////////////////////////////////////////////////////////////////////////
    QCheckBox *allCheckBox = new QCheckBox("all", m_mainWindow);
    QPushButton *playButton = new QPushButton("play", m_mainWindow);
    playButton->setFixedSize(55, 25);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *addButton = new QPushButton("add", m_mainWindow);
    addButton->setFixedSize(55, 25);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *downloadButton = new QPushButton("download", m_mainWindow);
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
            songLabel->setText(QFontMetrics(font()).elidedText(m_likeDownloadDatas[dIndex]->m_songName, Qt::ElideRight, 130));
            songLabel->setToolTip(m_likeDownloadDatas[dIndex]->m_songName);
            grid->addWidget(songLabel, index, j*4 + 1, 1, 2, Qt::AlignLeft);
            grid->addWidget(new QLabel(m_likeDownloadDatas[dIndex]->m_time, m_mainWindow), index, j*4 + 3);
        }
        index++;
    }
    ////////////////////////////////////////////////////////////////////////////
    QLabel *secondLabel = new QLabel(m_mainWindow);
    secondLabel->setText(QString("Other '<font color=blue> %1 </font>' things").arg(songName));
    grid->addWidget(secondLabel, index++, 0, 1, 5);
    ////////////////////////////////////////////////////////////////////////////
    QLabel *picLabel1 = new QLabel(m_mainWindow);
    picLabel1->setStyleSheet("background:red");
    picLabel1->setFixedSize(100, 100);
    QLabel *picLabel2 = new QLabel(m_mainWindow);
    picLabel2->setStyleSheet("background:blue");
    picLabel2->setFixedSize(100, 100);
    QLabel *picLabel3 = new QLabel(m_mainWindow);
    picLabel3->setStyleSheet("background:green");
    picLabel3->setFixedSize(100, 100);
    ////////////////////////////////////////////////////////////////////////////
    grid->addWidget(picLabel1, index, 0, 1, 2);
    grid->addWidget(picLabel2, index, 3, 1, 2);
    grid->addWidget(picLabel3, index++, 6, 1, 2);
    grid->addWidget(new QLabel(songName, m_mainWindow), index, 0, 1, 2);
    grid->addWidget(new QLabel(songName, m_mainWindow), index, 3, 1, 2);
    grid->addWidget(new QLabel(songName, m_mainWindow), index++, 6, 1, 2);

    m_mainWindow->layout()->addWidget(function);
}

QList<int> MusicSimilarFoundWidget::foundCheckedItem()
{
    QList<int> list;
    for(int i=0; i<m_checkBoxs.count(); ++i)
    {
        if(m_checkBoxs[i]->isChecked())
        {
            list << i;
        }
    }
    return list;
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
    QList<int> list = foundCheckedItem();
    qDebug() << list.count();
}

void MusicSimilarFoundWidget::downloadButtonClicked()
{
    QList<int> list = foundCheckedItem();
    qDebug() << list.count();
}

void MusicSimilarFoundWidget::addButtonClicked()
{
    QList<int> list = foundCheckedItem();
    qDebug() << list.count();
}
