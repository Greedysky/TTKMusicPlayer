#include "musicabstractitemquerywidget.h"
#include "musicimageutils.h"

MusicAbstractItemQueryWidget::MusicAbstractItemQueryWidget(QWidget *parent)
    : QWidget(parent),
      m_songButton(nullptr),
      m_iconLabel(nullptr),
      m_statusLabel(nullptr),
      m_infoLabel(nullptr),
      m_tableWidget(nullptr),
      m_networkRequest(nullptr),
      m_shareType(MusicSongSharingWidget::Module::Null)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_mainWidget= new QWidget(this);
    m_mainWidget->setObjectName("MainWidget");
    m_mainWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_mainWidget->objectName(), TTK::UI::BackgroundStyle10));

    m_statusLabel = new QLabel(tr("Loading now ... "), m_mainWidget);
    m_statusLabel->setStyleSheet(TTK::UI::FontStyle05 + TTK::UI::FontStyle01);

    QVBoxLayout *mLayout = new QVBoxLayout(m_mainWidget);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWidget->setLayout(mLayout);

    m_container = new QStackedWidget(this);
    m_container->hide();

    layout->addWidget(m_mainWidget);
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
    delete m_tableWidget;
    delete m_mainWidget;
    delete m_networkRequest;
}

void MusicAbstractItemQueryWidget::setCurrentValue(const QString &value)
{
    m_value = value;
}

void MusicAbstractItemQueryWidget::setCurrentKey(const QString &id)
{
    Q_UNUSED(id);
}

void MusicAbstractItemQueryWidget::downloadFinished(const QByteArray &bytes)
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

        if(!pix.isNull())
        {
            QPixmap cv(":/image/lb_playlist_cover");
            pix = pix.scaled(QSize(180, 180));
            TTK::Image::fusionPixmap(cv, pix, QPoint(0, 0));
            m_iconLabel->setPixmap(cv);
        }
    }
}

void MusicAbstractItemQueryWidget::playAllButtonClicked()
{
    m_tableWidget->checkedItemsState(true);
    m_tableWidget->downloadDataFrom(true);
}

void MusicAbstractItemQueryWidget::shareButtonClicked()
{
    MusicSongSharingWidget::Item item;
    item.m_id = m_currentPlaylistItem.m_id;
    item.m_name = m_currentPlaylistItem.m_name;
    item.m_cover = m_currentPlaylistItem.m_coverUrl;

    const MusicAbstractQueryRequest *req = m_tableWidget->queryInput();
    if(req)
    {
        item.m_server  = req->queryServer();
    }

    MusicSongSharingWidget widget(this);
    widget.initialize(m_shareType, item);
    widget.exec();
}

void MusicAbstractItemQueryWidget::playButtonClicked()
{
    m_tableWidget->downloadDataFrom(true);
}

void MusicAbstractItemQueryWidget::downloadButtonClicked()
{
    m_tableWidget->downloadBatchData();
}

void MusicAbstractItemQueryWidget::addButtonClicked()
{
    m_tableWidget->downloadDataFrom(false);
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

void MusicAbstractItemQueryWidget::createFirstWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(songWidget);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    middleFuncWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 5, 0, 5);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(TTK_SPACE + tr("All"), middleFuncWidget);
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

    connect(allCheckBox, SIGNAL(clicked(bool)), m_tableWidget, SLOT(checkedItemsState(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));

    vLayout->addWidget(middleFuncWidget);
    vLayout->addWidget(m_tableWidget);
    vLayout->addStretch(1);
    songWidget->setLayout(vLayout);

    m_tableWidget->show();
    m_container->addWidget(songWidget);
}

void MusicAbstractItemQueryWidget::createSecondWidget()
{
    QWidget *songWidget = new QWidget(m_container);
    QVBoxLayout *vLayout = new QVBoxLayout(songWidget);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    m_infoLabel = new QLabel(this);
    m_infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle03);
    vLayout->addWidget(m_infoLabel);

    songWidget->setLayout(vLayout);
    m_container->addWidget(songWidget);
}

void MusicAbstractItemQueryWidget::setSongCountText()
{
    const MusicAbstractQueryRequest *req = m_tableWidget->queryInput();
    if(!req)
    {
        return;
    }

    if(m_songButton)
    {
        m_songButton->setText(tr("SongItems") + QString("(%1)").arg(req->items().count()));
    }
}
