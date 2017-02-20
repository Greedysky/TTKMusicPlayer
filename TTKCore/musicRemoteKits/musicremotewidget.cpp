#include "musicremotewidget.h"
#include "musicremotewidgetforcircle.h"
#include "musicremotewidgetfordiamond.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musictinyuiobject.h"
#include "musicclickedslider.h"
#include "musicsettingmanager.h"

MusicRemoteWidget::MusicRemoteWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent)
{
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint);

    drawWindowShadow(false);
    setMouseTracking(true);

    m_showMainWindow = new QPushButton(this);
    m_PreSongButton = new QPushButton(this);
    m_NextSongButton = new QPushButton(this);
    m_PlayButton = new QPushButton(this);
    m_SettingButton = new QPushButton(this);
    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("mainWidget");

    m_showMainWindow->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    m_showMainWindow->setIcon(QIcon(":/image/lb_player_logo"));
    m_SettingButton->setIcon(QIcon(":/tiny/btn_setting_normal"));

    m_showMainWindow->setToolTip(tr("showMainWindow"));
    m_PreSongButton->setToolTip(tr("Previous"));
    m_NextSongButton->setToolTip(tr("Next"));
    m_PlayButton->setToolTip(tr("Play"));
    m_SettingButton->setToolTip(tr("showSetting"));

    m_PreSongButton->setFixedSize(28, 28);
    m_NextSongButton->setFixedSize(28, 28);
    m_PlayButton->setFixedSize(28, 28);
    m_SettingButton->setFixedSize(28, 28);
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
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));
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

QString MusicRemoteWidget::getClassName()
{
    return staticMetaObject.className();
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

void MusicRemoteWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setWindowFlags( menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.setStyleSheet(MusicUIObject::MMenuStyle03);
    menu.addAction(QIcon(":/contextMenu/btn_selected"), tr("WindowTop"))->setEnabled(false);
    menu.addAction(tr("showMainWindow"), this, SIGNAL(musicWindowSignal()));
    menu.addSeparator();

    menu.addAction(tr("CircleRemote"))->setEnabled(
                !MObject_cast(MusicRemoteWidgetForCircle*, this));
    menu.addAction(tr("SquareRemote"))->setEnabled(
                !MObject_cast(MusicRemoteWidgetForSquare*, this));
    menu.addAction(tr("RectangleRemote"))->setEnabled(
                !MObject_cast(MusicRemoteWidgetForRectangle*, this));
    menu.addAction(tr("SimpleStyleRemote"))->setEnabled(
                !MObject_cast(MusicRemoteWidgetForSimpleStyle*, this));
    menu.addAction(tr("ComplexStyleRemote"))->setEnabled(
                !MObject_cast(MusicRemoteWidgetForComplexStyle*, this));
    menu.addAction(tr("DiamondRemote"))->setEnabled(
                !MObject_cast(MusicRemoteWidgetForDiamond*, this));
    menu.addAction(tr("quit"), this, SLOT(close()));
    connect(&menu, SIGNAL(triggered(QAction*)), SIGNAL(musicRemoteTypeChanged(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicRemoteWidget::adjustPostion(QWidget *w)
{
    QSize windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    w->move( windowSize.width() - w->width() - 150, w->height() + 70);
}
