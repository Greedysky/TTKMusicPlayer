#include "musicalbumquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcerequest.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicstringutils.h"

#include "qrencode/qrcodewidget.h"

#include <qmath.h>

MusicAlbumQueryTableWidget::MusicAlbumQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicAlbumQueryTableWidget::~MusicAlbumQueryTableWidget()
{
    clearAllItems();
}

void MusicAlbumQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAlbumFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAlbumFinished()));
    }
}



MusicAlbumQueryWidget::MusicAlbumQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_shareType = MusicSongSharingWidget::Album;
    m_queryTableWidget = new MusicAlbumQueryTableWidget(this);
    m_queryTableWidget->hide();
    m_downloadRequest = G_DOWNLOAD_QUERY_PTR->getQueryRequest(this);
    connect(m_downloadRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

void MusicAlbumQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_downloadRequest->setQueryAllRecords(false);
    m_downloadRequest->setQuerySimplify(true);
    m_downloadRequest->startToSearch(MusicAbstractQueryRequest::MusicQuery, MusicUtils::String::artistName(name));
}

void MusicAlbumQueryWidget::setSongNameById(const QString &id)
{
    MusicAbstractQueryRequest *v = G_DOWNLOAD_QUERY_PTR->getAlbumRequest(this);
    m_queryTableWidget->setQueryInput(v);
    m_queryTableWidget->startSearchQuery(id);
    connect(v, SIGNAL(createAlbumInfoItem(MusicResultsItem)), SLOT(createAlbumInfoItem(MusicResultsItem)));
}

void MusicAlbumQueryWidget::resizeWindow()
{
    m_queryTableWidget->resizeWindow();
    if(!m_resizeWidgets.isEmpty())
    {
        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN;

        TTKResizeWidget *data = &m_resizeWidgets[1];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 210 + width));

        data = &m_resizeWidgets[2];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180 + width));

        data = &m_resizeWidgets[3];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180 + width));

        data = &m_resizeWidgets[4];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180 + width));

        data = &m_resizeWidgets[5];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180 + width));
    }
}

void MusicAlbumQueryWidget::queryAllFinished()
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downloadRequest->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noAlbum"));
    }
    else
    {
        bool hasItem = false;
        for(const MusicObject::MusicSongInformation &info : qAsConst(musicSongInfos))
        {
            if(m_songNameFull.contains(info.m_songName))
            {
                hasItem = true;
                setSongNameById(info.m_albumId);
                break;
            }
        }

        if(!hasItem)
        {
            m_statusLabel->setPixmap(QPixmap(":/image/lb_noAlbum"));
        }
    }
}

void MusicAlbumQueryWidget::queryAlbumFinished()
{
    MusicAbstractQueryRequest *d = m_queryTableWidget->getQueryInput();
    if(!d)
    {
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(d->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noAlbum"));
    }
    else
    {
        setSongCountText();
    }
}

void MusicAlbumQueryWidget::createAlbumInfoItem(const MusicResultsItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidgets.isEmpty())
    {
        QStringList lists = item.m_description.split(TTK_STR_SPLITER);
        if(lists.count() < 4)
        {
            return;
        }

        MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
        connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
        {
            download->startToDownload(item.m_coverUrl);
        }

        for(int i=0; i<lists.count(); ++i)
        {
            if(lists[i].isEmpty())
            {
                lists[i] = STRING_NULL;
            }
        }

        TTKResizeWidget *data = &m_resizeWidgets[0];
        data->m_label->setText(tr("<font color=#158FE1> Alubm > %1 </font>").arg(lists[0]));

        data = &m_resizeWidgets[1];
        data->m_label->setToolTip(lists[0]);
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 210));

        data = &m_resizeWidgets[2];
        data->m_label->setToolTip(tr("Singer: %1").arg(item.m_name));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180));

        data = &m_resizeWidgets[3];
        data->m_label->setToolTip(tr("Language: %1").arg(lists[1]));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180));

        data = &m_resizeWidgets[4];
        data->m_label->setToolTip(tr("Company: %1").arg(lists[2]));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180));

        data = &m_resizeWidgets[5];
        data->m_label->setToolTip(tr("Year: %1").arg(lists[3]));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180));
    }
}

void MusicAlbumQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01 + MusicUIObject::MQSSPushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    grid->addWidget(firstLabel);
    //
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(210, 180);
    //

    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 5, 0);
    QLabel *albumLabel = new QLabel(topLineWidget);
    QFont albumFont = albumLabel->font();
    albumFont.setPixelSize(20);
    albumLabel->setFont(albumFont);
    albumLabel->setStyleSheet(MusicUIObject::MQSSFontStyle01);
    albumLabel->setText(STRING_NULL);
    QLabel *singerLabel = new QLabel(topLineWidget);
    singerLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    singerLabel->setText(STRING_NULL);
    QLabel *languageLabel = new QLabel(topLineWidget);
    languageLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    languageLabel->setText(STRING_NULL);
    QLabel *companyLabel = new QLabel(topLineWidget);
    companyLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    companyLabel->setText(STRING_NULL);
    QLabel *yearLabel = new QLabel(topLineWidget);
    yearLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    yearLabel->setText(STRING_NULL);

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
    connect(playAllButton, SIGNAL(clicked()), SLOT(playAllButtonClicked()));
    connect(shareButton, SIGNAL(clicked()), SLOT(shareButtonClicked()));
    //
    QWidget *topRightWidget = new QWidget(topFuncWidget);
    QGridLayout *topRightLayout = new QGridLayout(topRightWidget);
    topRightLayout->setContentsMargins(0, 0, 0, 0);
    topRightLayout->setSpacing(0);

    QLabel *numberLabel = new QLabel(topRightWidget);
    numberLabel->setAlignment(Qt::AlignCenter);
    numberLabel->setStyleSheet(MusicUIObject::MQSSFontStyle06 + MusicUIObject::MQSSColorStyle05);
    int number = 9;
    numberLabel->setText(QString("%1.%2").arg(number).arg(1));
    topRightLayout->addWidget(numberLabel, 0, 0);
    for(int i=1; i<=5; ++i)
    {
        QLabel *label = new QLabel(topRightWidget);
        label->setPixmap(QPixmap((ceil(number / 2.0) - i) >= 0 ? ":/tiny/lb_star" : ":/tiny/lb_unstar"));
        topRightLayout->addWidget(label, 0, i);
    }

    QLabel *numberTextLabel = new QLabel(tr("Score:"), topRightWidget);
    topRightLayout->addWidget(numberTextLabel, 1, 0);
    for(int i=1; i<=5; ++i)
    {
        QLabel *label = new QLabel(topRightWidget);
        label->setFixedSize(26, 22);
        label->setPixmap(QPixmap(":/tiny/lb_unstar"));
        topRightLayout->addWidget(label, 1, i);
    }

    QLabel *marginBottmLabel = new QLabel(topRightWidget);
    marginBottmLabel->setFixedHeight(40);
    topRightLayout->addWidget(marginBottmLabel, 2, 0);
    topRightWidget->setLayout(topRightLayout);

    QRCodeQWidget *code = new QRCodeQWidget(QByteArray(), QSize(90, 90), topRightWidget);
    code->setMargin(2);
    code->setIcon(":/image/lb_app_logo", 0.23);
    topRightLayout->addWidget(code, 3, 2, 1, 6);

    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncLayout->addWidget(topRightWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    //

    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    QHBoxLayout *hlayout = new QHBoxLayout(functionWidget);
    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("songItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(m_songButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    connect(group, SIGNAL(buttonClicked(int)), m_container, SLOT(setCurrentIndex(int)));

#ifdef Q_OS_UNIX
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets.push_back({firstLabel, firstLabel->font()});
    m_resizeWidgets.push_back({albumLabel, albumLabel->font()});
    m_resizeWidgets.push_back({singerLabel, singerLabel->font()});
    m_resizeWidgets.push_back({languageLabel, languageLabel->font()});
    m_resizeWidgets.push_back({companyLabel, companyLabel->font()});
    m_resizeWidgets.push_back({yearLabel, yearLabel->font()});
}
