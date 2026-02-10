#include "musicmainrecommendwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicrightareawidget.h"
#include "musicqueryitemwidget.h"
#include "musictoastlabel.h"
#include "musicpagequerywidget.h"
#include "musicdownloadwidget.h"
#include "musicsongscontainerwidget.h"
#include "ttkclickedgroup.h"

#define QUERY_WIDGET_WIDTH  (G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - LEFT_SIDE_WIDTH_MIN)

MusicNewSongRecommendQueryTableWidget::MusicNewSongRecommendQueryTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(5);

    delete m_labelDelegate;
    m_labelDelegate = nullptr;
    setItemDelegateForColumn(0, nullptr);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 230);
    headerView->resizeSection(1, 20);
    headerView->resizeSection(2, 20);
    headerView->resizeSection(3, 40);
    headerView->resizeSection(4, 5);
}

MusicNewSongRecommendQueryTableWidget::~MusicNewSongRecommendQueryTableWidget()
{
    removeItems();
}

void MusicNewSongRecommendQueryTableWidget::startToSearchByValue(const QString &value)
{
    if(!G_NETWORK_PTR->isOnline())
    {
        removeItems();
        return;
    }

    m_networkRequest->startToSearch(value);
}

void MusicNewSongRecommendQueryTableWidget::downloadQueryResult(int row)
{
    if(!isValid(row))
    {
        return;
    }

    MusicDownloadWidget *widget = new MusicDownloadWidget(this);
    widget->initialize(m_networkRequest, row);
    widget->show();
}

void MusicNewSongRecommendQueryTableWidget::resizeGeometry()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 230 + (width - WINDOW_WIDTH_MIN) / 2.0);

    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerView->sectionSize(0)));
    }
}

void MusicNewSongRecommendQueryTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || row >= rowCount() - 1)
    {
        return;
    }

    addSearchMusicToPlaylist(row, true);
}

void MusicNewSongRecommendQueryTableWidget::itemCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 1);
    if(it)
    {
        it->setIcon(QIcon());
    }

    it = item(m_previousColorRow, 2);
    if(it)
    {
        it->setIcon(QIcon());
    }

    it = item(m_previousColorRow, 3);
    if(it)
    {
        it->setIcon(QIcon());
        it->setText(m_networkRequest->items()[m_previousColorRow].m_duration);
    }

    if(it = item(row, 1))
    {
        it->setIcon(QIcon(":/contextMenu/btn_play"));
    }

    if(it = item(row, 2))
    {
        it->setIcon(QIcon(":/contextMenu/btn_add"));
    }

    if(it = item(row, 3))
    {
        it->setText({});
        it->setIcon(QIcon(":/contextMenu/btn_download"));
    }

    MusicQueryTableWidget::itemCellEntered(row, column);

    if(column == 1 || column == 2 || column == 3)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
}

void MusicNewSongRecommendQueryTableWidget::itemCellClicked(int row, int column)
{
    MusicQueryTableWidget::itemCellClicked(row, column);

    switch(column)
    {
        case 1: addSearchMusicToPlaylist(row, true); break;
        case 2: addSearchMusicToPlaylist(row, false); break;
        case 3: downloadQueryResult(row); break;
        default: break;
    }
}

void MusicNewSongRecommendQueryTableWidget::removeItems()
{
    MusicQueryTableWidget::removeItems();
    setColumnCount(5);
}

void MusicNewSongRecommendQueryTableWidget::createResultItem(const MusicResultInfoItem &result)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setToolTip(TTK::generateSongName(result.m_songName, result.m_artistName));
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(0)));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    setItem(count, 2, item);

                      item = new QTableWidgetItem(result.m_duration);
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    setItem(count, 4, item);
}

void MusicNewSongRecommendQueryTableWidget::addSearchMusicToPlaylist(int row, bool play)
{
    if(!isValid(row))
    {
        return;
    }

    TTK::MusicSongInformationList songInfos(m_networkRequest->items());
    if(row >= songInfos.count())
    {
        return;
    }

    downloadDataFrom(&songInfos[row], play);
}

