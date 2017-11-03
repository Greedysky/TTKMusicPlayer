#include "musicartistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcethread.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictime.h"

#include "qrencode/qrcodewidget.h"

#include <qmath.h>

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
    m_shareType = MusicSongSharingWidget::Artist;
    m_foundTableWidget = new MusicArtistFoundTableWidget(this);
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
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

void MusicArtistFoundWidget::setSongNameById(const QString &id)
{
    MusicDownLoadQueryThreadAbstract *v = M_DOWNLOAD_QUERY_PTR->getArtistThread(this);
    m_foundTableWidget->setQueryInput(v);
    m_foundTableWidget->startSearchQuery(id);
    connect(v, SIGNAL(createArtistInfoItem(MusicPlaylistItem)), SLOT(createArtistInfoItem(MusicPlaylistItem)));
}

void MusicArtistFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();
}

void MusicArtistFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInformations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noArtist"));
    }
    else
    {
        bool hasItem = false;
        foreach(const MusicObject::MusicSongInformation &info, musicSongInfos)
        {
            if(m_songNameFull.contains(info.m_songName))
            {
                hasItem = true;
                setSongNameById(info.m_artistId);
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
    MusicObject::MusicSongInformations musicSongInfos(m_foundTableWidget->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noArtist"));
    }
    else
    {
        setSongCountText();
    }
}

void MusicArtistFoundWidget::createArtistInfoItem(const MusicPlaylistItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidget.isEmpty())
    {
        QLabel *label = m_resizeWidget[0];
        label->setText(tr("<font color=#158FE1> Artist > %1 </font>").arg(item.m_name));

        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != "null")
        {
            download->startToDownload(item.m_coverUrl);
        }

        label = m_resizeWidget[1];
        label->setToolTip(item.m_name);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidget[2];
        label->setToolTip(tr("NickName: %1").arg(item.m_nickname));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidget[3];
        label->setToolTip(tr("Country: %1").arg(item.m_tags));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidget[4];
        label->setToolTip(tr("Birth: %1").arg(item.m_updateTime));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        m_infoLabel->setText(item.m_description);
    }
}

void MusicArtistFoundWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    initSecondWidget();
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
    grid->addWidget(firstLabel);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(180, 180);

    ////////////////////////////////////////////////////////////////////////////

    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 5, 0);
    QLabel *artistLabel = new QLabel(topLineWidget);
    QFont artistFont = artistLabel->font();
    artistFont.setPixelSize(20);
    artistLabel->setFont(artistFont);
    artistLabel->setStyleSheet(MusicUIObject::MFontStyle01);
    artistLabel->setText("-");
    QLabel *nickNameLabel = new QLabel(topLineWidget);
    nickNameLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    nickNameLabel->setText("-");
    QLabel *countryLabel = new QLabel(topLineWidget);
    countryLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    countryLabel->setText("-");
    QLabel *birthLabel = new QLabel(topLineWidget);
    birthLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    birthLabel->setText("-");

    topLineLayout->addWidget(artistLabel);
    topLineLayout->addWidget(nickNameLabel);
    topLineLayout->addWidget(countryLabel);
    topLineLayout->addWidget(birthLabel);
    topLineWidget->setLayout(topLineLayout);

    QWidget *topButtonWidget = new QWidget(topFuncWidget);
    topButtonWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    QHBoxLayout *topButtonLayout = new QHBoxLayout(topButtonWidget);
    topButtonLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *playAllButton = new QPushButton(tr("playAll"), topButtonWidget);
    QPushButton *shareButton = new QPushButton(tr("share"), topButtonWidget);
    playAllButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
    playAllButton->setIconSize(QSize(14, 14));
    playAllButton->setCursor(QCursor(Qt::PointingHandCursor));
    shareButton->setCursor(QCursor(Qt::PointingHandCursor));
    playAllButton->setFixedSize(90, 30);
    shareButton->setFixedSize(55, 30);
    topButtonLayout->addWidget(playAllButton);
    topButtonLayout->addWidget(shareButton);
    topButtonLayout->addStretch(1);
    topButtonWidget->setLayout(topButtonLayout);
    topLineLayout->addWidget(topButtonWidget);
    connect(playAllButton, SIGNAL(clicked()), SLOT(playAllButtonClicked()));
    connect(shareButton, SIGNAL(clicked()), SLOT(shareButtonClicked()));
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
    topFuncLayout->addWidget(topLineWidget);
    topFuncLayout->addWidget(topRightWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    ////////////////////////////////////////////////////////////////////////////

    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    QHBoxLayout *hlayout = new QHBoxLayout(functionWidget);
    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("songItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(m_songButton);
    QPushButton *infoButton = new QPushButton(functionWidget);
    infoButton->setText(tr("Info"));
    infoButton->setFixedSize(100, 25);
    infoButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(infoButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    group->addButton(infoButton, 1);
    connect(group, SIGNAL(buttonClicked(int)), m_container, SLOT(setCurrentIndex(int)));

#ifdef Q_OS_UNIX
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //////////////////////////////////////////////////////////////////////
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidget << firstLabel << artistLabel << nickNameLabel << countryLabel << birthLabel;
}
