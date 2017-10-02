#include "musictoplistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>

MusicTopListFoundTableWidget::MusicTopListFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicTopListFoundTableWidget::~MusicTopListFoundTableWidget()
{
    clearAllItems();
}

QString MusicTopListFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicTopListFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}



MusicTopListFoundWidget::MusicTopListFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_toplistTableWidget = new MusicTopListFoundTableWidget(this);

    MusicDownLoadQueryThreadAbstract *thread = M_DOWNLOAD_QUERY_PTR->getToplistThread(this);
    m_toplistTableWidget->setQueryInput(thread);
    connect(thread, SIGNAL(createToplistInfoItem(MusicPlaylistItem)), SLOT(createToplistInfoItem(MusicPlaylistItem)));
}

MusicTopListFoundWidget::~MusicTopListFoundWidget()
{
    delete m_toplistTableWidget;
}

QString MusicTopListFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicTopListFoundWidget::setSongName(const QString &name)
{
    createLabels();

    MusicFoundAbstractWidget::setSongName(name);
    m_toplistTableWidget->startSearchQuery(QString());
}

void MusicTopListFoundWidget::resizeWindow()
{
    m_toplistTableWidget->resizeWindow();

    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    width = width - WINDOW_WIDTH_MIN;

    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, 410 + width));
    m_playCountLabel->setText(MusicUtils::Widget::elidedText(m_playCountLabel->font(), m_playCountLabel->toolTip(), Qt::ElideRight, 410 + width));
    m_updateTimeLabel->setText(MusicUtils::Widget::elidedText(m_updateTimeLabel->font(), m_updateTimeLabel->toolTip(), Qt::ElideRight, 410 + width));
}

void MusicTopListFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInformations musicSongInfos(m_toplistTableWidget->getMusicSongInfos());
    m_songItemsLabel->setText("|" + tr("songItems") + QString("(%1)").arg(musicSongInfos.count()));
}

void MusicTopListFoundWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

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
    firstLabel->setText(tr("<font color=#169AF3> Rank </font>"));
    grid->addWidget(firstLabel);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(180, 180);

    ////////////////////////////////////////////////////////////////////////////
    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 5, 0);
    m_nameLabel = new QLabel(topLineWidget);
    QFont toplistFont = m_nameLabel->font();
    toplistFont.setPixelSize(20);
    m_nameLabel->setFont(toplistFont);
    m_nameLabel->setStyleSheet(MusicUIObject::MFontStyle01);
    m_playCountLabel = new QLabel(topLineWidget);
    m_playCountLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    m_updateTimeLabel = new QLabel(topLineWidget);
    m_updateTimeLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    m_descriptionLabel = new QLabel(topLineWidget);
    m_descriptionLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    m_descriptionLabel->setWordWrap(true);

    topLineLayout->addWidget(m_nameLabel);
    topLineLayout->addWidget(m_playCountLabel);
    topLineLayout->addWidget(m_updateTimeLabel);
    topLineLayout->addWidget(m_descriptionLabel);
    topLineWidget->setLayout(topLineLayout);

    ////////////////////////////////////////////////////////////////////////////
    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    ////////////////////////////////////////////////////////////////////////////
    m_songItemsLabel = new QLabel("|" + tr("songItems") + QString("(-)"), function);
    m_songItemsLabel->setFixedHeight(50);
    m_songItemsLabel->setStyleSheet(MusicUIObject::MFontStyle04);
    grid->addWidget(m_songItemsLabel);

    QWidget *middleFuncWidget = new QWidget(function);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 0, 0, 0);
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
    connect(allCheckBox, SIGNAL(clicked(bool)), m_toplistTableWidget, SLOT(setSelectedAllItems(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    grid->addWidget(middleFuncWidget);
    //////////////////////////////////////////////////////////////////////
    grid->addWidget(m_toplistTableWidget);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

}

void MusicTopListFoundWidget::createToplistInfoItem(const MusicPlaylistItem &item)
{
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, 410));

    m_playCountLabel->setToolTip(tr("PlayCount: %1").arg(item.m_playCount));
    m_playCountLabel->setText(MusicUtils::Widget::elidedText(m_playCountLabel->font(), m_playCountLabel->toolTip(), Qt::ElideRight, 410));

    m_updateTimeLabel->setToolTip(tr("UpdateTime: %1").arg(item.m_updateTime));
    m_updateTimeLabel->setText(MusicUtils::Widget::elidedText(m_updateTimeLabel->font(), m_updateTimeLabel->toolTip(), Qt::ElideRight, 410));

    m_descriptionLabel->setToolTip(tr("Description: %1").arg(item.m_description));
    m_descriptionLabel->setText(m_descriptionLabel->toolTip());

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != "null")
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicTopListFoundWidget::downLoadFinished(const QByteArray &data)
{
    if(m_iconLabel)
    {
        QPixmap pix;
        pix.loadFromData(data);
        m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
    }
}

void MusicTopListFoundWidget::playButtonClicked()
{
    m_toplistTableWidget->downloadDataFrom(true);
}

void MusicTopListFoundWidget::downloadButtonClicked()
{
    foreach(int index, m_toplistTableWidget->getSelectedItems())
    {
        m_toplistTableWidget->musicDownloadLocal(index);
    }
}

void MusicTopListFoundWidget::addButtonClicked()
{
    m_toplistTableWidget->downloadDataFrom(false);
}