bool MusicNewSongRecommendQueryTableWidget::downloadDataFrom(TTK::MusicSongInformation *info, bool play)
{
    if(!G_NETWORK_PTR->isOnline())
    {
        MusicToastLabel::popup(tr("No resource found"));
        return false;
    }

    m_networkRequest->startToQueryResult(info, TTK_BN_128);

    if(info->m_songProps.isEmpty())
    {
        MusicToastLabel::popup(tr("No resource found"));
        return false;
    }

    const TTK::MusicSongProperty &prop = info->m_songProps.first();

    MusicResultDataItem item;
    item.m_name = TTK::generateSongName(info->m_songName, info->m_artistName);
    item.m_time = info->m_duration;
    item.m_id = info->m_songId;
    item.m_nickName = prop.m_url;
    item.m_description = prop.m_format;
    item.m_count = prop.m_size;
    item.m_title = play ? MUSIC_PLAY_NOW : MUSIC_PLAY_LATER;

    if(m_networkRequest)
    {
        item.m_id = m_networkRequest->queryServer() + item.m_id;
    }

    MusicSongsContainerWidget::instance()->addSongBufferToPlaylist(item);
    return true;
}


MusicItemRecommendQueryWidget::MusicItemRecommendQueryWidget(RecommendModule module, QWidget *parent)
    : QWidget(parent),
      m_module(module),
      m_networkRequest(nullptr)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_gridLayout->setVerticalSpacing(5);
    setLayout(m_gridLayout);
}

MusicItemRecommendQueryWidget::~MusicItemRecommendQueryWidget()
{
    delete m_gridLayout;
    delete m_networkRequest;
}

void MusicItemRecommendQueryWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    m_networkRequest = query;
    switch(m_module)
    {
        case RecommendModule::Album: connect(m_networkRequest, SIGNAL(createAlbumItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem))); break;
        case RecommendModule::Artist: connect(m_networkRequest, SIGNAL(createArtistListItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem))); break;
        case RecommendModule::Playlist:
        case RecommendModule::PlaylistHQ: connect(m_networkRequest, SIGNAL(createPlaylistItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem))); break;
        default: break;
    }
}

void MusicItemRecommendQueryWidget::createResultItem(const MusicResultDataItem &item)
{
    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->hide();
    label->setShowTime(false);
    label->setShowCount(false);
    label->setValueLabel(m_module != RecommendModule::Artist);
    label->setResultDataItem(item, new MusicCoverSourceRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize;
    int row = 0, col = 0;

    if(m_module != RecommendModule::Album)
    {
        row = 0;
        col = m_resizeWidgets.count();
    }
    else
    {
        int rowNumber = lineNumber;
        if(rowNumber > m_networkRequest->pageSize())
        {
            rowNumber = m_networkRequest->pageSize();
        }

        row = m_resizeWidgets.count() / (rowNumber / 2);
        col = m_resizeWidgets.count() % (rowNumber / 2);
    }

    if(m_resizeWidgets.count() < lineNumber)
    {
        label->show();
        m_gridLayout->addWidget(label, row, col, Qt::AlignLeft | Qt::AlignTop);
    }
    m_resizeWidgets.append(label);
}

void MusicItemRecommendQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    switch(m_module)
    {
        case RecommendModule::Album: MusicRightAreaWidget::instance()->showAlbumFound({}, item.m_id); break;
        case RecommendModule::Artist: MusicRightAreaWidget::instance()->showArtistFound({}, item.m_id); break;
        case RecommendModule::Playlist:
        case RecommendModule::PlaylistHQ: MusicRightAreaWidget::instance()->showPlaylistFound(item.m_id); break;
        default: break;
    }
}

