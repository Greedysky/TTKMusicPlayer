#include "musiclocalmanagerwidget.h"
#include "musicsettingmanager.h"
#include "musicitemsearchedit.h"

MusicLocalManagerSongsTableWidget::MusicLocalManagerSongsTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 460);
    headerview->resizeSection(1, 85);
    headerview->resizeSection(2, 105);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);

    m_songs = new MusicSongList;
    MusicUtils::Widget::setTransparent(this, 150);
}

MusicLocalManagerSongsTableWidget::~MusicLocalManagerSongsTableWidget()
{
    clearAllItems();
    delete m_songs;
}

void MusicLocalManagerSongsTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    m_songs->clear();
}

//void MusicLocalManagerSongsTableWidget::addItems(const QStringList &path)
//{
//    QHeaderView *headerview = horizontalHeader();
//    for(int i = 0; i < path.count(); ++i)
//    {
//        QFileInfo fin(path[i]);

//        QTableWidgetItem *item = new QTableWidgetItem;
//        item->setToolTip(fin.fileName());
//        item->setText(" " + MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(0) - 20));
//        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
//        setItem(i, 0, item);

//                         item = new QTableWidgetItem;
//        item->setToolTip(MusicUtils::Number::sizeByte2Label(fin.size()));
//        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
//        QtItemSetTextAlignment(item, Qt::AlignRight | Qt::AlignVCenter);
//        setItem(i, 1, item);

//                         item = new QTableWidgetItem(fin.lastModified().date().toString(Qt::ISODate));
//        QtItemSetTextAlignment(item, Qt::AlignCenter);
//        setItem(i, 2, item);

//                         item = new QTableWidgetItem;
//        item->setIcon(QIcon(":/contextMenu/btn_audition"));
//        setItem(i, 3, item);

//                         item = new QTableWidgetItem;
//        item->setIcon(QIcon(":/contextMenu/btn_add"));
//        setItem(i, 4, item);

//        m_songs->append(MusicSong(fin.absoluteFilePath()));
//    }
//}

void MusicLocalManagerSongsTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
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
    mainLayout->setContentsMargins(20, 20, 20, 20);
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
    m_tabWidget->setStyleSheet(MusicUIObject::MQSSTabWidgetStyle01 + "QTabBar::tab{ width:120px; }");
    mainLayout->addWidget(m_tabWidget);
    //
    QWidget *songWidget = new QWidget(m_tabWidget);
    songWidget->setStyleSheet("background:rgb(200,200,200)");
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

    typeIndexChanged(0);

    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(typeIndexChanged(int)));
}

MusicLocalManagerWidget::~MusicLocalManagerWidget()
{

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

void MusicLocalManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}
