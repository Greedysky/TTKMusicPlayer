#include "musicmainrecommendwidget.h"
#include "musicmainrecommendrequest.h"
#include "musicrightareawidget.h"
#include "musicqueryitemwidget.h"
#include "musictoastlabel.h"
#include "musicdownloadwidget.h"
#include "musicsongscontainerwidget.h"

#include <QButtonGroup>

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

    TTK_INFO_STREAM(item.m_name);
//    MusicSongsContainerWidget::instance()->addSongBufferToPlaylist(item);
    return true;
}


MusicNewAlbumsRecommendQueryWidget::MusicNewAlbumsRecommendQueryWidget(QWidget *parent)
    : QLabel(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_gridLayout->setVerticalSpacing(5);
    setLayout(m_gridLayout);

    m_networkRequest = new MusicNewAlbumsRecommendRequest(this);
    connect(m_networkRequest, SIGNAL(createAlbumItem(MusicResultDataItem)), SLOT(createAlbumItem(MusicResultDataItem)));
    m_networkRequest->startToSearch("ALL");
}

MusicNewAlbumsRecommendQueryWidget::~MusicNewAlbumsRecommendQueryWidget()
{
    delete m_gridLayout;
    delete m_networkRequest;
}

void MusicNewAlbumsRecommendQueryWidget::resizeGeometry()
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
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize / 2;
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        if(i < 2 * lineNumber)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i / lineNumber, i % lineNumber, Qt::AlignLeft | Qt::AlignTop);
            m_resizeWidgets[i]->show();
        }
    }
}

void MusicNewAlbumsRecommendQueryWidget::createAlbumItem(const MusicResultDataItem &item)
{
    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    label->hide();
    label->setShowTime(false);
    label->setShowCount(false);
    label->setResultDataItem(item, new MusicCoverSourceRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize / 2;
    if(m_resizeWidgets.count() < 2 * lineNumber)
    {
        label->show();
        m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft | Qt::AlignTop);
    }

    m_resizeWidgets.append(label);
}


MusicPlaylistRecommendQueryWidget::MusicPlaylistRecommendQueryWidget(QWidget *parent)
    : QLabel(parent),
      m_networkRequest(nullptr)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_gridLayout->setVerticalSpacing(5);
    setLayout(m_gridLayout);
}

MusicPlaylistRecommendQueryWidget::~MusicPlaylistRecommendQueryWidget()
{
    delete m_gridLayout;
    delete m_networkRequest;
}

void MusicPlaylistRecommendQueryWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    m_networkRequest = query;
    connect(m_networkRequest, SIGNAL(createPlaylistItem(MusicResultDataItem)), SLOT(createPlaylistItem(MusicResultDataItem)));
    m_networkRequest->startToSearch("0");
}

void MusicPlaylistRecommendQueryWidget::createPlaylistItem(const MusicResultDataItem &item)
{
    MusicSquareQueryItemWidget *label = new MusicSquareQueryItemWidget(this);
    label->hide();
    label->setShowTime(false);
    label->setShowCount(false);
    label->setResultDataItem(item, new MusicCoverSourceRequest(this));

    const int lineSize = MusicSquareQueryItemWidget::LINE_SPACING_SIZE;
    const int lineNumber = (QUERY_WIDGET_WIDTH  - lineSize / 2) / lineSize;
    if(m_resizeWidgets.count() < lineNumber)
    {
        label->show();
        m_gridLayout->addWidget(label, 0, m_resizeWidgets.count(), Qt::AlignLeft | Qt::AlignTop);
    }

    m_resizeWidgets.append(label);
}

void MusicPlaylistRecommendQueryWidget::resizeGeometry()
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
    for(int i = 0; i < m_resizeWidgets.count(); ++i)
    {
        if(i < lineNumber)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], 0, i % lineNumber, Qt::AlignLeft | Qt::AlignTop);
            m_resizeWidgets[i]->show();
        }
    }
}


MusicMainRecommendWidget::MusicMainRecommendWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("MainWidget");
    m_mainWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_mainWidget->objectName(), TTK::UI::BackgroundStyle10));
    layout->addWidget(m_mainWidget);

    QVBoxLayout *containerLayout = new QVBoxLayout(m_mainWidget);
    containerLayout->setContentsMargins(12, 0, 12, 0);
    containerLayout->setSpacing(6);
    m_mainWidget->setLayout(containerLayout);

    createTopWidget();
    createContainerWidget();
}

MusicMainRecommendWidget::~MusicMainRecommendWidget()
{
    delete m_newSongsWidget;
    delete m_newAlbumsWidget;
    delete m_highPlaylistWidget;
    delete m_playlistWidget;
    delete m_container;
    delete m_mainWidget;
}

