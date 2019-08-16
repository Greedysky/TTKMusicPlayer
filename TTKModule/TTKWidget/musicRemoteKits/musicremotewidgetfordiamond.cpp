#include "musicremotewidgetfordiamond.h"

MusicRemoteWidgetForDiamond::MusicRemoteWidgetForDiamond(QWidget *parent)
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

void MusicRemoteWidgetForDiamond::paintEvent(QPaintEvent* event)
{
    MusicRemoteWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPolygon pts;
    pts.setPoints(4, 8, 80 , 80, 8, 154, 81, 80, 154);
    QPainterPath path;
    path.addPolygon(pts);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 50)));
}
