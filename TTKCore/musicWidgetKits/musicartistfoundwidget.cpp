#include "musicartistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcethread.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictime.h"

#include "qrcodewidget.h"

#include <qmath.h>
#include <QCheckBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QScrollArea>

MusicArtistFoundTableWidget::MusicArtistFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicArtistFoundTableWidget::~MusicArtistFoundTableWidget()
{
    clearAllItems();
}

QString MusicArtistFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicArtistFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryArtistFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryArtistFinished()));
    }
}



MusicArtistFoundWidget::MusicArtistFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_iconLabel = nullptr;

    m_artistTableWidget = new MusicArtistFoundTableWidget(this);
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicArtistFoundWidget::~MusicArtistFoundWidget()
{
    delete m_artistTableWidget;
    delete m_downloadThread;
    delete m_iconLabel;
}

QString MusicArtistFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicArtistFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->setQuerySimplify(true);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery,
                                      MusicUtils::String::artistName(name));
}

void MusicArtistFoundWidget::resizeWindow()
{
    m_artistTableWidget->resizeWindow();
}

void MusicArtistFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noArtist"));
    }
    else
    {
        bool hasItem = false;
        foreach(const MusicObject::MusicSongInfomation &info, musicSongInfos)
        {
            if(m_songNameFull.contains(info.m_songName))
            {
                hasItem = true;
                m_artistTableWidget->hide();
                m_artistTableWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getArtistThread(this));
                m_artistTableWidget->startSearchQuery(info.m_artistId);
                break;
            }
        }

        if(!hasItem)
        {
            m_statusLabel->setPixmap(QPixmap(":/image/lb_noArtist"));
        }
    }
}

void MusicArtistFoundWidget::queryArtistFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_artistTableWidget->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noArtist"));
    }
    else
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_artistTableWidget->show();
        MusicObject::MusicSongInfomation currentInfo = musicSongInfos.first();

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
        firstLabel->setText(tr("<font color=#169AF3> Artist > %1 </font>").arg(currentInfo.m_singerName));
        grid->addWidget(firstLabel);
        ////////////////////////////////////////////////////////////////////////////
        QWidget *topFuncWidget = new QWidget(function);
        QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

        m_iconLabel = new QLabel(topFuncWidget);
        m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
        m_iconLabel->setFixedSize(180, 180);

        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        foreach(const MusicObject::MusicSongInfomation &info, musicSongInfos)
        {
            if(!info.m_smallPicUrl.isEmpty() && info.m_smallPicUrl != "null")
            {
                download->startToDownload(info.m_smallPicUrl);
                break;
            }
        }
        ////////////////////////////////////////////////////////////////////////////

        QWidget *topRightWidget = new QWidget(topFuncWidget);
        QGridLayout *topRightLayout = new QGridLayout(topRightWidget);
        topRightLayout->setContentsMargins(0, 0, 0, 0);
        topRightLayout->setSpacing(0);

        MusicTime::timeSRand();
        QLabel *numberLabel = new QLabel(topRightWidget);
        numberLabel->setAlignment(Qt::AlignCenter);
        numberLabel->setStyleSheet(MusicUIObject::MFontStyle06 + MusicUIObject::MColorStyle05);
        int number = qrand()%10;
        numberLabel->setText(QString("%1.%2").arg(number).arg(qrand()%10));
        topRightLayout->addWidget(numberLabel, 0, 0);
        for(int i=1; i<=5; ++i)
        {
            QLabel *label = new QLabel(topRightWidget);
            label->setPixmap(QPixmap( (ceil(number/2.0) - i) >= 0 ? ":/tiny/lb_star" : ":/tiny/lb_unstar"));
            topRightLayout->addWidget(label, 0, i);
        }

        QLabel *numberTextLabel = new QLabel(tr("Score:"), topRightWidget);
        numberTextLabel->setAlignment(Qt::AlignCenter);
        topRightLayout->addWidget(numberTextLabel, 1, 0);
        for(int i=1; i<=5; ++i)
        {
            QLabel *label = new QLabel(topRightWidget);
            label->setFixedSize(26, 22);
            label->setPixmap(QPixmap(":/tiny/lb_unstar"));
            topRightLayout->addWidget(label, 1, i);
        }

        QLabel *marginBottmLabel = new QLabel(topRightWidget);
        marginBottmLabel->setFixedHeight(40);
        topRightLayout->addWidget(marginBottmLabel, 2, 0);
        topRightWidget->setLayout(topRightLayout);

        QRCodeQWidget *code = new QRCodeQWidget(QByteArray(), QSize(90, 90), topRightWidget);
        code->setMargin(2);
        code->setIcon(":/image/lb_player_logo", 0.23);
        topRightLayout->addWidget(code, 3, 2, 1, 6);

        topFuncLayout->addWidget(m_iconLabel);
        topFuncLayout->addStretch(1);
        topFuncLayout->addWidget(topRightWidget);
        topFuncWidget->setLayout(topFuncLayout);
        grid->addWidget(topFuncWidget);
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

#ifdef Q_OS_UNIX
        allCheckBox->setFocusPolicy(Qt::NoFocus);
        playButton->setFocusPolicy(Qt::NoFocus);
        addButton->setFocusPolicy(Qt::NoFocus);
        downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

        middleFuncLayout->addWidget(marginLabel);
        middleFuncLayout->addWidget(allCheckBox);
        middleFuncLayout->addStretch(1);
        middleFuncLayout->addWidget(playButton);
        middleFuncLayout->addWidget(addButton);
        middleFuncLayout->addWidget(downloadButton);
        connect(allCheckBox, SIGNAL(clicked(bool)), m_artistTableWidget, SLOT(setSelectedAllItems(bool)));
        connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
        connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
        connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
        grid->addWidget(middleFuncWidget);
        //////////////////////////////////////////////////////////////////////
        grid->addWidget(m_artistTableWidget);
        grid->addStretch(1);

        function->setLayout(grid);
        m_mainWindow->layout()->addWidget(function);
    }
}

void MusicArtistFoundWidget::downLoadFinished(const QByteArray &data)
{
    if(m_iconLabel)
    {
        QPixmap pix;
        pix.loadFromData(data);
        m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
    }
}

void MusicArtistFoundWidget::playButtonClicked()
{
    m_artistTableWidget->downloadDataFrom(true);
}

void MusicArtistFoundWidget::downloadButtonClicked()
{
    foreach(int index, m_artistTableWidget->getSelectedItems())
    {
        m_artistTableWidget->musicDownloadLocal(index);
    }
}

void MusicArtistFoundWidget::addButtonClicked()
{
    m_artistTableWidget->downloadDataFrom(false);
}
