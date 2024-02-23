#include "musicconnectlocalwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicanimationstackedwidget.h"
#include "musictextsliderwidget.h"
#include "musictoolsetsuiobject.h"
#include "musiclocalmanagerwidget.h"
#include "musicrightareawidget.h"
#include "ttkclickedlabel.h"

Q_DECLARE_METATYPE(MusicDeviceInfoItem)

MusicConnectLocalWidget::MusicConnectLocalWidget(QWidget *parent)
    : QWidget(parent),
      m_mediaWidget(nullptr)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);

    m_deviceInfo = new MusicDeviceInfoModule;
    m_stackedWidget = new MusicAnimationStackedWidget(this);
    m_stackedWidget->setLength(LEFT_SIDE_WIDTH_MIN, MusicAnimationStackedWidget::Module::LeftToRight);
    vBox->addWidget(m_stackedWidget);
    setLayout(vBox);

    initFirstWidget();
    initSecondWidget();

    m_stackedWidget->setCurrentIndex(0);
}

MusicConnectLocalWidget::~MusicConnectLocalWidget()
{
    delete m_deviceInfo;
    delete m_mediaWidget;
    delete m_stackedWidget;
}

void MusicConnectLocalWidget::initFirstWidget()
{
    QWidget *firstWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(firstWidget);
    QLabel *textLabel = new QLabel(tr("Please choose the way"), firstWidget);
    textLabel->setStyleSheet(TTK::UI::FontStyle04 + TTK::UI::ColorStyle02);

    QPushButton *firButton = new QPushButton(tr("Mobile Media"), firstWidget);
    firButton->setStyleSheet(TTK::UI::PushButtonStyle08 + TTK::UI::PushButtonStyle13);
    firButton->setIcon(QIcon(":/toolSets/lb_wired"));
    firButton->setIconSize(QSize(50, 50));
    firButton->setFixedSize(200, 90);
    firButton->setCursor(Qt::PointingHandCursor);
    connect(firButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetSecond()));

    QPushButton *secButton = new QPushButton(tr("Media Library"), firstWidget);
    secButton->setStyleSheet(TTK::UI::PushButtonStyle08 + TTK::UI::PushButtonStyle13);
    secButton->setIcon(QIcon(":/toolSets/lb_media"));
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

void MusicConnectLocalWidget::initSecondWidget()
{
    QWidget *secondWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(secondWidget);
    ///////////////////////////////////////////////////////
    QWidget *topWidget = new QWidget(secondWidget);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);

    QPushButton *backButton = new QPushButton(tr("< "), topWidget);
    backButton->setStyleSheet(TTK::UI::PushButtonStyle09);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked()), SLOT(changeStatckedWidgetFirst()));

    m_deviceButton = new QPushButton(topWidget);
    m_deviceButton->setStyleSheet(TTK::UI::TransferChangedDev + TTK::UI::MenuStyle02);
    m_deviceButton->setFixedSize(80, 20);
    m_deviceButton->setCursor(Qt::PointingHandCursor);
    m_deviceButton->setMenu(new QMenu(m_deviceButton));
    connect(m_deviceButton->menu(), SIGNAL(triggered(QAction*)), SLOT(deviceTypeChanged(QAction*)));

    QPushButton *refreshButton = new QPushButton(topWidget);
    refreshButton->setStyleSheet(TTK::UI::TransferRefreshDev);
    refreshButton->setFixedSize(20, 20);
    refreshButton->setCursor(Qt::PointingHandCursor);
    connect(refreshButton, SIGNAL(clicked()), SLOT(updateDeviceInfo()));

    topWidgetLayout->addWidget(backButton);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(m_deviceButton);
    topWidgetLayout->addWidget(refreshButton);
    ///////////////////////////////////////////////////////

    TTKClickedLabel *pixLabel = new TTKClickedLabel(secondWidget);
    pixLabel->setPixmap(QPixmap(":/toolSets/lb_mobile_usb"));
    connect(pixLabel, SIGNAL(clicked()), SLOT(openTransferFilesToMobile()));

    m_deviceInfoLabel = new QLabel(secondWidget);
    m_deviceInfoLabel->setStyleSheet(TTK::UI::FontStyle03);

    m_deviceSizeLabel = new MusicTextSliderWidget(secondWidget);
    m_deviceSizeLabel->setFixedSize(270, 20);
    ///////////////////////////////////////////////////////
    QWidget *buttonWidget = new QWidget(secondWidget);
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout(buttonWidget);

    QPushButton *importSong = new QPushButton(secondWidget);
    importSong->setStyleSheet(TTK::UI::TransferSong);
    importSong->setFixedSize(130, 96);
    importSong->setCursor(Qt::PointingHandCursor);
    connect(importSong, SIGNAL(clicked()), SLOT(openTransferFilesToMobile()));

    QPushButton *importRing = new QPushButton(secondWidget);
    importRing->setStyleSheet(TTK::UI::TransferRing);
    importRing->setFixedSize(130, 96);
    importRing->setCursor(Qt::PointingHandCursor);
    connect(importRing, SIGNAL(clicked()), SLOT(openTransferFilesToMobile()));

    buttonWidgetLayout->addWidget(importSong);
    buttonWidgetLayout->addWidget(importRing);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    m_deviceButton->setFocusPolicy(Qt::NoFocus);
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

