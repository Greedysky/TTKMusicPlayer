#include "musicscreensaverwidget.h"
#include "musicapplicationmodule.h"
#include "musictoolsetsuiobject.h"
#include "musicdownloadqueuerequest.h"
#include "musicimageutils.h"
#include "musicwidgetutils.h"
#include "ttkdesktopwrapper.h"
#include "ttkglobalhelper.h"

#include "qsync/qsyncutils.h"

#define OS_ITEM_SIZE QSize(165, 110)

static constexpr int OS_COUNT = 10;
static constexpr int LINE_SPACING_SIZE = 160;

static constexpr const char *OS_SCREEN_DIR = "Screen";
static constexpr const char *OS_SCREENSAVER_URL = "ScreenSaver";
static constexpr const char *OS_WALLPAPER_NAME = "wallpaper.png";
static constexpr const char *OS_WALLBAR_NAME = "wallbar.png";
static constexpr const char *OS_WALLNAIL_NAME = "thumbnail.png";

MusicScreenSaverHoverItem::MusicScreenSaverHoverItem(QLabel *parent)
    : QLabel(parent),
      m_index(-1),
      m_parent(parent)
{
    setFixedSize(OS_ITEM_SIZE + QSize(8, 8));
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(TTK::UI::BackgroundStyle01);

    hide();

    m_enableButton = new QPushButton(this);
    m_enableButton->setCursor(Qt::PointingHandCursor);
    m_enableButton->setStyleSheet(TTK::UI::ScreenItemDisable);
    m_enableButton->setGeometry((width() - 38) / 2, (height() - 38) / 2, 38, 38);
#ifdef Q_OS_UNIX
    m_enableButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_enableButton, SIGNAL(clicked()), SLOT(switchButtonState()));
}

void MusicScreenSaverHoverItem::setFilePath(const QString &path)
{
    m_path = path;
    const QPixmap pix(m_path);
    m_parent->setPixmap(pix);
    setPixmap(pix);
}

void MusicScreenSaverHoverItem::setStatus(int index, bool status)
{
    m_index = index;
    if(!status)
    {
        switchButtonState();
    }
}

void MusicScreenSaverHoverItem::display(const QPoint &point)
{
    move(point - rect().center());
    show();
}

void MusicScreenSaverHoverItem::switchButtonState()
{
    if(m_enableButton->styleSheet().contains(TTK::UI::ScreenItemDisable))
    {
        const QPixmap pix(TTK::Image::grayScalePixmap(QPixmap(m_path), 70));
        m_parent->setPixmap(pix);
        setPixmap(pix);
        m_enableButton->setStyleSheet(TTK::UI::ScreenItemEnable);
        Q_EMIT itemClicked(m_index, false);
    }
    else
    {
        const QPixmap pix(m_path);
        m_parent->setPixmap(pix);
        setPixmap(pix);
        m_enableButton->setStyleSheet(TTK::UI::ScreenItemDisable);
        Q_EMIT itemClicked(m_index, true);
    }
}

void MusicScreenSaverHoverItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    hide();
}

void MusicScreenSaverHoverItem::focusOutEvent(QFocusEvent *event)
{
    QLabel::focusOutEvent(event);
    hide();
}

void MusicScreenSaverHoverItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    TTK::setBorderShadow(this, &painter);

    const QPixmap &pixmap = QtLablePixmap(this);
    painter.drawPixmap(QRect(QPoint(4, 4), OS_ITEM_SIZE), pixmap.scaled(OS_ITEM_SIZE));
}



MusicScreenSaverListItem::MusicScreenSaverListItem(QObject *object, QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(155, 100);

    m_hoverItem = new MusicScreenSaverHoverItem(this);
    m_hoverItem->setParent(parent);
    connect(m_hoverItem, SIGNAL(itemClicked(int,bool)), object, SLOT(currentItemClicked(int,bool)));
}

MusicScreenSaverListItem::~MusicScreenSaverListItem()
{
    delete m_hoverItem;
}

void MusicScreenSaverListItem::setFilePath(const QString &path)
{
    m_hoverItem->setFilePath(path);
}

void MusicScreenSaverListItem::setStatus(int index, bool status)
{
    m_hoverItem->setStatus(index, status);
}

void MusicScreenSaverListItem::setHoverVisible(bool v)
{
    if(v)
    {
        m_hoverItem->display(mapToParent(rect().center()));
    }
    else
    {
        m_hoverItem->hide();
    }
}

void MusicScreenSaverListItem::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);
    setHoverVisible(true);
}



MusicScreenSaverListWidget::MusicScreenSaverListWidget(QWidget *parent)
    : QWidget(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_gridLayout->setContentsMargins(7, 7, 7, 7);
    setLayout(m_gridLayout);
}

MusicScreenSaverListWidget::~MusicScreenSaverListWidget()
{
    qDeleteAll(m_items);
}