void MusicItemRecommendQueryWidget::resizeGeometry()
{
    if(m_resizeWidgets.isEmpty() || !m_gridLayout)
    {
        return;
    }

    for(QWidget *widget : qAsConst(m_resizeWidgets))
    {
        widget->hide();
        m_gridLayout->removeWidget(widget);
    }

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize;
    int row = 0, col = 0, rowNumber = 0;

    if(m_module != RecommendModule::Album)
    {
        row = TTK_RN_MAX;
        col = lineNumber;
        rowNumber = lineNumber;
    }
    else
    {
        rowNumber = lineNumber / 2;
        if(rowNumber > m_networkRequest->pageSize() / 2)
        {
            rowNumber = m_networkRequest->pageSize() / 2;
        }

        row = rowNumber;
        col = rowNumber;
        rowNumber = 2 * rowNumber;
    }

    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        if(i < rowNumber)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i / row, i % col, Qt::AlignLeft | Qt::AlignTop);
            m_resizeWidgets[i]->show();
        }
    }
}


MusicItemMoreRecommendQueryWidget::MusicItemMoreRecommendQueryWidget(RecommendModule module, QWidget *parent)
    : QWidget(parent),
      m_categoryChanged(false),
      m_module(module),
      m_pageQueryWidget(nullptr),
      m_networkRequest(nullptr)
{
    setStyleSheet(TTK::UI::BackgroundStyle01);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(12, 0, 12, 0);
    setLayout(layout);

    m_mainWidget = new QWidget(this);

    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(m_mainWidget);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_mainWidget->setLayout(mainWidgetLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout->addWidget(scrollArea);

    initialize();
}

MusicItemMoreRecommendQueryWidget::~MusicItemMoreRecommendQueryWidget()
{
    delete m_areasGroup;
    delete m_gridLayout;
    delete m_pageQueryWidget;
    delete m_mainWidget;
    delete m_networkRequest;
}

void MusicItemMoreRecommendQueryWidget::resizeGeometry()
{
    if(m_resizeWidgets.isEmpty() || !m_gridLayout)
    {
        return;
    }

    for(QWidget *widget : qAsConst(m_resizeWidgets))
    {
        m_gridLayout->removeWidget(widget);
    }

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        m_gridLayout->addWidget(m_resizeWidgets[i], i / lineNumber, i % lineNumber, Qt::AlignLeft);
    }
}

void MusicItemMoreRecommendQueryWidget::buttonClicked(int index)
{
    removeItems(m_gridLayout);
    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}

void MusicItemMoreRecommendQueryWidget::categoryChanged(int index)
{
    switch(m_module)
    {
        case RecommendModule::Album:
        {
            QString key;
            switch(index)
            {
                case 0: key = "ALL"; break;
                case 1: key = "ZH"; break;
                case 2: key = "EA"; break;
                case 3: key = "KR"; break;
                case 4: key = "JP"; break;
                default: break;
            }

            if(!key.isEmpty())
            {
                removeItems(m_gridLayout);
                m_categoryChanged = true;
                m_networkRequest->startToSearch(key);
            }
            break;
        }
        case RecommendModule::Artist: break;
        case RecommendModule::Playlist: break;
        case RecommendModule::PlaylistHQ:
        {
            removeItems(m_gridLayout);
            m_categoryChanged = true;
            m_networkRequest->startToSearch(QString::number(index));
            break;
        }
        default: break;
    }
}

void MusicItemMoreRecommendQueryWidget::createResultItem(const MusicResultDataItem &item)
{
    if(!m_pageQueryWidget)
    {
        m_pageQueryWidget = new MusicPageQueryWidget(this);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        QVBoxLayout *mainLayout = TTKObjectCast(QVBoxLayout*, m_mainWidget->layout());
        mainLayout->addWidget(m_pageQueryWidget->createPageWidget(this, m_networkRequest->pageTotalSize()));
        mainLayout->addStretch(1);
    }

    if(m_categoryChanged && m_pageQueryWidget)
    {
        m_categoryChanged = false;
        m_pageQueryWidget->reset(m_networkRequest->pageTotalSize());
    }

    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentItemClicked(MusicResultDataItem)));
    label->setShowTime(false);
    label->setShowCount(false);
    label->setValueLabel(m_module != RecommendModule::Artist);
    label->setResultDataItem(item, new MusicCoverSourceRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);
    m_resizeWidgets.append(label);
}

