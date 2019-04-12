#include "musicsongsearchonlinewidget.h"
#include "musiclocalsongsearchrecordconfigmanager.h"
#include "musicdownloadbackgroundthread.h"
#include "musicmessagebox.h"
#include "musiccoremplayer.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicdatadownloadthread.h"
#include "musicdownloadqueryfactory.h"
#include "musicrightareawidget.h"
#include "musicgiflabelwidget.h"
#include "musicdownloadbatchwidget.h"
#include "musictoastlabel.h"
#include "musictime.h"
#include "musicapplication.h"
#include "musicwidgetheaders.h"

#include <QButtonGroup>

MusicSongSearchTableWidget::MusicSongSearchTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent), m_mediaPlayer(nullptr)
{
    setColumnCount(9);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 273);
    headerview->resizeSection(2, 105);
    headerview->resizeSection(3, 105);
    headerview->resizeSection(4, 55);
    headerview->resizeSection(5, 23);
    headerview->resizeSection(6, 26);
    headerview->resizeSection(7, 26);
    headerview->resizeSection(8, 26);

    m_previousAuditionRow = -1;
    m_queryAllRecords = true;

    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicSongSearchTableWidget::~MusicSongSearchTableWidget()
{
    auditionStop();
    delete m_mediaPlayer;
    clearAllItems();
}

void MusicSongSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    //
    MusicSearchRecords records;
    MusicLocalSongSearchRecordConfigManager search(this);
    if(!search.readConfig())
    {
        return;
    }
    search.readSearchData( records );
    MusicSearchRecord record;
    record.m_name = text;
    record.m_time = QString::number(MusicTime::timeStamp());
    records.insert(0, record);
    search.writeSearchData( records );
    //
    if(!m_downLoadManager)
    {
        MusicQueryItemTableWidget::startSearchQuery(text);
    }
    else
    {
        const QString &quality = m_downLoadManager->getSearchQuality();
        MusicQueryItemTableWidget::startSearchQuery(text);
        m_downLoadManager->setSearchQuality(quality);
    }
    //
    m_loadingLabel->run(true);
    m_downLoadManager->setQueryAllRecords(m_queryAllRecords);
    m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void MusicSongSearchTableWidget::startSearchSingleQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    //
    if(!m_downLoadManager)
    {
        MusicQueryItemTableWidget::startSearchQuery(text);
    }
    else
    {
        const QString &quality = m_downLoadManager->getSearchQuality();
        MusicQueryItemTableWidget::startSearchQuery(text);
        m_downLoadManager->setSearchQuality(quality);
    }
    //
    m_loadingLabel->run(true);
    m_downLoadManager->setQueryAllRecords(m_queryAllRecords);
    m_downLoadManager->startToSingleSearch(text);
}

void MusicSongSearchTableWidget::musicDownloadLocal(int row)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || (row >= rowCount() - 1) || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongSearchTableWidget::auditionStop()
{
    if(m_mediaPlayer)
    {
        m_mediaPlayer->stop();
    }
}

void MusicSongSearchTableWidget::auditionToMusic(int row)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(musicSongInfos.isEmpty() || row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    MusicApplication *w = MusicApplication::instance();
    if(w->isPlaying())
    {
        w->musicStatePlay();
    }
    ///stop current media play while audition starts.

    if(m_mediaPlayer == nullptr)
    {
        m_mediaPlayer = new MusicCoreMPlayer(this);
        connect(m_mediaPlayer, SIGNAL(finished(int)), SLOT(mediaAutionPlayError(int)));
    }

    m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, musicSongInfos[row].m_songAttrs.first().m_url);
    m_mediaPlayer->play();

    if(m_previousAuditionRow != -1)
    {
        item(m_previousAuditionRow, 0)->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    }
    item(m_previousAuditionRow = row, 0)->setData(MUSIC_AUDIT_ROLE, AUDITION_PLAY);

    emit auditionIsPlaying(false);
}

void MusicSongSearchTableWidget::auditionToMusicStop(int row)
{
    if(m_mediaPlayer)
    {
        m_mediaPlayer->stop();
    }

    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    QTableWidgetItem *it = item(row, 0);
    if(it)
    {
        it->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    }
    emit auditionIsPlaying(true);
}

