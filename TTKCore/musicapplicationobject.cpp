#include "musicapplicationobject.h"
#include "musicmobiledeviceswidget.h"
#include "musictimerwidget.h"
#include "musictimerautoobject.h"
#include "musicmessagebox.h"
#include "musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicregeditmanager.h"
#include "musicsourceupdatewidget.h"
#include "musicsoundeffectswidget.h"
#include "musicmessageaboutdialog.h"
#include "musicnumberdefine.h"
#include "musicapplication.h"
#include "musictopareawidget.h"
#include "musicwidgetutils.h"

#include "qdevicewatcher.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPropertyAnimation>

#define MARGIN_SIDE     5
#define MARGIN_SIDE_BY  1

MusicApplicationObject *MusicApplicationObject::m_instance = nullptr;

MusicApplicationObject::MusicApplicationObject(QObject *parent)
    : QObject(parent), m_mobileDeviceWidget(nullptr)
{
    Q_INIT_RESOURCE(MusicPlayer);
    m_instance = this;

    musicResetWindow();

    m_opacityAnimation = new QPropertyAnimation(parent, "windowOpacity", this);
    m_sideAnimation = new QPropertyAnimation(parent, "geometry", this);;
    m_opacityAnimation->setDuration(MT_S2MS);
    m_sideAnimation->setDuration(250*MT_MS);

    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
    m_setWindowToTop = false;

    m_deviceWatcher = new QDeviceWatcher(this);
    connect(m_deviceWatcher, SIGNAL(deviceChanged(bool)), SLOT(musicDeviceChanged(bool)));
    connect(m_deviceWatcher, SIGNAL(deviceAdded(QString)), SLOT(musicDeviceNameChanged(QString)));
    m_deviceWatcher->appendEventReceiver(this);
    m_deviceWatcher->start();

    musicToolSetsParameter();
}

MusicApplicationObject::~MusicApplicationObject()
{
    Q_CLEANUP_RESOURCE(MusicPlayer);
    delete m_mobileDeviceWidget;
    delete m_musicTimerAutoObj;
    delete m_opacityAnimation;
    delete m_sideAnimation;
    delete m_deviceWatcher;
}

QString MusicApplicationObject::getClassName()
{
    return staticMetaObject.className();
}

MusicApplicationObject *MusicApplicationObject::instance()
{
    return m_instance;
}

void MusicApplicationObject::getParameterSetting()
{
#ifdef Q_OS_WIN
    if(M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt())
    {
        MusicRegeditManager regeditManager;
        regeditManager.setMusicRegeditAssociateFileIcon();
    }
#endif
}

void MusicApplicationObject::windowCloseAnimationOpacity()
{
    float v = M_SETTING_PTR->value(MusicSettingManager::BgTransparentChoiced).toInt();
    v = MusicUtils::Widget::reRenderValue<float>(1, 0.35, v);
    m_opacityAnimation->stop();
    m_opacityAnimation->setStartValue(v);
    m_opacityAnimation->setEndValue(0);
    m_opacityAnimation->start();
    QTimer::singleShot(MT_S2MS, qApp, SLOT(quit()));
}

void MusicApplicationObject::soureUpdateCheck()
{
    MusicSourceUpdateNotifyWidget *w = new MusicSourceUpdateNotifyWidget;
    w->show();
    w->start();
}

