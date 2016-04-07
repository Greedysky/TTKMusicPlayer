#include "musicsongsearchonlinewidget.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbackgrounddownload.h"
#include "musiclocalsongsearchrecordconfigmanager.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musiccoremplayer.h"
#include "musicdownloadmgmtwidget.h"
#include "musicitemdelegate.h"

#include <QDateTime>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>

MusicSongSearchOnlineTableWidget::MusicSongSearchOnlineTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent), m_audition(nullptr)
{
    setColumnCount(6);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 225);
    headerview->resizeSection(2, 176);
    headerview->resizeSection(3, 50);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);
    MusicUtils::setTransparent(this, 255);

    m_previousAuditionRow = -1;
    M_CONNECTION->setValue("MusicSongSearchOnlineTableWidget", this);
}

MusicSongSearchOnlineTableWidget::~MusicSongSearchOnlineTableWidget()
{
    delete m_audition;
    clearAllItems();
}

void MusicSongSearchOnlineTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DisConnection);
        return;
    }
    ////////////////////////////////////////////////
    MusicSearchRecord record;
    MusicLocalSongSearchRecordConfigManager search(this);
    if(!search.readSearchXMLConfig())
    {
        return;
    }
    search.readSearchConfig( record );
    record.m_names.insert(0, text);
    record.m_times.insert(0, QString::number(QDateTime::currentMSecsSinceEpoch()));
    search.writeSearchConfig( record );
    ////////////////////////////////////////////////
    QString currentQuality;
    emit getQualityString(currentQuality);
    if(!currentQuality.isEmpty())
    {
        m_downLoadManager->setSearchQuality(currentQuality);
    }
    m_downLoadManager->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, m_searchText = text);
}

void MusicSongSearchOnlineTableWidget::researchQueryByQuality()
{
    startSearchQuery(m_searchText);
}

void MusicSongSearchOnlineTableWidget::searchDataDwonloadFinished()
{
    if(m_downloadDatas.count() == 3)
    {
        emit muiscSongToPlayListChanged(m_downloadDatas[0], m_downloadDatas[1], m_downloadDatas[2]);
    }
    m_downloadDatas.clear();
}

void MusicSongSearchOnlineTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(6);
}

void MusicSongSearchOnlineTableWidget::createSearchedItems(const QString &songname,
                                const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    item->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(songname, Qt::ElideRight, 220));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(artistname, Qt::ElideRight, 170));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/image/addtoplaylist")));
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/share/musicdownload")));
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
    emit auditionIsPlaying( item(row, 0)->data(MUSIC_AUDIT_ROLE).toInt() == AUDITION_STOP );
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
        case 4: auditionToMusic(row); break;
        case 5: addSearchMusicToPlayList(row); break;
    }
}

void MusicSongSearchOnlineTableWidget::auditionToMusic(int row)
{
    MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(musicSongInfos.isEmpty() || row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    if(m_audition == nullptr)
    {
        m_audition = new MusicCoreMPlayer(this);
    }
    m_audition->setMedia(MusicCoreMPlayer::MusicCategory,
                         musicSongInfos[row].m_songAttrs.first().m_url, -1);

    if(m_previousAuditionRow != -1)
    {
        item(m_previousAuditionRow, 0)->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    }
    item(m_previousAuditionRow = row, 0)->setData(MUSIC_AUDIT_ROLE, AUDITION_PLAY);
    emit auditionIsPlaying(false);
}

void MusicSongSearchOnlineTableWidget::auditionToMusicStop(int row)
{
    if(m_audition)
    {
        m_audition->stop();
    }
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    item(row, 0)->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    emit auditionIsPlaying(true);
}

void MusicSongSearchOnlineTableWidget::addSearchMusicToPlayList(int row)
{
    if(!M_NETWORK->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DisConnection);
        return;
    }
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    emit showDownLoadInfoFor(MusicObject::DownLoading);

    MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicSongInfomation musicSongInfo = musicSongInfos[row];
    MusicSongAttribute musicSongAttr = musicSongInfo.m_songAttrs.first();
    QString musicSong = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
    QString downloadName = QString("%1%2.%3").arg(MUSIC_DOWNLOAD_AL)
                                             .arg(musicSong).arg(musicSongAttr.m_format);

    MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicSongAttr.m_url, downloadName,
                                                                     MusicDownLoadThreadAbstract::Download_Music, this);
    connect(downSong, SIGNAL(musicDownLoadFinished(QString)), SLOT(searchDataDwonloadFinished()));
    downSong->startToDownload();

    (new MusicTextDownLoadThread(musicSongInfo.m_lrcUrl, LRC_DOWNLOAD_AL + musicSong + LRC_FILE,
                                 MusicDownLoadThreadAbstract::Download_Lrc, this))->startToDownload();
