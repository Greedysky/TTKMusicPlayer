#include "musicsongsearchonlinewidget.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbgthemedownload.h"
#include "musicmydownloadrecordobject.h"
#include "musiclocalsongsearchrecordobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"

#include <QDateTime>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>

MusicSongSearchOnlineTableWidget::MusicSongSearchOnlineTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setColumnCount(6);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,215);
    headerview->resizeSection(2,166);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,26);
    headerview->resizeSection(5,26);
    setTransparent(255);

    M_Connection->setValue("MusicSongSearchOnlineTableWidget", this);
}

MusicSongSearchOnlineTableWidget::~MusicSongSearchOnlineTableWidget()
{
    clearAllItems();
}

void MusicSongSearchOnlineTableWidget::startSearchQuery(const QString &text)
{
    ////////////////////////////////////////////////
    QStringList names, times;
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig())
    {
        return;
    }
    search.readSearchConfig(names, times);
    names.insert(0, text);
    times.insert(0, QString::number(QDateTime::currentMSecsSinceEpoch()));
    search.writeSearchConfig(names, times);
    ////////////////////////////////////////////////
    m_downLoadManager->startSearchSong(Music, text);
}

void MusicSongSearchOnlineTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(6);
}

void MusicSongSearchOnlineTableWidget::creatSearchedItems(const QString &songname,
                               const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem(songname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem(artistname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/addtoplaylist")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/musicdownload")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 5, item);
}

void MusicSongSearchOnlineTableWidget::listCellClicked(int row, int col)
{
    MusicQueryTableWidget::listCellClicked(row, col);
    switch(col)
    {
        case 4:
            addSearchMusicToPlayList(row);
            break;
        case 5:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }
}

void MusicSongSearchOnlineTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    QAction *playAction = rightClickMenu.addAction(QIcon(":/contextMenu/play"), tr("musicPlay"));
    QAction *addAction = rightClickMenu.addAction(tr("musicAdd"));
    rightClickMenu.insertAction(rightClickMenu.actions().first(), addAction);
    rightClickMenu.insertAction(addAction, playAction);
    m_actionGroup->addAction( playAction );
    m_actionGroup->addAction( addAction );
    rightClickMenu.exec(QCursor::pos());
}

void MusicSongSearchOnlineTableWidget::actionGroupClick(QAction *action)
{
    MusicQueryTableWidget::actionGroupClick(action);

    int row = currentRow();
    switch( findActionGroup(action) )
    {
        case 4:
        case 5: addSearchMusicToPlayList(row); break;
    }
}

void MusicSongSearchOnlineTableWidget::addSearchMusicToPlayList(int row)
{
    emit showDownLoadInfoFor(MusicObject::Buffing);
    musicDownloadLocal(row);
    emit muiscSongToPlayListChanged( item(row, 2)->text() + " - " + item(row, 1)->text() );
}

void MusicSongSearchOnlineTableWidget::musicDownloadLocal(int row)
{
    emit showDownLoadInfoFor(MusicObject::DownLoading);
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());
    QString musicSong =  item(row, 2)->text() + " - " + item(row, 1)->text() ;
    QString downloadName = MUSIC_DOWNLOAD + musicSong + MUSIC_FILE;

    ////////////////////////////////////////////////
    QStringList name, path;
    MusicMyDownloadRecordObject down(this);
    if(!down.readDownloadXMLConfig())
    {
        return;
    }
    down.readDownloadConfig(name,path);
    name<<musicSong;
    path<<QFileInfo(downloadName).absoluteFilePath();
    down.writeDownloadConfig(name, path);
    ////////////////////////////////////////////////

    MusicDataDownloadThread *downSong = new MusicDataDownloadThread(
                                            musicSongInfo[row][0], downloadName,this);
    downSong->startToDownload();

    (new MusicTextDownLoadThread(musicSongInfo[row][1],LRC_DOWNLOAD +
                                 musicSong + LRC_FILE,this))->startToDownload();
    (new MusicData2DownloadThread(musicSongInfo[row][2],
          ART_DOWNLOAD + musicSongInfo[row][3] + SKN_FILE,this))->startToDownload();

    ///download big picture
    new MusicBgThemeDownload(musicSongInfo[row][3], musicSongInfo[row][3], this);
}

void MusicSongSearchOnlineTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0)
    {
        return;
    }
    addSearchMusicToPlayList(row);
}



MusicSongSearchOnlineWidget::MusicSongSearchOnlineWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setMargin(0);
    boxLayout->setSpacing(0);

    QWidget *toolWidget = new QWidget(this);
    toolWidget->setFixedHeight(65);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    toolWidget->setAutoFillBackground(true);
    toolWidget->setPalette(pal);

    m_searchTableWidget = new  MusicSongSearchOnlineTableWidget(this);
    boxLayout->addWidget(toolWidget);
    boxLayout->addWidget(m_searchTableWidget);
    setLayout(boxLayout);

    createToolWidget();
}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    delete m_textLabel;
    delete m_searchTableWidget;
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name) const
{
    m_searchTableWidget->startSearchQuery(name);
    m_textLabel->setText(tr(" find <font color=red> %1 </font> result").arg(name));
}

void MusicSongSearchOnlineWidget::createToolWidget()
{
    m_textLabel = new QLabel(this);
    m_textLabel->setGeometry(0, 0, 280, 30);
    m_textLabel->setText(tr(" find no result"));

    QLabel *colorLabel = new QLabel(this);
    colorLabel->setStyleSheet(MusicUIObject::MCustomStyle16);
    colorLabel->setGeometry(0, 30, width(), 35);

    QCheckBox *label_checkBox = new QCheckBox(this);
    label_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    label_checkBox->setGeometry(15, 40, 20, 20);
    connect(label_checkBox, SIGNAL(clicked(bool)), m_searchTableWidget,
                            SLOT(setSelectedAllItems(bool)));

    QLabel *Label1 = new QLabel(tr("Song"), this);
    Label1->setStyleSheet(MusicUIObject::MCustomStyle17);
    Label1->setGeometry(120, 40, 60, 20);
    QLabel *Label2 = new QLabel(tr("Artist"), this);
    Label2->setStyleSheet(MusicUIObject::MCustomStyle17);
    Label2->setGeometry(310, 40, 60, 20);
    QLabel *Label3 = new QLabel(tr("Operator"), this);
    Label3->setStyleSheet(MusicUIObject::MCustomStyle17);
    Label3->setGeometry(435, 40, 60, 20);

    QPushButton *playButton = new QPushButton(tr("Play"), this);
    playButton->setGeometry(295, 5, 70, 20);
    playButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *addButton = new QPushButton(tr("Add"), this);
    addButton->setGeometry(370, 5, 70, 20);
    addButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("Download"), this);
    downloadButton->setGeometry(445, 5, 70, 20);
    downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(playButton, 0);
    buttonGroup->addButton(addButton, 1);
    buttonGroup->addButton(downloadButton, 2);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
}

void MusicSongSearchOnlineWidget::buttonClicked(int index)
{
    MIntList list = m_searchTableWidget->getSelectedItems();
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    foreach(int row, list)
    {
        switch(index)
        {
            case 0:case 1:
                m_searchTableWidget->listCellClicked(row, 4);
                break;
            case 2:
                m_searchTableWidget->listCellClicked(row, 5);
                break;
            default:
                break;
        }
    }
}
