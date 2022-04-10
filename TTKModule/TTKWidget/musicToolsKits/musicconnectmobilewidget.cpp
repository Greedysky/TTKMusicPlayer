#include "musicconnectmobilewidget.h"
#include "musicconnecttransferwidget.h"
#include "musicanimationstackedwidget.h"
#include "musictextsliderwidget.h"
#include "musicclickedlabel.h"
#include "musictoolsetsuiobject.h"

#include "qrencode/qrcodewidget.h"

Q_DECLARE_METATYPE(MusicDeviceInfoItem)

MusicConnectMobileWidget::MusicConnectMobileWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);

    m_deviceInfo = new MusicDeviceInfoModule;

    m_stackedWidget = new MusicAnimationStackedWidget(this);
    m_stackedWidget->setLength(LEFT_SIDE_WIDTH_MIN, MusicAnimationStackedWidget::LeftToRight);
    vBox->addWidget(m_stackedWidget);
    setLayout(vBox);

    initFirstWidget();
    initSecondWidget();
    initThirdWidget();

    m_stackedWidget->setCurrentIndex(CONNECT_MOBILE_INDEX_0);
}

MusicConnectMobileWidget::~MusicConnectMobileWidget()
{
    delete m_deviceInfo;
    delete m_stackedWidget;
}

void MusicConnectMobileWidget::initFirstWidget()
{
    QWidget *firstWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(firstWidget);
    QLabel *textLabel = new QLabel(tr("Please choose the way"), firstWidget);
    textLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04 + MusicUIObject::MQSSColorStyle09);

    QPushButton *firButton = new QPushButton(tr("Wired Mode"), firstWidget);
    firButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle09 + MusicUIObject::MQSSColorStyle09);
    firButton->setIcon(QIcon(":/toolSets/lb_wired"));
    firButton->setIconSize(QSize(50, 50));
    firButton->setFixedSize(200, 90);
    firButton->setCursor(Qt::PointingHandCursor);
    connect(firButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetSecond()));

    QPushButton *secButton = new QPushButton(tr("Wirel Mode"), firstWidget);
    secButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle09 + MusicUIObject::MQSSColorStyle09);
    secButton->setIcon(QIcon(":/toolSets/lb_wireless"));
    secButton->setIconSize(QSize(50, 50));
    secButton->setFixedSize(200, 90);
    secButton->setCursor(Qt::PointingHandCursor);
    connect(secButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetThird()));

#ifdef Q_OS_UNIX
    firButton->setFocusPolicy(Qt::NoFocus);
    secButton->setFocusPolicy(Qt::NoFocus);
