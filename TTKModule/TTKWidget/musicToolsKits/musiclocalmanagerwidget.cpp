#include "musiclocalmanagerwidget.h"
#include "musicsettingmanager.h"
#include "musicitemsearchedit.h"
#include "musicgiflabelwidget.h"
#include "musicsongmeta.h"
#include "musicfileutils.h"
#include "musicformats.h"

#include <QTimer>
#include <QButtonGroup>
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QStandardPaths>
#else
#  include <QDesktopServices>
#endif

MusicLocalManagerSongsTableWidget::MusicLocalManagerSongsTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(6);

    QHeaderView *headerview = horizontalHeader();
    headerview->setVisible(true);
    headerview->resizeSection(0, 200);
    headerview->resizeSection(1, 100);
    headerview->resizeSection(2, 100);
    headerview->resizeSection(3, 100);
    headerview->resizeSection(4, 100);
    headerview->resizeSection(5, 200);

    setAlternatingRowColors(true);
    setFrameShape(QFrame::Box);
    setTextElideMode(Qt::ElideRight);
    setWordWrap(false);

    setHorizontalHeaderLabels({tr("Title"), tr("Artist"), tr("Album"), tr("Year"), tr("Genre"), tr("Path")});
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    horizontalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle04);

    m_songs = new MusicSongList;
}

MusicLocalManagerSongsTableWidget::~MusicLocalManagerSongsTableWidget()
{
    removeItems();
    delete m_songs;
}

void MusicLocalManagerSongsTableWidget::updateSongsList(const MusicSongInfoItemList &songs)
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
        item->setToolTip(v.m_year);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 3, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_genre);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_path);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);

        m_songs->append(MusicSong(v.m_path));
    }
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
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(musicPlayClicked()));
    menu.addAction(tr("Download More..."), this, SLOT(musicSongDownload()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = !m_songs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(musicFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(musicOpenFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(musicAlbumQueryWidget()));
    menu.addSeparator();

    menu.exec(QCursor::pos());
}

#define TAB_BUTTON_ON   "QPushButton{ border-radius:0px; background-color:#158FE1; color:white; }"
#define TAB_BUTTON_OFF  "QPushButton{ border-radius:0px; background-color:#BFBFBF; color:white; }"

MusicLocalManagerWidget::MusicLocalManagerWidget(QWidget *parent)
    : QWidget(parent),
      MusicItemSearchInterfaceClass(),
      m_currentIndex(0)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle12 + MusicUIObject::MQSSColorStyle09);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(20, 20, 20, 10);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);

    QWidget *topWidget = new QWidget(mainWidget);
    mainLayout->addWidget(topWidget);

    QVBoxLayout *topLayout = new QVBoxLayout(topWidget);
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(0, 10, 0, 10);
    topWidget->setLayout(topLayout);

    QWidget *functionWidget = new QWidget(topWidget);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(0, 10, 0, 10);
    functionWidget->setLayout(functionWidgetLayout);

    QLabel *pLabel = new QLabel(tr("Media Library"), functionWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(33);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MQSSColorStyle11);
    functionWidgetLayout->addWidget(pLabel);

    m_sizeLabel = new QLabel(functionWidget);
    m_sizeLabel->setStyleSheet(MusicUIObject::MQSSColorStyle11);
    functionWidgetLayout->addWidget(m_sizeLabel);
    functionWidgetLayout->addStretch(1);

    QPushButton *refresh = new QPushButton(tr("Refresh"), functionWidget);
    refresh->setIcon(QIcon(":/functions/btn_similar_hover"));
    refresh->setFixedSize(90, 30);
    refresh->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    refresh->setCursor(QCursor(Qt::PointingHandCursor));
    functionWidgetLayout->addWidget(refresh);

    QPushButton *button = new QPushButton(tr("Settings"), functionWidget);
    button->setIcon(QIcon(":/functions/btn_setting_hover"));
    button->setFixedSize(90, 30);
    button->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    functionWidgetLayout->addWidget(button);

    m_searchEdit = new MusicItemQueryEdit(this);
    m_searchEdit->setFixedHeight(33);
    topLayout->addWidget(functionWidget);
    topLayout->addWidget(m_searchEdit);

    QWidget *tabWidget = new QWidget(mainWidget);
    QHBoxLayout *tabWidgetLayout = new QHBoxLayout(tabWidget);
    tabWidgetLayout->setSpacing(0);
    tabWidgetLayout->setContentsMargins(0, 0, 0, 0);
    tabWidget->setLayout(tabWidgetLayout);
    mainLayout->addWidget(tabWidget);

    QPushButton *songButton = new QPushButton(tr("Song"), functionWidget);
    songButton->setFixedSize(90, 30);
    songButton->setStyleSheet(TAB_BUTTON_ON);
    songButton->setCursor(QCursor(Qt::PointingHandCursor));
    tabWidgetLayout->addWidget(songButton);

    QPushButton *artistButton = new QPushButton(tr("Artist"), functionWidget);
    artistButton->setFixedSize(90, 30);
    artistButton->setStyleSheet(TAB_BUTTON_OFF);
    artistButton->setCursor(QCursor(Qt::PointingHandCursor));
    tabWidgetLayout->addWidget(artistButton);

    QPushButton *albumButton = new QPushButton(tr("Album"), functionWidget);
    albumButton->setFixedSize(90, 30);
    albumButton->setStyleSheet(TAB_BUTTON_OFF);
    albumButton->setCursor(QCursor(Qt::PointingHandCursor));
    tabWidgetLayout->addWidget(albumButton);

    QPushButton *yearButton = new QPushButton(tr("Year"), functionWidget);
    yearButton->setFixedSize(90, 30);
    yearButton->setStyleSheet(TAB_BUTTON_OFF);
    yearButton->setCursor(QCursor(Qt::PointingHandCursor));
    tabWidgetLayout->addWidget(yearButton);

    QPushButton *genreButton = new QPushButton(tr("Genre"), functionWidget);
    genreButton->setFixedSize(90, 30);
    genreButton->setStyleSheet(TAB_BUTTON_OFF);
    genreButton->setCursor(QCursor(Qt::PointingHandCursor));
    tabWidgetLayout->addWidget(genreButton);
    tabWidgetLayout->addStretch(1);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(songButton, 0);
    buttonGroup->addButton(artistButton, 1);
    buttonGroup->addButton(albumButton, 2);
    buttonGroup->addButton(yearButton, 3);
    buttonGroup->addButton(genreButton, 4);
    QtButtonGroupConnect(buttonGroup, this, typeIndexChanged);