void MusicScreenSaverListWidget::addCellItem(QObject *object, const QString &path, int index, bool status)
{
    MusicScreenSaverListItem *item = new MusicScreenSaverListItem(object, this);
    item->setFilePath(path);
    item->setStatus(index, status);

    m_gridLayout->addWidget(item, m_items.count() / 4, m_items.count() % 4, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicScreenSaverListWidget::resizeWindow()
{
    for(MusicScreenSaverListItem *item : qAsConst(m_items))
    {
        item->setHoverVisible(false);
        m_gridLayout->removeWidget(item);
    }

    const int lineNumber = width() / LINE_SPACING_SIZE;
    for(int i = 0; i < m_items.count(); ++i)
    {
        m_gridLayout->addWidget(m_items[i], i / lineNumber, i % lineNumber, Qt::AlignLeft | Qt::AlignTop);
    }
}

void MusicScreenSaverListWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}



MusicScreenSaverWidget::MusicScreenSaverWidget(QWidget *parent)
    : QWidget(parent),
      m_currentState(false)
{
    setStyleSheet(TTK::UI::BackgroundStyle10 + TTK::UI::ColorStyle02);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    //
    QWidget *topWidget = new QWidget(mainWidget);
    topWidget->setFixedHeight(60);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(15, 10, 20, 10);

    QLabel *pLabel = new QLabel(tr("Screen Saver"), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(TTK::UI::ColorStyle01);

    QLabel *iLabel = new QLabel(tr("Popular singers spend your leisure time with you"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);
    iLabel->setStyleSheet(TTK::UI::ColorStyle03);

    QLabel *wLabel = new QLabel(tr("Wait"), topWidget);
    QLabel *mLabel = new QLabel(tr("Min"), topWidget);

    m_inputEdit = new QLineEdit(topWidget);
    m_inputEdit->setFixedWidth(50);
    m_inputEdit->setEnabled(false);
    m_inputEdit->setAlignment(Qt::AlignCenter);
    m_inputEdit->setStyleSheet(TTK::UI::LineEditStyle01);

    m_caseButton = new QPushButton(topWidget);
    m_caseButton->setFixedSize(44, 20);
    m_caseButton->setCursor(Qt::PointingHandCursor);
    m_caseButton->setStyleSheet(TTK::UI::ScreenSaverOff);
#ifdef Q_OS_UNIX
    m_caseButton->setFocusPolicy(Qt::NoFocus);
#endif

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(wLabel);
    topWidgetLayout->addWidget(m_inputEdit);
    topWidgetLayout->addWidget(mLabel);
    topWidgetLayout->addWidget(m_caseButton);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    //
    QFrame *frame = new QFrame(mainWidget);
    frame->setFixedHeight(1);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setStyleSheet(TTK::UI::ColorStyle05);
    mainLayout->addWidget(frame);
    //
    QWidget *functionWidget = new QWidget(mainWidget);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);

    m_backgroundList = new MusicScreenSaverListWidget(functionWidget);
    functionWidgetLayout->addWidget(m_backgroundList);

    connect(m_inputEdit, SIGNAL(textChanged(QString)), SLOT(inputDataChanged()));
    connect(m_caseButton, SIGNAL(clicked()), SLOT(switchButtonState()));

    initialize();
    applyParameter();
}

void MusicScreenSaverWidget::applyParameter()
{
    const bool state = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int value = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();

    m_inputEdit->setText(QString::number(value));
    if(state != m_currentState)
    {
        switchButtonState();
    }
}

QVector<bool> MusicScreenSaverWidget::parseSettingParameter()
{
    QVector<bool> statusVector;
    statusVector.fill(true, OS_COUNT);

    const QString &value = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverIndex).toString();
    const QStringList items(value.split(";"));

    for(const QString &item : qAsConst(items))
    {
        const QStringList itemStatus(item.split(","));
        if(itemStatus.count() == 2)
        {
            const int index = itemStatus[0].toInt();
            const bool status = itemStatus[1].toInt();
            statusVector[index] = status;
        }
    }
    return statusVector;
}

void MusicScreenSaverWidget::resizeWidget()
{
    m_backgroundList->resizeWindow();
}

void MusicScreenSaverWidget::inputDataChanged()
{
    const bool state = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    if(state)
    {
        G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
        MusicApplicationModule::instance()->applyParameter();
    }
}

void MusicScreenSaverWidget::switchButtonState()
{
    if(m_currentState)
    {
        m_currentState = false;
        m_caseButton->setStyleSheet(TTK::UI::ScreenSaverOff);
    }
    else
    {
        m_currentState = true;
        m_caseButton->setStyleSheet(TTK::UI::ScreenSaverOn);
        G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
    }

    m_inputEdit->setEnabled(m_currentState);
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, m_currentState);
    MusicApplicationModule::instance()->applyParameter();
}

void MusicScreenSaverWidget::downLoadFinished(const QString &bytes)
{
    QVector<bool> statusVector(parseSettingParameter());
    if(bytes.contains(OS_WALLNAIL_NAME))
    {
        const int index = TTK::String::splitToken(bytes, OS_SCREEN_DIR, TTK_SEPARATOR).toInt();
        if(index < 0 || index >= statusVector.count())
        {
            return;
        }
        m_backgroundList->addCellItem(this, bytes, index, statusVector[index]);
    }
}

