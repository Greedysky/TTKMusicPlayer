#include "musiclocalmanagerwidget.h"
#include "musicsettingmanager.h"
#include "musicitemsearchedit.h"
#include "musicsongmeta.h"
#include "musicfileutils.h"
#include "musicformats.h"

MusicLocalManagerSongsTableWidget::MusicLocalManagerSongsTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(6);

    QHeaderView *headerview = horizontalHeader();
    headerview->setVisible(true);
    headerview->resizeSection(0, 300);
    headerview->resizeSection(1, 200);
    headerview->resizeSection(2, 200);
    headerview->resizeSection(3, 100);
    headerview->resizeSection(4, 100);
    headerview->resizeSection(5, 400);

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
    setRowCount(songs.count());

    for(int i = 0; i < songs.count(); ++i)
    {
        MusicSongMeta meta;
        QString check;
        const bool state = meta.read(songs[i]);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(state ? meta.artist() + " - " + meta.title() : TTK_DEFAULT_STR);
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

        m_songs->append(MusicSong(meta.fileRelatedPath()));
    }
}

void MusicLocalManagerSongsTableWidget::removeItems()
{
    MusicAbstractSongsListTableWidget::removeItems();
    m_songs->clear();
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
    mainLayout->setSpacing(10);
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
    functionWidgetLayout->setContentsMargins(0, 10, 0, 30);
    functionWidget->setLayout(functionWidgetLayout);

    QLabel *pLabel = new QLabel(tr("Media Library"), functionWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MQSSColorStyle11);
    functionWidgetLayout->addWidget(pLabel);

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

    m_fileWatcher = new QFileSystemWatcher(this);

    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(typeIndexChanged(int)));
    connect(m_fileWatcher, SIGNAL(directoryChanged(QString)), SLOT(mediaPathChanged(QString)));
}

MusicLocalManagerWidget::~MusicLocalManagerWidget()
{
    delete m_tabWidget;
    delete m_searchEdit;
    delete m_fileWatcher;
}

void MusicLocalManagerWidget::initialize()
{
    mediaPathChanged("/home/greedysky/qmmp_all/files/modplug/");
    m_fileWatcher->addPath("/home/greedysky/qmmp_all/files/modplug/");
}

void MusicLocalManagerWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty())
    {
        const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        m_resizeWidgets[0]->setFixedWidth((width - WINDOW_WIDTH_MIN) + 540);
    }
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

void MusicLocalManagerWidget::mediaPathChanged(const QString &path)
{
    TTKStatic_cast(MusicLocalManagerSongsTableWidget*, m_tabWidget->widget(0))->
    updateSongsList(MusicUtils::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats()));
}

void MusicLocalManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}
