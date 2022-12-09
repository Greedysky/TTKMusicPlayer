#include "musicabstractitemquerywidget.h"
#include "musicimageutils.h"

MusicAbstractItemQueryWidget::MusicAbstractItemQueryWidget(QWidget *parent)
    : QWidget(parent),
      m_mainWindow(nullptr),
      m_songButton(nullptr),
      m_iconLabel(nullptr),
      m_statusLabel(nullptr),
      m_infoLabel(nullptr),
      m_queryTableWidget(nullptr),
      m_networkRequest(nullptr),
      m_shareType(MusicSongSharingWidget::Module::Null)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    initialize();

    m_container = new QStackedWidget(this);
    m_container->hide();

    layout->addWidget(m_mainWindow);
    setLayout(layout);
}

MusicAbstractItemQueryWidget::~MusicAbstractItemQueryWidget()
{
    while(!m_resizeWidgets.isEmpty())
    {
        delete m_resizeWidgets.takeLast().m_label;
    }

    delete m_iconLabel;
    delete m_statusLabel;
    delete m_infoLabel;
    delete m_songButton;
    delete m_queryTableWidget;
    delete m_mainWindow;
    delete m_networkRequest;
}

void MusicAbstractItemQueryWidget::setSongName(const QString &name)
{
    m_songNameFull = name;
}

void MusicAbstractItemQueryWidget::initialize()
{
    m_mainWindow = new QWidget(this);
    m_mainWindow->setObjectName("MainWindow");
    m_mainWindow->setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MQSSBackgroundStyle10));

    m_statusLabel = new QLabel(tr("Loading now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04 + MusicUIObject::MQSSFontStyle01);

    QVBoxLayout *mLayout = new QVBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);
}

void MusicAbstractItemQueryWidget::downLoadFinished(const QByteArray &bytes)
{
    if(m_iconLabel)
    {
        QPixmap pix;
        if(bytes.isEmpty())
        {
            TTK_ERROR_STREAM("Input byte data is empty");
            pix = QPixmap(1, 1);
            pix.fill(Qt::transparent);
        }
        else
        {
            pix.loadFromData(bytes);
        }

        QPixmap cv(":/image/lb_playlist_cover");
        pix = pix.scaled(QSize(180, 180));
        MusicUtils::Image::fusionPixmap(cv, pix, QPoint(0, 0));
        m_iconLabel->setPixmap(cv);
    }
}

void MusicAbstractItemQueryWidget::playAllButtonClicked()
{
    m_queryTableWidget->checkedItemsState(true);
    m_queryTableWidget->downloadDataFrom(true);
}

void MusicAbstractItemQueryWidget::shareButtonClicked()
{
    QVariantMap data;
    data["id"] = m_currentPlaylistItem.m_id;
    data["songName"] = m_currentPlaylistItem.m_name;
    data["smallUrl"] = m_currentPlaylistItem.m_coverUrl;

    const MusicAbstractQueryRequest *d = m_queryTableWidget->queryInput();
    if(d)
    {
        data["queryServer"] = d->queryServer();
    }

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setData(m_shareType, data);
    shareWidget.exec();
}

void MusicAbstractItemQueryWidget::playButtonClicked()
{
    m_queryTableWidget->downloadDataFrom(true);
}

void MusicAbstractItemQueryWidget::downloadButtonClicked()
{
    m_queryTableWidget->downloadBatchData(true);
}

void MusicAbstractItemQueryWidget::addButtonClicked()
{
    m_queryTableWidget->downloadDataFrom(false);
}

void MusicAbstractItemQueryWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicAbstractItemQueryWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicAbstractItemQueryWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicAbstractItemQueryWidget::initFirstWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    middleFuncWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 5, 0, 5);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(" " + tr("All"), middleFuncWidget);
    QPushButton *playButton = new QPushButton(tr("Play"), middleFuncWidget);
    playButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
    playButton->setIconSize(QSize(14, 14));
    playButton->setFixedSize(55, 25);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *addButton = new QPushButton(tr("Add"), middleFuncWidget);
    addButton->setFixedSize(55, 25);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *downloadButton = new QPushButton(tr("Download"), middleFuncWidget);
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
    connect(allCheckBox, SIGNAL(clicked(bool)), m_queryTableWidget, SLOT(checkedItemsState(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));

    vlayout->addWidget(middleFuncWidget);
    //
    vlayout->addWidget(m_queryTableWidget);
    vlayout->addStretch(1);
    songWidget->setLayout(vlayout);

    m_queryTableWidget->show();
    m_container->addWidget(songWidget);
}

void MusicAbstractItemQueryWidget::initSecondWidget()
{
    QWidget *songWidget = new QWidget(m_container);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    m_infoLabel = new QLabel(this);
    m_infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSFontStyle03);
    vlayout->addWidget(m_infoLabel);
    songWidget->setLayout(vlayout);
    m_container->addWidget(songWidget);
}

void MusicAbstractItemQueryWidget::setSongCountText()
{
    const MusicAbstractQueryRequest *d = m_queryTableWidget->queryInput();
    if(!d)
    {
        return;
    }

    const MusicObject::MusicSongInformationList songInfos(d->songInfoList());
    if(m_songButton)
    {
        m_songButton->setText(tr("SongItems") + QString("(%1)").arg(songInfos.count()));
    }
}
