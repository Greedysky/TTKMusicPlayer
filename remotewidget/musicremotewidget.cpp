#include "musicremotewidget.h"
#include "musicremotewidgetforcircle.h"
#include "musicremotewidgetfordiamond.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsquare.h"

#include <QBitmap>
#include <QPainter>

MusicRemoteWidget::MusicRemoteWidget(QWidget *parent) :
    MusicAbstractMoveWidget(parent)
{
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint);
    drawWindowShadow(false);

    QBitmap bmp(size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 4, 4);
    setMask(bmp);

    setMouseTracking(true);

    m_showMainWindow = new QPushButton(this);
    m_PreSongButton = new QPushButton(this);
    m_NextSongButton = new QPushButton(this);
    m_PlayButton = new QPushButton(this);
    m_SettingButton = new QPushButton(this);
    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("mainWidget");

    m_showMainWindow->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_showMainWindow->setIcon(QIcon(":/image/windowicon"));
    m_PreSongButton->setIcon(QIcon(":/desktopTool/previousP"));
    m_NextSongButton->setIcon(QIcon(":/desktopTool/nextP"));
    m_PlayButton->setIcon(QIcon(":/desktopTool/play"));
    m_SettingButton->setIcon(QIcon(":/desktopTool/setting"));
    m_showMainWindow->setToolTip(tr("showMainWindow"));
    m_PreSongButton->setToolTip(tr("Privious"));
    m_NextSongButton->setToolTip(tr("Next"));
    m_PlayButton->setToolTip(tr("Play"));
    m_SettingButton->setToolTip(tr("showSetting"));
    m_showMainWindow->setCursor(QCursor(Qt::PointingHandCursor));
    m_PreSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_NextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_PlayButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_SettingButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_showMainWindow,SIGNAL(clicked()),SIGNAL(musicWindowSignal()));
    connect(m_PlayButton,SIGNAL(clicked()),SIGNAL(musicKeySignal()));
    connect(m_PreSongButton,SIGNAL(clicked()),SIGNAL(musicPlayPriviousSignal()));
    connect(m_NextSongButton,SIGNAL(clicked()),SIGNAL(musicPlayNextSignal()));
    connect(m_SettingButton,SIGNAL(clicked()),SIGNAL(musicSettingSignal()));

    m_volumeWidget = new QWidget(m_mainWidget);
    QHBoxLayout *volumnLayout = new QHBoxLayout(m_volumeWidget);
    volumnLayout->setContentsMargins(0,0,0,0);
    volumnLayout->setSpacing(1);
    m_volumeLabel = new QLabel(m_volumeWidget);
    m_volumeLabel->setStyleSheet(MusicUIObject::MCustomStyle26);
    m_volumeLabel->setFixedSize(QSize(20,20));
    m_volumeSlider = new QSlider(Qt::Horizontal, m_volumeWidget);
    m_volumeSlider->setRange(0,100);
    m_volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle04);
    m_volumeSlider->setFixedWidth(45);
    volumnLayout->addWidget(m_volumeLabel);
    volumnLayout->addWidget(m_volumeSlider);
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_volumeSlider,SIGNAL(valueChanged(int)),SLOT(musicVolumeChanged(int)));
}

MusicRemoteWidget::~MusicRemoteWidget()
{
    delete m_volumeLabel;
    delete m_volumeSlider;
    delete m_volumeWidget;
    delete m_showMainWindow;
    delete m_PreSongButton;
    delete m_NextSongButton;
    delete m_PlayButton;
    delete m_SettingButton;
    delete m_mainWidget;
}

void MusicRemoteWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle03);
    menu.addAction(QIcon(":/share/selected"),tr("WindowTop"))->setEnabled(false);
    menu.addAction(tr("showMainWindow"), this, SIGNAL(musicWindowSignal()));
    menu.addSeparator();

    menu.addAction(tr("CircleRemote"))->setEnabled(
                !qobject_cast<MusicRemoteWidgetForCircle*>(this));
    menu.addAction(tr("SquareRemote"))->setEnabled(
                !qobject_cast<MusicRemoteWidgetForSquare*>(this));
    menu.addAction(tr("RectangleRemote"))->setEnabled(
                !qobject_cast<MusicRemoteWidgetForRectangle*>(this));
    menu.addAction(tr("DiamondRemote"))->setEnabled(
                !qobject_cast<MusicRemoteWidgetForDiamond*>(this));
    menu.addAction(tr("quit"), this, SLOT(close()));
    connect(&menu, SIGNAL(triggered(QAction*)), SIGNAL(musicRemoteTypeChanged(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicRemoteWidget::showPlayStatus(bool status) const
{
    m_PlayButton->setIcon(QIcon(QString::fromUtf8(!status ? ":/desktopTool/stop"
                                                          : ":/desktopTool/play")) );
}

void MusicRemoteWidget::setVolumeValue(int index)
{
    blockSignals(true);
    m_volumeSlider->setValue(index);
    musicVolumeChanged(index);
    blockSignals(false);
}

void MusicRemoteWidget::musicVolumeChanged(int index)
{
    emit musicVolumeSignal(index);
    m_volumeLabel->setStyleSheet(index > 0 ? MusicUIObject::MCustomStyle24
                                           : MusicUIObject::MCustomStyle25);
}
