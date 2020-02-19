#include "musicscreensaverwidget.h"
#include "musicapplicationobject.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musictoolsetsuiobject.h"
#include "musicdownloadqueuecache.h"
#include "musicimageutils.h"
#///Oss import
#include "qoss/qossconf.h"

#include <QEvent>
#include <QTimer>
#include <QBoxLayout>
#include <QApplication>

#define ITEM_COUNT          4
#define OS_COUNT            10
#define OS_SCREENSAVER_URL  "ScreenSaver"
#define OS_WALLPAPER_NAME   "wallpaper.png"
#define OS_WALLBAR_NAME     "wallbar.png"
#define OS_WALLNAIL_NAME    "thumbnail.png"

MusicScreenSaverListItem::MusicScreenSaverListItem(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(155, 100);
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

    m_enableButton = new QPushButton(this);
    m_enableButton->setCursor(Qt::PointingHandCursor);
    m_enableButton->setStyleSheet(MusicUIObject::MQSSScreenItemDisable);
    m_enableButton->setGeometry((155 - 38) / 2, (100 - 38) / 2, 38, 38);
    m_enableButton->hide();

    connect(m_enableButton, SIGNAL(clicked()), SLOT(caseButtonOnAndOff()));
}

void MusicScreenSaverListItem::setFilePath(const QString &path)
{
    m_path = path;
    setPixmap(QPixmap(m_path));
}

void MusicScreenSaverListItem::caseButtonOnAndOff()
{
    if(m_enableButton->styleSheet().contains(MusicUIObject::MQSSScreenItemDisable))
    {
        setPixmap(MusicUtils::Image::grayScalePixmap(QPixmap(m_path)));
        m_enableButton->setStyleSheet(MusicUIObject::MQSSScreenItemEnable);
    }
    else
    {
        setPixmap(QPixmap(m_path));
        m_enableButton->setStyleSheet(MusicUIObject::MQSSScreenItemDisable);
    }
}

void MusicScreenSaverListItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_enableButton->hide();
}

void MusicScreenSaverListItem::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_enableButton->show();
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

void MusicScreenSaverListWidget::createItem(const QString &path)
{
    MusicScreenSaverListItem *item = new MusicScreenSaverListItem(this);
    item->setFilePath(path);

    m_gridLayout->addWidget(item, m_items.count() / ITEM_COUNT, m_items.count() % ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}



MusicScreenSaverWidget::MusicScreenSaverWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle17 + MusicUIObject::MQSSColorStyle09);

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
    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(50);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *pLabel = new QLabel(tr("Screen Saver "), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MQSSColorStyle13);

    QLabel *iLabel = new QLabel(tr("Spend Your Leisure Time With You"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);

    QLabel *wLabel = new QLabel(tr("Wait"), topWidget);
    QLabel *mLabel = new QLabel(tr("Min"), topWidget);

    m_inputEdit = new QLineEdit(topWidget);
    m_inputEdit->setFixedWidth(50);
    m_inputEdit->setEnabled(false);
    m_inputEdit->setAlignment(Qt::AlignCenter);
    m_inputEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    m_caseButton = new QPushButton(topWidget);
    m_caseButton->setFixedSize(44, 20);
    m_caseButton->setCursor(Qt::PointingHandCursor);
    m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOff);

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
    QFrame *frame = new QFrame(this);
    frame->setFixedHeight(1);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setStyleSheet(MusicUIObject::MQSSColorStyle14);
    mainLayout->addWidget(frame);
    //
    QWidget *functionWidget = new QWidget(this);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);

    m_backgroundList = new MusicScreenSaverListWidget(this);
    functionWidgetLayout->addWidget(m_backgroundList);

    connect(m_inputEdit, SIGNAL(textChanged(QString)), SLOT(inputDataChanged()));
    connect(m_caseButton, SIGNAL(clicked()), SLOT(caseButtonOnAndOff()));

    initialize();
    applySettingParameter();
}

MusicScreenSaverWidget::~MusicScreenSaverWidget()
{

}

void MusicScreenSaverWidget::applySettingParameter()
{
    const bool state = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int value = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();

    m_inputEdit->setText(QString::number(value));
    if(state)
    {
        caseButtonOnAndOff();
    }
}

void MusicScreenSaverWidget::inputDataChanged()
{
    const bool state = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    if(state)
    {
        M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
        MusicApplicationObject::instance()->applySettingParameter();
    }
}

void MusicScreenSaverWidget::caseButtonOnAndOff()
{
    const bool state = m_caseButton->styleSheet().contains(":/toolSets/btn_saver_off");
    if(state)
    {
        m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOn);
        M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
    }
    else
    {
        m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOff);
    }

    m_inputEdit->setEnabled(state);
    M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, state);
    MusicApplicationObject::instance()->applySettingParameter();
}

void MusicScreenSaverWidget::downLoadDataChanged(const QString &data)
{
    if(data.contains(OS_WALLNAIL_NAME))
    {
        m_backgroundList->createItem(data);
    }
}

void MusicScreenSaverWidget::initialize()
{
    m_downloadQueue = new MusicDownloadQueueCache(MusicObject::DownloadBigBackground, this);
    connect(m_downloadQueue, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataChanged(QString)));

    MusicDownloadQueueDatas datas;
    for(int i=1; i<=OS_COUNT; i++)
    {
        const QString &url = QOSSConf::generateDataBucketUrl() + QString("%1/%2/").arg(OS_SCREENSAVER_URL).arg(i);
        const QString &path = QString("%1%2/").arg(SCREEN_DIR_FULL).arg(i);
        QDir().mkpath(path);

        MusicDownloadQueueData wallData;
        wallData.m_url = url + OS_WALLPAPER_NAME;
        wallData.m_savePath = path + OS_WALLPAPER_NAME;
        datas << wallData;

        MusicDownloadQueueData barData;
        barData.m_url = url + OS_WALLBAR_NAME;
        barData.m_savePath = path + OS_WALLBAR_NAME;
        datas << barData;

        MusicDownloadQueueData nailData;
        nailData.m_url = url + OS_WALLNAIL_NAME;
        nailData.m_savePath = path + OS_WALLNAIL_NAME;
        datas << nailData;
    }

    m_downloadQueue->addImageQueue(datas);
    m_downloadQueue->startToDownload();
}


MusicScreenSaverBackgroundWidget::MusicScreenSaverBackgroundWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_state = false;
    m_isRunning = false;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));

    setStyleSheet("background:blue");
    hide();

    qApp->installEventFilter(this);
}

MusicScreenSaverBackgroundWidget::~MusicScreenSaverBackgroundWidget()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    delete m_timer;
}

void MusicScreenSaverBackgroundWidget::applySettingParameter()
{
    m_state = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int value = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    m_state = (m_state && (value > 0));
    if(value > 0)
    {
        m_timer->setInterval(value * MT_M2MS);
    }
}

void MusicScreenSaverBackgroundWidget::timeout()
{
    m_isRunning = true;
    setParent(nullptr);
    showFullScreen();
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
            m_timer->stop();
            m_timer->start();
        }
        else
        {
            m_timer->stop();
        }
    }
    return QWidget::eventFilter(watched, event);
}
