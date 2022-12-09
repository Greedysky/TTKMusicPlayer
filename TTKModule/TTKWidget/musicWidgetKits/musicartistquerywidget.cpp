#include "musicartistquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadcoverrequest.h"
#include "musicquerymovierequest.h"
#include "musicrightareawidget.h"
#include "musicpagequerywidget.h"
#include "musictinyuiobject.h"
#include "musicsettingmanager.h"

#include "qrencode/qrcodewidget.h"

#include <qmath.h>
#include <malloc.h>

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
    m_playButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPlaylist + MusicUIObject::MQSSPushButtonStyle01);
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

void MusicArtistAlbumsItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_updateLabel->setToolTip(item.m_updateTime);
    m_updateLabel->setText(MusicUtils::Widget::elidedText(m_updateLabel->font(), m_updateLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != TTK_NULL_STR)
    {
        MusicDownloadCoverRequest *d = new MusicDownloadCoverRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startRequest(item.m_coverUrl);
    }
}

void MusicArtistAlbumsItemWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicImageRenderer *render = new MusicImageRenderer(sender());
    connect(render, SIGNAL(renderFinished(QPixmap)), SLOT(renderFinished(QPixmap)));
    render->setInputData(bytes, m_iconLabel->size());
    render->start();
}

void MusicArtistAlbumsItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
    m_playButton->raise();
}

void MusicArtistAlbumsItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData.m_id);
}



MusicArtistMvsQueryWidget::MusicArtistMvsQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_pageQueryWidget(nullptr)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    QWidget *function = new QWidget(m_mainWindow);
    m_gridLayout = new QGridLayout(function);
    function->setLayout(m_gridLayout);
    m_mainWindow->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Module::Artist;

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(m_networkRequest, SIGNAL(createMovieItem(MusicResultDataItem)), SLOT(createArtistMvsItem(MusicResultDataItem)));
}

MusicArtistMvsQueryWidget::~MusicArtistMvsQueryWidget()
{
    delete m_gridLayout;
}

void MusicArtistMvsQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    MusicQueryMovieRequest *d = TTKObject_cast(MusicQueryMovieRequest*, m_networkRequest);
    d->startToSearch(m_songNameFull);
}

void MusicArtistMvsQueryWidget::setSongNameById(const QString &id)
{
    setSongName(id);
}

void MusicArtistMvsQueryWidget::resizeWidget()
{
    if(!m_resizeWidgets.isEmpty())
    {
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i].m_label);
        }

        const int lineNumber = width() / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistMvsQueryWidget::createArtistMvsItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        m_initialized = true;
        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWindow);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
        m_mainWindow->layout()->addWidget(m_pageQueryWidget->createPageWidget(m_mainWindow, pageTotal));
    }

    MusicArtistAlbumsItemWidget *label = new MusicArtistAlbumsItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(QString)), SLOT(currentItemClicked(QString)));
    label->setResultDataItem(item);

    const int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicArtistMvsQueryWidget::currentItemClicked(const QString &id)
{
    MusicRightAreaWidget::instance()->musicMovieSearch(id);
}

void MusicArtistMvsQueryWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast().m_label;
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
    m_pageQueryWidget->page(index, pageTotal);
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}



MusicArtistAlbumsQueryWidget::MusicArtistAlbumsQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    QWidget *function = new QWidget(m_mainWindow);
    m_gridLayout = new QGridLayout(function);
    function->setLayout(m_gridLayout);
    m_mainWindow->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Module::Artist;

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeAlbumRequest(this);
    connect(m_networkRequest, SIGNAL(createAlbumItem(MusicResultDataItem)), SLOT(createArtistAlbumsItem(MusicResultDataItem)));
}

MusicArtistAlbumsQueryWidget::~MusicArtistAlbumsQueryWidget()
{
    delete m_gridLayout;
}

void MusicArtistAlbumsQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_networkRequest->startToSingleSearch(m_songNameFull);
}

void MusicArtistAlbumsQueryWidget::setSongNameById(const QString &id)
{
    MusicAbstractItemQueryWidget::setSongName(id);
    m_networkRequest->startToSingleSearch(m_songNameFull);
}

