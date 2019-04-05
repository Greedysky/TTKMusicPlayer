#include "musicremotewidget.h"
#include "musicremotewidgetforcircle.h"
#include "musicremotewidgetfordiamond.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musicremotewidgetforstrip.h"
#include "musicremotewidgetforripples.h"
#include "musicremotewidgetforrayswave.h"
#include "musictinyuiobject.h"
#include "musicclickedslider.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"

MusicRemoteWidget::MusicRemoteWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent)
{
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);

    drawWindowShadow(false);
    setMouseTracking(true);

    m_showMainWindow = new QPushButton(this);
    m_PreSongButton = new QPushButton(this);
    m_NextSongButton = new QPushButton(this);
    m_PlayButton = new QPushButton(this);
    m_SettingButton = new QPushButton(this);
#ifdef Q_OS_UNIX
    m_showMainWindow->setFocusPolicy(Qt::NoFocus);
    m_PreSongButton->setFocusPolicy(Qt::NoFocus);
    m_NextSongButton->setFocusPolicy(Qt::NoFocus);
    m_PlayButton->setFocusPolicy(Qt::NoFocus);
    m_SettingButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("mainWidget");

    m_showMainWindow->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    m_showMainWindow->setIcon(QIcon(":/image/lb_app_logo"));
    m_SettingButton->setIcon(QIcon(":/tiny/btn_setting_hover"));

    m_showMainWindow->setToolTip(tr("showMainWindow"));
    m_PreSongButton->setToolTip(tr("Previous"));
    m_NextSongButton->setToolTip(tr("Next"));
    m_PlayButton->setToolTip(tr("Play"));
    m_SettingButton->setToolTip(tr("showSetting"));

    m_PreSongButton->setFixedSize(28, 28);
    m_NextSongButton->setFixedSize(28, 28);
    m_PlayButton->setFixedSize(28, 28);
    m_SettingButton->setFixedSize(20, 20);
    m_showMainWindow->setFixedSize(30, 30);

    m_PreSongButton->setStyleSheet(MusicUIObject::MKGTinyBtnPrevious);
    m_NextSongButton->setStyleSheet(MusicUIObject::MKGTinyBtnNext);
    m_PlayButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlay);
    m_SettingButton->setStyleSheet(MusicUIObject::MKGTinyBtnSetting);
    m_mainWidget->setStyleSheet(QString("#mainWidget{%1}").arg(MusicUIObject::MBackgroundStyle04));

    m_showMainWindow->setCursor(QCursor(Qt::PointingHandCursor));
    m_PreSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_NextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_PlayButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_SettingButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_showMainWindow, SIGNAL(clicked()), SIGNAL(musicWindowSignal()));
    connect(m_PlayButton, SIGNAL(clicked()), SIGNAL(musicKeySignal()));
    connect(m_PreSongButton, SIGNAL(clicked()), SIGNAL(musicPlayPreviousSignal()));
    connect(m_NextSongButton, SIGNAL(clicked()), SIGNAL(musicPlayNextSignal()));
    connect(m_SettingButton, SIGNAL(clicked()), SIGNAL(musicSettingSignal()));

    m_volumeWidget = new QWidget(m_mainWidget);
    QHBoxLayout *volumeLayout = new QHBoxLayout(m_volumeWidget);
    volumeLayout->setContentsMargins(0, 0, 0, 0);
    volumeLayout->setSpacing(1);

    m_volumeButton = new QToolButton(m_volumeWidget);
    m_volumeButton->setFixedSize(QSize(16, 16));

    m_volumeSlider = new MusicClickedSlider(Qt::Horizontal, m_volumeWidget);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_volumeSlider->setFixedWidth(45);

    volumeLayout->addWidget(m_volumeButton);
    volumeLayout->addWidget(m_volumeSlider);
    m_volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_volumeButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicVolumeMute()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SLOT(musicVolumeChanged(int)));
}

