#include "musicartistquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicqueryitemwidget.h"
#include "musicrightareawidget.h"
#include "musicpagequerywidget.h"
#include "musicratinglabel.h"

#include <cmath>
#include "qrencode/qrcodewidget.h"

MusicArtistMvsQueryWidget::MusicArtistMvsQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_pageQueryWidget(nullptr)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    QWidget *function = new QWidget(m_mainWidget);
    m_gridLayout = new QGridLayout(function);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    function->setLayout(m_gridLayout);
    m_mainWidget->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Module::Artist;

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeArtistMovieRequest(this);
    connect(m_networkRequest, SIGNAL(createMovieItem(MusicResultDataItem)), SLOT(createArtistMvsItem(MusicResultDataItem)));
}

MusicArtistMvsQueryWidget::~MusicArtistMvsQueryWidget()
{
    delete m_gridLayout;
}

void MusicArtistMvsQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearch(value);
}

void MusicArtistMvsQueryWidget::setCurrentKey(const QString &id)
{
    setCurrentValue(id);
}

void MusicArtistMvsQueryWidget::resizeGeometry()
{
    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    for(const Data &data : qAsConst(m_resizeWidgets))
    {
        m_gridLayout->removeWidget(data.m_label);
    }

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}

void MusicArtistMvsQueryWidget::createArtistMvsItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        m_initialized = true;
        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWidget);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        m_mainWidget->layout()->addWidget(m_pageQueryWidget->createPageWidget(m_mainWidget, m_networkRequest->pageTotalSize()));
    }

    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setShowCount(false);
    label->setValueLabel(false);
    label->setResultDataItem(item, G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);
    m_resizeWidgets.append({label, label->font()});
}

void MusicArtistMvsQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    MusicRightAreaWidget::instance()->showMovieSearchedFound({}, item.m_id);
}

void MusicArtistMvsQueryWidget::buttonClicked(int index)
{
    removeItems(m_gridLayout);
    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}



MusicArtistAlbumsQueryWidget::MusicArtistAlbumsQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_pageQueryWidget(nullptr)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    QWidget *function = new QWidget(m_mainWidget);
    m_gridLayout = new QGridLayout(function);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    function->setLayout(m_gridLayout);
    m_mainWidget->layout()->addWidget(function);
    m_container->show();

    m_shareType = MusicSongSharingWidget::Module::Artist;

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeArtistAlbumRequest(this);
    connect(m_networkRequest, SIGNAL(createAlbumItem(MusicResultDataItem)), SLOT(createArtistAlbumsItem(MusicResultDataItem)));
}

MusicArtistAlbumsQueryWidget::~MusicArtistAlbumsQueryWidget()
{
    delete m_gridLayout;
}

void MusicArtistAlbumsQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearch(value);
}

void MusicArtistAlbumsQueryWidget::setCurrentKey(const QString &id)
{
    setCurrentValue(id);
}

void MusicArtistAlbumsQueryWidget::resizeGeometry()
{
    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    for(const Data &data : qAsConst(m_resizeWidgets))
    {
        m_gridLayout->removeWidget(data.m_label);
    }

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}

void MusicArtistAlbumsQueryWidget::createArtistAlbumsItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        m_initialized = true;
        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWidget);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        m_mainWidget->layout()->addWidget(m_pageQueryWidget->createPageWidget(m_mainWidget, m_networkRequest->pageTotalSize()));
    }

    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setShowTime(true);
    label->setShowCount(false);
    label->setResultDataItem(item, G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);
    m_resizeWidgets.append({label, label->font()});
}

void MusicArtistAlbumsQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    MusicRightAreaWidget::instance()->showAlbumFound({}, item.m_id);
}

void MusicArtistAlbumsQueryWidget::buttonClicked(int index)
{
    removeItems(m_gridLayout);
    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}



MusicArtistQueryWidget::MusicArtistQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_artistAlbums(nullptr),
      m_artistMvs(nullptr)
{
    m_tableWidget = new MusicItemQueryTableWidget(this);
    m_tableWidget->hide();

    m_shareType = MusicSongSharingWidget::Module::Artist;

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeSongRequest(this);
    connect(m_networkRequest, SIGNAL(downloadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicArtistQueryWidget::~MusicArtistQueryWidget()
{
    delete m_artistAlbums;
    delete m_artistMvs;
}

void MusicArtistQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    m_networkRequest->startToSearch(TTK::generateSongArtist(value));
}

void MusicArtistQueryWidget::setCurrentKey(const QString &id)
{
    MusicAbstractItemQueryWidget::setCurrentValue(id);
    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makeArtistRequest(this);
    m_tableWidget->setQueryInput(req);
    m_tableWidget->startToSearchByValue(id);

    connect(req, SIGNAL(downloadDataChanged(QString)), SLOT(queryArtistFinished()));
    connect(req, SIGNAL(createArtistItem(MusicResultDataItem)), SLOT(createArtistItem(MusicResultDataItem)));
}

void MusicArtistQueryWidget::resizeGeometry()
{
    m_tableWidget->resizeGeometry();

    if(m_artistAlbums)
    {
        m_artistAlbums->resizeGeometry();
    }

    if(m_artistMvs)
    {
        m_artistMvs->resizeGeometry();
    }

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 180;

    Data *data = &m_resizeWidgets[1];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[2];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width - 20));

    data = &m_resizeWidgets[3];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
}