void MusicArtistAlbumsQueryWidget::resizeWidget()
{
    if(!m_resizeWidgets.isEmpty())
    {
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i].m_label);
        }

        const int lineNumber = width() / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistAlbumsQueryWidget::createArtistAlbumsItem(const MusicResultDataItem &item)
{
    MusicArtistAlbumsItemWidget *label = new MusicArtistAlbumsItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(QString)), SLOT(currentItemClicked(QString)));
    label->setResultDataItem(item);

    const int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicArtistAlbumsQueryWidget::currentItemClicked(const QString &id)
{
    MusicRightAreaWidget::instance()->musicAlbumSearch(id);
}



MusicArtistQueryTableWidget::MusicArtistQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicArtistQueryTableWidget::~MusicArtistQueryTableWidget()
{
    removeItems();
}

void MusicArtistQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryArtistFinished()") != -1)
    {
        connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryArtistFinished()));
    }
}



MusicArtistQueryWidget::MusicArtistQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_artistAlbums(nullptr),
      m_artistMvs(nullptr)
{
    m_queryTableWidget = new MusicArtistQueryTableWidget(this);
    m_queryTableWidget->hide();

    m_shareType = MusicSongSharingWidget::Module::Artist;

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicArtistQueryWidget::~MusicArtistQueryWidget()
{
    delete m_artistAlbums;
    delete m_artistMvs;
}

void MusicArtistQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_networkRequest->setQueryLite(true);
    m_networkRequest->setQueryAllRecords(false);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::QueryType::Music, MusicUtils::String::artistName(name));
}

void MusicArtistQueryWidget::setSongNameById(const QString &id)
{
    MusicAbstractItemQueryWidget::setSongName(id);
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeArtistRequest(this);
    m_queryTableWidget->setQueryInput(d);
    m_queryTableWidget->startSearchQuery(id);
    connect(d, SIGNAL(createArtistItem(MusicResultDataItem)), SLOT(createArtistItem(MusicResultDataItem)));
}

void MusicArtistQueryWidget::resizeWidget()
{
    m_queryTableWidget->resizeSection();

    if(!m_resizeWidgets.isEmpty())
    {
        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN + 180;

        TTKResizeWidget *data = &m_resizeWidgets[1];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[2];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width - 20));

        data = &m_resizeWidgets[3];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[4];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
    }

    if(m_artistAlbums)
    {
        m_artistAlbums->resizeWidget();
    }

    if(m_artistMvs)
    {
        m_artistMvs->resizeWidget();
    }
}

void MusicArtistQueryWidget::queryAllFinished()
{
    const MusicObject::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(songInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_no_artist_found"));
    }
    else
    {
        bool hasItem = false;
        for(const MusicObject::MusicSongInformation &info : qAsConst(songInfos))
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
            m_statusLabel->setPixmap(QPixmap(":/image/lb_no_artist_found"));
        }
    }
}

void MusicArtistQueryWidget::queryArtistFinished()
{
    const MusicAbstractQueryRequest *d = m_queryTableWidget->queryInput();
    if(!d)
    {
        return;
    }

    const MusicObject::MusicSongInformationList songInfos(d->songInfoList());
    if(songInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_no_artist_found"));
    }
    else
    {
        setSongCountText();
    }
}

void MusicArtistQueryWidget::createArtistItem(const MusicResultDataItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidgets.isEmpty())
    {
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != TTK_NULL_STR)
        {
            MusicDownloadCoverRequest *d = new MusicDownloadCoverRequest(this);
            connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            d->startRequest(item.m_coverUrl);
        }

        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN + 180;

        TTKResizeWidget *data = &m_resizeWidgets[0];
        data->m_label->setText(tr("<font color=#158FE1> Artist > %1 </font>").arg(item.m_name));

        data = &m_resizeWidgets[1];
        data->m_label->setToolTip(item.m_name);
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[2];
        data->m_label->setToolTip(tr("NickName: %1").arg(item.m_nickName));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width - 20));

        data = &m_resizeWidgets[3];
        data->m_label->setToolTip(tr("Country: %1").arg(item.m_tags));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[4];
        data->m_label->setToolTip(tr("Birth: %1").arg(item.m_updateTime));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        m_infoLabel->setText(item.m_description);
    }
}

