#include "musiclrcfloatsettingwidget.h"
#include <QTimer>

MusicLrcFloatSettingWidget::MusicLrcFloatSettingWidget(QWidget *parent)
    : MusicLrcFloatAbstractWidget(parent)
{
    m_rectIn = QRect(350, 120, 165, 210);
    m_rectOut = QRect(515, 171, 165, 105);
    setGeometry(m_rectOut);
    setObjectName("MusicLrcFloatSettingWidget");
    setStyleSheet("#MusicLrcFloatSettingWidget{background:rgba(0, 0, 0, 100);}");
}

void MusicLrcFloatSettingWidget::show()
{
    MusicLrcFloatAbstractWidget::show();
    animationIn();
}

void MusicLrcFloatSettingWidget::leaveEvent(QEvent *)
{
    animationOut();
    QTimer::singleShot(m_animation->duration(),this,SLOT(close()));
}
