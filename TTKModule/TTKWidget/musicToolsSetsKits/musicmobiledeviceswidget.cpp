#include "musicmobiledeviceswidget.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"

MusicMobileDevicesWidget::MusicMobileDevicesWidget(QWidget *parent)
    : MusicClickedLabel(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::Config::ScreenSize).toSize();
    setGeometry(windowSize.width() - 266 - 15, windowSize.height() - 85 - 55, 266, 85);
    setPixmap(QPixmap(":/toolSets/lb_mobile_devices"));

    m_closeButton = new QToolButton(this);
    m_closeButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_closeButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_closeButton->setToolTip(tr("Close"));
    m_closeButton->setGeometry(235, 2, 28, 22);

    connect(m_closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(this, SIGNAL(clicked()), SLOT(showMobileManager()));
}

MusicMobileDevicesWidget::~MusicMobileDevicesWidget()
{
    delete m_closeButton;
}

void MusicMobileDevicesWidget::showMobileManager()
{
    hide();
    MusicLocalSongsManagerWidget *w = GENERATE_SINGLE_WIDGET_CLASS(MusicLocalSongsManagerWidget);
#ifdef Q_OS_WIN
    w->findExtraDevicePath(G_SETTING_PTR->value(MusicSettingManager::Config::ExtraDevicePath).toString());
#else
    Q_UNUSED(w);
#endif
}