void MusicSongSearchTableWidget::setSearchQuality(const QString &quality)
{
    MusicQueryItemTableWidget::startSearchQuery(QString());
    m_downLoadManager->setSearchQuality(quality);
}

void MusicSongSearchTableWidget::resizeWindow()
{
    const int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)/3.0 + 273);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)/3.0 + 105);
    headerview->resizeSection(3, (width - WINDOW_WIDTH_MIN)/3.0 + 105);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));

        it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));

        it = item(i, 3);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(3) - 31));
    }
}

void MusicSongSearchTableWidget::listCellEntered(int row, int column)
{
    MusicQueryItemTableWidget::listCellEntered(row, column);
    if(column == 7 || column == 8)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
}

void MusicSongSearchTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 7:
            addSearchMusicToPlaylist(row);
            break;
        case 8:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }

    const QTableWidgetItem *it = item(row, 0);
    if(it)
    {
        emit auditionIsPlaying(it->data(MUSIC_AUDIT_ROLE).toInt() == AUDITION_STOP);
    }
}

void MusicSongSearchTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(9);
}

void MusicSongSearchTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    item->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 30));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 2, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_albumName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(3) - 30));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 3, item);

                      item = new QTableWidgetItem(songItem.m_time);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(songItem.m_type);
    setItem(count, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_add"));
    setItem(count, 7, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_download"));
    setItem(count, 8, item);
}

void MusicSongSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0|| (row >= rowCount() - 1))
    {
        return;
    }
    addSearchMusicToPlaylist(row);
}

void MusicSongSearchTableWidget::actionGroupClick(QAction *action)
{
//    MusicQueryItemTableWidget::actionGroupClick(action);
    const int row = currentRow();
    if(!m_downLoadManager || row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    const MusicObject::MusicSongInformation &info = musicSongInfos[row];

    switch( action->data().toInt() )
    {
        case 0: musicDownloadLocal(row); break;
        case 1: emit restartSearchQuery(info.m_songName); break;
        case 2: MusicRightAreaWidget::instance()->musicArtistFound(info.m_singerName, info.m_artistId); break;
        case 3: emit restartSearchQuery(info.m_singerName + " - " + info.m_songName); break;
        case 4: auditionToMusic(row); break;
        case 5: addSearchMusicToPlaylist(row); break;
        case 6: musicSongDownload(row); break;
        case 7: MusicRightAreaWidget::instance()->musicAlbumFound(info.m_albumName, info.m_albumId); break;
        default: break;
    }
}

void MusicSongSearchTableWidget::searchDataDwonloadFinished()
{
    if(m_downloadData.isValid())
    {
        emit musicSongToPlaylistChanged(m_downloadData.m_songName, m_downloadData.m_time, m_downloadData.m_format, true);
    }
    m_downloadData.clear();
}

void MusicSongSearchTableWidget::musicSongDownload(int row)
{
    if(row < 0)
    {
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongSearchTableWidget::mediaAutionPlayError(int code)
{
    if(DEFAULT_LEVEL_NORMAL == code)
    {
        m_mediaPlayer->stop();

        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->defaultLabel(this, tr("Audio Play Time out!"));
    }
}

void MusicSongSearchTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryItemTableWidget::resizeEvent(event);
    resizeWindow();
}

void MusicSongSearchTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryItemTableWidget::contextMenuEvent(event);

    QMenu rightClickMenu(this);
    m_actionGroup->addAction(rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay")))->setData(4);
    m_actionGroup->addAction(rightClickMenu.addAction(tr("musicAdd")))->setData(5);
    m_actionGroup->addAction(rightClickMenu.addAction(tr("downloadMore...")))->setData(6);

    createContextMenu(rightClickMenu);

    if(!m_actionGroup->actions().isEmpty())
    {
        const QString &albumName = (currentRow() != -1 && rowCount() > 0) ? item(currentRow(), 3)->toolTip() : QString();
        QAction *lastAction = m_actionGroup->actions().last();
        QAction *action = m_actionGroup->addAction(tr("search '%1'").arg(albumName));
        action->setData(7);
        rightClickMenu.insertAction(lastAction, action);
    }
    rightClickMenu.exec(QCursor::pos());
}

void MusicSongSearchTableWidget::addSearchMusicToPlaylist(int row)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }

    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    emit showDownLoadInfoFor(MusicObject::DW_DownLoading);

    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    const MusicObject::MusicSongInformation &musicSongInfo = musicSongInfos[row];
    const MusicObject::MusicSongAttribute &musicSongAttr = musicSongInfo.m_songAttrs.first();

    const QString &musicSong = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
    const QString &musicEnSong = MusicUtils::Algorithm::mdII(musicSong, ALG_DOWNLOAD_KEY, true);
    const QString &downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(musicSongAttr.m_format);

    MusicDataDownloadThread *download = new MusicDataDownloadThread(musicSongAttr.m_url, downloadName, MusicObject::DownloadMusic, this);
    connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(searchDataDwonloadFinished()));
    download->startToDownload();

    M_DOWNLOAD_QUERY_PTR->getDownloadSmallPicThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + musicSongInfo.m_singerName + SKN_FILE,
                                                    MusicObject::DownloadSmallBackground, this)->startToDownload();
    ///download big picture
    M_DOWNLOAD_QUERY_PTR->getDownloadBigPicThread(musicSongInfo.m_singerName, musicSongInfo.m_singerName, this)->startToDownload();

    m_downloadData.clear();
    m_downloadData.m_songName = musicEnSong;
    m_downloadData.m_time =  item(row, 4)->text();
    m_downloadData.m_format = musicSongAttr.m_format;
}



