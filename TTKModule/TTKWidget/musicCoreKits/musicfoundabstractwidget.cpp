#include "musicfoundabstractwidget.h"
#include "musicuiobject.h"

MusicFoundAbstractWidget::MusicFoundAbstractWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mainWindow = nullptr;
    m_songButton = nullptr;
    m_iconLabel = nullptr;
    m_infoLabel = nullptr;
    m_statusLabel = nullptr;
    m_foundTableWidget = nullptr;
    m_downloadThread = nullptr;
    m_container = new QStackedWidget(this);
    m_container->hide();
    m_shareType = MusicSongSharingWidget::Null;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    initWidget();

    layout->addWidget(m_mainWindow);
    setLayout(layout);
}

MusicFoundAbstractWidget::~MusicFoundAbstractWidget()
{
    qDeleteAll(m_resizeWidgets);
    delete m_iconLabel;
    delete m_statusLabel;
    delete m_infoLabel;
    delete m_songButton;
//    delete m_downloadThread;
    delete m_foundTableWidget;
//    delete m_container;
    delete m_mainWindow;
}

void MusicFoundAbstractWidget::setSongName(const QString &name)
{
    m_songNameFull = name;
}

void MusicFoundAbstractWidget::initWidget()
{
    m_mainWindow = new QWidget(this);
    m_mainWindow->setObjectName("MainWindow");
    m_mainWindow->setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MBackgroundStyle17));

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MFontStyle05 + MusicUIObject::MFontStyle01);

    QVBoxLayout *mLayout = new QVBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);
}

void MusicFoundAbstractWidget::downLoadFinished(const QByteArray &data)
{
    if(m_iconLabel)
    {
        QPixmap pix;
        pix.loadFromData(data);
        QPixmap cv(":/image/lb_playlist_cover");
        pix = pix.scaled(QSize(180, 180));
        MusicUtils::Widget::fusionPixmap(cv, pix, QPoint(0, 0));
        m_iconLabel->setPixmap(cv);
    }
}

void MusicFoundAbstractWidget::playAllButtonClicked()
{
    m_foundTableWidget->setSelectedAllItems(true);
    m_foundTableWidget->downloadDataFrom(true);
}

void MusicFoundAbstractWidget::shareButtonClicked()
{
    QVariantMap data;
    data["id"] = m_currentPlaylistItem.m_id;
    data["songName"] = m_currentPlaylistItem.m_name;
    data["smallUrl"] = m_currentPlaylistItem.m_coverUrl;

    const MusicDownLoadQueryThreadAbstract *th = m_foundTableWidget->getQueryInput();
    if(th)
    {
        data["queryServer"] = th->getQueryServer();
    }

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setData(m_shareType, data);
    shareWidget.exec();
}

void MusicFoundAbstractWidget::playButtonClicked()
{
    m_foundTableWidget->downloadDataFrom(true);
}

void MusicFoundAbstractWidget::downloadButtonClicked()
{
    m_foundTableWidget->downloadBatchData(true);
}

void MusicFoundAbstractWidget::addButtonClicked()
{
    m_foundTableWidget->downloadDataFrom(false);
}

void MusicFoundAbstractWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::initFirstWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    middleFuncWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
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
    connect(allCheckBox, SIGNAL(clicked(bool)), m_foundTableWidget, SLOT(setSelectedAllItems(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));

    vlayout->addWidget(middleFuncWidget);
    //
    vlayout->addWidget(m_foundTableWidget);
    vlayout->addStretch(1);
    songWidget->setLayout(vlayout);

    m_foundTableWidget->show();
    m_container->addWidget(songWidget);
}

void MusicFoundAbstractWidget::initSecondWidget()
{
    QWidget *songWidget = new QWidget(m_container);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    m_infoLabel = new QLabel(this);
    m_infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MFontStyle03);
    vlayout->addWidget(m_infoLabel);
    songWidget->setLayout(vlayout);
    m_container->addWidget(songWidget);
}

void MusicFoundAbstractWidget::setSongCountText()
{
    const MusicDownLoadQueryThreadAbstract *d = m_foundTableWidget->getQueryInput();
    if(!d)
    {
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(d->getMusicSongInfos());
    if(m_songButton)
    {
        m_songButton->setText(tr("songItems") + QString("(%1)").arg(musicSongInfos.count()));
    }
}
