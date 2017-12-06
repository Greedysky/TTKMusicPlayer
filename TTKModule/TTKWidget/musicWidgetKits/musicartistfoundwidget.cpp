#include "musicartistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcethread.h"
#include "musictinyuiobject.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictime.h"
#include "musicrightareawidget.h"
#include "qrencode/qrcodewidget.h"

#include <qmath.h>

#define MIN_LABEL_SIZE  150
#define MAX_LABEL_SIZE  200

MusicArtistAlbumsItemWidget::MusicArtistAlbumsItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(MIN_LABEL_SIZE, MAX_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlaylist + MusicUIObject::MPushButtonStyle01);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, MIN_LABEL_SIZE, MIN_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, MIN_LABEL_SIZE, 25);
    m_nameLabel->setText(" - ");

    m_updateLabel = new QLabel(this);
    m_updateLabel->setGeometry(0, 175, MIN_LABEL_SIZE, 25);
    m_updateLabel->setText(" - ");
}

MusicArtistAlbumsItemWidget::~MusicArtistAlbumsItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_updateLabel;
}

QString MusicArtistAlbumsItemWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicArtistAlbumsItemWidget::setMusicItem(const MusicPlaylistItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(),
                                                        Qt::ElideRight, MIN_LABEL_SIZE));
    m_updateLabel->setToolTip(item.m_updateTime);
    m_updateLabel->setText(MusicUtils::Widget::elidedText(m_updateLabel->font(), m_updateLabel->toolTip(),
                                                          Qt::ElideRight, MIN_LABEL_SIZE));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != "null")
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicArtistAlbumsItemWidget::downLoadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    if(!pix.isNull())
    {
        m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
    }
    m_playButton->raise();
}

void MusicArtistAlbumsItemWidget::currentItemClicked()
{
    emit currentItemClicked(m_itemData.m_id);
}



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
    m_albumsContainer = nullptr;
    m_shareType = MusicSongSharingWidget::Artist;
    m_foundTableWidget = new MusicArtistFoundTableWidget(this);
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicArtistFoundWidget::~MusicArtistFoundWidget()
{
    qDeleteAll(m_albumsWidgets);
    delete m_albumsContainer;
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
    MusicFoundAbstractWidget::setSongName(id);
    MusicDownLoadQueryThreadAbstract *v = M_DOWNLOAD_QUERY_PTR->getArtistThread(this);
    m_foundTableWidget->setQueryInput(v);
    m_foundTableWidget->startSearchQuery(id);
    connect(v, SIGNAL(createArtistInfoItem(MusicPlaylistItem)), SLOT(createArtistInfoItem(MusicPlaylistItem)));
}

void MusicArtistFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();

    if(m_albumsContainer && !m_albumsWidgets.isEmpty())
    {
        QGridLayout *gridLayout = MStatic_cast(QGridLayout*, m_albumsContainer->layout());
        if(gridLayout)
        {
            for(int i=0; i<m_albumsWidgets.count(); ++i)
            {
                gridLayout->removeWidget(m_albumsWidgets[i]);
            }

            int lineNumber = width()/MAX_LABEL_SIZE;
            for(int i=0; i<m_albumsWidgets.count(); ++i)
            {
                gridLayout->addWidget(m_albumsWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
            }
        }
    }
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

    if(!m_resizeWidgets.isEmpty())
    {
        QLabel *label = m_resizeWidgets[0];
        label->setText(tr("<font color=#158FE1> Artist > %1 </font>").arg(item.m_name));

        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != "null")
        {
            download->startToDownload(item.m_coverUrl);
        }

        label = m_resizeWidgets[1];
        label->setToolTip(item.m_name);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[2];
        label->setToolTip(tr("NickName: %1").arg(item.m_nickname));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[3];
        label->setToolTip(tr("Country: %1").arg(item.m_tags));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[4];
        label->setToolTip(tr("Birth: %1").arg(item.m_updateTime));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        m_infoLabel->setText(item.m_description);
    }
}

void MusicArtistFoundWidget::createAlbumsItem(const MusicPlaylistItem &item)
{
    if(!m_albumsContainer)
    {
        return;
    }

    MusicArtistAlbumsItemWidget *label = new MusicArtistAlbumsItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(QString)), SLOT(currentItemClicked(QString)));
    label->setMusicItem(item);

    QGridLayout *gridLayout = MStatic_cast(QGridLayout*, m_albumsContainer->layout());
    int lineNumber = width()/MAX_LABEL_SIZE;
    gridLayout->addWidget(label, m_albumsWidgets.count()/lineNumber,
                                 m_albumsWidgets.count()%lineNumber, Qt::AlignCenter);
    m_albumsWidgets << label;
}

void MusicArtistFoundWidget::currentItemClicked(const QString &id)
{
    MusicRightAreaWidget::instance()->musicArtistAlbums(id);
}

void MusicArtistFoundWidget::setCurrentIndex(int index)
{
    while(!m_albumsWidgets.isEmpty())
    {
        delete m_albumsWidgets.takeLast();
    }
    delete m_albumsContainer;
    m_albumsContainer = nullptr;

    if(index == 2)
    {
        initThirdWidget();
    }

    m_container->setCurrentIndex(index);
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
    QPushButton *albumsButton = new QPushButton(functionWidget);
    albumsButton->setText(tr("Albums"));
    albumsButton->setFixedSize(100, 25);
    albumsButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(albumsButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    group->addButton(infoButton, 1);
    group->addButton(albumsButton, 2);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setCurrentIndex(int)));

#ifdef Q_OS_UNIX
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
    albumsButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //////////////////////////////////////////////////////////////////////
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets << firstLabel << artistLabel << nickNameLabel << countryLabel << birthLabel;
}

void MusicArtistFoundWidget::initThirdWidget()
{
    m_albumsContainer = new QWidget(m_container);
    m_container->addWidget(m_albumsContainer);

    QGridLayout *gridLayout = new QGridLayout(m_albumsContainer);
    gridLayout->setVerticalSpacing(35);
    m_albumsContainer->setLayout(gridLayout);

    MusicDownLoadQueryThreadAbstract *album = M_DOWNLOAD_QUERY_PTR->getAlbumThread(this);
    connect(album, SIGNAL(createAlbumInfoItem(MusicPlaylistItem)), SLOT(createAlbumsItem(MusicPlaylistItem)));
    album->startToSingleSearch(m_songNameFull);
}
