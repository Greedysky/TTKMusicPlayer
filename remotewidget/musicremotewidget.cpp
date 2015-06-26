#include "musicremotewidget.h"

MusicRemoteWidget::MusicRemoteWidget(QWidget *parent) :
    MusicMoveWidgetAbstract(parent)
{
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint);
    setMouseTracking(true);

    m_showMainWindow = new QPushButton(this);
    m_PreSongButton = new QPushButton(this);
    m_NextSongButton = new QPushButton(this);
    m_PlayButton = new QPushButton(this);
    m_SettingButton = new QPushButton(this);
    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("mainWidget");
//    m_mainWidget->setStyleSheet("#mainWidget{background-color:rgba(0,0,0,200)}");

    m_showMainWindow->setStyleSheet("QPushButton{background:transparent;border-none;}\
                                     QPushButton::hover{image:url(:/image/shadow);}");
    m_showMainWindow->setIcon(QIcon(QPixmap(":/image/windowicon")));
    m_PreSongButton->setIcon(QIcon(QPixmap(":/desktopTool/previousP")));
    m_NextSongButton->setIcon(QIcon(QPixmap(":/desktopTool/nextP")));
    m_PlayButton->setIcon(QIcon(QPixmap(":/desktopTool/play")));
    m_SettingButton->setIcon(QIcon(QPixmap(":/desktopTool/setting")));
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

    m_volumnWidget = new QWidget(m_mainWidget);
    QHBoxLayout *volumnLayout = new QHBoxLayout(m_volumnWidget);
    volumnLayout->setContentsMargins(0,0,0,0);
    volumnLayout->setSpacing(1);
    m_volumnLabel = new QLabel(m_volumnWidget);
    m_volumnLabel->setStyleSheet("background-color:transparent;border-image: url(:/image/soundmax)");
    m_volumnLabel->setFixedSize(QSize(20,20));
    m_volumnSlider = new QSlider(Qt::Horizontal, m_volumnWidget);
    m_volumnSlider->setRange(0,100);
    m_volumnSlider->setStyleSheet(MusicObject::MusicRemoteControlSlider);
    m_volumnSlider->setFixedWidth(45);
    volumnLayout->addWidget(m_volumnLabel);
    volumnLayout->addWidget(m_volumnSlider);
    m_volumnSlider->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_volumnSlider,SIGNAL(valueChanged(int)),SLOT(musicVolumeChanged(int)));
}

MusicRemoteWidget::~MusicRemoteWidget()
{
    delete m_volumnLabel;
    delete m_volumnSlider;
    delete m_volumnWidget;
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
    menu.setStyleSheet(MusicObject::MusicRemoteControlMenu);
    menu.addAction(QIcon(QPixmap(":/share/selected")),tr("WindowTop"))->setEnabled(false);
    menu.addSeparator();
    menu.addAction(tr("showMainWindow"),this,SIGNAL(musicWindowSignal()));
    menu.addAction(tr("quit"),this,SLOT(close()));
    menu.exec(QCursor::pos());
}

void MusicRemoteWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void MusicRemoteWidget::showPlayStatus(bool status)
{
    m_PlayButton->setIcon(!status ? QIcon(QPixmap(QString::fromUtf8(":/desktopTool/stop")))
                                  : QIcon(QPixmap(QString::fromUtf8(":/desktopTool/play"))) );
}


void MusicRemoteWidget::setVolumeValue(int index)
{
    blockSignals(true);
    m_volumnSlider->setValue(index);
    musicVolumeChanged(index);
    blockSignals(false);
}

void MusicRemoteWidget::musicVolumeChanged(int index)
{
    emit musicVolumeSignal(index);
    if(index > 0)
        m_volumnLabel->setStyleSheet("border-image:url(:/image/soundmax);");
    else
        m_volumnLabel->setStyleSheet("border-image:url(:/image/soundmin)");
}