MusicSongSearchOnlineWidget::MusicSongSearchOnlineWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    boxLayout->setMargin(0);
    boxLayout->setSpacing(0);

    QWidget *toolWidget = new QWidget(this);
    toolWidget->setFixedHeight(80);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    toolWidget->setAutoFillBackground(true);
    toolWidget->setPalette(pal);

    m_searchTableWidget = new  MusicSongSearchTableWidget(this);
    boxLayout->addWidget(toolWidget);
    boxLayout->addWidget(m_searchTableWidget);
    setLayout(boxLayout);

    createToolWidget(toolWidget);
    connect(m_searchTableWidget, SIGNAL(auditionIsPlaying(bool)), SLOT(auditionIsPlaying(bool)));
    connect(m_searchTableWidget, SIGNAL(restartSearchQuery(QString)), MusicRightAreaWidget::instance(), SLOT(musicSongSearchedFound(QString)));
}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    qDeleteAll(m_resizeLabels);
    delete m_playButton;
    delete m_textLabel;
    delete m_searchTableWidget;
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name)
{
    startSearchQuery(name, true);
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name, bool all)
{
    setResizeLabelText(name);
    if(m_resizeLabels.count() == 5)
    {
        MStatic_cast(QCheckBox*, m_resizeLabels[4])->setChecked(false);
    }
    m_searchTableWidget->setQueryAllRecords(all);
    m_searchTableWidget->startSearchQuery(name);
}

void MusicSongSearchOnlineWidget::startSearchSingleQuery(const QString &name)
{
    setResizeLabelText(name);
    if(m_resizeLabels.count() == 5)
    {
        MStatic_cast(QCheckBox*, m_resizeLabels[4])->setChecked(false);
    }
    m_searchTableWidget->setQueryAllRecords(true);
    m_searchTableWidget->startSearchSingleQuery(name);
}

void MusicSongSearchOnlineWidget::researchQueryByQuality(const QString &name, const QString &quality)
{
    m_searchTableWidget->setSearchQuality(quality);
    startSearchQuery(name, false);
}

void MusicSongSearchOnlineWidget::resizeWindow()
{
    setResizeLabelText( m_textLabel->toolTip() );
    m_searchTableWidget->resizeWindow();
}

void MusicSongSearchOnlineWidget::auditionStop()
{
    m_searchTableWidget->auditionStop();
}