void MusicConnectLocalWidget::updateDeviceDisplay(const QString &text)
{
    m_deviceInfoLabel->setToolTip(text);
    m_deviceInfoLabel->setText(TTK::Widget::elidedText(font(), text, Qt::ElideRight, 250));

    m_deviceSizeLabel->setValue(m_currentDeviceItem.m_usedBytes);
    m_deviceSizeLabel->setMaxValue(m_currentDeviceItem.m_totalBytes);
    m_deviceSizeLabel->setFrontText(tr("Used:%1GB").arg(m_currentDeviceItem.m_usedBytes));
    m_deviceSizeLabel->setBackText(tr("Total:%1GB").arg(m_currentDeviceItem.m_totalBytes));
}

void MusicConnectLocalWidget::changeStatckedWidgetFirst()
{
    m_stackedWidget->start(0);
}

void MusicConnectLocalWidget::changeStatckedWidgetSecond()
{
    m_stackedWidget->start(1);
}

void MusicConnectLocalWidget::changeStatckedWidgetThird()
{
    if(!m_mediaWidget)
    {
        m_mediaWidget = new MusicLocalManagerWidget(this);
    }

    MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::LocalManagerWidget, m_mediaWidget);
}

void MusicConnectLocalWidget::openTransferFilesToMobile()
{
    if(!m_currentDeviceItem.isValid())
    {
        return;
    }

    MusicConnectTransferWidget widget(this);
    widget.setDeviceInfoItem(&m_currentDeviceItem);
    widget.exec();
}

void MusicConnectLocalWidget::deviceTypeChanged(QAction *action)
{
    m_currentDeviceItem = action->data().value<MusicDeviceInfoItem>();
    updateDeviceDisplay(action->text());
}

void MusicConnectLocalWidget::updateDeviceInfo()
{
    for(QAction *action : m_deviceButton->menu()->actions())
    {
        m_deviceButton->menu()->removeAction(action);
    }

    for(const MusicDeviceInfoItem &item : m_deviceInfo->removableDrive())
    {
        m_deviceButton->menu()->addAction(item.m_name + "(" + item.m_path + ")")->setData(QVariant::fromValue<MusicDeviceInfoItem>(item));
    }

    if(m_deviceButton->menu()->actions().isEmpty())
    {
        m_deviceButton->setEnabled(false);
        m_currentDeviceItem.clear();
        updateDeviceDisplay({});
    }
    else
    {
        m_deviceButton->setEnabled(true);
    }
}