void MusicItemMoreRecommendQueryWidget::currentItemClicked(const MusicResultDataItem &item)
{
    switch(m_module)
    {
        case RecommendModule::Album: MusicRightAreaWidget::instance()->showAlbumFound({}, item.m_id); break;
        case RecommendModule::Artist: MusicRightAreaWidget::instance()->showArtistFound({}, item.m_id); break;
        case RecommendModule::Playlist:
        case RecommendModule::PlaylistHQ: MusicRightAreaWidget::instance()->showPlaylistFound(item.m_id); break;
        default: break;
    }
}

void MusicItemMoreRecommendQueryWidget::initialize()
{
    QWidget *topWidget = new QWidget(m_mainWidget);
    topWidget->setFixedHeight(65);
    m_mainWidget->layout()->addWidget(topWidget);

    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(0, 20, 12, 10);
    topWidget->setLayout(topWidgetLayout);

    QWidget *frame = new QWidget(topWidget);
    frame->setFixedSize(3, 36);
    frame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topWidgetLayout->addWidget(frame);

    QLabel *label = new QLabel(topWidget);
    label->setStyleSheet(TTK::UI::FontStyle06);
    topWidgetLayout->addWidget(label);
    topWidgetLayout->addStretch(1);

    TTKClickedLabel *backButton = new TTKClickedLabel("<<", topWidget);
    backButton->setAlignment(Qt::AlignBottom);
    backButton->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle01 + TTK::UI::FontStyle03);
    topWidgetLayout->addWidget(backButton, 0, Qt::AlignBottom);
    connect(backButton, SIGNAL(clicked()), parent(), SLOT(moreItemChangedToArea()));

    QWidget *container = new QWidget(m_mainWidget);
    m_mainWidget->layout()->addWidget(container);

    m_gridLayout = new QGridLayout(container);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_gridLayout->setVerticalSpacing(5);
    container->setLayout(m_gridLayout);

    m_areasGroup = new TTKClickedGroup(this);
    connect(m_areasGroup, SIGNAL(clicked(int)), SLOT(categoryChanged(int)));

    switch(m_module)
    {
        case RecommendModule::Album:
        {
            label->setText(tr("New Albums"));

            QWidget *spacing = new QWidget(topWidget);
            spacing->setFixedWidth(10);
            topWidgetLayout->insertWidget(2, spacing);

            TTKClickedLabel *allButton = new TTKClickedLabel(tr("All"), topWidget);
            allButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(allButton, 0);
            topWidgetLayout->insertWidget(3, allButton);

            QWidget *frame1 = new QWidget(topWidget);
            frame1->setFixedSize(2, topWidget->height() / 4);
            frame1->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(4, frame1, 0, Qt::AlignBottom);

            TTKClickedLabel *zhButton = new TTKClickedLabel(tr("ZH"), topWidget);
            zhButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(zhButton, 1);
            topWidgetLayout->insertWidget(5, zhButton);

            QWidget *frame2 = new QWidget(topWidget);
            frame2->setFixedSize(2, topWidget->height() / 4);
            frame2->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(6, frame2, 0, Qt::AlignBottom);

            TTKClickedLabel *eaButton = new TTKClickedLabel(tr("EA"), topWidget);
            eaButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(eaButton, 2);
            topWidgetLayout->insertWidget(7, eaButton);

            QWidget *frame3 = new QWidget(topWidget);
            frame3->setFixedSize(2, topWidget->height() / 4);
            frame3->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(8, frame3, 0, Qt::AlignBottom);

            TTKClickedLabel *krButton = new TTKClickedLabel(tr("KR"), topWidget);
            krButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(krButton, 3);
            topWidgetLayout->insertWidget(9, krButton);

            QWidget *frame4 = new QWidget(topWidget);
            frame4->setFixedSize(2, topWidget->height() / 4);
            frame4->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(10, frame4, 0, Qt::AlignBottom);

            TTKClickedLabel *jpButton = new TTKClickedLabel(tr("JP"), topWidget);
            jpButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(jpButton, 4);
            topWidgetLayout->insertWidget(11, jpButton);

            m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeNewAlbumRequest(this);
            connect(m_networkRequest, SIGNAL(createAlbumItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem)));
            m_networkRequest->startToSearch("ALL");
            break;
        }
        case RecommendModule::Artist:
        {
            label->setText(tr("Popular Artists"));

            m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeHotArtistListRequest(this);
            connect(m_networkRequest, SIGNAL(createArtistListItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem)));
            m_networkRequest->startToSearch({});
            break;
        }
        case RecommendModule::Playlist:
        {
            label->setText(tr("Recommended Playlist"));

            m_networkRequest = G_DOWNLOAD_QUERY_PTR->makePlaylistRecommendRequest(this);
            connect(m_networkRequest, SIGNAL(createPlaylistItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem)));
            m_networkRequest->startToSearch({});
            break;
        }
        case RecommendModule::PlaylistHQ:
        {
            label->setText(tr("Premium Playlist"));

            QWidget *spacing = new QWidget(topWidget);
            spacing->setFixedWidth(10);
            topWidgetLayout->insertWidget(2, spacing);

            TTKClickedLabel *allButton = new TTKClickedLabel(tr("All"), topWidget);
            allButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(allButton, 0);
            topWidgetLayout->insertWidget(3, allButton);

            QWidget *frame1 = new QWidget(topWidget);
            frame1->setFixedSize(2, topWidget->height() / 4);
            frame1->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(4, frame1, 0, Qt::AlignBottom);

            TTKClickedLabel *zhButton = new TTKClickedLabel(tr("ZH"), topWidget);
            zhButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(zhButton, 1);
            topWidgetLayout->insertWidget(5, zhButton);

            QWidget *frame2 = new QWidget(topWidget);
            frame2->setFixedSize(2, topWidget->height() / 4);
            frame2->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(6, frame2, 0, Qt::AlignBottom);

            TTKClickedLabel *eaButton = new TTKClickedLabel(tr("EA"), topWidget);
            eaButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(eaButton, 2);
            topWidgetLayout->insertWidget(7, eaButton);

            QWidget *frame3 = new QWidget(topWidget);
            frame3->setFixedSize(2, topWidget->height() / 4);
            frame3->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(8, frame3, 0, Qt::AlignBottom);

            TTKClickedLabel *krButton = new TTKClickedLabel(tr("KR"), topWidget);
            krButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(krButton, 3);
            topWidgetLayout->insertWidget(9, krButton);

            QWidget *frame4 = new QWidget(topWidget);
            frame4->setFixedSize(2, topWidget->height() / 4);
            frame4->setStyleSheet(TTK::UI::BackgroundStyle03);
            topWidgetLayout->insertWidget(10, frame4, 0, Qt::AlignBottom);

            TTKClickedLabel *jpButton = new TTKClickedLabel(tr("JP"), topWidget);
            jpButton->setAlignment(Qt::AlignBottom);
            m_areasGroup->addWidget(jpButton, 4);
            topWidgetLayout->insertWidget(11, jpButton);

            m_networkRequest = G_DOWNLOAD_QUERY_PTR->makePlaylistHighRequest(this);
            connect(m_networkRequest, SIGNAL(createPlaylistItem(MusicResultDataItem)), SLOT(createResultItem(MusicResultDataItem)));
            m_networkRequest->startToSearch("全部");
            break;
        }
        default: break;
    }
}

