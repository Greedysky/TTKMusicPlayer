#include "musicsongsearchonlinewidget.h"
#include "musicbackgrounddownload.h"
#include "musiclocalsongsearchrecordconfigmanager.h"
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
#include "musictime.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>

MusicSongSearchOnlineTableWidget::MusicSongSearchOnlineTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent), m_audition(nullptr)
{
    setColumnCount(8);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 293);
    headerview->resizeSection(2, 193);
    headerview->resizeSection(3, 55);
    headerview->resizeSection(4, 23);
    headerview->resizeSection(5, 26);
    headerview->resizeSection(6, 26);
    headerview->resizeSection(7, 26);

    m_previousAuditionRow = -1;
    m_queryAllRecords = true;

    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicSongSearchOnlineTableWidget::~MusicSongSearchOnlineTableWidget()
{
    auditionStop();
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
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    ////////////////////////////////////////////////
    MusicSearchRecords records;
    MusicLocalSongSearchRecordConfigManager search(this);
    if(!search.readSearchXMLConfig())
    {
        return;
    }
    search.readSearchConfig( records );
    MusicSearchRecord record;
    record.m_name = text;
    record.m_time = QString::number(MusicTime::timeStamp());
    records.insert(0, record);
    search.writeSearchConfig( records );
    ////////////////////////////////////////////////
    if(!m_downLoadManager)
    {
        MusicQueryItemTableWidget::startSearchQuery(text);
    }
    else
    {
        QString quality = m_downLoadManager->getSearchQuality();
        MusicQueryItemTableWidget::startSearchQuery(text);
        m_downLoadManager->setSearchQuality(quality);
    }
    ////////////////////////////////////////////////
    m_loadingLabel->show();
    m_loadingLabel->start();
    m_downLoadManager->setQueryAllRecords(m_queryAllRecords);
    m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
}

void MusicSongSearchOnlineTableWidget::musicDownloadLocal(int row)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || (row >= rowCount() - 1) || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongSearchOnlineTableWidget::auditionStop()
{
    if(m_audition)
    {
        m_audition->stop();
    }
}

void MusicSongSearchOnlineTableWidget::auditionToMusic(int row)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(musicSongInfos.isEmpty() || row < 0 || (row >= rowCount() - 1))
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

    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    item(row, 0)->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    emit auditionIsPlaying(true);
}

void MusicSongSearchOnlineTableWidget::setSearchQuality(const QString &quality)
{
    MusicQueryItemTableWidget::startSearchQuery(QString());
    m_downLoadManager->setSearchQuality(quality);
}

void MusicSongSearchOnlineTableWidget::resizeWindow()
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN)*0.4 + 293);
    headerview->resizeSection(2, (width - WINDOW_WIDTH_MIN)*0.4 + 173);
    headerview->resizeSection(3, (width - WINDOW_WIDTH_MIN)*0.2 + 55);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, width - WINDOW_WIDTH_MIN + 290));

        it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, width - WINDOW_WIDTH_MIN + 170));
    }
}

void MusicSongSearchOnlineTableWidget::listCellEntered(int row, int column)
{
    MusicQueryItemTableWidget::listCellEntered(row, column);
    if(column == 6 || column == 7)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
}

void MusicSongSearchOnlineTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 6:
            addSearchMusicToPlayList(row);
            break;
        case 7:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }

    emit auditionIsPlaying( item(row, 0)->data(MUSIC_AUDIT_ROLE).toInt() == AUDITION_STOP );
}

void MusicSongSearchOnlineTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(8);
}

void MusicSongSearchOnlineTableWidget::createSearchedItems(const MusicSearchedItem &songItem)
{
    int count = rowCount();
    setRowCount(count + 1);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    item->setData(MUSIC_AUDIT_ROLE, AUDITION_STOP);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), songItem.m_songname, Qt::ElideRight, 285));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songItem.m_songname);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), songItem.m_artistname, Qt::ElideRight, 165));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songItem.m_artistname);
    setItem(count, 2, item);

                      item = new QTableWidgetItem(songItem.m_time);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/tiny/lb_server_type")));
    item->setToolTip(songItem.m_type);
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_add")));
    setItem(count, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_download")));
    setItem(count, 7, item);
}

void MusicSongSearchOnlineTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0|| (row >= rowCount() - 1))
    {
        return;
    }
    addSearchMusicToPlayList(row);
}