#ifndef USE_MULTIPLE_QUERY
    (new MusicData2DownloadThread(musicSongInfo.m_smallPicUrl, ART_DOWNLOAD_AL + musicSongInfo.m_singerName + SKN_FILE,
                                  MusicDownLoadThreadAbstract::Download_SmlBG, this))->startToDownload();
#else
    (new MusicDataDownloadThread(musicSongInfo.m_smallPicUrl, ART_DOWNLOAD_AL + musicSongInfo.m_singerName + SKN_FILE,
                                 MusicDownLoadThreadAbstract::Download_SmlBG, this))->startToDownload();
#endif
    ///download big picture
    (new MusicBackgroundDownload(musicSongInfo.m_singerName, musicSongInfo.m_singerName, this))->startToDownload();

    m_downloadDatas.clear();
    m_downloadDatas << musicSong << item(row, 3)->text() << musicSongAttr.m_format;
}

void MusicSongSearchOnlineTableWidget::musicDownloadLocal(int row)
{
    MusicDownloadMgmtWidget mgmt(this);
    mgmt.setSongName(item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip(), MusicDownLoadQueryThreadAbstract::MusicQuery);
}

void MusicSongSearchOnlineTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0)
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
    connect(m_searchTableWidget, SIGNAL(auditionIsPlaying(bool)), SLOT(auditionIsPlaying(bool)));
}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    delete m_playButton;
    delete m_textLabel;
    delete m_searchTableWidget;
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name) const
{
    m_searchTableWidget->startSearchQuery(name);
    m_textLabel->setText(tr("&nbsp;find <font color=red> %1 </font> result")
                         .arg(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 170)));
}

void MusicSongSearchOnlineWidget::createToolWidget()
{
    m_textLabel = new QLabel(this);
    m_textLabel->setGeometry(0, 0, 280, 30);
    m_textLabel->setTextFormat(Qt::RichText);
    m_textLabel->setText(tr("&nbsp;find no result"));

    QLabel *colorLabel = new QLabel(this);
    colorLabel->setStyleSheet(MusicUIObject::MCustomStyle17);
    colorLabel->setGeometry(0, 30, width(), 35);

    QCheckBox *label_checkBox = new QCheckBox(this);
    label_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    label_checkBox->setGeometry(7, 40, 20, 20);
    connect(label_checkBox, SIGNAL(clicked(bool)), m_searchTableWidget,
                            SLOT(setSelectedAllItems(bool)));

    QLabel *Label1 = new QLabel(tr("Song"), this);
    Label1->setStyleSheet(MusicUIObject::MCustomStyle18);
    Label1->setGeometry(120, 40, 60, 20);
    QLabel *Label2 = new QLabel(tr("Artist"), this);
    Label2->setStyleSheet(MusicUIObject::MCustomStyle18);
    Label2->setGeometry(310, 40, 60, 20);
    QLabel *Label3 = new QLabel(tr("Operator"), this);
    Label3->setStyleSheet(MusicUIObject::MCustomStyle18);
    Label3->setGeometry(450, 40, 60, 20);

    m_playButton = new QPushButton(tr("Play"), this);
    m_playButton->setGeometry(315, 5, 70, 20);
    m_playButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *addButton = new QPushButton(tr("Add"), this);
    addButton->setGeometry(390, 5, 70, 20);
    addButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("Download"), this);
    downloadButton->setGeometry(465, 5, 70, 20);
    downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_playButton, 0);
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
            case 0:
                m_playButton->text() == tr("Play") ? m_searchTableWidget->auditionToMusic(row)
                                                   : m_searchTableWidget->auditionToMusicStop(row);
                break;
            case 1:
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

void MusicSongSearchOnlineWidget::auditionIsPlaying(bool play)
{
    m_playButton->setText(play ? tr("Play") : tr("Stop"));
}
