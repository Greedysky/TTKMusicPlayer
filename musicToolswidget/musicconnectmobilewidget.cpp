#include "musicconnectmobilewidget.h"

#include <QStackedWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

MusicConnectMobileWidget::MusicConnectMobileWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);
    m_stackedWidget = new QStackedWidget(this);
    vBox->addWidget(m_stackedWidget);
    setLayout(vBox);

    initFirstWidget();
    initSecondWidget();
    initThirdWidget();

    changeStatckedWidgetFirst();
}

MusicConnectMobileWidget::~MusicConnectMobileWidget()
{
    delete m_stackedWidget;
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
    firButton->setIcon(QIcon(":/appTools/wired"));
    firButton->setIconSize(QSize(50, 50));
    firButton->setFixedSize(200, 90);
    connect(firButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetSecond()));

    QPushButton *secButton = new QPushButton(tr("Wirel Mode"), firstWidget);
    secButton->setStyleSheet(buttonStyle);
    secButton->setIcon(QIcon(":/appTools/wireless"));
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
    pixLabel->setPixmap(QPixmap(":/appTools/wired_con"));

    QLabel *label1 = new QLabel(tr("use Wired Mode"), secondWidget);
    label1->setStyleSheet("font-size:18px");
    QLabel *label2 = new QLabel(tr("android phone open usb debug"), secondWidget);
    label2->setStyleSheet("font-size:15px");

    vBox->addWidget(backButton);
    vBox->addStretch(4);
    vBox->addWidget(pixLabel, 0, Qt::AlignCenter);
    vBox->addWidget(label1, 0, Qt::AlignCenter);
    vBox->addWidget(label2, 0, Qt::AlignCenter);
    vBox->addStretch(5);

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

    vBox->addWidget(backButton);
    vBox->addStretch(9);

    thirdWidget->setLayout(vBox);

    m_stackedWidget->addWidget(thirdWidget);
}

void MusicConnectMobileWidget::changeStatckedWidgetFirst()
{
    m_stackedWidget->setCurrentIndex(0);
}

void MusicConnectMobileWidget::changeStatckedWidgetSecond()
{
    m_stackedWidget->setCurrentIndex(1);
}

void MusicConnectMobileWidget::changeStatckedWidgetThird()
{
    m_stackedWidget->setCurrentIndex(2);
}
