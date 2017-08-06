#include "musicplaylistfoundinfowidget.h"
#include "musicplaylistfoundtablewidget.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicwidgetutils.h"
#include "musictime.h"

#include "qrcodewidget.h"

#include <qmath.h>
#include <QCheckBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QStackedWidget>

MusicPlaylistFoundInfoWidget::MusicPlaylistFoundInfoWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_iconLabel = nullptr;
    m_songButton = nullptr;
    m_container = new QStackedWidget(this);
    m_playlistTableWidget = new MusicPlaylistFoundTableWidget(this);

    initFirstWidget();
    initSecondWidget();
}

MusicPlaylistFoundInfoWidget::~MusicPlaylistFoundInfoWidget()
{
    delete m_iconLabel;
    delete m_infoLabel;
    delete m_songButton;
//    delete m_container;
    delete m_playlistTableWidget;
}

QString MusicPlaylistFoundInfoWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlaylistFoundInfoWidget::resizeWindow()
{
    m_playlistTableWidget->resizeWindow();
    if(!m_resizeWidget.isEmpty())
    {
        int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        width = width - WINDOW_WIDTH_MIN;

        QLabel *label = m_resizeWidget[0];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidget[1];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidget[2];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));

        label = m_resizeWidget[3];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 220 + width));
    }
}

void MusicPlaylistFoundInfoWidget::setMusicPlaylistItem(const MusicPlaylistItem &item, QObject *obj)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_infoLabel->setText(item.m_description);
    m_playlistTableWidget->startSearchQuery(item.m_id);

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

    QWidget *firstTopFuncWidget = new QWidget(function);
    QHBoxLayout *firstTopFuncLayout = new QHBoxLayout(firstTopFuncWidget);
    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#169AF3> Playlist > %1 </font>").arg(item.m_name));
    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(90, 30);
    backButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(backButton, SIGNAL(clicked()), obj, SLOT(backToPlayListMenu()));
    firstTopFuncLayout->addWidget(firstLabel);
    firstTopFuncLayout->addWidget(backButton);
    grid->addWidget(firstTopFuncWidget);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(180, 180);

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(item.m_coverUrl);
    ////////////////////////////////////////////////////////////////////////////

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
    creatorLabel->setToolTip(tr("Creator: %1").arg(item.m_nickname));
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
    ////////////////////////////////////////////////////////////////////////////
    QWidget *topRightWidget = new QWidget(topFuncWidget);
    QGridLayout *topRightLayout = new QGridLayout(topRightWidget);
    topRightLayout->setContentsMargins(0, 0, 0, 0);
    topRightLayout->setSpacing(0);

    MusicTime::timeSRand();
    QLabel *numberLabel = new QLabel(topRightWidget);
    numberLabel->setAlignment(Qt::AlignCenter);
    numberLabel->setStyleSheet(MusicUIObject::MFontStyle06 + MusicUIObject::MColorStyle05);
    int number = qrand()%10;
    numberLabel->setText(QString("%1.%2").arg(number).arg(qrand()%10));
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
    code->setIcon(":/image/lb_player_logo", 0.23);
    topRightLayout->addWidget(code, 3, 2, 1, 6);

    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncLayout->addWidget(topRightWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *functionWidget = new QWidget(this);
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
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    group->addButton(infoButton, 1);
    connect(group, SIGNAL(buttonClicked(int)), m_container, SLOT(setCurrentIndex(int)));

    grid->addWidget(functionWidget);
    ////////////////////////////////////////////////////////////////////////////

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    playAllButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
    infoButton->setFocusPolicy(Qt::NoFocus);
#endif

    grid->addWidget(m_container);
    grid->addStretch(1);
    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidget << playlistLabel << creatorLabel << tagsLabel << updateLabel;
}

void MusicPlaylistFoundInfoWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    m_playlistTableWidget->setQueryInput(query);
    m_playlistTableWidget->setConnectObject(this);
}

void MusicPlaylistFoundInfoWidget::queryAllFinished()
{
    MusicObject::MusicSongInfomations musicSongInfos(m_playlistTableWidget->getMusicSongInfos());
    m_songButton->setText(tr("songItems") + QString("(%1)").arg(musicSongInfos.count()));
}

void MusicPlaylistFoundInfoWidget::downLoadFinished(const QByteArray &data)
{
    if(m_iconLabel)
    {
        QPixmap pix;
        pix.loadFromData(data);
        m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
    }
}

void MusicPlaylistFoundInfoWidget::playAllButtonClicked()
{
    m_playlistTableWidget->setSelectedAllItems(true);
    m_playlistTableWidget->downloadDataFrom(true);
}

void MusicPlaylistFoundInfoWidget::shareButtonClicked()
{

}

void MusicPlaylistFoundInfoWidget::playButtonClicked()
{
    m_playlistTableWidget->downloadDataFrom(true);
}

void MusicPlaylistFoundInfoWidget::downloadButtonClicked()
{
    foreach(int index, m_playlistTableWidget->getSelectedItems())
    {
        m_playlistTableWidget->musicDownloadLocal(index);
    }
}

void MusicPlaylistFoundInfoWidget::addButtonClicked()
{
    m_playlistTableWidget->downloadDataFrom(false);
}

void MusicPlaylistFoundInfoWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}

void MusicPlaylistFoundInfoWidget::initFirstWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 5, 0, 5);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(" " + tr("all"), middleFuncWidget);
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

#ifdef Q_OS_UNIX
    allCheckBox->setFocusPolicy(Qt::NoFocus);
    playButton->setFocusPolicy(Qt::NoFocus);
    addButton->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    middleFuncLayout->addWidget(marginLabel);
    middleFuncLayout->addWidget(allCheckBox);
    middleFuncLayout->addStretch(1);
    middleFuncLayout->addWidget(playButton);
    middleFuncLayout->addWidget(addButton);
    middleFuncLayout->addWidget(downloadButton);
    connect(allCheckBox, SIGNAL(clicked(bool)), m_playlistTableWidget, SLOT(setSelectedAllItems(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));

    vlayout->addWidget(middleFuncWidget);
    //////////////////////////////////////////////////////////////////////
    vlayout->addWidget(m_playlistTableWidget);
    songWidget->setLayout(vlayout);

    m_container->addWidget(songWidget);
}

void MusicPlaylistFoundInfoWidget::initSecondWidget()
{
    QWidget *songWidget = new QWidget(m_container);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    m_infoLabel = new QLabel(this);
    m_infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_infoLabel->setWordWrap(true);
    vlayout->addWidget(m_infoLabel);
    songWidget->setLayout(vlayout);
    m_container->addWidget(songWidget);
}
