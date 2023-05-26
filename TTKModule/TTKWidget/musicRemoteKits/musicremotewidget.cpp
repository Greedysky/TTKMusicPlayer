#include "musicremotewidget.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musicremotewidgetforstrip.h"
#include "musicremotewidgetforripple.h"
#include "musictinyuiobject.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "ttkclickedslider.h"

MusicRemoteWidget::MusicRemoteWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);

    drawWindowShadow(false);
    setMouseTracking(true);

    m_showMainWindowButton = new QPushButton(this);
    m_preSongButton = new QPushButton(this);
    m_nextSongButton = new QPushButton(this);
    m_playButton = new QPushButton(this);
    m_settingButton = new QPushButton(this);
#ifdef Q_OS_UNIX
    m_showMainWindowButton->setFocusPolicy(Qt::NoFocus);
    m_preSongButton->setFocusPolicy(Qt::NoFocus);
    m_nextSongButton->setFocusPolicy(Qt::NoFocus);
    m_playButton->setFocusPolicy(Qt::NoFocus);
    m_settingButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("MainWidget");

    m_showMainWindowButton->setStyleSheet(TTK::UI::PushButtonStyle02);
    m_showMainWindowButton->setIcon(QIcon(":/image/lb_app_logo"));

    m_showMainWindowButton->setToolTip(tr("Show MainWindow"));
    m_preSongButton->setToolTip(tr("Previous"));
    m_nextSongButton->setToolTip(tr("Next"));
    m_playButton->setToolTip(tr("Play"));
    m_settingButton->setToolTip(tr("Settings"));

    m_preSongButton->setFixedSize(28, 28);
    m_nextSongButton->setFixedSize(28, 28);
    m_playButton->setFixedSize(28, 28);
    m_settingButton->setFixedSize(20, 20);
    m_showMainWindowButton->setFixedSize(22, 22);

    m_preSongButton->setStyleSheet(TTK::UI::TinyBtnPrevious);
    m_nextSongButton->setStyleSheet(TTK::UI::TinyBtnNext);
    m_playButton->setStyleSheet(TTK::UI::TinyBtnPlay);
    m_settingButton->setStyleSheet(TTK::UI::TinyBtnSetting);
    m_mainWidget->setStyleSheet(QString("#MainWidget{%1}").arg(TTK::UI::BackgroundStyle04));

    m_showMainWindowButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_preSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_nextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_settingButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_showMainWindowButton, SIGNAL(clicked()), SIGNAL(showMainWindow()));
    connect(m_playButton, SIGNAL(clicked()), SIGNAL(playStateChanged()));
    connect(m_preSongButton, SIGNAL(clicked()), SIGNAL(playPreviousChanged()));
    connect(m_nextSongButton, SIGNAL(clicked()), SIGNAL(playNextChanged()));
    connect(m_settingButton, SIGNAL(clicked()), SIGNAL(settingChanged()));

    m_volumeWidget = new QWidget(m_mainWidget);
    QHBoxLayout *volumeLayout = new QHBoxLayout(m_volumeWidget);
    volumeLayout->setContentsMargins(0, 0, 0, 0);
    volumeLayout->setSpacing(1);

    m_volumeButton = new QToolButton(m_volumeWidget);
    m_volumeButton->setFixedSize(QSize(16, 16));

    m_volumeSlider = new TTKClickedSlider(Qt::Horizontal, m_volumeWidget);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(TTK::UI::SliderStyle01);
    m_volumeSlider->setFixedWidth(42);

    volumeLayout->addWidget(m_volumeButton);
    volumeLayout->addWidget(m_volumeSlider);
    m_volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_volumeButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(volumeMute()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SLOT(volumeSliderChanged(int)));
}

MusicRemoteWidget::~MusicRemoteWidget()
{
    delete m_volumeButton;
    delete m_volumeSlider;
    delete m_volumeWidget;
    delete m_showMainWindowButton;
    delete m_preSongButton;
    delete m_nextSongButton;
    delete m_playButton;
    delete m_settingButton;
    delete m_mainWidget;
}