void MusicMainRecommendWidget::resizeGeometry()
{
    m_newSongsWidget->resizeGeometry();
    m_newAlbumsWidget->resizeGeometry();
    m_highPlaylistWidget->resizeGeometry();
    m_playlistWidget->resizeGeometry();
}

void MusicMainRecommendWidget::createTopWidget()
{
    QWidget *widget = new QWidget(m_mainWidget);
    widget->setFixedHeight(30);
    widget->setStyleSheet(TTK::UI::PushButtonStyle01 + TTK::UI::PushButtonStyle13);
    m_mainWidget->layout()->addWidget(widget);

    QHBoxLayout *topLayout = new QHBoxLayout(widget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(25);
    topLayout->addStretch(1);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QPushButton *discovery = new QPushButton(tr("Discovery"), widget);
    discovery->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(discovery, MusicRightAreaWidget::MainRecommendWidget);
    topLayout->addWidget(discovery);

    QPushButton *recommend = new QPushButton(tr("Recommend"), widget);
    recommend->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(recommend, MusicRightAreaWidget::SongRecommendWidget);
    topLayout->addWidget(recommend);

    QPushButton *toplist = new QPushButton(tr("Toplist"), widget);
    toplist->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(toplist, MusicRightAreaWidget::ToplistWidget);
    topLayout->addWidget(toplist);

    QPushButton *artists = new QPushButton(tr("Artists"), widget);
    artists->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(artists, MusicRightAreaWidget::ArtistCategoryWidget);
    topLayout->addWidget(artists);

    QPushButton *category = new QPushButton(tr("Category"), widget);
    category->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(category, MusicRightAreaWidget::PlaylistCategoryWidget);
    topLayout->addWidget(category);

    topLayout->addStretch(1);
    QtButtonGroupConnect(buttonGroup, MusicRightAreaWidget::instance(), functionClicked, TTK_SLOT);

#ifdef Q_OS_UNIX
    discovery->setFocusPolicy(Qt::NoFocus);
    recommend->setFocusPolicy(Qt::NoFocus);
    toplist->setFocusPolicy(Qt::NoFocus);
    artists->setFocusPolicy(Qt::NoFocus);
    category->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicMainRecommendWidget::createContainerWidget()
{
    m_container = new QWidget(m_mainWidget);
    m_container->setFixedHeight(1100);

    QVBoxLayout *layout = new QVBoxLayout(m_container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    m_container->setLayout(layout);

    QScrollArea *scrollArea = new QScrollArea(m_mainWidget);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_container);
    m_mainWidget->layout()->addWidget(scrollArea);

    scrollArea->setObjectName("ScrollArea");
    scrollArea->setStyleSheet(QString("#%1{ %2 }").arg(scrollArea->objectName(), TTK::UI::BackgroundStyle10));
    m_container->setObjectName("Ccontainer");
    m_container->setStyleSheet(QString("#%1{ %2 }").arg(m_container->objectName(), TTK::UI::BackgroundStyle10));

    createContainerTopWidget();
    createContainerMiddleTopWidget();
    createContainerMiddleWidget();
//    createContainerMiddleBottomWidget();

    layout->addStretch(1);
}

void MusicMainRecommendWidget::createContainerTopWidget()
{
    QWidget *widget = new QWidget(m_mainWidget);
    widget->setFixedHeight(430);
    m_container->layout()->addWidget(widget);

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
    leftTopWidget->setFixedHeight(20);
    leftLayout->addWidget(leftTopWidget);

    QHBoxLayout *leftTopLayout = new QHBoxLayout(leftTopWidget);
    leftTopLayout->setContentsMargins(0, 0, 0, 0);
    leftTopWidget->setLayout(leftTopLayout);

    QWidget *leftTopFrame = new QWidget(leftTopWidget);
    leftTopFrame->setFixedWidth(3);
    leftTopFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    leftTopLayout->addWidget(leftTopFrame);

    QLabel *leftTopText = new QLabel("新歌首发", leftTopWidget);
    leftTopText->setStyleSheet(TTK::UI::FontStyle04);
    leftTopLayout->addWidget(leftTopText);
    leftTopLayout->addStretch(1);

//    QPushButton *leftTopBtton = new QPushButton("更多", leftTopWidget);
//    leftTopBtton->setStyleSheet(TTK::UI::PushButtonStyle02);
//    leftTopLayout->addWidget(leftTopBtton);

    QWidget *leftMiddle = new QWidget(leftWidget);
    leftMiddle->setFixedHeight(1);
    leftMiddle->setStyleSheet(TTK::UI::BackgroundStyle03);
    leftLayout->addWidget(leftMiddle);

    m_newSongsWidget = new MusicNewSongRecommendQueryTableWidget(leftWidget);
    leftLayout->addWidget(m_newSongsWidget);
    m_newSongsWidget->setQueryInput(new MusicNewSongsRecommendRequest(m_newSongsWidget));
    m_newSongsWidget->startToSearchByValue("0");

    QWidget *rightWidget = new QWidget(widget);
    layout->addWidget(rightWidget, 1);

    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightWidget->setLayout(rightLayout);

    QWidget *rightTopWidget = new QWidget(rightWidget);
    rightTopWidget->setFixedHeight(20);
    rightLayout->addWidget(rightTopWidget);

    QHBoxLayout *rightTopLayout = new QHBoxLayout(rightTopWidget);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);
    rightTopWidget->setLayout(rightTopLayout);

    QWidget *rightTopFrame = new QWidget(rightTopWidget);
    rightTopFrame->setFixedWidth(3);
    rightTopFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    rightTopLayout->addWidget(rightTopFrame);

    QLabel *rightTopText = new QLabel("新碟上架", rightTopWidget);
    rightTopText->setStyleSheet(TTK::UI::FontStyle04);
    rightTopLayout->addWidget(rightTopText);
    rightTopLayout->addStretch(1);

//    QPushButton *rightTopBtton = new QPushButton("更多", rightTopWidget);
//    rightTopBtton->setStyleSheet(TTK::UI::PushButtonStyle02);
//    rightTopLayout->addWidget(rightTopBtton);

    m_newAlbumsWidget = new MusicNewAlbumsRecommendQueryWidget(rightWidget);
    rightLayout->addWidget(m_newAlbumsWidget);
}

void MusicMainRecommendWidget::createContainerMiddleTopWidget()
{
    QWidget *widget = new QWidget(m_mainWidget);
    widget->setFixedHeight(260);
    m_container->layout()->addWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);

    QWidget *topWidget = new QWidget(widget);
    topWidget->setFixedHeight(35);
    layout->addWidget(topWidget);

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *topFrame = new QWidget(topWidget);
    topFrame->setFixedSize(3, 30);
    topFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topLayout->addWidget(topFrame, 0, Qt::AlignVCenter);

    QLabel *topText = new QLabel("精选歌单", topWidget);
    topText->setStyleSheet(TTK::UI::FontStyle04);
    topLayout->addWidget(topText);

    m_highPlaylistWidget = new MusicPlaylistRecommendQueryWidget(widget);
    m_highPlaylistWidget->setQueryInput(new MusicPlaylistHighqualityRecommendRequest(this));
    layout->addWidget(m_highPlaylistWidget);
}

