#include "musicsimilarfoundwidget.h"
#include "musicuiobject.h"
#ifndef USE_MULTIPLE_QUERY
#  include "musicdownloadquerysinglethread.h"
#else
#  include "musicdownloadquerymultiplethread.h"
#endif

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
        int index = 0;
        QString songName = m_downloadThread->getSearchedText();

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
            for(int j=0; j<5; j+=4)
            {
                QCheckBox *box = new QCheckBox(m_mainWindow);
                m_checkBoxs << box;
                grid->addWidget(box, index, j);
                grid->addWidget(new QLabel("TTTT5555555555TTTTT", m_mainWindow), index, j + 1, 1, 2, Qt::AlignLeft);
                grid->addWidget(new QLabel("05:01", m_mainWindow), index, j + 3);
            }
            index++;
        }

        QLabel *secondLabel = new QLabel(m_mainWindow);
        secondLabel->setText(QString("Other '<font color=blue> %1 </font>' things").arg(songName));
        grid->addWidget(secondLabel, index++, 0, 1, 5);

        if(songName.contains("-"))
        {
            for(int i=0; i<4; ++i)
            {
                for(int j=0; j<5; j+=4)
                {
                    grid->addWidget(new QCheckBox(m_mainWindow), index, j);
                    grid->addWidget(new QLabel("TTTT5555555555TTTTT", m_mainWindow), index, j + 1, 1, 2, Qt::AlignLeft);
                    grid->addWidget(new QLabel("05:01", m_mainWindow), index, j + 3);
                }
                index++;
            }
        }

        m_mainWindow->layout()->addWidget(function);
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

}

void MusicSimilarFoundWidget::downloadButtonClicked()
{

}

void MusicSimilarFoundWidget::addButtonClicked()
{

}
