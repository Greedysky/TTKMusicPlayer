#include "musicplaylistfoundinfowidget.h"
#include "musicplaylistfoundtablewidget.h"
#include "musicdownloadsourcethread.h"
#include "musicplaylistfoundcommentswidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicwidgetutils.h"
#include "musictime.h"

#include "qrencode/qrcodewidget.h"

#include <qmath.h>

MusicPlaylistFoundInfoWidget::MusicPlaylistFoundInfoWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_shareType = MusicSongSharingWidget::Playlist;
    m_commentsWidget = nullptr;
    m_foundTableWidget = new MusicPlaylistFoundTableWidget(this);
    m_container->show();

    initFirstWidget();
    initSecondWidget();
//    initThirdWidget();
}

MusicPlaylistFoundInfoWidget::~MusicPlaylistFoundInfoWidget()
{
    delete m_commentsWidget;
}

void MusicPlaylistFoundInfoWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();
    if(!m_resizeWidgets.isEmpty())
    {
        int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN;

        QLabel *label = m_resizeWidgets[0];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[1];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[2];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidgets[3];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));
    }
}

void MusicPlaylistFoundInfoWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicPlaylistFoundInfoWidget::setMusicResultsItem(const MusicResultsItem &item, QObject *obj)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_currentPlaylistItem = item;

    setSongName(item.m_id);
    m_infoLabel->setText(item.m_description);
    m_foundTableWidget->startSearchQuery(item.m_id);

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QWidget *firstTopFuncWidget = new QWidget(function);
    QHBoxLayout *firstTopFuncLayout = new QHBoxLayout(firstTopFuncWidget);
    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> Playlist > %1 </font>").arg(item.m_name));
    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    backButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(backButton, SIGNAL(clicked()), obj, SLOT(backToPlaylistMenu()));
    firstTopFuncLayout->addWidget(firstLabel);
    firstTopFuncLayout->addWidget(backButton);
    grid->addWidget(firstTopFuncWidget);
    //
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(210, 180);

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }

    //
    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 5, 0);
    QLabel *playlistLabel = new QLabel(topLineWidget);
    QFont playlistFont = playlistLabel->font();
    playlistFont.setPixelSize(20);
    playlistLabel->setFont(playlistFont);
    playlistLabel->setStyleSheet(MusicUIObject::MFontStyle01);
    playlistLabel->setToolTip(item.m_name);
    playlistLabel->setText(MusicUtils::Widget::elidedText(playlistFont, playlistLabel->toolTip(), Qt::ElideRight, 220));
    QLabel *creatorLabel = new QLabel(topLineWidget);
    creatorLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    creatorLabel->setToolTip(tr("Creator: %1").arg(item.m_nickName));
    creatorLabel->setText(MusicUtils::Widget::elidedText(creatorLabel->font(), creatorLabel->toolTip(), Qt::ElideRight, 220));
    QLabel *tagsLabel = new QLabel(topLineWidget);
    tagsLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    tagsLabel->setToolTip(tr("Tags: %1").arg(item.m_tags));
    tagsLabel->setText(MusicUtils::Widget::elidedText(tagsLabel->font(), tagsLabel->toolTip(), Qt::ElideRight, 220));
    QLabel *updateLabel = new QLabel(topLineWidget);
    updateLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    updateLabel->setToolTip(tr("Update: %1").arg(item.m_updateTime));
    updateLabel->setText(MusicUtils::Widget::elidedText(updateLabel->font(), updateLabel->toolTip(), Qt::ElideRight, 220));

    topLineLayout->addWidget(playlistLabel);
    topLineLayout->addWidget(creatorLabel);
    topLineLayout->addWidget(tagsLabel);
    topLineLayout->addWidget(updateLabel);
    topLineWidget->setLayout(topLineLayout);

    QWidget *topButtonWidget = new QWidget(topFuncWidget);
    topButtonWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
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
    QPushButton *infoButton = new QPushButton(functionWidget);
    infoButton->setText(tr("Info"));
    infoButton->setFixedSize(100, 25);
    infoButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(infoButton);
    functionWidget->setLayout(hlayout);
    QPushButton *commentsButton = new QPushButton(functionWidget);
    commentsButton->setText(tr("Comments"));
    commentsButton->setFixedSize(100, 25);
    commentsButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(commentsButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    group->addButton(infoButton, 1);
    group->addButton(commentsButton, 2);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setCurrentIndex(int)));

    grid->addWidget(functionWidget);
    //

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    playAllButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
    commentsButton->setFocusPolicy(Qt::NoFocus);
#endif

    grid->addWidget(m_container);
    grid->addStretch(1);
    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets << playlistLabel << creatorLabel << tagsLabel << updateLabel;
}

void MusicPlaylistFoundInfoWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    m_foundTableWidget->setQueryInput(query);
    MStatic_cast(MusicPlaylistFoundTableWidget*, m_foundTableWidget)->setConnectObject(this);
}

void MusicPlaylistFoundInfoWidget::setCurrentIndex(int index)
{
    if(m_foundTableWidget) m_foundTableWidget->hide();
    if(m_infoLabel) m_infoLabel->hide();

    delete m_commentsWidget;
    m_commentsWidget = nullptr;

    if(index == 0)
    {
        m_foundTableWidget->show();
    }
    else if(index == 1)
    {
        m_infoLabel->show();
    }
    else if(index == 2)
    {
        initThirdWidget();
    }

    m_container->setCurrentIndex(index);
}

void MusicPlaylistFoundInfoWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicPlaylistFoundInfoWidget::initThirdWidget()
{
    m_commentsWidget = new MusicPlaylistFoundCommentsWidget(this);
    m_commentsWidget->initWidget(false);
    m_container->addWidget(m_commentsWidget);
    m_commentsWidget->setCurrentSongName(m_songNameFull);
}
