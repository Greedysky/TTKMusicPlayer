#include "musicalbumfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musictime.h"

#include "qrencode/qrcodewidget.h"

#include <qmath.h>

MusicAlbumFoundTableWidget::MusicAlbumFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicAlbumFoundTableWidget::~MusicAlbumFoundTableWidget()
{
    clearAllItems();
}

void MusicAlbumFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAlbumFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAlbumFinished()));
    }
}



MusicAlbumFoundWidget::MusicAlbumFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_shareType = MusicSongSharingWidget::Album;
    m_foundTableWidget = new MusicAlbumFoundTableWidget(this);
    m_foundTableWidget->hide();
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

void MusicAlbumFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->setQuerySimplify(true);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, MusicUtils::String::artistName(name));
}

void MusicAlbumFoundWidget::setSongNameById(const QString &id)
{
    MusicDownLoadQueryThreadAbstract *v = M_DOWNLOAD_QUERY_PTR->getAlbumThread(this);
    m_foundTableWidget->setQueryInput(v);
    m_foundTableWidget->startSearchQuery(id);
    connect(v, SIGNAL(createAlbumInfoItem(MusicResultsItem)), SLOT(createAlbumInfoItem(MusicResultsItem)));
}

void MusicAlbumFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();
    if(!m_resizeWidgets.isEmpty())
    {
        int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN;

        QLabel *label = m_resizeWidgets[1];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[2];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[3];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[4];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[5];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));
    }
}

void MusicAlbumFoundWidget::queryAllFinished()
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(musicSongInfos.isEmpty())
    {
        m_statusLabel->setPixmap(QPixmap(":/image/lb_noAlbum"));
    }
    else
    {
        bool hasItem = false;
        foreach(const MusicObject::MusicSongInformation &info, musicSongInfos)
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

void MusicAlbumFoundWidget::queryAlbumFinished()
{
    MusicDownLoadQueryThreadAbstract *d = m_foundTableWidget->getQueryInput();
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

void MusicAlbumFoundWidget::createAlbumInfoItem(const MusicResultsItem &item)
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

        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
        {
            download->startToDownload(item.m_coverUrl);
        }

        for(int i=0; i<lists.count(); ++i)
        {
            if(lists[i].isEmpty())
            {
                lists[i] = "-";
            }
        }

        QLabel *label = m_resizeWidgets[0];
        label->setText(tr("<font color=#158FE1> Alubm > %1 </font>").arg(lists[0]));

        label = m_resizeWidgets[1];
        label->setToolTip(lists[0]);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[2];
        label->setToolTip(tr("Singer: %1").arg(item.m_name));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[3];
        label->setToolTip(tr("Language: %1").arg(lists[1]));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[4];
        label->setToolTip(tr("Company: %1").arg(lists[2]));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));

        label = m_resizeWidgets[5];
        label->setToolTip(tr("Year: %1").arg(lists[3]));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220));
    }
}

void MusicAlbumFoundWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    m_container->show();

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
    albumLabel->setStyleSheet(MusicUIObject::MFontStyle01);
    albumLabel->setText("-");
    QLabel *singerLabel = new QLabel(topLineWidget);
    singerLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    singerLabel->setText("-");
    QLabel *languageLabel = new QLabel(topLineWidget);
    languageLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    languageLabel->setText("-");
    QLabel *companyLabel = new QLabel(topLineWidget);
    companyLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    companyLabel->setText("-");
    QLabel *yearLabel = new QLabel(topLineWidget);
    yearLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    yearLabel->setText("-");

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
    numberLabel->setStyleSheet(MusicUIObject::MFontStyle06 + MusicUIObject::MColorStyle05);
    int number = 9;
    numberLabel->setText(QString("%1.%2").arg(number).arg(1));
    topRightLayout->addWidget(numberLabel, 0, 0);
    for(int i=1; i<=5; ++i)
    {
        QLabel *label = new QLabel(topRightWidget);
        label->setPixmap(QPixmap( (ceil(number/2.0) - i) >= 0 ? ":/tiny/lb_star" : ":/tiny/lb_unstar"));
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
    functionWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
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

    m_resizeWidgets << firstLabel << albumLabel << singerLabel << languageLabel << companyLabel << yearLabel;
}
