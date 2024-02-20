#include "musicnetworkconnectiontestwidget.h"
#include "ui_musicnetworkconnectiontestwidget.h"
#include "musicnetworktestthread.h"
#include "musicalgorithmutils.h"

static constexpr const char *CHECK_WWW_VISIT = "dm81Smp4VzI3eHFVNzV4aHgyU2RYN2paNDhJPQ==";
static constexpr const char *CHECK_NORMAL_VISIT = "aGEvbU52TkJVMzQ0Z0hoSllOd2wwZz09";
static constexpr const char *CHECK_VIP_VISIT = "ZjlBQUFxY25KM1BKd2h3VzZVZ1F5dVJHMThEYlFkTWU=";
static constexpr const char *CHECK_DOWNLOAD_VISIT = "bkIwTS9NZE9wSUZyLythbm5YQVJVeDBtQStBPQ==";
static constexpr const char *CHECK_SERVER_VISIT = "T2tjRlYvUVUvVUVORDg4dHI1NlorOUR0SHVvZ3RUQ1g=";

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
    m_nameText->setFixedWidth(325);
    m_stateText->setAlignment(Qt::AlignCenter);

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
    m_thread->stop();
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
    m_stateText->setStyleSheet(TTK::UI::ColorStyle07);
    m_thread->start();
}

void MusicNetworkConnectionItem::stop()
{
    m_iconLabel->setPixmap(QPixmap(":/tiny/lb_question"));

    m_stateText->setText(tr("Undetected"));
    m_stateText->setStyleSheet(TTK::UI::ColorStyle03);
    if(m_thread->isRunning())
    {
        m_thread->stop();
    }
}

void MusicNetworkConnectionItem::testFinshed(bool state)
{
    m_stateText->setText(state ? tr("Detected") : tr("Failed"));
    m_iconLabel->setPixmap(QPixmap(state ? ":/tiny/lb_right" : ":/tiny/lb_error"));
    m_stateText->setStyleSheet(state ? TTK::UI::ColorStyle09 : TTK::UI::ColorStyle11);

    Q_EMIT networkConnectionTestChanged();
}


MusicNetworkConnectionTestWidget::MusicNetworkConnectionTestWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicNetworkConnectionTestWidget),
      m_index(0)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->iconLabel->setType(MusicGifLabelWidget::Module::CheckBlue);
    m_ui->textLabel->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->startButton->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->startButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->verticalLayout->setSpacing(3);
    m_ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

    MusicNetworkConnectionItem *item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check www visit"));
    item->setUrl(TTK::Algorithm::mdII(CHECK_WWW_VISIT, false));
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check normal visit"));
    item->setUrl(TTK::Algorithm::mdII(CHECK_NORMAL_VISIT, false));
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check vip visit"));
    item->setUrl(TTK::Algorithm::mdII(CHECK_VIP_VISIT, false));
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check download visit"));
    item->setUrl(TTK::Algorithm::mdII(CHECK_DOWNLOAD_VISIT, false));
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("Check server visit"));
    item->setUrl(TTK::Algorithm::mdII(CHECK_SERVER_VISIT, false));
    connect(item, SIGNAL(networkConnectionTestChanged()), SLOT(testFinshed()));
    m_ui->verticalLayout->addWidget(item);

    connect(m_ui->startButton, SIGNAL(clicked()), SLOT(buttonStateChanged()));
}

MusicNetworkConnectionTestWidget::~MusicNetworkConnectionTestWidget()
{
    TTKRemoveSingleWidget(className());
    qDeleteAll(m_connectionItems);
    delete m_ui;
}

void MusicNetworkConnectionTestWidget::show()
{
    m_ui->iconLabel->updateRender();
    MusicAbstractMoveWidget::show();
}

void MusicNetworkConnectionTestWidget::buttonStateChanged()
{
    m_index = 0;
    if(!m_ui->iconLabel->isRunning())
    {
        m_ui->iconLabel->start();
        m_ui->startButton->setText(tr("Stop"));

        for(MusicNetworkConnectionItem *item : qAsConst(m_connectionItems))
        {
            item->start();
        }
    }
    else
    {
        m_ui->iconLabel->stop();
        m_ui->startButton->setText(tr("Start"));

        for(MusicNetworkConnectionItem *item : qAsConst(m_connectionItems))
        {
            item->stop();
        }
    }
}

void MusicNetworkConnectionTestWidget::testFinshed()
{
    if(++m_index == 5)
    {
        m_ui->iconLabel->stop();
        m_ui->startButton->setText(tr("Start"));
    }
}