#endif

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
    ///////////////////////////////////////////////////////
    QWidget *topWidget = new QWidget(secondWidget);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);

    QPushButton *backButton = new QPushButton(tr("< "), topWidget);
    backButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle10);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked()), SLOT(changeStatckedWidgetFirst()));

    QPushButton *deviceButton = new QPushButton(topWidget);
    deviceButton->setStyleSheet(MusicUIObject::MQSSTransferChangedDev);
    deviceButton->setFixedSize(80, 20);
    deviceButton->setCursor(Qt::PointingHandCursor);
    deviceButton->setMenu(&m_popMenu);
    connect(&m_popMenu, SIGNAL(triggered(QAction*)), SLOT(deviceTypeChanged(QAction*)));

    QPushButton *refreshButton = new QPushButton(topWidget);
    refreshButton->setStyleSheet(MusicUIObject::MQSSTransferRefreshDev);
    refreshButton->setFixedSize(20, 20);
    refreshButton->setCursor(Qt::PointingHandCursor);
    connect(refreshButton, SIGNAL(clicked()), SLOT(updateDeviceInfo()));

    topWidgetLayout->addWidget(backButton);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(deviceButton);
    topWidgetLayout->addWidget(refreshButton);
    ///////////////////////////////////////////////////////

    MusicClickedLabel *pixLabel = new MusicClickedLabel(secondWidget);
    pixLabel->setPixmap(QPixmap(":/toolSets/lb_mobile_usb"));
    connect(pixLabel, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    m_deviceInfoLabel = new QLabel(secondWidget);
    m_deviceInfoLabel->setStyleSheet(MusicUIObject::MQSSFontStyle03);

    m_deviceSizeLabel = new MusicTextSliderWidget(secondWidget);
    m_deviceSizeLabel->setFixedSize(270, 20);
    ///////////////////////////////////////////////////////
    QWidget *buttonWidget = new QWidget(secondWidget);
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout(buttonWidget);

    QPushButton *importSong = new QPushButton(secondWidget);
    importSong->setStyleSheet(MusicUIObject::MQSSTransferSong);
    importSong->setFixedSize(130, 96);
    importSong->setCursor(Qt::PointingHandCursor);
    connect(importSong, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    QPushButton *importRing = new QPushButton(secondWidget);
    importRing->setStyleSheet(MusicUIObject::MQSSTransferRing);
    importRing->setFixedSize(130, 96);
    importRing->setCursor(Qt::PointingHandCursor);
    connect(importRing, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    buttonWidgetLayout->addWidget(importSong);
    buttonWidgetLayout->addWidget(importRing);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    deviceButton->setFocusPolicy(Qt::NoFocus);
    refreshButton->setFocusPolicy(Qt::NoFocus);
    importSong->setFocusPolicy(Qt::NoFocus);
    importRing->setFocusPolicy(Qt::NoFocus);
#endif

    vBox->addWidget(topWidget);
    vBox->addStretch(4);
    vBox->addWidget(pixLabel, 0, Qt::AlignCenter);
    vBox->addWidget(m_deviceInfoLabel, 0, Qt::AlignCenter);
    vBox->addStretch(2);
    vBox->addWidget(m_deviceSizeLabel, 0, Qt::AlignCenter);
    vBox->addStretch(2);
    vBox->addWidget(buttonWidget, 0, Qt::AlignCenter);
    vBox->addStretch(3);

    secondWidget->setLayout(vBox);
    m_stackedWidget->addWidget(secondWidget);
    updateDeviceInfo();
}

void MusicConnectMobileWidget::initThirdWidget()
{
    QWidget *thirdWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(thirdWidget);

    QPushButton *backButton = new QPushButton(tr("< "), thirdWidget);
    backButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle10);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetFirst()));

    QLabel *label = new QLabel(tr("Use mobile app to connect"), thirdWidget);
    label->setStyleSheet(MusicUIObject::MQSSFontStyle04);

    QRCodeQWidget *code = new QRCodeQWidget(QByteArray(), QSize(130, 130), this);
    code->setMargin(8);
    code->setIcon(":/image/lb_app_logo", 0.23);

    QLabel *label2 = new QLabel(tr("\t1. Client and app must in the same wifi"), thirdWidget);
    label2->setStyleSheet(MusicUIObject::MQSSFontStyle03);
    QLabel *label3 = new QLabel(tr("\t2. Use scanning by mobile app"), thirdWidget);
    label3->setStyleSheet(MusicUIObject::MQSSFontStyle03);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
#endif

    vBox->addWidget(backButton);
    vBox->addStretch(3);
    vBox->addWidget(label, 0, Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(code, 0, Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(label2, 0, Qt::AlignVCenter);
    vBox->addWidget(label3, 0, Qt::AlignVCenter);
    vBox->addStretch(5);

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
    if(m_currentDeviceItem.m_path.isEmpty())
    {
        return;
    }

    MusicConnectTransferWidget w(this);
    w.setDeviceInfoItem(&m_currentDeviceItem);
    w.exec();
}

void MusicConnectMobileWidget::deviceTypeChanged(QAction *action)
{
    m_currentDeviceItem = action->data().value<MusicDeviceInfoItem>();
    m_deviceInfoLabel->setToolTip(action->text());
    m_deviceInfoLabel->setText(MusicUtils::Widget::elidedText(font(), action->text(), Qt::ElideRight, 250));

    m_deviceSizeLabel->setMaxValue(m_currentDeviceItem.m_totalBytes);
    m_deviceSizeLabel->setValue(m_currentDeviceItem.m_availableBytes);
    m_deviceSizeLabel->setFrontText(tr("Used:%1GB").arg(m_currentDeviceItem.m_availableBytes));
    m_deviceSizeLabel->setBackText(tr("Total:%1GB").arg(m_currentDeviceItem.m_totalBytes));
}

void MusicConnectMobileWidget::updateDeviceInfo()
{
    for(QAction *action : m_popMenu.actions())
    {
        m_popMenu.removeAction(action);
    }

    for(const MusicDeviceInfoItem &item : m_deviceInfo->removableDrive())
    {
        m_popMenu.addAction(item.m_name + "(" + item.m_path + ")")->setData(QVariant::fromValue<MusicDeviceInfoItem>(item));
    }
}
