#include "musicalbumfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"
#include "musicitemdelegate.h"
#include "musicuiobject.h"

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>

MusicAlbumFoundTableWidget::MusicAlbumFoundTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 477);
    headerview->resizeSection(2, 60);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);

    MusicUtils::UWidget::setTransparent(this, 255);
    setStyleSheet( styleSheet() + MusicUIObject::MTableWidgetStyle02);

    m_checkBoxDelegate = new MusicQueryTableDelegate(this);
    setItemDelegateForColumn(0, m_checkBoxDelegate);
}

MusicAlbumFoundTableWidget::~MusicAlbumFoundTableWidget()
{
    clearAllItems();
    delete m_checkBoxDelegate;
}

QString MusicAlbumFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicAlbumFoundTableWidget::listCellClicked(int row, int column)
{

}

void MusicAlbumFoundTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(5);
}

void MusicAlbumFoundTableWidget::createSearchedItems(const QString &songname, const QString &artistname,
                                                     const QString &time)
{
    int count = rowCount();
    setRowCount(count + 1);

    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::UWidget::elidedText(font(), artistname + " - " + songname, Qt::ElideRight, 400));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(songname);
    setItem(count, 1, item);

                      item = new QTableWidgetItem(time);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(count, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_add")));
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_download")));
    setItem(count, 4, item);

    setFixedHeight(rowHeight(0)*rowCount());
}

void MusicAlbumFoundTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicAbstractTableWidget::resizeEvent(event);
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.9 + 477);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.1 + 60);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::UWidget::elidedText(font(), it->toolTip(), Qt::ElideRight, width - WINDOW_WIDTH_MIN + 400));
    }
}



MusicAlbumFoundWidget::MusicAlbumFoundWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    m_mainWindow = new QWidget(this);
    m_mainWindow->setStyleSheet(MusicUIObject::MBackgroundStyle17);
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MCustomStyle04 + MusicUIObject::MCustomStyle01);
    QHBoxLayout *mLayout = new QHBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);

    m_albumThread = nullptr;
    m_albumTableWidget = new MusicAlbumFoundTableWidget(this);
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicAlbumFoundWidget::~MusicAlbumFoundWidget()
{
    delete m_albumTableWidget;
    delete m_downloadThread;
    delete m_albumThread;
    delete m_statusLabel;
    delete m_mainWindow;
}

QString MusicAlbumFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicAlbumFoundWidget::setSongName(const QString &name)
{
    m_songNameFull = name;
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name.split("-").front().trimmed());
}

void MusicAlbumFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noAlbum"));
    }
    else
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        MusicObject::MusicSongInfomation currentInfo;
        foreach(const MusicObject::MusicSongInfomation &info, musicSongInfos)
        {
            if(m_songNameFull.contains(info.m_songName))
            {
                currentInfo = info;
                m_albumThread = M_DOWNLOAD_QUERY_PTR->getAlbumThread(this);
                m_albumThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, info.m_albumId);
                connect(m_albumThread, SIGNAL(createSearchedItems(QString,QString,QString)),
                        m_albumTableWidget, SLOT(createSearchedItems(QString,QString,QString)));
                break;
            }
        }

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
        firstLabel->setText(QString("<font color=#169AF3> Alubm > %1 </font>").arg(currentInfo.m_albumId));
        grid->addWidget(firstLabel);
        ////////////////////////////////////////////////////////////////////////////
        QWidget *topFuncWidget = new QWidget(function);
        QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

        QLabel *iconLabel = new QLabel(topFuncWidget);
        iconLabel->setPixmap(QPixmap(":/image/lb_warning"));
        iconLabel->setStyleSheet("background:red");
        iconLabel->setFixedSize(250, 250);

        QWidget *topLineWidget = new QWidget(topFuncWidget);
        QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
        QLabel *albumLabel = new QLabel(topLineWidget);
        albumLabel->setText(currentInfo.m_albumId);
        QLabel *singerLabel = new QLabel(topLineWidget);
        singerLabel->setText(tr("Singer: %1").arg(currentInfo.m_singerName));
        QLabel *languageLabel = new QLabel(topLineWidget);
        languageLabel->setText(tr("Language: %1"));
        QLabel *companyLabel = new QLabel(topLineWidget);
        companyLabel->setText(tr("Company: %1"));
        QLabel *yearLabel = new QLabel(topLineWidget);
        yearLabel->setText(tr("Year: %1"));
        topLineLayout->addWidget(albumLabel);
        topLineLayout->addWidget(singerLabel);
        topLineLayout->addWidget(languageLabel);
        topLineLayout->addWidget(companyLabel);
        topLineLayout->addWidget(yearLabel);
        topLineWidget->setLayout(topLineLayout);

        QWidget *topButtonWidget = new QWidget(topFuncWidget);
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

        topFuncLayout->addWidget(iconLabel);
        topFuncLayout->addWidget(topLineWidget);
        topFuncWidget->setLayout(topFuncLayout);
        grid->addWidget(topFuncWidget);
        ////////////////////////////////////////////////////////////////////////////
        QLabel *songItems = new QLabel(tr("songItems") + QString("(%1)").arg(10), function);
        grid->addWidget(songItems);

        QWidget *middleFuncWidget = new QWidget(function);
        QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
        middleFuncLayout->setContentsMargins(0, 0, 0, 0);
        QLabel *marginLabel = new QLabel(middleFuncWidget);
        marginLabel->setFixedWidth(1);
        QCheckBox *allCheckBox = new QCheckBox(tr("all"), middleFuncWidget);
        QPushButton *playButton = new QPushButton(tr("play"), middleFuncWidget);
        playButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
        playButton->setIconSize(QSize(14, 14));
        playButton->setFixedSize(55, 25);
        playButton->setCursor(QCursor(Qt::PointingHandCursor));
        QPushButton *addButton = new QPushButton(tr("add"), middleFuncWidget);
        addButton->setFixedSize(55, 25);
        addButton->setCursor(QCursor(Qt::PointingHandCursor));
        QPushButton *downloadButton = new QPushButton(tr("download"), middleFuncWidget);
        downloadButton->setFixedSize(55, 25);
        downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

        middleFuncLayout->addWidget(marginLabel);
        middleFuncLayout->addWidget(allCheckBox);
        middleFuncLayout->addStretch(1);
        middleFuncLayout->addWidget(playButton);
        middleFuncLayout->addWidget(addButton);
        middleFuncLayout->addWidget(downloadButton);
        connect(allCheckBox, SIGNAL(clicked(bool)), SLOT(selectAllItems(bool)));
        connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
        connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
        connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));

        grid->addWidget(middleFuncWidget);
        //////////////////////////////////////////////////////////////////////
        grid->addWidget(m_albumTableWidget);
        grid->addStretch(1);

        function->setLayout(grid);
        m_mainWindow->layout()->addWidget(function);

//        m_resizeWidget << allCheckBox;
    }
}

void MusicAlbumFoundWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicAlbumFoundWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

//    if(!m_resizeWidget.isEmpty())
//    {
//        int height = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height();

//        QWidget *w = m_resizeWidget.first();
//        qDebug() << height - WINDOW_HEIGHT_MIN;
//        w->move(w->x(), w->y() + height - WINDOW_HEIGHT_MIN);
//    }
}