#ifdef Q_OS_UNIX
    refresh->setFocusPolicy(Qt::NoFocus);
    button->setFocusPolicy(Qt::NoFocus);
    songButton->setFocusPolicy(Qt::NoFocus);
    artistButton->setFocusPolicy(Qt::NoFocus);
    albumButton->setFocusPolicy(Qt::NoFocus);
    yearButton->setFocusPolicy(Qt::NoFocus);
    genreButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_songWidget = new MusicLocalManagerSongsTableWidget(mainWidget);
    mainLayout->addWidget(m_songWidget);

    m_loadingLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Module::CicleBlue, this);
    m_loadingLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_searchEdit->editor()->setPlaceholderText(tr("Please input search song words!"));

    QTimer::singleShot(MT_ONCE, this, SLOT(refreshItems()));
    connect(refresh, SIGNAL(clicked()), SLOT(refreshItems()));
    connect(button, SIGNAL(clicked()), SLOT(updateMediaLibraryPath()));
    connect(m_searchEdit->editor(), SIGNAL(cursorPositionChanged(int,int)), SLOT(searchResultChanged(int,int)));
}

MusicLocalManagerWidget::~MusicLocalManagerWidget()
{
    delete m_sizeLabel;
    delete m_searchEdit;
    delete m_loadingLabel;
    delete m_songWidget;
}

void MusicLocalManagerWidget::typeIndexChanged(int index)
{
    m_currentIndex = index;
    const QList<QAbstractButton*> &buttons = TTKObject_cast(QButtonGroup*, sender())->buttons();

    for(int i = 0; i < buttons.count(); ++i)
    {
        buttons[i]->setStyleSheet(i == index ? TAB_BUTTON_ON : TAB_BUTTON_OFF);
    }

    switch(index)
    {
        case 0:
        {
            m_searchEdit->editor()->setPlaceholderText(tr("Please input search song words!"));
            break;
        }
        case 1:
        {
            m_searchEdit->editor()->setPlaceholderText(tr("Please input search artist words!"));
            break;
        }
        case 2:
        {
            m_searchEdit->editor()->setPlaceholderText(tr("Please input search album words!"));
            break;
        }
        case 3:
        {
            m_searchEdit->editor()->setPlaceholderText(tr("Please input search year words!"));
            break;
        }
        case 4:
        {
            m_searchEdit->editor()->setPlaceholderText(tr("Please input search genre words!"));
            break;
        }
        default: break;
    }

    m_searchEdit->editor()->clear();
}

void MusicLocalManagerWidget::refreshItems()
{
    if(m_loadingLabel->isRunning())
    {
        return;
    }

    m_containerItems.clear();
    m_sizeLabel->clear();
    m_songWidget->removeItems();
    m_searchEdit->editor()->clear();

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

    m_loadingLabel->run(true);
    const QStringList &files = MusicUtils::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats());
    m_sizeLabel->setText(tr("   (Songs Totol: %1)").arg(files.size()));

    MusicSongMeta meta;
    for(const QString &file : files)
    {
        QString check;
        const bool state = meta.read(file);

        MusicSongInfoItem info;
        info.m_title = state ? MusicObject::generateSongName(meta.title(), meta.artist()) : TTK_DEFAULT_STR;
        info.m_artist = state ? ((check = meta.artist()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR;
        info.m_album = state ? ((check = meta.album()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR;
        info.m_year = state ? ((check = meta.year()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR;
        info.m_genre = state ? ((check = meta.genre()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR;
        info.m_path = file;
        m_containerItems << info;

        qApp->processEvents();
    }

    m_songWidget->updateSongsList(m_containerItems);
    m_loadingLabel->run(false);
}

void MusicLocalManagerWidget::updateMediaLibraryPath()
{
    const QString &path = MusicUtils::File::openDirectoryDialog(this);
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
        switch(m_currentIndex)
        {
            case 0: v = m_containerItems[i].m_title; break;
            case 1: v = m_containerItems[i].m_artist; break;
            case 2: v = m_containerItems[i].m_album; break;
            case 3: v = m_containerItems[i].m_year; break;
            case 4: v = m_containerItems[i].m_genre; break;
            default: break;
        }

        if(v.contains(m_searchEdit->editor()->text().trimmed(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    MusicSongInfoItemList data;
    for(const int index : qAsConst(result))
    {
        data.append(m_containerItems[index]);
    }

    m_songWidget->removeItems();
    m_searchResultCache.insert(column, result);
    m_songWidget->updateSongsList(data);
}

void MusicLocalManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_loadingLabel->move((width() - m_loadingLabel->width()) / 2, (height() + 150 - m_loadingLabel->height()) / 2);
}
