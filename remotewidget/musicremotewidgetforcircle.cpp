#include "musicremotewidgetforcircle.h"
#include "musicsettingmanager.h"

MusicRemoteWidgetForCircle::MusicRemoteWidgetForCircle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200,200,100,100);
    setAttribute(Qt::WA_TranslucentBackground);

    QSize windowSize = M_SETTING->value(MusicSettingManager::ScreenSize).toSize();
    move( windowSize.width() - width() - 150, height() + 70);

    m_PreSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_NextSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_PlayButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_SettingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_showMainWindow->setIconSize(QSize(40,40));
    m_mainWidget->setStyleSheet("#mainWidget{" + MusicUIObject::MCustomStyle09 + "}");

    QGridLayout* grid = new QGridLayout(this);
    m_PreSongButton->setFixedSize(30,30);
    m_NextSongButton->setFixedSize(30,30);
    m_PlayButton->setFixedSize(30,30);
    m_SettingButton->setFixedSize(30,30);
    m_mainWidget->setFixedSize(70,70);
    grid->addWidget(m_PlayButton,0,1,Qt::AlignCenter);
    grid->addWidget(m_PreSongButton,1,0,Qt::AlignCenter);
    grid->addWidget(m_NextSongButton,1,2,Qt::AlignCenter);
    grid->addWidget(m_SettingButton,2,1,Qt::AlignCenter);
    grid->addWidget(m_mainWidget,1,1,Qt::AlignCenter);

    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(m_mainWidget);
    mainWidgetLayout->setContentsMargins(0,0,0,0);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->addWidget(m_showMainWindow);
    mainWidgetLayout->setAlignment(m_showMainWindow,Qt::AlignCenter);
    mainWidgetLayout->addWidget(m_volumeWidget);
    m_showMainWindow->setFixedSize(30,40);

}

void MusicRemoteWidgetForCircle::paintEvent(QPaintEvent* event)
{
    MusicRemoteWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(15, 15, 135, 135);
    path.addEllipse(32, 32, 100, 100);
    path.setFillRule(Qt::OddEvenFill);
    painter.fillPath(path, QBrush(QColor(0,0,0,125)));
    painter.end();
}
