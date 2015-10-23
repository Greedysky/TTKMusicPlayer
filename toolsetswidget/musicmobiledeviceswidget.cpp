#include "musicmobiledeviceswidget.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musicsettingmanager.h"

#include <QToolButton>
#include <QBitmap>
#include <QPainter>

MusicMobileDevicesWidget::MusicMobileDevicesWidget(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint );
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    QBitmap bmp(size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 4, 4);
    setMask(bmp);

    QSize windowSize = M_SETTING->value(MusicSettingManager::ScreenSize).toSize();
    setGeometry(windowSize.width() - 246, windowSize.height() - 169, 246, 169);
    setPixmap(QPixmap(":/image/mobile"));

    m_closeButton = new QToolButton(this);
    m_closeButton->setIcon(QIcon(":/share/searchclosed"));
    m_closeButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_closeButton->setToolTip(tr("Close"));
    m_closeButton->setGeometry(215, 2, 28, 22);
    connect(m_closeButton, SIGNAL(clicked()), SLOT(close()));

    m_openButton = new QToolButton(this);
    m_openButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
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
    MusicLocalSongsManagerWidget().exec();
}