void MusicItemMoreRecommendQueryWidget::removeItems(QLayout *layout)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast();
        if(layout)
        {
            layout->removeWidget(w);
        }
        delete w;
    }
}


MusicMainRecommendWidget::MusicMainRecommendWidget(QWidget *parent)
    : QWidget(parent),
      m_itemMoreWidget(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_mainWidget = new QStackedWidget(this);
    m_mainWidget->setObjectName("MainWidget");
    m_mainWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_mainWidget->objectName(), TTK::UI::BackgroundStyle10));
    layout->addWidget(m_mainWidget);

    createHomeWidget();
}

MusicMainRecommendWidget::~MusicMainRecommendWidget()
{
    delete m_newSongsWidget;
    delete m_newAlbumsWidget;
    delete m_artistsWidget;
    delete m_hqPlaylistWidget;
    delete m_playlistWidget;
    delete m_areasGroup;
    delete m_homeContainer;
    delete m_homeWidget;
    delete m_itemMoreWidget;
    delete m_mainWidget;
}

void MusicMainRecommendWidget::resizeGeometry()
{
    m_newSongsWidget->resizeGeometry();
    m_newAlbumsWidget->resizeGeometry();
    m_artistsWidget->resizeGeometry();
    m_hqPlaylistWidget->resizeGeometry();
    m_playlistWidget->resizeGeometry();

    if(m_itemMoreWidget)
    {
        m_itemMoreWidget->resizeGeometry();
    }
}