void MusicMainRecommendWidget::createContainerMiddleWidget()
{
    QWidget *widget = new QWidget(m_mainWidget);
    widget->setFixedHeight(260);
    m_container->layout()->addWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);

    QWidget *topWidget = new QWidget(widget);
    topWidget->setFixedHeight(35);
    layout->addWidget(topWidget);

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *topFrame = new QWidget(topWidget);
    topFrame->setFixedSize(3, 30);
    topFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topLayout->addWidget(topFrame, 0, Qt::AlignVCenter);

    QLabel *topText = new QLabel("推荐歌单", topWidget);
    topText->setStyleSheet(TTK::UI::FontStyle04);
    topLayout->addWidget(topText);

    m_playlistWidget = new MusicPlaylistRecommendQueryWidget(widget);
    m_playlistWidget->setQueryInput(new MusicPlaylistRecommendRequest(this));
    layout->addWidget(m_playlistWidget);
}

void MusicMainRecommendWidget::createContainerMiddleBottomWidget()
{
    QWidget *widget = new QWidget(m_mainWidget);
    widget->setFixedHeight(300);
    m_container->layout()->addWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setLayout(layout);

    QWidget *topWidget = new QWidget(widget);
    topWidget->setFixedHeight(35);
    layout->addWidget(topWidget);

    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topWidget->setLayout(topLayout);

    QWidget *topFrame = new QWidget(topWidget);
    topFrame->setFixedSize(3, 30);
    topFrame->setStyleSheet(TTK::UI::BackgroundStyle13);
    topLayout->addWidget(topFrame, 0, Qt::AlignVCenter);

    QLabel *topText = new QLabel("推荐MV", topWidget);
    topText->setStyleSheet(TTK::UI::FontStyle04);
    topLayout->addWidget(topText);

    QWidget *middle = new QWidget(widget);
    middle->setFixedHeight(1);
    middle->setStyleSheet(TTK::UI::BackgroundStyle03);
    layout->addWidget(middle);

    QWidget *bottom = new QWidget(widget);
    bottom->setStyleSheet("background:rgb(12,65,88)");
    layout->addWidget(bottom);
}
