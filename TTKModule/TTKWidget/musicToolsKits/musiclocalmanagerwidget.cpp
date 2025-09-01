#include "musiclocalmanagerwidget.h"
#include "musicsongscontainerwidget.h"
#include "musicitemsearchedit.h"
#include "musicgiflabelwidget.h"
#include "musicsongmeta.h"
#include "musicfileutils.h"
#include "musicformats.h"
#include "ttktabbutton.h"

#include <QTimer>
#include <QButtonGroup>
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QStandardPaths>
#else
#  include <QDesktopServices>
#endif

namespace TTK
{
    /*!
     * Generate song artist.
    */
    inline static QString generateArtist(const QString &v) noexcept
    {
        return v.isEmpty() ? QObject::tr("Various Artist") : v;
    }
    /*!
     * Generate song album.
    */
    inline static QString generateAlbum(const QString &v) noexcept
    {
        return v.isEmpty() ? QObject::tr("Various Album") : v;
    }
    /*!
     * Generate song year.
    */
    inline static QString generateYear(const QString &v) noexcept
    {
        return v.isEmpty() ? QObject::tr("Various Year") : v;
    }
    /*!
     * Generate song genre.
    */
    inline static QString generateGenre(const QString &v) noexcept
    {
        return v.isEmpty() ? QObject::tr("Various Genre") : v;
    }
}


MusicLocalManagerStatisticTableWidget::MusicLocalManagerStatisticTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->setVisible(true);
    headerView->resizeSection(0, 100);
    headerView->resizeSection(1, 55);

    setAlternatingRowColors(true);
    setFrameShape(QFrame::Box);
    setTextElideMode(Qt::ElideRight);
    setWordWrap(false);
    hide();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    horizontalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle04);
}

MusicLocalManagerStatisticTableWidget::~MusicLocalManagerStatisticTableWidget()
{
    removeItems();
}

void MusicLocalManagerStatisticTableWidget::addCellItem(const MusicSongStatisticItem &song)
{
    setRowCount(song.count());

    int index = 0;
    for(auto it = song.constBegin(); it != song.constEnd(); ++it, ++index)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(it.key());
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(index, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(QString::number(it.value()));
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(index, 1, item);
    }
}

void MusicLocalManagerStatisticTableWidget::resizeSection() const
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 100 + (width - WINDOW_WIDTH_MIN) / 2.0 / 3.0);
    headerView->resizeSection(1, 55 + (width - WINDOW_WIDTH_MIN) / 2.0 / 3.0);
}



MusicLocalManagerSongsTableWidget::MusicLocalManagerSongsTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(7);

    QHeaderView *headerView = horizontalHeader();
    headerView->setVisible(true);
    headerView->resizeSection(0, 200);
    headerView->resizeSection(1, 100);
    headerView->resizeSection(2, 100);
    headerView->resizeSection(3, 40);
    headerView->resizeSection(4, 100);
    headerView->resizeSection(5, 100);
    headerView->resizeSection(6, 200);

    setAlternatingRowColors(true);
    setFrameShape(QFrame::Box);
    setTextElideMode(Qt::ElideRight);
    setWordWrap(false);

    setHorizontalHeaderLabels({tr("Title"), tr("Artist"), tr("Album"), tr("Track"), tr("Year"), tr("Genre"), tr("Path")});
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    horizontalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle04);

    m_songs = new MusicSongList;
}

MusicLocalManagerSongsTableWidget::~MusicLocalManagerSongsTableWidget()
{
    removeItems();
    delete m_songs;
}

void MusicLocalManagerSongsTableWidget::addCellItems(const MusicSongInfoItemList &songs)
{
    setRowCount(songs.count());

    for(int i = 0; i < songs.count(); ++i)
    {
        const MusicSongInfoItem &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(v.m_title);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(v.m_artist);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_album);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_track);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 3, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_year);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_genre);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_path);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 6, item);

        m_songs->append(MusicSong(v.m_path));
    }
}

void MusicLocalManagerSongsTableWidget::resizeSection() const
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 200 + (width - WINDOW_WIDTH_MIN) / 4.0);
    headerView->resizeSection(1, 100 + (width - WINDOW_WIDTH_MIN) / 4.0);
    headerView->resizeSection(2, 100 + (width - WINDOW_WIDTH_MIN) / 4.0);
    headerView->resizeSection(6, 200 + (width - WINDOW_WIDTH_MIN) / 4.0);
}

void MusicLocalManagerSongsTableWidget::removeItems()
{
    MusicAbstractSongsListTableWidget::removeItems();
    m_songs->clear();
}

void MusicLocalManagerSongsTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractSongsListTableWidget::contextMenuEvent(event);

    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(playClicked()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = !(m_songs->isEmpty() || TTK::String::isNetworkUrl(currentSongPath()));
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(openFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(showAlbumQueryWidget()));
    menu.addSeparator();
    menu.exec(QCursor::pos());
}