void MusicArtistQueryWidget::setCurrentIndex(int index)
{
    if(m_queryTableWidget)
    {
        m_queryTableWidget->hide();
    }

    if(m_infoLabel)
    {
        m_infoLabel->hide();
    }

    delete m_artistAlbums;
    m_artistAlbums = nullptr;

    delete m_artistMvs;
    m_artistMvs = nullptr;

    if(index == 0)
    {
        m_queryTableWidget->show();
    }
    else if(index == 1)
    {
        m_infoLabel->show();
    }
    else if(index == 2)
    {
        initThirdWidget();
    }
    else if(index == 3)
    {
        initFourthWidget();
    }

    m_container->setCurrentIndex(index > 2 ? 2 : index);
}

void MusicArtistQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    initSecondWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01 + MusicUIObject::MQSSPushButtonStyle03);
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
    artistLabel->setStyleSheet(MusicUIObject::MQSSFontStyle01);
    artistLabel->setText(TTK_DEFAULT_STR);
    QLabel *nickNameLabel = new QLabel(topLineWidget);
    nickNameLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    nickNameLabel->setText(TTK_DEFAULT_STR);
    QLabel *countryLabel = new QLabel(topLineWidget);
    countryLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    countryLabel->setText(TTK_DEFAULT_STR);
    QLabel *birthLabel = new QLabel(topLineWidget);
    birthLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    birthLabel->setText(TTK_DEFAULT_STR);

    topLineLayout->addWidget(artistLabel);
    topLineLayout->addWidget(nickNameLabel);
    topLineLayout->addWidget(countryLabel);
    topLineLayout->addWidget(birthLabel);
    topLineWidget->setLayout(topLineLayout);

    QWidget *topButtonWidget = new QWidget(topFuncWidget);
    topButtonWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    QHBoxLayout *topButtonLayout = new QHBoxLayout(topButtonWidget);
    topButtonLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *playAllButton = new QPushButton(tr("Play All"), topButtonWidget);
    QPushButton *shareButton = new QPushButton(tr("Share"), topButtonWidget);
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
    numberLabel->setStyleSheet(MusicUIObject::MQSSFontStyle05 + MusicUIObject::MQSSColorStyle05);
    int number = 9;
    numberLabel->setText(QString("%1.%2").arg(number).arg(1));
    topRightLayout->addWidget(numberLabel, 0, 0);
    for(int i = 1; i <= 5; ++i)
    {
        QLabel *label = new QLabel(topRightWidget);
        label->setPixmap(QPixmap((ceil(number / 2.0) - i) >= 0 ? ":/tiny/lb_star" : ":/tiny/lb_unstar"));
        topRightLayout->addWidget(label, 0, i);
    }

    QLabel *numberTextLabel = new QLabel(tr("Score:"), topRightWidget);
    numberTextLabel->setAlignment(Qt::AlignCenter);
    topRightLayout->addWidget(numberTextLabel, 1, 0);
    for(int i = 1; i <= 5; ++i)
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
    functionWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    QHBoxLayout *hlayout = new QHBoxLayout(functionWidget);
    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("SongItems"));
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
    QPushButton *mvsButton = new QPushButton(functionWidget);
    mvsButton->setText(tr("Movies"));
    mvsButton->setFixedSize(100, 25);
    mvsButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(mvsButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_songButton, 0);
    buttonGroup->addButton(infoButton, 1);
    buttonGroup->addButton(albumsButton, 2);
    buttonGroup->addButton(mvsButton, 3);
    QtButtonGroupConnect(buttonGroup, this, setCurrentIndex);

#ifdef Q_OS_UNIX
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
    albumsButton->setFocusPolicy(Qt::NoFocus);
    mvsButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets.push_back({firstLabel, firstLabel->font()});
    m_resizeWidgets.push_back({artistLabel, artistLabel->font()});
    m_resizeWidgets.push_back({nickNameLabel, nickNameLabel->font()});
    m_resizeWidgets.push_back({countryLabel, countryLabel->font()});
    m_resizeWidgets.push_back({birthLabel, birthLabel->font()});
}

void MusicArtistQueryWidget::initThirdWidget()
{
    m_artistAlbums = new MusicArtistAlbumsQueryWidget(m_container);
    m_container->addWidget(m_artistAlbums);
    m_artistAlbums->setSongName(m_songNameFull);
}

void MusicArtistQueryWidget::initFourthWidget()
{
    m_artistMvs = new MusicArtistMvsQueryWidget(m_container);
    m_container->addWidget(m_artistMvs);
    m_artistMvs->setSongName(m_songNameFull);
}