void MusicMainRecommendWidget::areaItemChangedToMore(int id)
{
    delete m_itemMoreWidget;
    m_itemMoreWidget = new MusicItemMoreRecommendQueryWidget(TTKStaticCast(RecommendModule, id), this);
    m_mainWidget->addWidget(m_itemMoreWidget);

    m_homeWidget->hide();
    m_mainWidget->setCurrentWidget(m_itemMoreWidget);
}

void MusicMainRecommendWidget::moreItemChangedToArea()
{
    m_homeWidget->show();
    m_itemMoreWidget->hide();
    m_mainWidget->setCurrentWidget(m_homeWidget);
}

void MusicMainRecommendWidget::createHomeWidget()
{
    m_homeWidget = new QWidget(m_mainWidget);
    m_homeWidget->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_mainWidget->addWidget(m_homeWidget);

    QVBoxLayout *layout = new QVBoxLayout(m_homeWidget);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(6);
    m_homeWidget->setLayout(layout);

    m_areasGroup = new TTKClickedGroup(m_homeWidget);
    connect(m_areasGroup, SIGNAL(clicked(int)), SLOT(areaItemChangedToMore(int)));

    createTopWidget();
    createContainerWidget();
}

void MusicMainRecommendWidget::createTopWidget()
{
    QWidget *widget = new QWidget(m_homeWidget);
    widget->setFixedHeight(40);
    widget->setStyleSheet(TTK::UI::PushButtonStyle01 + TTK::UI::PushButtonStyle13);
    m_homeWidget->layout()->addWidget(widget);

    QHBoxLayout *topLayout = new QHBoxLayout(widget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(25);
    topLayout->addStretch(1);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    TTKClickedLabel *discovery = new TTKClickedLabel(tr("Discovery"), widget);
    discovery->setStyleSheet(TTK::UI::FontStyle03);
    clickedGroup->addWidget(discovery, MusicRightAreaWidget::MainRecommendWidget);
    topLayout->addWidget(discovery);

    TTKClickedLabel *recommend = new TTKClickedLabel(tr("Recommend"), widget);
    recommend->setStyleSheet(TTK::UI::FontStyle03);
    clickedGroup->addWidget(recommend, MusicRightAreaWidget::SongRecommendWidget);
    topLayout->addWidget(recommend);

    TTKClickedLabel *toplist = new TTKClickedLabel(tr("Toplist"), widget);
    toplist->setStyleSheet(TTK::UI::FontStyle03);
    clickedGroup->addWidget(toplist, MusicRightAreaWidget::ToplistWidget);
    topLayout->addWidget(toplist);

    TTKClickedLabel *artists = new TTKClickedLabel(tr("Artists"), widget);
    artists->setStyleSheet(TTK::UI::FontStyle03);
    clickedGroup->addWidget(artists, MusicRightAreaWidget::ArtistCategoryWidget);
    topLayout->addWidget(artists);

    TTKClickedLabel *category = new TTKClickedLabel(tr("Category"), widget);
    category->setStyleSheet(TTK::UI::FontStyle03);
    clickedGroup->addWidget(category, MusicRightAreaWidget::PlaylistCategoryWidget);
    topLayout->addWidget(category);

    topLayout->addStretch(1);
    connect(clickedGroup, SIGNAL(clicked(int)), MusicRightAreaWidget::instance(), SLOT(functionClicked(int)));
}

void MusicMainRecommendWidget::createContainerWidget()
{
    m_homeContainer = new QWidget(m_homeWidget);

    QVBoxLayout *layout = new QVBoxLayout(m_homeContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    m_homeContainer->setLayout(layout);

    QScrollArea *scrollArea = new QScrollArea(m_homeWidget);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_homeContainer);
    m_homeWidget->layout()->addWidget(scrollArea);

    createContainerTopWidget();
    createContainerMiddleTopWidget();
    createContainerMiddleWidget();
    createContainerMiddleBottomWidget();

    layout->addStretch(1);
}

void MusicMainRecommendWidget::createContainerTopWidget()
{
    QWidget *widget = new QWidget(m_homeWidget);
    widget->setFixedHeight(430);
    m_homeContainer->layout()->addWidget(widget);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    widget->setLayout(layout);

    QWidget *leftWidget = new QWidget(widget);
    layout->addWidget(leftWidget, 1);

    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftWidget->setLayout(leftLayout);

    QWidget *leftTopWidget = new QWidget(leftWidget);
    leftTopWidget->setFixedHeight(30);
    leftLayout->addWidget(leftTopWidget);

    QHBoxLayout *leftTopLayout = new QHBoxLayout(leftTopWidget);
    leftTopLayout->setContentsMargins(0, 0, 0, 0);
    leftTopWidget->setLayout(leftTopLayout);

    QWidget *leftTopFrame = new QWidget(leftTopWidget);
    leftTopFrame->setFixedWidth(3);
    leftTopFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    leftTopLayout->addWidget(leftTopFrame);

    QLabel *leftTopLabel = new QLabel(tr("New Songs"), leftTopWidget);
    leftTopLabel->setStyleSheet(TTK::UI::FontStyle04);
    leftTopLayout->addWidget(leftTopLabel);
    leftTopLayout->addStretch(1);

    QWidget *leftMiddle = new QWidget(leftWidget);
    leftMiddle->setFixedHeight(1);
    leftMiddle->setStyleSheet(TTK::UI::BackgroundStyle03);
    leftLayout->addWidget(leftMiddle);

    m_newSongsWidget = new MusicNewSongRecommendQueryTableWidget(leftWidget);
    leftLayout->addWidget(m_newSongsWidget);
    m_newSongsWidget->setQueryInput(G_DOWNLOAD_QUERY_PTR->makeNewSongRequest(m_newSongsWidget));
    m_newSongsWidget->startToSearchByValue("0");

    QWidget *rightWidget = new QWidget(widget);
    layout->addWidget(rightWidget, 1);

    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightWidget->setLayout(rightLayout);

    QWidget *rightTopWidget = new QWidget(rightWidget);
    rightTopWidget->setFixedHeight(30);
    rightLayout->addWidget(rightTopWidget);

    QHBoxLayout *rightTopLayout = new QHBoxLayout(rightTopWidget);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);
    rightTopWidget->setLayout(rightTopLayout);

    QWidget *rightTopFrame = new QWidget(rightTopWidget);
    rightTopFrame->setFixedWidth(3);
    rightTopFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    rightTopLayout->addWidget(rightTopFrame);

    TTKClickedLabel *rightTopLabel = new TTKClickedLabel(tr("New Albums"), rightTopWidget);
    rightTopLabel->setStyleSheet(TTK::UI::FontStyle04);
    rightTopLayout->addWidget(rightTopLabel);
    rightTopLayout->addStretch(1);
    m_areasGroup->addWidget(rightTopLabel, RecommendModule::Album);

    m_newAlbumsWidget = new MusicItemRecommendQueryWidget(RecommendModule::Album, rightWidget);
    rightLayout->addWidget(m_newAlbumsWidget);

    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makeNewAlbumRequest(m_newAlbumsWidget);
    m_newAlbumsWidget->setQueryInput(req);
    req->setPageSize(8);
    req->startToSearch("ALL");
}

