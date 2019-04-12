#include "musicartistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcethread.h"
#include "musicdownloadquerymoviethread.h"
#include "musicdownloadsimilarthread.h"
#include "musicrightareawidget.h"
#include "musicpagingwidgetobject.h"
#include "musictinyuiobject.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictime.h"
#include "qrencode/qrcodewidget.h"

#include <qmath.h>

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  200
#define LINE_SPACING_SIZE  200

MusicArtistAlbumsItemWidget::MusicArtistAlbumsItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlaylist + MusicUIObject::MPushButtonStyle01);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setText(" - ");

    m_updateLabel = new QLabel(this);
    m_updateLabel->setGeometry(0, 175, WIDTH_LABEL_SIZE, 25);
    m_updateLabel->setText(" - ");
}

MusicArtistAlbumsItemWidget::~MusicArtistAlbumsItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_updateLabel;
}

void MusicArtistAlbumsItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_updateLabel->setToolTip(item.m_updateTime);
    m_updateLabel->setText(MusicUtils::Widget::elidedText(m_updateLabel->font(), m_updateLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
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
        QPixmap cv(":/image/lb_album_cover");
        pix = pix.scaled(m_iconLabel->size());
        MusicUtils::Widget::fusionPixmap(pix, cv, QPoint(0, 0));
        m_iconLabel->setPixmap(pix);
    }
    m_playButton->raise();
}

void MusicArtistAlbumsItemWidget::currentItemClicked()
{
    emit currentItemClicked(m_itemData.m_id);
}



MusicArtistMvsFoundWidget::MusicArtistMvsFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;
    m_pagingWidgetObject = nullptr;
    m_firstInit = false;

    QWidget *function = new QWidget(m_mainWindow);
    m_gridLayout = new QGridLayout(function);
    function->setLayout(m_gridLayout);
    m_mainWindow->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Artist;
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getMovieThread(this);
    connect(m_downloadThread, SIGNAL(createMovieInfoItem(MusicResultsItem)), SLOT(createArtistMvsItem(MusicResultsItem)));
}

MusicArtistMvsFoundWidget::~MusicArtistMvsFoundWidget()
{
    delete m_gridLayout;
}

void MusicArtistMvsFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    MusicDownLoadQueryMovieThread *v = MStatic_cast(MusicDownLoadQueryMovieThread*, m_downloadThread);
    v->startToSearch(m_songNameFull);
}

void MusicArtistMvsFoundWidget::setSongNameById(const QString &id)
{
    setSongName(id);
}

void MusicArtistMvsFoundWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty())
    {
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i]);
        }

        const int lineNumber = width()/LINE_SPACING_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistMvsFoundWidget::createArtistMvsItem(const MusicResultsItem &item)
{
    if(!m_firstInit)
    {
        m_firstInit = true;
        m_pagingWidgetObject = new MusicPagingWidgetObject(m_mainWindow);
        connect(m_pagingWidgetObject, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

        const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
        m_mainWindow->layout()->addWidget(m_pagingWidgetObject->createPagingWidget(m_mainWindow, total));
    }

    MusicArtistAlbumsItemWidget *label = new MusicArtistAlbumsItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(QString)), SLOT(currentItemClicked(QString)));
    label->setMusicResultsItem(item);

    const int lineNumber = width()/LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicArtistMvsFoundWidget::currentItemClicked(const QString &id)
{
    MusicRightAreaWidget::instance()->musicMovieSearch(id);
}

void MusicArtistMvsFoundWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast();
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
    m_pagingWidgetObject->paging(index, total);
    m_downloadThread->startToPage(m_pagingWidgetObject->currentIndex() - 1);
}



MusicArtistSimilarFoundWidget::MusicArtistSimilarFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;
    QWidget *function = new QWidget(m_mainWindow);
    m_gridLayout = new QGridLayout(function);
    function->setLayout(m_gridLayout);
    m_mainWindow->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Artist;
    m_similarThread = M_DOWNLOAD_QUERY_PTR->getSimilarArtistThread(this);
    connect(m_similarThread, SIGNAL(createSimilarItem(MusicResultsItem)), SLOT(createArtistSimilarItem(MusicResultsItem)));
}