MusicRemoteWidget::~MusicRemoteWidget()
{
    delete m_volumeButton;
    delete m_volumeSlider;
    delete m_volumeWidget;
    delete m_showMainWindow;
    delete m_PreSongButton;
    delete m_NextSongButton;
    delete m_PlayButton;
    delete m_SettingButton;
    delete m_mainWidget;
}

void MusicRemoteWidget::showPlayStatus(bool status) const
{
    m_PlayButton->setStyleSheet(status ? MusicUIObject::MKGTinyBtnPlay : MusicUIObject::MKGTinyBtnPause);
}

void MusicRemoteWidget::setVolumeValue(int index)
{
    blockSignals(true);
    m_volumeSlider->setValue(index);
    musicVolumeChanged(index);
    blockSignals(false);
}

void MusicRemoteWidget::setLabelText(const QString &text)
{
    Q_UNUSED(text);
}

int MusicRemoteWidget::mapRemoteTypeIndex()
{
    if(MObject_cast(MusicRemoteWidgetForCircle*, this)) return Circle;
    else if(MObject_cast(MusicRemoteWidgetForSquare*, this)) return Square;
    else if(MObject_cast(MusicRemoteWidgetForRectangle*, this)) return Rectangle;
    else if(MObject_cast(MusicRemoteWidgetForSimpleStyle*, this)) return SimpleStyle;
    else if(MObject_cast(MusicRemoteWidgetForComplexStyle*, this)) return ComplexStyle;
    else if(MObject_cast(MusicRemoteWidgetForDiamond*, this)) return Diamond;
    else if(MObject_cast(MusicRemoteWidgetForRipples*, this)) return Ripples;
    else if(MObject_cast(MusicRemoteWidgetForRaysWave*, this)) return RaysWave;
    else return Null;
}

void MusicRemoteWidget::musicVolumeChanged(int value)
{
    emit musicVolumeSignal(value);

    QString style = MusicUIObject::MKGTinyBtnSoundWhite;
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
    M_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetModeChoiced, mapRemoteTypeIndex());
}

bool MusicRemoteWidget::close()
{
    M_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetModeChoiced, Null);
    return MusicAbstractMoveWidget::close();
}

void MusicRemoteWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractMoveWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setWindowFlags( menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.setStyleSheet(MusicUIObject::MMenuStyle03);
    menu.addAction(QIcon(":/contextMenu/btn_selected"), tr("WindowTop"))->setEnabled(false);
    menu.addAction(tr("showMainWindow"), this, SIGNAL(musicWindowSignal()));
    menu.addSeparator();

    QAction * action = menu.addAction(tr("CircleRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForCircle*, this));
    action->setData(Circle);
    action = menu.addAction(tr("SquareRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForSquare*, this));
    action->setData(Square);
    action = menu.addAction(tr("RectangleRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForRectangle*, this));
    action->setData(Rectangle);
    action = menu.addAction(tr("SimpleStyleRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForSimpleStyle*, this));
    action->setData(SimpleStyle);
    action = menu.addAction(tr("ComplexStyleRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForComplexStyle*, this));
    action->setData(ComplexStyle);
    action = menu.addAction(tr("DiamondRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForDiamond*, this));
    action->setData(Diamond);
    action = menu.addAction(tr("RipplesRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForRipples*, this));
    action->setData(Ripples);
    action = menu.addAction(tr("RaysWaveRemote"));
    action->setEnabled(!MObject_cast(MusicRemoteWidgetForRaysWave*, this));
    action->setData(RaysWave);
    menu.addAction(tr("quit"), this, SLOT(close()));
    connect(&menu, SIGNAL(triggered(QAction*)), SIGNAL(musicRemoteTypeChanged(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicRemoteWidget::adjustPostion(QWidget *w)
{
    const QSize &windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    w->move( windowSize.width() - w->width() - 150, w->height() + 70);
}