void MusicSongSearchOnlineWidget::buttonClicked(int index)
{
    MIntList list = m_searchTableWidget->getSelectedItems();
    list.removeOne(m_searchTableWidget->rowCount() - 1);
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
                m_playButton->text() == tr("Play") ? m_searchTableWidget->auditionToMusic(row) : m_searchTableWidget->auditionToMusicStop(row);
                break;
            case 1:
                m_searchTableWidget->listCellClicked(row, 7);
                break;
//            case 2:
//                m_searchTableWidget->listCellClicked(row, 8);
//                break;
            default:
                break;
        }
    }

    if(index == 2)
    {
        MusicDownLoadQueryThreadAbstract *d = m_searchTableWidget->getQueryInput();
        if(!d)
        {
            return;
        }

        MusicObject::MusicSongInformations selectedItems, musicSongInfos(d->getMusicSongInfos());
        foreach(int index, list)
        {
            if(index < 0 || index >= musicSongInfos.count())
            {
                continue;
            }

            selectedItems << musicSongInfos[index];
        }

        MusicDownloadBatchWidget *w = new MusicDownloadBatchWidget(this);
        w->setSongName(selectedItems, MusicDownLoadQueryThreadAbstract::MusicQuery);
        w->show();
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
    m_textLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    funcLayout->addWidget(m_textLabel);

    m_playButton = new QPushButton(tr("Play"), this);
    m_playButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
    m_playButton->setIconSize(QSize(14, 14));
    m_playButton->setFixedSize(70, 25);
    m_playButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    funcLayout->addWidget(m_playButton);

    QPushButton *addButton = new QPushButton(tr("Add"), this);
    addButton->setFixedSize(70, 25);
    addButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    funcLayout->addWidget(addButton);

    QPushButton *downloadButton = new QPushButton(tr("Download"), this);
    downloadButton->setFixedSize(70, 25);
    downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    funcLayout->addWidget(downloadButton);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_playButton, 0);
    buttonGroup->addButton(addButton, 1);
    buttonGroup->addButton(downloadButton, 2);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
    funcWidget->setLayout(funcLayout);
    wLayout->addWidget(funcWidget);

    //
    QWidget *labelWidget = new QWidget(widget);
    QHBoxLayout *labelLayout = new QHBoxLayout(labelWidget);
    labelLayout->setContentsMargins(7, 0, 10, 0);
    labelLayout->setSpacing(10);
    labelWidget->setStyleSheet(MusicUIObject::MBackgroundStyle03);

    QCheckBox *labelCheckBox = new QCheckBox(this);
    labelCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(labelCheckBox, SIGNAL(clicked(bool)), m_searchTableWidget, SLOT(setSelectedAllItems(bool)));
    labelLayout->addWidget(labelCheckBox, 3);

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
    addButton->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
    labelCheckBox->setFocusPolicy(Qt::NoFocus);
#endif

    QLabel *label1 = new QLabel(tr("Song"), this);
    label1->setAlignment(Qt::AlignCenter);
    label1->setStyleSheet(MusicUIObject::MFontStyle01);
    labelLayout->addWidget(label1, 1);
    m_resizeLabels << label1;

    QLabel *label2 = new QLabel(tr("Artist"), this);
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet(MusicUIObject::MFontStyle01);
    labelLayout->addWidget(label2, 1);
    m_resizeLabels << label2;

    QLabel *label3 = new QLabel(tr("Album"), this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setStyleSheet(MusicUIObject::MFontStyle01);
    labelLayout->addWidget(label3, 3);
    m_resizeLabels << label3;

    QLabel *label4 = new QLabel(tr("Operator"), this);
    label4->setAlignment(Qt::AlignCenter);
    label4->setStyleSheet(MusicUIObject::MFontStyle01);
    labelLayout->addWidget(label4, 4);
    m_resizeLabels << label4;

    m_resizeLabels << labelCheckBox;

    labelWidget->setLayout(labelLayout);
    wLayout->addWidget(labelWidget);
}

void MusicSongSearchOnlineWidget::setResizeLabelText(const QString &name)
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    if(m_resizeLabels.count() == 5)
    {
        m_resizeLabels[0]->setFixedWidth((width - WINDOW_WIDTH_MIN)/3.0 + 270);
        m_resizeLabels[1]->setFixedWidth((width - WINDOW_WIDTH_MIN)/3.0 + 255);
        m_resizeLabels[2]->setFixedWidth((width - WINDOW_WIDTH_MIN)/3.0 + 90);
        m_resizeLabels[3]->setFixedWidth((width - WINDOW_WIDTH_MIN)*0 + 150);
    }

    width = width - WINDOW_WIDTH_MIN + 240;
    m_textLabel->setText(tr("&nbsp;find <font color=#158FE1> %1 </font> result").arg(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, width)));
    m_textLabel->setToolTip(name);
}
