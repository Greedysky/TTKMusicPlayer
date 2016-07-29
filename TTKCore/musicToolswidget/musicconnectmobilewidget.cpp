#include "musicconnectmobilewidget.h"
#include "musicconnecttransferwidget.h"
#include "musicanimationstackedwidget.h"
#include "qrcodewidget.h"

#include <QLabel>
#include <QPainter>
#include <QBoxLayout>
#include <QPushButton>

MusicConnectMobileWidget::MusicConnectMobileWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);
    m_stackedWidget = new MusicAnimationStackedWidget(this);
    m_stackedWidget->setLength(320, MusicAnimationStackedWidget::LeftToRight);
    vBox->addWidget(m_stackedWidget);
    setLayout(vBox);

    initFirstWidget();
    initSecondWidget();
    initThirdWidget();

    m_stackedWidget->setCurrentIndex(0);
}

MusicConnectMobileWidget::~MusicConnectMobileWidget()
{
    delete m_stackedWidget;
}

QString MusicConnectMobileWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicConnectMobileWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255, 50));
    painter.end();

    QWidget::paintEvent(event);
}

void MusicConnectMobileWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicConnectMobileWidget::initFirstWidget()
{
    QWidget *firstWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(firstWidget);
    QLabel *textLabel = new QLabel(tr("Please choose the way"), firstWidget);
    textLabel->setStyleSheet("font-size:20px");

    const QString buttonStyle = "QPushButton{border-radius:5px;font-size:20px; \
                                background:rgba(0,0,0,50);} \
                                QPushButton::hover{background:rgba(0,0,0,100);}";

    QPushButton *firButton = new QPushButton(tr("Wired Mode"), firstWidget);
    firButton->setStyleSheet(buttonStyle);
    firButton->setIcon(QIcon(":/toolSets/lb_wired"));
    firButton->setIconSize(QSize(50, 50));
    firButton->setFixedSize(200, 90);
    connect(firButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetSecond()));

    QPushButton *secButton = new QPushButton(tr("Wirel Mode"), firstWidget);
    secButton->setStyleSheet(buttonStyle);
    secButton->setIcon(QIcon(":/toolSets/lb_wireless"));
    secButton->setIconSize(QSize(50, 50));
    secButton->setFixedSize(200, 90);
    connect(secButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetThird()));

    vBox->addStretch(1);
    vBox->addWidget(textLabel, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(firButton, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(secButton, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);

    firstWidget->setLayout(vBox);

    m_stackedWidget->addWidget(firstWidget);
}

void MusicConnectMobileWidget::initSecondWidget()
{
    QWidget *secondWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(secondWidget);

    QPushButton *backButton = new QPushButton(tr("< back"), secondWidget);
    backButton->setStyleSheet("QPushButton{border-radius:2px;background:rgba(0,0,0,50);}");
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetFirst()));

    QLabel *pixLabel = new QLabel(secondWidget);
    pixLabel->setPixmap(QPixmap(":/toolSets/lb_wired_con"));

    QLabel *label1 = new QLabel(tr("use Wired Mode"), secondWidget);
    label1->setStyleSheet("font-size:18px");
    QLabel *label2 = new QLabel(tr("android phone open usb debug"), secondWidget);
    label2->setStyleSheet("font-size:15px");

    QPushButton *openButton = new QPushButton(tr("transfer"), secondWidget);
    openButton->setStyleSheet("QPushButton{border-radius:2px;background:rgba(0,0,0,50);}");
    openButton->setFixedSize(80, 40);
    openButton->setCursor(Qt::PointingHandCursor);
    connect(openButton, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    vBox->addWidget(backButton);
    vBox->addStretch(4);
    vBox->addWidget(pixLabel, 0, Qt::AlignCenter);
    vBox->addWidget(label1, 0, Qt::AlignCenter);
    vBox->addWidget(label2, 0, Qt::AlignCenter);
    vBox->addStretch(2);
    vBox->addWidget(openButton, 0, Qt::AlignCenter);
    vBox->addStretch(3);

    secondWidget->setLayout(vBox);

    m_stackedWidget->addWidget(secondWidget);
}

void MusicConnectMobileWidget::initThirdWidget()
{
    QWidget *thirdWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(thirdWidget);

    QPushButton *backButton = new QPushButton(tr("< back"), thirdWidget);
    backButton->setStyleSheet("QPushButton{border-radius:2px;background:rgba(0,0,0,50);}");
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetFirst()));

    QLabel *label1 = new QLabel(tr("use mobile app to connect"), thirdWidget);
    label1->setStyleSheet("font-size:20px");

    QRCodeQWidget *code = new QRCodeQWidget(QByteArray(), QSize(130, 130), this);
    code->setMargin(8);
    code->setIcon(":/image/lb_player_logo", 0.23);

    QLabel *label2 = new QLabel(tr("\t1. client and app must in the same wifi"), thirdWidget);
    label1->setStyleSheet("font-size:15px");
    QLabel *label3 = new QLabel(tr("\t2. use scanning by mobile app"), thirdWidget);
    label1->setStyleSheet("font-size:15px");

    QPushButton *openButton = new QPushButton(tr("transfer"), thirdWidget);
    openButton->setStyleSheet("QPushButton{border-radius:2px;background:rgba(0,0,0,50);}");
    openButton->setFixedSize(80, 40);
    openButton->setCursor(Qt::PointingHandCursor);
    connect(openButton, SIGNAL(clicked()), SLOT(openTransferFiles2Wifi()));

    vBox->addWidget(backButton);
    vBox->addStretch(3);
    vBox->addWidget(label1, 0, Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(code, 0, Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(label2, 0, Qt::AlignVCenter);
    vBox->addWidget(label3, 0, Qt::AlignVCenter);
    vBox->addStretch(2);
    vBox->addWidget(openButton, 0, Qt::AlignCenter);
    vBox->addStretch(3);

    thirdWidget->setLayout(vBox);

    m_stackedWidget->addWidget(thirdWidget);
}

void MusicConnectMobileWidget::changeStatckedWidgetFirst()
{
    m_stackedWidget->start(0);
}

void MusicConnectMobileWidget::changeStatckedWidgetSecond()
{
    m_stackedWidget->start(1);
}

void MusicConnectMobileWidget::changeStatckedWidgetThird()
{
    m_stackedWidget->start(2);
}

void MusicConnectMobileWidget::openTransferFiles2Mobile()
{
    MusicConnectTransferWidget w(this);
    w.openTransferFiles(0);
    w.exec();
}

void MusicConnectMobileWidget::openTransferFiles2Wifi()
{
    MusicConnectTransferWidget w(this);
    w.openTransferFiles(1);
    w.exec();
}
