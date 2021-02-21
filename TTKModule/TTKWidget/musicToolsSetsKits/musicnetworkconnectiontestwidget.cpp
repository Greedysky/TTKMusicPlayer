#include "musicnetworkconnectiontestwidget.h"
#include "ui_musicnetworkconnectiontestwidget.h"
#include "musicnetworktestthread.h"
#include "musicuiobject.h"
#include "musicglobal.h"
#include "musicsinglemanager.h"

#define CHECK_WWW_VISIT       "www.baidu.com"
#define CHECK_NORMAL_VISIT    "github.com"
#define CHECK_VIP_VISIT       "qmmp.ylsoftware.com"
#define CHECK_DOWNLOAD_VISIT  "download.qt.io"
#define CHECK_SERVER_VISIT    "download.csdn.net"

MusicNetworkConnectionItem::MusicNetworkConnectionItem(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_iconLabel = new QLabel(this);
    m_nameText = new QLabel(this);
    m_stateText = new QLabel(this);

    m_iconLabel->setFixedWidth(32);
    m_nameText->setFixedWidth(340);

    layout->addWidget(m_iconLabel);
    layout->addWidget(m_nameText);
    layout->addWidget(m_stateText);

    m_thread = new MusicNetworkTestThread(this);
    connect(m_thread, SIGNAL(networkConnectionTestChanged(bool)), SLOT(testFinshed(bool)));
    stop();

    setLayout(layout);
}

MusicNetworkConnectionItem::~MusicNetworkConnectionItem()
{
    if(m_thread->isRunning())
    {
        m_thread->stopAndQuitThread();
    }
    delete m_thread;
    delete m_iconLabel;
    delete m_nameText;
    delete m_stateText;
}

void MusicNetworkConnectionItem::setText(const QString &text)
{
    m_nameText->setText(text);
}

void MusicNetworkConnectionItem::setUrl(const QString &url)
{
    m_thread->setUrl(url);
}

void MusicNetworkConnectionItem::start()
{
    m_stateText->setText(tr("Detecting"));
    m_stateText->setStyleSheet(MusicUIObject::MQSSColorStyle08);
    m_thread->start();
}

void MusicNetworkConnectionItem::stop()
{
    m_iconLabel->setPixmap(QPixmap(":/tiny/lb_question"));

    m_stateText->setText(tr("Not Detected"));
    m_stateText->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    if(m_thread->isRunning())
    {
        m_thread->stopAndQuitThread();
    }
}

void MusicNetworkConnectionItem::testFinshed(bool state)
{
    if(state)
    {
        m_stateText->setText(tr("Detected"));
        m_iconLabel->setPixmap(QPixmap(":/tiny/lb_right"));
        m_stateText->setStyleSheet(MusicUIObject::MQSSColorStyle07);
    }
    else
    {
        m_stateText->setText(tr("Detected Failed"));
        m_iconLabel->setPixmap(QPixmap(":/tiny/lb_error"));
        m_stateText->setStyleSheet(MusicUIObject::MQSSColorStyle05);
    }

    Q_EMIT networkConnectionTestChanged();
}


MusicNetworkConnectionTestWidget::MusicNetworkConnectionTestWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicNetworkConnectionTestWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->iconLabel->setType(MusicGifLabelWidget::Gif_Check_Blue);
    m_ui->textLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->startButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->startButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->verticalLayout->setSpacing(3);
    m_ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

    MusicNetworkConnectionItem *item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check www Visit"));
    item->setUrl(CHECK_WWW_VISIT);
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check Normal Visit"));
    item->setUrl(CHECK_NORMAL_VISIT);
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check Vip Visit"));
    item->setUrl(CHECK_VIP_VISIT);
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check Download Visit"));
    item->setUrl(CHECK_DOWNLOAD_VISIT);
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check Server Visit"));
    item->setUrl(CHECK_SERVER_VISIT);
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    m_countIndex = 0;

    connect(m_ui->startButton, SIGNAL(clicked()), SLOT(buttonStateChanged()));
}

MusicNetworkConnectionTestWidget::~MusicNetworkConnectionTestWidget()
{
    G_SINGLE_MANAGER_PTR->removeObject(getClassName());
    qDeleteAll(m_connectionItems);
    delete m_ui;
}

void MusicNetworkConnectionTestWidget::show()
{
    m_ui->iconLabel->timeout();
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveWidget::show();
}

void MusicNetworkConnectionTestWidget::buttonStateChanged()
{
    m_countIndex = 0;
    if(!m_ui->iconLabel->isRunning())
    {
        m_ui->iconLabel->start();
        m_ui->startButton->setText(tr("stopTest"));
        for(MusicNetworkConnectionItem *item : qAsConst(m_connectionItems))
        {
            item->start();
        }
    }
    else
    {
        m_ui->iconLabel->stop();
        m_ui->startButton->setText(tr("startTest"));
        for(MusicNetworkConnectionItem *item : qAsConst(m_connectionItems))
        {
            item->stop();
        }
    }
}

void MusicNetworkConnectionTestWidget::testFinshed()
{
    if(++m_countIndex == 5)
    {
        m_ui->iconLabel->stop();
        m_ui->startButton->setText(tr("startTest"));
    }
}