MusicArtistSimilarFoundWidget::~MusicArtistSimilarFoundWidget()
{
    delete m_gridLayout;
}

void MusicArtistSimilarFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_similarThread->startToSearch(m_songNameFull);
}

void MusicArtistSimilarFoundWidget::setSongNameById(const QString &id)
{
    MusicFoundAbstractWidget::setSongName(id);
    m_similarThread->startToSearch(m_songNameFull);
}

void MusicArtistSimilarFoundWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty())
    {
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i]);
        }

        const int lineNumber = width()/HEIGHT_LABEL_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistSimilarFoundWidget::createArtistSimilarItem(const MusicResultsItem &item)
{
    MusicArtistAlbumsItemWidget *label = new MusicArtistAlbumsItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(QString)), SLOT(currentItemClicked(QString)));
    label->setMusicResultsItem(item);

    const int lineNumber = width()/LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicArtistSimilarFoundWidget::currentItemClicked(const QString &id)
{
    MusicRightAreaWidget::instance()->musicArtistSearch(id);
}



MusicArtistAlbumsFoundWidget::MusicArtistAlbumsFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;
    QWidget *function = new QWidget(m_mainWindow);
    m_gridLayout = new QGridLayout(function);
    function->setLayout(m_gridLayout);
    m_mainWindow->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Artist;
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getAlbumThread(this);
    connect(m_downloadThread, SIGNAL(createAlbumInfoItem(MusicResultsItem)), SLOT(createArtistAlbumsItem(MusicResultsItem)));
}

MusicArtistAlbumsFoundWidget::~MusicArtistAlbumsFoundWidget()
{
    delete m_gridLayout;
}

void MusicArtistAlbumsFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->startToSingleSearch(m_songNameFull);
}

void MusicArtistAlbumsFoundWidget::setSongNameById(const QString &id)
{
    MusicFoundAbstractWidget::setSongName(id);
    m_downloadThread->startToSingleSearch(m_songNameFull);
}

void MusicArtistAlbumsFoundWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty())
    {
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i]);
        }

        const int lineNumber = width()/LINE_SPACING_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistAlbumsFoundWidget::createArtistAlbumsItem(const MusicResultsItem &item)
{
    MusicArtistAlbumsItemWidget *label = new MusicArtistAlbumsItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(QString)), SLOT(currentItemClicked(QString)));
    label->setMusicResultsItem(item);

    const int lineNumber = width()/LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicArtistAlbumsFoundWidget::currentItemClicked(const QString &id)
{
    MusicRightAreaWidget::instance()->musicAlbumSearch(id);
}



MusicArtistFoundTableWidget::MusicArtistFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicArtistFoundTableWidget::~MusicArtistFoundTableWidget()
{
    clearAllItems();
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
    m_artistAlbums = nullptr;
    m_artistMvs = nullptr;
    m_artistSim = nullptr;
    m_shareType = MusicSongSharingWidget::Artist;
    m_foundTableWidget = new MusicArtistFoundTableWidget(this);
    m_foundTableWidget->hide();
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicArtistFoundWidget::~MusicArtistFoundWidget()
{
    delete m_artistAlbums;
    delete m_artistMvs;
    delete m_artistSim;
}

void MusicArtistFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->setQuerySimplify(true);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, MusicUtils::String::artistName(name));
}

void MusicArtistFoundWidget::setSongNameById(const QString &id)
{
    MusicFoundAbstractWidget::setSongName(id);
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getArtistThread(this);
    m_foundTableWidget->setQueryInput(d);
    m_foundTableWidget->startSearchQuery(id);
    connect(d, SIGNAL(createArtistInfoItem(MusicResultsItem)), SLOT(createArtistInfoItem(MusicResultsItem)));
}

void MusicArtistFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();

    if(m_artistAlbums) m_artistAlbums->resizeWindow();
    if(m_artistMvs) m_artistMvs->resizeWindow();
    if(m_artistSim) m_artistSim->resizeWindow();
}

void MusicArtistFoundWidget::queryAllFinished()
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downloadThread->getMusicSongInfos());
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
    const MusicDownLoadQueryThreadAbstract *d = m_foundTableWidget->getQueryInput();
    if(!d)
    {
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(d->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noArtist"));
    }
    else
    {
        setSongCountText();
    }
}

