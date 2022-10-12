#include "musiclocalmanagerwidget.h"
#include "musicsettingmanager.h"
#include "musicitemsearchedit.h"
#include "musicgiflabelwidget.h"
#include "musicsongmeta.h"
#include "musicfileutils.h"
#include "musicformats.h"

#include <QTimer>

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

void MusicLocalManagerSongsTableWidget::updateSongsList(const QStringList &songs)
{
    MusicSongMeta meta;
    setRowCount(songs.count());

    for(int i = 0; i < songs.count(); ++i)
    {
        QString check;
        const bool state = meta.read(songs[i]);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(state ? MusicObject::generateSongName(meta.title(), meta.artist()) : TTK_DEFAULT_STR);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(state ? ((check = meta.artist()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setToolTip(state ? ((check = meta.album()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

                item = new QTableWidgetItem;
        item->setToolTip(state ? ((check = meta.year()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 3, item);

                item = new QTableWidgetItem;
        item->setToolTip(state ? ((check = meta.genre()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);

                item = new QTableWidgetItem;
        item->setToolTip(meta.fileRelatedPath());
        item->setText(item->toolTip());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);

        qApp->processEvents();
        m_songs->append(MusicSong(meta.fileRelatedPath()));
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



MusicLocalManagerWidget::MusicLocalManagerWidget(QWidget *parent)
    : QWidget(parent)
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
    pLabelFont.setPixelSize(30);
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
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif
    m_searchEdit = new MusicItemQueryEdit(this);
    m_searchEdit->setFixedHeight(33);
    topLayout->addWidget(functionWidget);
    topLayout->addWidget(m_searchEdit);

    m_tabWidget = new QTabWidget(mainWidget);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);
    m_tabWidget->setStyleSheet(MusicUIObject::MQSSTabWidgetStyle01);
    mainLayout->addWidget(m_tabWidget);
    //
    QWidget *songWidget = new MusicLocalManagerSongsTableWidget(m_tabWidget);
    m_tabWidget->addTab(songWidget, tr("Song"));
    //
    QWidget *artistWidget = new QWidget(m_tabWidget);
    artistWidget->setStyleSheet("background:rgb(160,160,160)");
    m_tabWidget->addTab(artistWidget, tr("Artist"));
    //
    QWidget *albumWidget = new QWidget(m_tabWidget);
    albumWidget->setStyleSheet("background:rgb(120,120,120)");
    m_tabWidget->addTab(albumWidget, tr("Album"));
    //
    QWidget *yearWidget = new QWidget(m_tabWidget);
    yearWidget->setStyleSheet("background:rgb(80,80,80)");
    m_tabWidget->addTab(yearWidget, tr("Year"));
    //
    QWidget *genreWidget = new QWidget(m_tabWidget);
    genreWidget->setStyleSheet("background:rgb(40,40,40)");
    m_tabWidget->addTab(genreWidget, tr("Genre"));

    m_loadingLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Module::CicleBlue, this);
    m_loadingLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

    QTimer::singleShot(MT_ONCE, this, SLOT(refreshItems()));
    connect(refresh, SIGNAL(clicked()), SLOT(refreshItems()));
    connect(button, SIGNAL(clicked()), SLOT(updateMediaLibraryPath()));
    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(typeIndexChanged(int)));
}

MusicLocalManagerWidget::~MusicLocalManagerWidget()
{
    delete m_sizeLabel;
    delete m_tabWidget;
    delete m_searchEdit;
    delete m_loadingLabel;
}

void MusicLocalManagerWidget::typeIndexChanged(int index)
{
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

    const QString &path = G_SETTING_PTR->value(MusicSettingManager::MediaLibraryPath).toString();
    if(!path.isEmpty())
    {
        m_loadingLabel->run(true);
        const QStringList &files = MusicUtils::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats());
        m_sizeLabel->setText(tr("   (Totol: %1)").arg(files.size()));

        TTKObject_cast(MusicLocalManagerSongsTableWidget*, m_tabWidget->currentWidget())->updateSongsList(files);
        m_loadingLabel->run(false);
    }
    else
    {
        m_sizeLabel->clear();
    }
}

void MusicLocalManagerWidget::updateMediaLibraryPath()
{
    m_sizeLabel->clear();
}

void MusicLocalManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_loadingLabel->move((width() - m_loadingLabel->width()) / 2, (height() + 150 - m_loadingLabel->height()) / 2);
}