MusicLocalManagerWidget::MusicLocalManagerWidget(QWidget *parent)
    : QWidget(parent),
      MusicItemSearchInterfaceClass()
{
    setStyleSheet(TTK::UI::BackgroundStyle10 + TTK::UI::ColorStyle02);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);

    QWidget *topWidget = new QWidget(mainWidget);
    mainLayout->addWidget(topWidget);

    QVBoxLayout *topLayout = new QVBoxLayout(topWidget);
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(0, 0, 0, 10);
    topWidget->setLayout(topLayout);

    QWidget *functionWidget = new QWidget(topWidget);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(0, 0, 0, 10);
    functionWidget->setLayout(functionWidgetLayout);

    QLabel *pLabel = new QLabel(tr("Media Library"), functionWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(33);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(TTK::UI::ColorStyle01);
    functionWidgetLayout->addWidget(pLabel);

    m_sizeLabel = new QLabel(functionWidget);
    m_sizeLabel->setStyleSheet(TTK::UI::ColorStyle01);
    functionWidgetLayout->addWidget(m_sizeLabel);
    functionWidgetLayout->addStretch(1);

    QPushButton *refresh = new QPushButton(tr("Refresh"), functionWidget);
    refresh->setIcon(QIcon(":/functions/btn_similar_hover"));
    refresh->setFixedSize(90, 30);
    refresh->setStyleSheet(TTK::UI::PushButtonStyle03);
    refresh->setCursor(QCursor(Qt::PointingHandCursor));
    functionWidgetLayout->addWidget(refresh);

    QPushButton *button = new QPushButton(tr("Settings"), functionWidget);
    button->setIcon(QIcon(":/functions/btn_setting_hover"));
    button->setFixedSize(90, 30);
    button->setStyleSheet(TTK::UI::PushButtonStyle03);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    functionWidgetLayout->addWidget(button);

    m_searchEdit = new MusicItemQueryEdit(topWidget);
    m_searchEdit->setFixedHeight(33);
    topLayout->addWidget(functionWidget);
    topLayout->addWidget(m_searchEdit);

    QWidget *tabWidget = new QWidget(mainWidget);
    QHBoxLayout *tabWidgetLayout = new QHBoxLayout(tabWidget);
    tabWidgetLayout->setSpacing(0);
    tabWidgetLayout->setContentsMargins(0, 0, 0, 0);
    tabWidget->setLayout(tabWidgetLayout);
    mainLayout->addWidget(tabWidget);

    m_tabButton = new TTKTabButton(tabWidget);
    m_tabButton->addButtons({tr("Song"), tr("Artist"), tr("Album"), tr("Year"), tr("Genre")});
    tabWidgetLayout->addWidget(m_tabButton);
    connect(m_tabButton, SIGNAL(clicked(int)), SLOT(typeIndexChanged(int)));

#ifdef Q_OS_UNIX
    refresh->setFocusPolicy(Qt::NoFocus);
    button->setFocusPolicy(Qt::NoFocus);
#endif

    QWidget *centerWidget = new QWidget(mainWidget);
    QHBoxLayout *centerWidgetLayout = new QHBoxLayout(centerWidget);
    centerWidgetLayout->setSpacing(2);
    centerWidgetLayout->setContentsMargins(0, 5, 0, 0);
    centerWidget->setLayout(centerWidgetLayout);
    mainLayout->addWidget(centerWidget);

    m_statisticTableWidget = new MusicLocalManagerStatisticTableWidget(centerWidget);
    centerWidgetLayout->addWidget(m_statisticTableWidget, 1);

    m_songTableWidget = new MusicLocalManagerSongsTableWidget(centerWidget);
    centerWidgetLayout->addWidget(m_songTableWidget, 3);

    m_loadingLabel = new MusicGifLabelValueWidget(this);
    m_loadingLabel->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_searchEdit->editor()->setPlaceholderText(tr("Please input search song words"));

    TTK_SIGNLE_SHOT(refreshItems, TTK_SLOT);
    connect(refresh, SIGNAL(clicked()), SLOT(refreshItems()));
    connect(button, SIGNAL(clicked()), SLOT(updateMediaLibraryPath()));
    connect(m_songTableWidget, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
    connect(m_searchEdit->editor(), SIGNAL(cursorPositionChanged(int,int)), SLOT(searchResultChanged(int,int)));
}

MusicLocalManagerWidget::~MusicLocalManagerWidget()
{
    delete m_sizeLabel;
    delete m_searchEdit;
    delete m_loadingLabel;
    delete m_songTableWidget;
    delete m_statisticTableWidget;
}

void MusicLocalManagerWidget::resizeWidget()
{
    m_songTableWidget->resizeSection();
    m_statisticTableWidget->resizeSection();
    m_loadingLabel->move((width() - m_loadingLabel->width()) / 2, (height() + 120 - m_loadingLabel->height()) / 2);
}

void MusicLocalManagerWidget::typeIndexChanged(int index)
{
    switch(index)
    {
        case 0: m_searchEdit->editor()->setPlaceholderText(tr("Please input search song words")); break;
        case 1: m_searchEdit->editor()->setPlaceholderText(tr("Please input search artist words")); break;
        case 2: m_searchEdit->editor()->setPlaceholderText(tr("Please input search album words")); break;
        case 3: m_searchEdit->editor()->setPlaceholderText(tr("Please input search year words")); break;
        case 4: m_searchEdit->editor()->setPlaceholderText(tr("Please input search genre words")); break;
        default: break;
    }

    clearSearchResult();
    m_searchEdit->editor()->clear();
    m_statisticTableWidget->setVisible(index != 0);
    updateStatisticWidget(index, m_containerItems);
}

void MusicLocalManagerWidget::refreshItems()
{
    if(m_loadingLabel->isRunning())
    {
        TTK::Core::enableBreakPoint(true);
        TTK_SIGNLE_SHOT(refreshItems, TTK_SLOT);
        return;
    }

    m_containerItems.clear();
    m_sizeLabel->clear();
    m_songTableWidget->removeItems();
    m_searchEdit->editor()->clear();
    TTK::Core::enableBreakPoint(false);

    QString path = G_SETTING_PTR->value(MusicSettingManager::MediaLibraryPath).toString();
    if(path.isEmpty())
    {
#if TTK_QT_VERSION_CHECK(5,0,0)
        path = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + TTK_SEPARATOR;
#else
        path = QDesktopServices::storageLocation(QDesktopServices::MusicLocation) + TTK_SEPARATOR;
#endif
        G_SETTING_PTR->setValue(MusicSettingManager::MediaLibraryPath, path);
    }

    m_loadingLabel->execute(true);
    const QStringList &files = TTK::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats());
    m_sizeLabel->setText(tr("   (Songs Totol: %1)").arg(files.size()));

    int count = 0;
    MusicSongMeta meta;
    for(const QString &path : qAsConst(files))
    {
        if(TTK::Core::isBreakPointEnabled())
        {
            break;
        }

        if(!meta.read(path))
        {
            continue;
        }

        MusicSongInfoItem info;
        info.m_title = TTK::generateSongName(meta.title(), meta.artist());
        info.m_artist = TTK::generateArtist(meta.artist());
        info.m_album = TTK::generateAlbum(meta.album());
        info.m_track = meta.trackNum();
        info.m_year = TTK::generateYear(meta.year());
        info.m_genre = TTK::generateGenre(meta.genre());
        info.m_path = path;
        m_containerItems << info;

        qApp->processEvents();
        m_loadingLabel->setValue(++count * 100.0f / files.size());
    }

    m_songTableWidget->addCellItems(m_containerItems);
    updateStatisticWidget(m_tabButton->currentIndex(), m_containerItems);
    m_loadingLabel->execute(false);
}