void MusicMainRecommendWidget::createContainerMiddleTopWidget()
{
    QWidget *widget = new QWidget(m_homeWidget);
    widget->setFixedHeight(260);
    m_homeContainer->layout()->addWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);

    QWidget *topWidget = new QWidget(widget);
    topWidget->setFixedHeight(30);
    layout->addWidget(topWidget);

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *topFrame = new QWidget(topWidget);
    topFrame->setFixedSize(3, 30);
    topFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topLayout->addWidget(topFrame, 0, Qt::AlignVCenter);

    TTKClickedLabel *topLabel = new TTKClickedLabel(tr("Popular Artists"), topWidget);
    topLabel->setStyleSheet(TTK::UI::FontStyle04);
    topLayout->addWidget(topLabel);
    topLayout->addStretch(1);
    m_areasGroup->addWidget(topLabel, RecommendModule::Artist);

    m_artistsWidget = new MusicItemRecommendQueryWidget(RecommendModule::Artist, widget);
    layout->addWidget(m_artistsWidget);

    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makeHotArtistListRequest(m_artistsWidget);
    m_artistsWidget->setQueryInput(req);
    req->setPageSize(8);
    req->startToSearch({});
}

void MusicMainRecommendWidget::createContainerMiddleWidget()
{
    QWidget *widget = new QWidget(m_homeWidget);
    widget->setFixedHeight(260);
    m_homeContainer->layout()->addWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);

    QWidget *topWidget = new QWidget(widget);
    topWidget->setFixedHeight(30);
    layout->addWidget(topWidget);

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *topFrame = new QWidget(topWidget);
    topFrame->setFixedSize(3, 30);
    topFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topLayout->addWidget(topFrame, 0, Qt::AlignVCenter);

    TTKClickedLabel *topLabel = new TTKClickedLabel(tr("Premium Playlist"), topWidget);
    topLabel->setStyleSheet(TTK::UI::FontStyle04);
    topLayout->addWidget(topLabel);
    topLayout->addStretch(1);
    m_areasGroup->addWidget(topLabel, RecommendModule::PlaylistHQ);

    m_hqPlaylistWidget = new MusicItemRecommendQueryWidget(RecommendModule::PlaylistHQ, widget);
    layout->addWidget(m_hqPlaylistWidget);

    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makePlaylistHighRequest(m_hqPlaylistWidget);
    m_hqPlaylistWidget->setQueryInput(req);
    req->setPageSize(8);
    req->startToSearch("0");
}

void MusicMainRecommendWidget::createContainerMiddleBottomWidget()
{
    QWidget *widget = new QWidget(m_homeWidget);
    widget->setFixedHeight(260);
    m_homeContainer->layout()->addWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);

    QWidget *topWidget = new QWidget(widget);
    topWidget->setFixedHeight(30);
    layout->addWidget(topWidget);

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *topFrame = new QWidget(topWidget);
    topFrame->setFixedSize(3, 30);
    topFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topLayout->addWidget(topFrame, 0, Qt::AlignVCenter);

    TTKClickedLabel *topLabel = new TTKClickedLabel(tr("Recommended Playlist"), topWidget);
    topLabel->setStyleSheet(TTK::UI::FontStyle04);
    topLayout->addWidget(topLabel);
    topLayout->addStretch(1);
    m_areasGroup->addWidget(topLabel, RecommendModule::Playlist);

    m_playlistWidget = new MusicItemRecommendQueryWidget(RecommendModule::Playlist, widget);
    layout->addWidget(m_playlistWidget);

    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makePlaylistRecommendRequest(m_playlistWidget);
    m_playlistWidget->setQueryInput(req);
    req->setPageSize(8);
    req->startToSearch({});
}
