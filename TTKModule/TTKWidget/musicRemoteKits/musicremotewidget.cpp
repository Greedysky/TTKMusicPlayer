#include "musicremotewidget.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musicremotewidgetforstrip.h"
#include "musicremotewidgetforripple.h"
#include "musictinyuiobject.h"
#include "musicclickedslider.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"

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
    m_mainWidget->setObjectName("mainWidget");

    m_showMainWindowButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02);
    m_showMainWindowButton->setIcon(QIcon(":/image/lb_app_logo"));

    m_showMainWindowButton->setToolTip(tr("showMainWindow"));
    m_preSongButton->setToolTip(tr("Previous"));
    m_nextSongButton->setToolTip(tr("Next"));
    m_playButton->setToolTip(tr("Play"));
    m_settingButton->setToolTip(tr("showSetting"));

    m_preSongButton->setFixedSize(28, 28);
    m_nextSongButton->setFixedSize(28, 28);
    m_playButton->setFixedSize(28, 28);
    m_settingButton->setFixedSize(20, 20);
    m_showMainWindowButton->setFixedSize(22, 22);

    m_preSongButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPrevious);
    m_nextSongButton->setStyleSheet(MusicUIObject::MQSSTinyBtnNext);
    m_playButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPlay);
    m_settingButton->setStyleSheet(MusicUIObject::MQSSTinyBtnSetting);
    m_mainWidget->setStyleSheet(QString("#mainWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle04));

    m_showMainWindowButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_preSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_nextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_settingButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_showMainWindowButton, SIGNAL(clicked()), SIGNAL(musicWindowChanged()));
    connect(m_playButton, SIGNAL(clicked()), SIGNAL(musicKeyChanged()));
    connect(m_preSongButton, SIGNAL(clicked()), SIGNAL(musicPlayPreviousChanged()));
    connect(m_nextSongButton, SIGNAL(clicked()), SIGNAL(musicPlayNextChanged()));
    connect(m_settingButton, SIGNAL(clicked()), SIGNAL(musicSettingChanged()));

    m_volumeWidget = new QWidget(m_mainWidget);
    QHBoxLayout *volumeLayout = new QHBoxLayout(m_volumeWidget);
    volumeLayout->setContentsMargins(0, 0, 0, 0);
    volumeLayout->setSpacing(1);

    m_volumeButton = new QToolButton(m_volumeWidget);
    m_volumeButton->setFixedSize(QSize(16, 16));

    m_volumeSlider = new MusicClickedSlider(Qt::Horizontal, m_volumeWidget);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle01);
    m_volumeSlider->setFixedWidth(42);

    volumeLayout->addWidget(m_volumeButton);
    volumeLayout->addWidget(m_volumeSlider);
    m_volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_volumeButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicVolumeMute()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SLOT(musicVolumeSliderChanged(int)));
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
    m_playButton->setStyleSheet(status ? MusicUIObject::MQSSTinyBtnPlay : MusicUIObject::MQSSTinyBtnPause);
}

void MusicRemoteWidget::setVolumeValue(int index)
{
    blockSignals(true);
    m_volumeSlider->setValue(index);
    musicVolumeChanged(index);
    blockSignals(false);
}

int MusicRemoteWidget::mapRemoteTypeIndex()
{
    if(TTKObject_cast(MusicRemoteWidgetForSquare*, this)) return Square;
    else if(TTKObject_cast(MusicRemoteWidgetForRectangle*, this)) return Rectangle;
    else if(TTKObject_cast(MusicRemoteWidgetForSimpleStyle*, this)) return SimpleStyle;
    else if(TTKObject_cast(MusicRemoteWidgetForComplexStyle*, this)) return ComplexStyle;
    else if(TTKObject_cast(MusicRemoteWidgetForRipple*, this)) return Ripple;
    else return Null;
}

void MusicRemoteWidget::setLabelText(const QString &text)
{
    Q_UNUSED(text);
}

void MusicRemoteWidget::musicVolumeSliderChanged(int value)
{
    Q_EMIT musicVolumeChanged(value);

    QString style = MusicUIObject::MQSSTinyBtnSoundWhite;
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
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle03);
    menu.addAction(QIcon(":/contextMenu/btn_selected"), tr("WindowTop"))->setEnabled(false);
    menu.addAction(tr("showMainWindow"), this, SIGNAL(musicWindowChanged()));
    menu.addSeparator();

    QAction * action = menu.addAction(tr("SquareRemote"));
    action->setEnabled(!TTKObject_cast(MusicRemoteWidgetForSquare*, this));
    action->setData(Square);
    action = menu.addAction(tr("RectangleRemote"));
    action->setEnabled(!TTKObject_cast(MusicRemoteWidgetForRectangle*, this));
    action->setData(Rectangle);
    action = menu.addAction(tr("SimpleStyleRemote"));
    action->setEnabled(!TTKObject_cast(MusicRemoteWidgetForSimpleStyle*, this));
    action->setData(SimpleStyle);
    action = menu.addAction(tr("ComplexStyleRemote"));
    action->setEnabled(!TTKObject_cast(MusicRemoteWidgetForComplexStyle*, this));
    action->setData(ComplexStyle);
    action = menu.addAction(tr("RippleRemote"));
    action->setEnabled(!TTKObject_cast(MusicRemoteWidgetForRipple*, this));
    action->setData(Ripple);
    menu.addAction(tr("quit"), this, SLOT(close()));
    connect(&menu, SIGNAL(triggered(QAction*)), SIGNAL(musicRemoteTypeChanged(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicRemoteWidget::adjustPostion(QWidget *w)
{
    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    w->move(windowSize.width() - w->width() - 150, w->height() + 70);
}