void MusicApplicationObject::sideAnimationByOn()
{
    if(!M_SETTING_PTR->value(MusicSettingManager::OtherSideByChoiced).toBool())
    {
        return;
    }

    MusicApplication *w = MusicApplication::instance();
    if(w->isFullScreen() || w->isMaximized())
    {
        return;
    }

    QPoint pt = w->mapToGlobal(w->rect().topLeft());
    if(-MARGIN_SIDE <= pt.x() && pt.x() <= MARGIN_SIDE)
    {
        m_leftSideByOn = true;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(-w->width() + MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
    }

    QWidget *widget = QApplication::desktop();
    pt = w->mapToGlobal(w->rect().topRight());
    if(-MARGIN_SIDE + widget->width() <= pt.x() && pt.x() <= MARGIN_SIDE + widget->width())
    {
        m_rightSideByOn = true;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(widget->width() - MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
    }
}

void MusicApplicationObject::sideAnimationByOff()
{
    if(!M_SETTING_PTR->value(MusicSettingManager::OtherSideByChoiced).toBool())
    {
        return;
    }

    MusicApplication *w = MusicApplication::instance();
    if(m_leftSideByOn)
    {
        m_leftSideByOn = false;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
    }
    else if(m_rightSideByOn)
    {
        QWidget *widget = QApplication::desktop();
        m_rightSideByOn = false;
        m_sideAnimation->stop();
        m_sideAnimation->setStartValue(w->geometry());
        m_sideAnimation->setEndValue(QRect(widget->width() - w->width() - MARGIN_SIDE_BY, w->y(), w->width(), w->height()));
        m_sideAnimation->start();
    }
}

void MusicApplicationObject::sideAnimationReset()
{
    if(!M_SETTING_PTR->value(MusicSettingManager::OtherSideByChoiced).toBool())
    {
        return;
    }

    if(m_leftSideByOn)
    {
        MusicApplication *w = MusicApplication::instance();
        w->move(1, w->y());
    }
    else if(m_rightSideByOn)
    {
        MusicApplication *w = MusicApplication::instance();
        w->move(QApplication::desktop()->width() - w->width() - 1, w->y());
    }
}

void MusicApplicationObject::musicAboutUs()
{
    MusicMessageAboutDialog().exec();
}

void MusicApplicationObject::musicVersionUpdate()
{
    MusicSourceUpdateWidget(MusicApplication::instance()).exec();
}

void MusicApplicationObject::musicTimerWidget()
{
    MusicTimerWidget timer;
    QStringList list;
    MusicApplication::instance()->getCurrentPlayList(list);
    timer.setSongStringList(list);
    timer.exec();
}

void MusicApplicationObject::musicSetWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    Qt::WindowFlags flags = MusicApplication::instance()->windowFlags();
    MusicApplication::instance()->setWindowFlags( m_setWindowToTop ?
                              (flags | Qt::WindowStaysOnTopHint) : (flags & ~Qt::WindowStaysOnTopHint) );
    MusicApplication::instance()->show();
}

void MusicApplicationObject::musicResetWindow()
{
    m_leftSideByOn = false;
    m_rightSideByOn = false;

    QWidget *widget = QApplication::desktop();
    M_SETTING_PTR->setValue(MusicSettingManager::ScreenSize, widget->size());
    M_SETTING_PTR->setValue(MusicSettingManager::WidgetSize, QSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN));

    MusicApplication::instance()->setGeometry((widget->width() - WINDOW_WIDTH_MIN)/2,
                                (widget->height() - WINDOW_HEIGHT_MIN)/2, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
}

void MusicApplicationObject::musicToolSetsParameter()
{
    m_musicTimerAutoObj->runTimerAutoConfig();
}

void MusicApplicationObject::musicDeviceNameChanged(const QString &name)
{
    M_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePathChoiced, name);
}

void MusicApplicationObject::musicDeviceChanged(bool state)
{
    delete m_mobileDeviceWidget;
    m_mobileDeviceWidget = nullptr;
    if(state)
    {
        m_mobileDeviceWidget = new MusicMobileDevicesWidget;
        m_mobileDeviceWidget->show();
    }
    else
    {
        M_SETTING_PTR->setValue(MusicSettingManager::ExtraDevicePathChoiced, QString());
    }
}

void MusicApplicationObject::musicSetEqualizer()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }
    MusicEqualizerDialog equalizer;
    equalizer.exec();
}

void MusicApplicationObject::musicSetSoundEffect()
{
    if(!closeCurrentEqualizer())
    {
        return;
    }
    MusicSoundEffectsWidget sound;
    sound.setParentConnect(this);
    sound.exec();
}

bool MusicApplicationObject::closeCurrentEqualizer()
{
    if(M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt() != 0)
    {
        MusicMessageBox message;
        message.setText(tr("we are opening the magic sound, if you want to close?"));
        if(message.exec())
        {
            return false;
        }
        emit enhancedMusicChanged(0);
    }
    return true;
}