void MusicArtistQueryWidget::setCurrentIndex(int index)
{
    if(m_tableWidget)
    {
        m_tableWidget->hide();
    }

    if(m_infoLabel)
    {
        m_infoLabel->hide();
    }

    delete m_artistAlbums;
    m_artistAlbums = nullptr;

    delete m_artistMvs;
    m_artistMvs = nullptr;

    if(index == 0 && m_tableWidget)
    {
        m_tableWidget->show();
    }
    else if(index == 1 && m_infoLabel)
    {
        m_infoLabel->show();
    }
    else if(index == 2)
    {
        m_artistAlbums = new MusicArtistAlbumsQueryWidget(m_container);
        m_container->addWidget(m_artistAlbums);
        m_artistAlbums->setCurrentValue(m_value);
    }
    else if(index == 3)
    {
        m_artistMvs = new MusicArtistMvsQueryWidget(m_container);
        m_container->addWidget(m_artistMvs);
        m_artistMvs->setCurrentValue(m_value);
    }

    m_container->setCurrentIndex(index > 2 ? 2 : index);
}

void MusicArtistQueryWidget::queryAllFinished()
{
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    if(songInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_no_artist_found"));
    }
    else
    {
        bool hasItem = false;
        for(const TTK::MusicSongInformation &info : qAsConst(songInfos))
        {
            if(m_value.contains(info.m_artistName))
            {
                hasItem = true;
                setCurrentKey(info.m_artistId);
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
    const MusicAbstractQueryRequest *req = m_tableWidget->queryInput();
    if(!req)
    {
        return;
    }

    const TTK::MusicSongInformationList &songInfos = req->items();
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

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *req = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 180;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setText(tr("<font color=#158FE1> Artist > %1 </font>").arg(item.m_name));

    data = &m_resizeWidgets[1];
    data->m_label->setToolTip(item.m_name);
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[2];
    data->m_label->setToolTip(tr("NickName: %1").arg(item.m_nickName));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width - 20));

    data = &m_resizeWidgets[3];
    data->m_label->setToolTip(tr("Birth: %1").arg(item.m_time));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    m_infoLabel->setText(item.m_description);
}

void MusicArtistQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    createFirstWidget();
    createSecondWidget();
    m_container->show();

    layout()->removeWidget(m_mainWidget);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWidget);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
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
    artistLabel->setStyleSheet(TTK::UI::FontStyle01);
    artistLabel->setText(TTK_DEFAULT_STR);

    QLabel *nickNameLabel = new QLabel(topLineWidget);
    nickNameLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    nickNameLabel->setText(TTK_DEFAULT_STR);

    QLabel *birthLabel = new QLabel(topLineWidget);
    birthLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    birthLabel->setText(TTK_DEFAULT_STR);

    topLineLayout->addWidget(artistLabel);
    topLineLayout->addWidget(nickNameLabel);
    topLineLayout->addWidget(birthLabel);
    topLineWidget->setLayout(topLineLayout);

    QWidget *topButtonWidget = new QWidget(topFuncWidget);
    topButtonWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
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
    numberLabel->setStyleSheet(TTK::UI::FontStyle06 + TTK::UI::ColorStyle11);

    const int number = 7 + TTK::random(3);
    numberLabel->setText(QString("%1.%2").arg(number).arg(TTK::random(10)));
    topRightLayout->addWidget(numberLabel, 0, 0);
    topRightLayout->addWidget(new MusicRatingLabel(std::ceil(number / 2.0), topRightWidget), 0, 1, 1, 6);

    QLabel *numberTextLabel = new QLabel(tr("Score:"), topRightWidget);
    topRightLayout->addWidget(numberTextLabel, 1, 0);
    topRightLayout->addWidget(new MusicRatingLabel(topRightWidget), 1, 1, 1, 6);

    QLabel *marginBottmLabel = new QLabel(topRightWidget);
    marginBottmLabel->setFixedHeight(40);
    topRightLayout->addWidget(marginBottmLabel, 2, 0);
    topRightWidget->setLayout(topRightLayout);

    QRCodeQWidget *code = new QRCodeQWidget({}, QSize(90, 90), topRightWidget);
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
    functionWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *hLayout = new QHBoxLayout(functionWidget);

    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("SongItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));
    hLayout->addWidget(m_songButton);

    QPushButton *infoButton = new QPushButton(functionWidget);
    infoButton->setText(tr("Info"));
    infoButton->setFixedSize(100, 25);
    infoButton->setCursor(QCursor(Qt::PointingHandCursor));
    hLayout->addWidget(infoButton);

    QPushButton *albumsButton = new QPushButton(functionWidget);
    albumsButton->setText(tr("Albums"));
    albumsButton->setFixedSize(100, 25);
    albumsButton->setCursor(QCursor(Qt::PointingHandCursor));
    hLayout->addWidget(albumsButton);

    QPushButton *mvsButton = new QPushButton(functionWidget);
    mvsButton->setText(tr("Movies"));
    mvsButton->setFixedSize(100, 25);
    mvsButton->setCursor(QCursor(Qt::PointingHandCursor));

    hLayout->addWidget(mvsButton);
    hLayout->addStretch(1);
    functionWidget->setLayout(hLayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_songButton, 0);
    buttonGroup->addButton(infoButton, 1);
    buttonGroup->addButton(albumsButton, 2);
    buttonGroup->addButton(mvsButton, 3);
    QtButtonGroupConnect(buttonGroup, this, setCurrentIndex, TTK_SLOT);

#ifdef Q_OS_UNIX
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
    albumsButton->setFocusPolicy(Qt::NoFocus);
    mvsButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWidget->layout()->addWidget(function);

    m_resizeWidgets.append({firstLabel, firstLabel->font()});
    m_resizeWidgets.append({artistLabel, artistLabel->font()});
    m_resizeWidgets.append({nickNameLabel, nickNameLabel->font()});
    m_resizeWidgets.append({birthLabel, birthLabel->font()});
}
