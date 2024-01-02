#include "musicalbumquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musiccoverrequest.h"
#include "musicratinglabel.h"

#include "qrencode/qrcodewidget.h"

MusicAlbumQueryTableWidget::MusicAlbumQueryTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicAlbumQueryTableWidget::~MusicAlbumQueryTableWidget()
{
    removeItems();
}

void MusicAlbumQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAlbumFinished()") != -1)
    {
        connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAlbumFinished()));
    }
}



MusicAlbumQueryWidget::MusicAlbumQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_shareType = MusicSongSharingWidget::Module::Album;
    m_queryTableWidget = new MusicAlbumQueryTableWidget(this);
    m_queryTableWidget->hide();

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

void MusicAlbumQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    m_networkRequest->setQueryType(MusicAbstractQueryRequest::QueryType::Music);
    m_networkRequest->startToSearch(TTK::String::artistName(value));
}

void MusicAlbumQueryWidget::setCurrentID(const QString &id)
{
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeAlbumRequest(this);
    m_queryTableWidget->setQueryInput(d);
    m_queryTableWidget->startSearchQuery(id);
    connect(d, SIGNAL(createAlbumItem(MusicResultDataItem)), SLOT(createAlbumItem(MusicResultDataItem)));
}

void MusicAlbumQueryWidget::resizeWidget()
{
    m_queryTableWidget->resizeSection();

    if(!m_resizeWidgets.isEmpty())
    {
        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN + 180;

        TTKResizeWidget *data = &m_resizeWidgets[1];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width + 30));

        data = &m_resizeWidgets[2];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[3];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[4];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[5];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
    }
}

void MusicAlbumQueryWidget::queryAllFinished()
{
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(songInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_no_album_found"));
    }
    else
    {
        bool hasItem = false;
        for(const TTK::MusicSongInformation &info : qAsConst(songInfos))
        {
            if(m_value.contains(info.m_songName))
            {
                hasItem = true;
                setCurrentID(info.m_albumId);
                break;
            }
        }

        if(!hasItem)
        {
            m_statusLabel->setPixmap(QPixmap(":/image/lb_no_album_found"));
        }
    }
}

void MusicAlbumQueryWidget::queryAlbumFinished()
{
    const MusicAbstractQueryRequest *d = m_queryTableWidget->queryInput();
    if(!d)
    {
        return;
    }

    const TTK::MusicSongInformationList songInfos(d->songInfoList());
    if(songInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_no_album_found"));
    }
    else
    {
        setSongCountText();
    }
}

void MusicAlbumQueryWidget::createAlbumItem(const MusicResultDataItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidgets.isEmpty())
    {
        if(TTK::isCoverValid(item.m_coverUrl))
        {
            MusicCoverRequest *d = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
            connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            d->startRequest(item.m_coverUrl);
        }

        QStringList list{item.m_count, item.m_category, item.m_description, item.m_updateTime};
        for(int i = 0; i < list.count(); ++i)
        {
            if(list[i].isEmpty())
            {
                list[i] = TTK_DEFAULT_STR;
            }
        }

        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN + 180;

        TTKResizeWidget *data = &m_resizeWidgets[0];
        data->m_label->setText(tr("<font color=#158FE1> Alubm > %1 </font>").arg(list[0]));

        data = &m_resizeWidgets[1];
        data->m_label->setToolTip(list[0]);
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width + 30));

        data = &m_resizeWidgets[2];
        data->m_label->setToolTip(tr("Singer: %1").arg(item.m_name));
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[3];
        data->m_label->setToolTip(tr("Language: %1").arg(list[1]));
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[4];
        data->m_label->setToolTip(tr("Company: %1").arg(list[2]));
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[5];
        data->m_label->setToolTip(tr("Year: %1").arg(list[3]));
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
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
    TTK::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
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
    albumLabel->setStyleSheet(TTK::UI::FontStyle01);
    albumLabel->setText(TTK_DEFAULT_STR);
    QLabel *singerLabel = new QLabel(topLineWidget);
    singerLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    singerLabel->setText(TTK_DEFAULT_STR);
    QLabel *languageLabel = new QLabel(topLineWidget);
    languageLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    languageLabel->setText(TTK_DEFAULT_STR);
    QLabel *companyLabel = new QLabel(topLineWidget);
    companyLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    companyLabel->setText(TTK_DEFAULT_STR);
    QLabel *yearLabel = new QLabel(topLineWidget);
    yearLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    yearLabel->setText(TTK_DEFAULT_STR);

    topLineLayout->addWidget(albumLabel);
    topLineLayout->addWidget(singerLabel);
    topLineLayout->addWidget(languageLabel);
    topLineLayout->addWidget(companyLabel);
    topLineLayout->addWidget(yearLabel);
    topLineWidget->setLayout(topLineLayout);

    QWidget *topButtonWidget = new QWidget(topFuncWidget);
    QHBoxLayout *topButtonLayout = new QHBoxLayout(topButtonWidget);
    topButtonLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *playAllButton = new QPushButton(tr("Play All"), topButtonWidget);
    QPushButton *shareButton = new QPushButton(tr("Share"), topButtonWidget);
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
    numberLabel->setStyleSheet(TTK::UI::FontStyle05 + TTK::UI::ColorStyle11);

    const int number = 7 + TTK::random(3);
    numberLabel->setText(QString("%1.%2").arg(number).arg(TTK::random(10)));
    topRightLayout->addWidget(numberLabel, 0, 0);
    topRightLayout->addWidget(new MusicRatingLabel(ceil(number / 2.0), topRightWidget), 0, 1, 1, 6);

    QLabel *numberTextLabel = new QLabel(tr("Score:"), topRightWidget);
    topRightLayout->addWidget(numberTextLabel, 1, 0);
    topRightLayout->addWidget(new MusicRatingLabel(topRightWidget), 1, 1, 1, 6);

    QLabel *marginBottmLabel = new QLabel(topRightWidget);
    marginBottmLabel->setFixedHeight(40);
    topRightLayout->addWidget(marginBottmLabel, 2, 0);
    topRightWidget->setLayout(topRightLayout);

    QRCodeQWidget *code = new QRCodeQWidget({}, QSize(90, 90), topRightWidget);
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
    functionWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *hlayout = new QHBoxLayout(functionWidget);
    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("SongItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(m_songButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_songButton, 0);
    QtButtonGroupConnect(buttonGroup, m_container, setCurrentIndex, TTK_SLOT);

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