void MusicArtistFoundWidget::createArtistInfoItem(const MusicResultsItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidgets.isEmpty())
    {
        QLabel *label = m_resizeWidgets[0];
        label->setText(tr("<font color=#158FE1> Artist > %1 </font>").arg(item.m_name));

        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
        {
            download->startToDownload(item.m_coverUrl);
        }

        label = m_resizeWidgets[1];
        label->setToolTip(item.m_name);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[2];
        label->setToolTip(tr("NickName: %1").arg(item.m_nickName));
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

void MusicArtistFoundWidget::setCurrentIndex(int index)
{
    if(m_foundTableWidget) m_foundTableWidget->hide();
    if(m_infoLabel) m_infoLabel->hide();

    delete m_artistAlbums;
    m_artistAlbums = nullptr;
    delete m_artistMvs;
    m_artistMvs = nullptr;
    delete m_artistSim;
    m_artistSim = nullptr;

    if(index == 0)
    {
        m_foundTableWidget->show();
    }
    else if(index == 1)
    {
        m_infoLabel->show();
    }
    else if(index == 2)
    {
        initFivethWidget();
    }
    else if(index == 3)
    {
        initThirdWidget();
    }
    else if(index == 4)
    {
        initFourthWidget();
    }

    m_container->setCurrentIndex(index > 2 ? 2 : index);
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
    //
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(210, 180);
    //

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
    //

    QWidget *topRightWidget = new QWidget(topFuncWidget);
    QGridLayout *topRightLayout = new QGridLayout(topRightWidget);
    topRightLayout->setContentsMargins(0, 0, 0, 0);
    topRightLayout->setSpacing(0);

    QLabel *numberLabel = new QLabel(topRightWidget);
    numberLabel->setAlignment(Qt::AlignCenter);
    numberLabel->setStyleSheet(MusicUIObject::MFontStyle06 + MusicUIObject::MColorStyle05);
    int number = 9;
    numberLabel->setText(QString("%1.%2").arg(number).arg(1));
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
    code->setIcon(":/image/lb_app_logo", 0.23);
    topRightLayout->addWidget(code, 3, 2, 1, 6);

    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncLayout->addWidget(topRightWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    //

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
    QPushButton *similarButton = new QPushButton(functionWidget);
    similarButton->setText(tr("Similar"));
    similarButton->setFixedSize(100, 25);
    similarButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(similarButton);
    QPushButton *albumsButton = new QPushButton(functionWidget);
    albumsButton->setText(tr("Albums"));
    albumsButton->setFixedSize(100, 25);
    albumsButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(albumsButton);
    QPushButton *mvsButton = new QPushButton(functionWidget);
    mvsButton->setText(tr("Mvs"));
    mvsButton->setFixedSize(100, 25);
    mvsButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(mvsButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    group->addButton(infoButton, 1);
    group->addButton(similarButton, 2);
    group->addButton(albumsButton, 3);
    group->addButton(mvsButton, 4);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setCurrentIndex(int)));

#ifdef Q_OS_UNIX
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
    similarButton->setFocusPolicy(Qt::NoFocus);
    albumsButton->setFocusPolicy(Qt::NoFocus);
    mvsButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets << firstLabel << artistLabel << nickNameLabel << countryLabel << birthLabel;
}

void MusicArtistFoundWidget::initThirdWidget()
{
    m_artistAlbums = new MusicArtistAlbumsFoundWidget(m_container);
    m_container->addWidget(m_artistAlbums);
    m_artistAlbums->setSongName(m_songNameFull);
}

void MusicArtistFoundWidget::initFourthWidget()
{
    m_artistMvs = new MusicArtistMvsFoundWidget(m_container);
    m_container->addWidget(m_artistMvs);
    m_artistMvs->setSongName(m_songNameFull);
}

void MusicArtistFoundWidget::initFivethWidget()
{
    m_artistSim = new MusicArtistSimilarFoundWidget(m_container);
    m_container->addWidget(m_artistSim);
    m_artistSim->setSongName(m_songNameFull);
}
