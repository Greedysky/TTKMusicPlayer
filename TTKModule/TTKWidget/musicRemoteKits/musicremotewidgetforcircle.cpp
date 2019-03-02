#include "musicremotewidgetforcircle.h"

MusicRemoteWidgetForCircle::MusicRemoteWidgetForCircle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 100, 100);
    adjustPostion(this);

    QGridLayout* grid = new QGridLayout(this);
    m_mainWidget->setFixedSize(70, 70);
    grid->addWidget(m_PlayButton, 0, 1, Qt::AlignCenter);
    grid->addWidget(m_PreSongButton, 1, 0, Qt::AlignCenter);
    grid->addWidget(m_NextSongButton, 1, 2, Qt::AlignCenter);
    grid->addWidget(m_SettingButton, 2, 1, Qt::AlignCenter);
    grid->addWidget(m_mainWidget, 1, 1, Qt::AlignCenter);

    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(m_mainWidget);
    mainWidgetLayout->setContentsMargins(0, 0, 0, 0);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->addWidget(m_showMainWindow);
    mainWidgetLayout->setAlignment(m_showMainWindow, Qt::AlignCenter);
    mainWidgetLayout->addWidget(m_volumeWidget);
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
    painter.translate(-2, -2);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 50)));
}