void MusicRemoteWidget::setCurrentPlayStatus(bool status) const
{
    m_playButton->setStyleSheet(status ? TTK::UI::TinyBtnPause : TTK::UI::TinyBtnPlay);
}

void MusicRemoteWidget::setVolumeValue(int index)
{
    blockSignals(true);
    m_volumeSlider->setValue(index);
    volumeChanged(index);
    blockSignals(false);
}

int MusicRemoteWidget::mapRemoteTypeIndex()
{
    if(TTKObjectCast(MusicRemoteWidgetForSquare*, this))
    {
        return Square;
    }
    else if(TTKObjectCast(MusicRemoteWidgetForRectangle*, this))
    {
        return Rectangle;
    }
    else if(TTKObjectCast(MusicRemoteWidgetForSimpleStyle*, this))
    {
        return SimpleStyle;
    }
    else if(TTKObjectCast(MusicRemoteWidgetForComplexStyle*, this))
    {
        return ComplexStyle;
    }
    else if(TTKObjectCast(MusicRemoteWidgetForRipple*, this))
    {
        return Ripple;
    }
    else
    {
        return Null;
    }
}

void MusicRemoteWidget::setLabelText(const QString &text)
{
    Q_UNUSED(text);
}

void MusicRemoteWidget::volumeSliderChanged(int value)
{
    QString style = TTK::UI::TinyBtnSoundWhite;
    if(66 < value && value <=100)
    {
        style += "QToolButton{ margin-left:-48px; }";
    }
    else if(33 < value && value <=66)
    {
        style += "QToolButton{ margin-left:-32px; }";
    }
    else if(0 < value && value <=33)
    {
        style += "QToolButton{ margin-left:-16px; }";
    }
    else
    {
        style += "QToolButton{ margin-left:-64px; }";
    }

    m_volumeButton->setStyleSheet(style);
    Q_EMIT volumeChanged(value);
}

void MusicRemoteWidget::show()
{
    MusicAbstractMoveWidget::show();
    G_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetMode, mapRemoteTypeIndex());
}

bool MusicRemoteWidget::close()
{
    G_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetMode, Null);
    return MusicAbstractMoveWidget::close();
}

void MusicRemoteWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractMoveWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setWindowFlags(menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.setStyleSheet(TTK::UI::MenuStyle03);
    menu.addAction(QIcon(":/contextMenu/btn_selected"), tr("WindowTop"))->setEnabled(false);
    menu.addAction(tr("Show MainWindow"), this, SIGNAL(showMainWindow()));
    menu.addSeparator();

    QAction * action = menu.addAction(tr("Square Remote"));
    action->setEnabled(!TTKObjectCast(MusicRemoteWidgetForSquare*, this));
    action->setData(Square);
    action = menu.addAction(tr("Rectangle Remote"));
    action->setEnabled(!TTKObjectCast(MusicRemoteWidgetForRectangle*, this));
    action->setData(Rectangle);
    action = menu.addAction(tr("Simple Style Remote"));
    action->setEnabled(!TTKObjectCast(MusicRemoteWidgetForSimpleStyle*, this));
    action->setData(SimpleStyle);
    action = menu.addAction(tr("Complex Style Remote"));
    action->setEnabled(!TTKObjectCast(MusicRemoteWidgetForComplexStyle*, this));
    action->setData(ComplexStyle);
    action = menu.addAction(tr("Ripple Remote"));
    action->setEnabled(!TTKObjectCast(MusicRemoteWidgetForRipple*, this));
    action->setData(Ripple);
    menu.addAction(tr("Quit"), this, SLOT(close()));
    connect(&menu, SIGNAL(triggered(QAction*)), SIGNAL(remoteTypeChanged(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicRemoteWidget::adjustPosition(QWidget *w)
{
    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    w->move(windowSize.width() - w->width() - 150, w->height() + 70);
}