void MusicLocalManagerWidget::updateMediaLibraryPath()
{
    const QString &path = TTK::File::getExistingDirectory(this);
    if(!path.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::MediaLibraryPath, path);
        refreshItems();
    }
}

void MusicLocalManagerWidget::searchResultChanged(int, int column)
{
    TTKIntList result;
    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        QString v;
        switch(m_tabButton->currentIndex())
        {
            case 0: v = m_containerItems[i].m_title; break;
            case 1: v = m_containerItems[i].m_artist; break;
            case 2: v = m_containerItems[i].m_album; break;
            case 3: v = m_containerItems[i].m_year; break;
            case 4: v = m_containerItems[i].m_genre; break;
            default: break;
        }

        if(v.contains(m_searchEdit->editor()->text(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    MusicSongInfoItemList items;
    for(const int index : qAsConst(result))
    {
        items.append(m_containerItems[index]);
    }

    m_searchResultLevel = column;
    m_searchResultItems.insert(column, result);

    m_songTableWidget->removeItems();
    m_songTableWidget->addCellItems(items);
    updateStatisticWidget(m_tabButton->currentIndex(), items);
}

void MusicLocalManagerWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    mappedSearchRow(m_searchEdit->editor()->text().length(), row);
    MusicSongsContainerWidget::instance()->addSongToPlaylist(QStringList(m_containerItems[row].m_path));
}

void MusicLocalManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWidget();
}

void MusicLocalManagerWidget::updateStatisticWidget(int index, const MusicSongInfoItemList &items)
{
    if(index == 0)
    {
        return;
    }

    QString label;
    MusicSongStatisticItem statistic;
    for(const MusicSongInfoItem &item : qAsConst(items))
    {
        QString data;
        switch(index)
        {
            case 0: break;
            case 1:
            {
                data = item.m_artist;
                label = tr("Artist");
                break;
            }
            case 2:
            {
                data = item.m_album;
                label = tr("Album");
                break;
            }
            case 3:
            {
                data = item.m_year;
                label = tr("Year");
                break;
            }
            case 4:
            {
                data = item.m_genre;
                label = tr("Genre");
                break;
            }
            default: break;
        }

        if(!statistic.contains(data))
        {
            statistic.insert(data, 1);
        }
        else
        {
            statistic.insert(data, statistic[data] + 1);
        }
    }

    m_statisticTableWidget->addCellItem(statistic);
    m_statisticTableWidget->setHorizontalHeaderLabels({label, tr("Count")});
}