void MusicScreenSaverWidget::currentItemClicked(int index, bool status)
{
    QVector<bool> statusVector(parseSettingParameter());
    statusVector[index] = status;

    QStringList items;
    for(int i = 0; i < statusVector.count(); ++i)
    {
        items << QString("%1,%2").arg(i).arg(statusVector[i]);
    }

    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverIndex, items.join(";"));
    MusicApplicationModule::instance()->applyParameter();
}

void MusicScreenSaverWidget::initialize()
{
    m_networkRequest = new MusicDownloadQueueRequest(TTK::Download::Background, this);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));

    MusicDownloadQueueDataList datas;
    for(int index = 0; index < OS_COUNT; ++index)
    {
        const QString &url = QSyncUtils::makeDataBucketUrl() + QString("%1/%2/").arg(OS_SCREENSAVER_URL).arg(index);
        const QString &prefix = QString("%1%2/%3/").arg(APPCACHE_DIR_FULL, OS_SCREEN_DIR).arg(index);
        QDir().mkpath(prefix);

        MusicDownloadQueueData wallData;
        wallData.m_url = url + OS_WALLPAPER_NAME;
        wallData.m_path = prefix + OS_WALLPAPER_NAME;
        datas << wallData;

        MusicDownloadQueueData barData;
        barData.m_url = url + OS_WALLBAR_NAME;
        barData.m_path = prefix + OS_WALLBAR_NAME;
        datas << barData;

        MusicDownloadQueueData nailData;
        nailData.m_url = url + OS_WALLNAIL_NAME;
        nailData.m_path = prefix + OS_WALLNAIL_NAME;
        datas << nailData;
    }

    m_networkRequest->addImageQueue(datas);
    m_networkRequest->startToRequest();
}



MusicScreenSaverBackgroundWidget::MusicScreenSaverBackgroundWidget(QWidget *parent)
    : MusicTransitionAnimationLabel(parent),
      m_state(false),
      m_isRunning(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_runningTimer = new QTimer(this);
    connect(m_runningTimer, SIGNAL(timeout()), SLOT(runningTimeout()));

    m_backgroundTimer = new QTimer(this);
    m_backgroundTimer->setInterval(15 * TTK_DN_S2MS);
    connect(m_backgroundTimer, SIGNAL(timeout()), SLOT(backgroundTimeout()));

    hide();
    qApp->installEventFilter(this);
}

MusicScreenSaverBackgroundWidget::~MusicScreenSaverBackgroundWidget()
{
    if(m_runningTimer->isActive())
    {
        m_runningTimer->stop();
    }
    delete m_runningTimer;

    if(m_backgroundTimer->isActive())
    {
        m_backgroundTimer->stop();
    }
    delete m_backgroundTimer;
}

void MusicScreenSaverBackgroundWidget::run()
{
    if(m_runningTimer->isActive())
    {
        m_runningTimer->stop();
        m_backgroundTimer->stop();
    }

    m_state = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int value = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    m_state = (m_state && (value > 0));

    if(m_state)
    {
        m_runningTimer->setInterval(value * TTK_DN_M2MS);
        m_runningTimer->start();
    }
}

void MusicScreenSaverBackgroundWidget::runningTimeout()
{
    if(!m_isRunning)
    {
        m_isRunning = true;
        setGeometry(TTKDesktopWrapper::screenGeometry());
        setParent(nullptr);
        showFullScreen();

        backgroundTimeout();
        m_backgroundTimer->start();
    }
}

void MusicScreenSaverBackgroundWidget::backgroundTimeout()
{
    QVector<bool> statusVector(MusicScreenSaverWidget::parseSettingParameter());
    QVector<int> intVector;
    for(int index = 0; index < OS_COUNT; ++index)
    {
        if(statusVector[index])
        {
            intVector << index;
        }
    }

    if(!intVector.isEmpty())
    {
        const int index = intVector[TTK::random(intVector.count())];
        const QString &prefix = QString("%1%2/%3/").arg(APPCACHE_DIR_FULL, OS_SCREEN_DIR).arg(index);

        QPixmap background(prefix + OS_WALLPAPER_NAME);
        const QPixmap bar(prefix + OS_WALLBAR_NAME);
        TTK::Image::fusionPixmap(background, bar, QPoint(100, 900));
        setPixmap(QPixmap(background.scaled(size())));
    }
}

bool MusicScreenSaverBackgroundWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()== QEvent::MouseButtonPress || event->type()== QEvent::MouseButtonRelease ||
       event->type()== QEvent::MouseButtonDblClick || event->type()== QEvent::MouseMove ||
       event->type()== QEvent::KeyPress || event->type()== QEvent::KeyRelease)
    {
        if(m_state)
        {
            if(m_isRunning)
            {
                m_isRunning = false;
                hide();
            }
            m_runningTimer->start();
        }
        else
        {
            m_runningTimer->stop();
            m_backgroundTimer->stop();
        }
    }
    return MusicTransitionAnimationLabel::eventFilter(watched, event);
}
