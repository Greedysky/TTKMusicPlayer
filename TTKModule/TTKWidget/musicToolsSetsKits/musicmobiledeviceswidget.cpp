#include "musicmobiledeviceswidget.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "musicsinglemanager.h"

MusicMobileDevicesWidget::MusicMobileDevicesWidget(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    QSize windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setGeometry(windowSize.width() - 246, windowSize.height() - 169, 246, 169);
    setPixmap(QPixmap(":/toolSets/lb_mobile_devices"));

    m_closeButton = new QToolButton(this);
    m_closeButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_closeButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_closeButton->setToolTip(tr("Close"));
    m_closeButton->setGeometry(215, 2, 28, 22);
    connect(m_closeButton, SIGNAL(clicked()), SLOT(close()));

    m_openButton = new QToolButton(this);
    m_openButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_openButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_openButton->setText(tr("MobileOpen"));
    m_openButton->setGeometry(160, 140, 75, 25);
    connect(m_openButton, SIGNAL(clicked()), SLOT(showMobileManager()));

}

MusicMobileDevicesWidget::~MusicMobileDevicesWidget()
{
    delete m_closeButton;
    delete m_openButton;
}

void MusicMobileDevicesWidget::showMobileManager()
{
    hide();
    M_SINGLE_MANAGER_WIDGET_NEW(MusicLocalSongsManagerWidget);
#ifdef Q_OS_WIN
    w->findExtraDevicePath(M_SETTING_PTR->value(MusicSettingManager::ExtraDevicePathChoiced).toString());
#endif
    w->show();
}
