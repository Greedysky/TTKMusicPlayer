#include "mountainwidget.h"

MountainWidget::MountainWidget(QWidget *parent)
    : QtOpenGLWidget(parent)
{
    setMinimumSize(580, 320);
}

void MountainWidget::addBuffer(float *)
{
    m_angle += m_angleSpeed;
    if(m_angle > 45.0f || m_angle < -45.0f)
    {
        m_angleSpeed = -m_angleSpeed;
    }
}
