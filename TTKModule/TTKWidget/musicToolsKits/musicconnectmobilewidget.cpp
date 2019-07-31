#include "musicconnectmobilewidget.h"
#include "musicconnecttransferwidget.h"
#include "musicanimationstackedwidget.h"
#include "musictextsliderwidget.h"
#include "musicclickedlabel.h"
#include "musicwidgetutils.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"
#include "musicotherdefine.h"

#include "qrencode/qrcodewidget.h"
#include <QDebug>
Q_DECLARE_METATYPE(MusicDeviceInfoItem)

MusicConnectMobileWidget::MusicConnectMobileWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);

    m_deviceInfo = new MusicDeviceInfoCore;

    m_stackedWidget = new MusicAnimationStackedWidget(this);
    m_stackedWidget->setLength(320, MusicAnimationStackedWidget::LeftToRight);
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

void MusicConnectMobileWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicConnectMobileWidget::initFirstWidget()
{
    QWidget *firstWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(firstWidget);
    QLabel *textLabel = new QLabel(tr("Please choose the way"), firstWidget);
    textLabel->setStyleSheet(MusicUIObject::MFontStyle05 + MusicUIObject::MColorStyle12);

    QPushButton *firButton = new QPushButton(tr("Wired Mode"), firstWidget);
    firButton->setStyleSheet(MusicUIObject::MPushButtonStyle11 + MusicUIObject::MColorStyle12);
    firButton->setIcon(QIcon(":/toolSets/lb_wired"));
    firButton->setIconSize(QSize(50, 50));
    firButton->setFixedSize(200, 90);
    firButton->setCursor(Qt::PointingHandCursor);
    connect(firButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetSecond()));

    QPushButton *secButton = new QPushButton(tr("Wirel Mode"), firstWidget);
    secButton->setStyleSheet(MusicUIObject::MPushButtonStyle11 + MusicUIObject::MColorStyle12);
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
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle12);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked()), SLOT(changeStatckedWidgetFirst()));

    QPushButton *deviceButton = new QPushButton(topWidget);
    deviceButton->setStyleSheet(MusicUIObject::MKGTransferChangedDev);
    deviceButton->setFixedSize(80, 20);
    deviceButton->setCursor(Qt::PointingHandCursor);
    deviceButton->setMenu(&m_popMenu);
    connect(&m_popMenu, SIGNAL(triggered(QAction*)), SLOT(deviceTypeChanged(QAction*)));

    QPushButton *refreshButton = new QPushButton(topWidget);
    refreshButton->setStyleSheet(MusicUIObject::MKGTransferRefreshDev);
    refreshButton->setFixedSize(20, 20);
    refreshButton->setCursor(Qt::PointingHandCursor);

    topWidgetLayout->addWidget(backButton);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(deviceButton);
    topWidgetLayout->addWidget(refreshButton);
    ///////////////////////////////////////////////////////

    MusicClickedLabel *pixLabel = new MusicClickedLabel(secondWidget);
    pixLabel->setPixmap(QPixmap(":/toolSets/lb_mobile_usb"));
    connect(pixLabel, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    m_deviceInfoLabel = new QLabel(secondWidget);
    m_deviceInfoLabel->setStyleSheet(MusicUIObject::MFontStyle03);

    m_deviceSizeLabel = new MusicTextSliderWidget(secondWidget);
    m_deviceSizeLabel->setFixedSize(270, 20);
    ///////////////////////////////////////////////////////
    QWidget *buttonWidget = new QWidget(secondWidget);
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout(buttonWidget);

    QPushButton *importSong = new QPushButton(secondWidget);
    importSong->setStyleSheet(MusicUIObject::MKGTransferSong);
    importSong->setFixedSize(130, 96);
    importSong->setCursor(Qt::PointingHandCursor);
    connect(importSong, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    QPushButton *importRing = new QPushButton(secondWidget);
    importRing->setStyleSheet(MusicUIObject::MKGTransferRing);
    importRing->setFixedSize(130, 96);
    importRing->setCursor(Qt::PointingHandCursor);
    connect(importRing, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    buttonWidgetLayout->addWidget(importSong);
    buttonWidgetLayout->addWidget(importRing);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    deviceButton->setFocusPolicy(Qt::NoFocus);
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
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle12);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetFirst()));

    QLabel *label = new QLabel(tr("use mobile app to connect"), thirdWidget);
    label->setStyleSheet(MusicUIObject::MFontStyle05);

    QRCodeQWidget *code = new QRCodeQWidget(QByteArray(), QSize(130, 130), this);
    code->setMargin(8);
    code->setIcon(":/image/lb_app_logo", 0.23);

    QLabel *label2 = new QLabel(tr("\t1. client and app must in the same wifi"), thirdWidget);
    label2->setStyleSheet(MusicUIObject::MFontStyle03);
    QLabel *label3 = new QLabel(tr("\t2. use scanning by mobile app"), thirdWidget);
    label3->setStyleSheet(MusicUIObject::MFontStyle03);

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
    m_deviceInfoLabel->setText(action->text());

    m_deviceSizeLabel->setMaxValue(m_currentDeviceItem.m_totalBytes);
    m_deviceSizeLabel->setValue(m_currentDeviceItem.m_availableBytes);
    m_deviceSizeLabel->setFrontText(tr("Used:%1GB").arg(m_currentDeviceItem.m_availableBytes));
    m_deviceSizeLabel->setBackText(tr("Total:%1GB").arg(m_currentDeviceItem.m_totalBytes));
}

void MusicConnectMobileWidget::updateDeviceInfo()
{
    foreach(QAction *action, m_popMenu.actions())
    {
        m_popMenu.removeAction(action);
    }

    foreach(const MusicDeviceInfoItem &item, m_deviceInfo->getRemovableDrive())
    {
        m_popMenu.addAction(item.m_name + "(" + item.m_path + ")")->setData(QVariant::fromValue<MusicDeviceInfoItem>(item));
    }
}
