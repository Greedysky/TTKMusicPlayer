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
#include "musiccryptographichash.h"
#include "musicsettingmanager.h"

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
    headerview->resizeSection(1, 315);
    headerview->resizeSection(2, 198);
    headerview->resizeSection(3, 60);
    headerview->resizeSection(4, 26);
    headerview->resizeSection(5, 26);
    MusicUtils::UWidget::setTransparent(this, 255);

    m_previousAuditionRow = -1;
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicSongSearchOnlineTableWidget::~MusicSongSearchOnlineTableWidget()
{
    delete m_audition;
    clearAllItems();
}

QString MusicSongSearchOnlineTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongSearchOnlineTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
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
    m_downLoadManager->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void MusicSongSearchOnlineTableWidget::setSearchQuality(const QString &quality)
{
    m_downLoadManager->setSearchQuality(quality);
}

void MusicSongSearchOnlineTableWidget::searchDataDwonloadFinished()
{
    if(m_downloadData.isValid())
    {
        emit muiscSongToPlayListChanged(m_downloadData.m_songName, m_downloadData.m_time,
                                        m_downloadData.m_format, true);
    }
    m_downloadData.clear();
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
    item->setText(MusicUtils::UWidget::elidedText(font(), songname, Qt::ElideRight, 300));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::UWidget::elidedText(font(), artistname, Qt::ElideRight, 180));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_add")));
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_download")));
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

void MusicSongSearchOnlineTableWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.4 + 315);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.4 + 198);
    headerview->resizeSection(3, (width - WINDOW_WIDTH_MIN)*0.2 + 60);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::UWidget::elidedText(font(), it->toolTip(), Qt::ElideRight, width - WINDOW_WIDTH_MIN + 300));

        it = item(i, 2);
        it->setText(MusicUtils::UWidget::elidedText(font(), it->toolTip(), Qt::ElideRight, width - WINDOW_WIDTH_MIN + 180));
    }
}

void MusicSongSearchOnlineTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    QAction *playAction = rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"));
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
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
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
    m_audition->setMedia(MusicCoreMPlayer::MusicCategory, musicSongInfos[row].m_songAttrs.first().m_url);

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
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    emit showDownLoadInfoFor(MusicObject::DW_DownLoading);

    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos[row];
    MusicObject::MusicSongAttribute musicSongAttr = musicSongInfo.m_songAttrs.first();
    QString musicSong = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
    QString musicEnSong = MusicCryptographicHash().encrypt(musicSong, DOWNLOAD_KEY);
    QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(musicSongAttr.m_format);
    MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicSongAttr.m_url, downloadName,
                                                                     MusicDownLoadThreadAbstract::Download_Music, this);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), SLOT(searchDataDwonloadFinished()));
    downSong->startToDownload();

//    (new MusicTextDownLoadThread(musicSongInfo.m_lrcUrl, LRC_DIR_FULL + musicSong + LRC_FILE,
//                                 MusicDownLoadThreadAbstract::Download_Lrc, this))->startToDownload();
#ifndef USE_MULTIPLE_QUERY
    (new MusicData2DownloadThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + musicSongInfo.m_singerName + SKN_FILE,
                                  MusicDownLoadThreadAbstract::Download_SmlBG, this))->startToDownload();
#else
    (new MusicDataDownloadThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + musicSongInfo.m_singerName + SKN_FILE,
                                 MusicDownLoadThreadAbstract::Download_SmlBG, this))->startToDownload();
#endif
    ///download big picture
    (new MusicBackgroundDownload(musicSongInfo.m_singerName, musicSongInfo.m_singerName, this))->startToDownload();

    m_downloadData.clear();
    m_downloadData.m_songName = musicEnSong;
    m_downloadData.m_time =  item(row, 3)->text();
    m_downloadData.m_format = musicSongAttr.m_format;
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

    createToolWidget(toolWidget);
    connect(m_searchTableWidget, SIGNAL(auditionIsPlaying(bool)), SLOT(auditionIsPlaying(bool)));
}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    delete m_playButton;
    delete m_textLabel;
    delete m_searchTableWidget;
}

QString MusicSongSearchOnlineWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name)
{
    setResizeLabelText(name);
    m_searchTableWidget->startSearchQuery(name);
}

void MusicSongSearchOnlineWidget::researchQueryByQuality(const QString &name, const QString &quality)
{
    m_searchTableWidget->setSearchQuality(quality);
    startSearchQuery(name);
}

void MusicSongSearchOnlineWidget::buttonClicked(int index)
{
    MusicObject::MIntList list = m_searchTableWidget->getSelectedItems();
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

void MusicSongSearchOnlineWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    setResizeLabelText( m_textLabel->toolTip() );
}

void MusicSongSearchOnlineWidget::createToolWidget(QWidget *widget)
{
    QVBoxLayout *wLayout = new QVBoxLayout(widget);
    wLayout->setContentsMargins(0, 0, 0, 0);
    wLayout->setSpacing(0);
    widget->setLayout(wLayout);

    QWidget *funcWidget = new QWidget(widget);
    QHBoxLayout *funcLayout = new QHBoxLayout(funcWidget);
    funcLayout->setContentsMargins(0, 0, 10, 0);
    funcLayout->setSpacing(10);

    m_textLabel = new QLabel(this);
    m_textLabel->setTextFormat(Qt::RichText);
    m_textLabel->setText(tr("&nbsp;find no result"));
    funcLayout->addWidget(m_textLabel);

    m_playButton = new QPushButton(tr("Play"), this);
    m_playButton->setFixedSize(70, 20);
    m_playButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    funcLayout->addWidget(m_playButton);

    QPushButton *addButton = new QPushButton(tr("Add"), this);
    addButton->setFixedSize(70, 20);
    addButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    funcLayout->addWidget(addButton);

    QPushButton *downloadButton = new QPushButton(tr("Download"), this);
    downloadButton->setFixedSize(70, 20);
    downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    funcLayout->addWidget(downloadButton);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_playButton, 0);
    buttonGroup->addButton(addButton, 1);
    buttonGroup->addButton(downloadButton, 2);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
    funcWidget->setLayout(funcLayout);
    wLayout->addWidget(funcWidget);

    //////////////////////////////////////////////////////////
    QWidget *labelWidget = new QWidget(widget);
    QHBoxLayout *labelLayout = new QHBoxLayout(labelWidget);
    labelLayout->setContentsMargins(7, 0, 10, 0);
    labelLayout->setSpacing(10);
    labelWidget->setStyleSheet(MusicUIObject::MCustomStyle17);

    QCheckBox *label_checkBox = new QCheckBox(this);
    label_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(label_checkBox, SIGNAL(clicked(bool)), m_searchTableWidget,
                            SLOT(setSelectedAllItems(bool)));
    labelLayout->addWidget(label_checkBox, 3);

    QLabel *Label1 = new QLabel(tr("Song"), this);
    Label1->setStyleSheet(MusicUIObject::MCustomStyle18);
    labelLayout->addWidget(Label1, 5);

    QLabel *Label2 = new QLabel(tr("Artist"), this);
    Label2->setStyleSheet(MusicUIObject::MCustomStyle18);
    labelLayout->addWidget(Label2, 3);

    QLabel *Label3 = new QLabel(tr("Operator"), this);
    Label3->setStyleSheet(MusicUIObject::MCustomStyle18);
    labelLayout->addWidget(Label3, 1);

    labelWidget->setLayout(labelLayout);
    wLayout->addWidget(labelWidget);
}

void MusicSongSearchOnlineWidget::setResizeLabelText(const QString &name)
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    width = width - WINDOW_WIDTH_MIN + 240;
    m_textLabel->setText(tr("&nbsp;find <font color=red> %1 </font> result")
                         .arg(MusicUtils::UWidget::elidedText(font(), name, Qt::ElideRight, width)));
    m_textLabel->setToolTip(name);
}