void MusicSongSearchOnlineTableWidget::actionGroupClick(QAction *action)
{
    MusicQueryItemTableWidget::actionGroupClick(action);
    int row = currentRow();
    switch( action->data().toInt() )
    {
        case 4: auditionToMusic(row); break;
        case 5: addSearchMusicToPlayList(row); break;
        case 6: musicSongDownload(row); break;
    }
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

void MusicSongSearchOnlineTableWidget::musicSongDownload(int row)
{
    if(row < 0)
    {
        return;
    }

    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongSearchOnlineTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryItemTableWidget::resizeEvent(event);
    resizeWindow();
}

void MusicSongSearchOnlineTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryItemTableWidget::contextMenuEvent(event);

    QMenu rightClickMenu(this);
    m_actionGroup->addAction( rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay")) )->setData(4);
    m_actionGroup->addAction( rightClickMenu.addAction(tr("musicAdd")) )->setData(5);
    m_actionGroup->addAction( rightClickMenu.addAction(tr("downloadMore...")) )->setData(6);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}

void MusicSongSearchOnlineTableWidget::addSearchMusicToPlayList(int row)
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
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

    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicObject::MusicSongInfomation musicSongInfo = musicSongInfos[row];
    MusicObject::MusicSongAttribute musicSongAttr = musicSongInfo.m_songAttrs.first();
    QString musicSong = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
    QString musicEnSong = MusicUtils::Algorithm::mdII(musicSong, ALG_DOWNLOAD_KEY, true);
    QString downloadName = QString("%1%2.%3").arg(CACHE_DIR_FULL).arg(musicEnSong).arg(musicSongAttr.m_format);
    MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicSongAttr.m_url, downloadName,
                                                                     MusicDownLoadThreadAbstract::Download_Music, this);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), SLOT(searchDataDwonloadFinished()));
    downSong->startToDownload();

    M_DOWNLOAD_QUERY_PTR->getDownloadSmallPicThread(musicSongInfo.m_smallPicUrl, ART_DIR_FULL + musicSongInfo.m_singerName + SKN_FILE,
                                                    MusicDownLoadThreadAbstract::Download_SmlBG, this)->startToDownload();
    ///download big picture
    (new MusicBackgroundDownload(musicSongInfo.m_singerName, musicSongInfo.m_singerName, this))->startToDownload();

    m_downloadData.clear();
    m_downloadData.m_songName = musicEnSong;
    m_downloadData.m_time =  item(row, 3)->text();
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

    m_searchTableWidget = new  MusicSongSearchOnlineTableWidget(this);
    boxLayout->addWidget(toolWidget);
    boxLayout->addWidget(m_searchTableWidget);
    setLayout(boxLayout);

    createToolWidget(toolWidget);
    connect(m_searchTableWidget, SIGNAL(auditionIsPlaying(bool)), SLOT(auditionIsPlaying(bool)));
    connect(m_searchTableWidget, SIGNAL(restartSearchQuery(QString)), MusicRightAreaWidget::instance(),
                                 SLOT(songResearchButtonSearched(QString)));
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
    startSearchQuery(name, true);
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &name, bool all)
{
    setResizeLabelText(name);
    m_searchTableWidget->setQueryAllRecords(all);
    m_searchTableWidget->startSearchQuery(name);
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
    MusicObject::MIntList list = m_searchTableWidget->getSelectedItems();
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
                m_playButton->text() == tr("Play") ? m_searchTableWidget->auditionToMusic(row)
                                                   : m_searchTableWidget->auditionToMusicStop(row);
                break;
            case 1:
                m_searchTableWidget->listCellClicked(row, 5);
                break;
            case 2:
                m_searchTableWidget->listCellClicked(row, 6);
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

    //////////////////////////////////////////////////////////
    QWidget *labelWidget = new QWidget(widget);
    QHBoxLayout *labelLayout = new QHBoxLayout(labelWidget);
    labelLayout->setContentsMargins(7, 0, 10, 0);
    labelLayout->setSpacing(10);
    labelWidget->setStyleSheet(MusicUIObject::MBackgroundStyle03);

    QCheckBox *labelCheckBox = new QCheckBox(this);
    labelCheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    connect(labelCheckBox, SIGNAL(clicked(bool)), m_searchTableWidget, SLOT(setSelectedAllItems(bool)));
    labelLayout->addWidget(labelCheckBox, 3);

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

    QLabel *label3 = new QLabel(tr("Operator"), this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setStyleSheet(MusicUIObject::MFontStyle01);
    labelLayout->addWidget(label3, 3);
    m_resizeLabels << label3;

    labelWidget->setLayout(labelLayout);
    wLayout->addWidget(labelWidget);
}

void MusicSongSearchOnlineWidget::setResizeLabelText(const QString &name)
{
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    if(m_resizeLabels.count() == 3)
    {
        m_resizeLabels[0]->setFixedWidth((width - WINDOW_WIDTH_MIN)*0.4 + 295);
        m_resizeLabels[1]->setFixedWidth((width - WINDOW_WIDTH_MIN)*0.4 + 285);
        m_resizeLabels[2]->setFixedWidth((width - WINDOW_WIDTH_MIN)*0.2 + 150);
    }

    width = width - WINDOW_WIDTH_MIN + 240;
    m_textLabel->setText(tr("&nbsp;find <font color=#80B7F1> %1 </font> result")
                         .arg(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, width)));
    m_textLabel->setToolTip(name);
}
