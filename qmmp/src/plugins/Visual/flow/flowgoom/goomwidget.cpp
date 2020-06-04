#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/qmmp.h>

#include "goomwidget.h"

GoomWidget::GoomWidget(QWidget *parent)
    : Florid(parent)
{
    m_useImage = false;
    m_update = false;
    m_goom = nullptr;
    m_fps = 25;

    setWindowTitle("Flow Goom Widget");
    setMinimumSize(150,150);

    createMenu();
    readSettings();
}

GoomWidget::~GoomWidget()
{
    if(m_goom)
    {
        goom_close(m_goom);
    }
    m_goom = nullptr;
}

void GoomWidget::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Goom");
    m_fps = settings.value("refresh_rate", 25).toInt();
    m_timer->setInterval(1000 / m_fps);
    settings.endGroup();

    if(!m_update)
    {
        m_update = true;
        foreach(QAction *act, m_fpsGroup->actions())
        {
            if(m_fps == act->data().toInt())
            {
                act->setChecked(true);
                break;
            }
        }
    }
}

void GoomWidget::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Goom");
    QAction *act = m_fpsGroup->checkedAction();
    settings.setValue("refresh_rate", act ? act->data().toInt() : 25);
    settings.endGroup();
}

void GoomWidget::hideEvent(QHideEvent *e)
{
    Florid::hideEvent(e);
    clearImage();
}

void GoomWidget::showEvent(QShowEvent *e)
{
    Florid::showEvent(e);
}

void GoomWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}

void GoomWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        m_menu->exec(e->globalPos());
    }
}

void GoomWidget::process(float *left, float *right)
{
    if(m_image.size() != size() || !m_goom)
    {
        if(!m_goom)
        {
            m_goom = goom_init(width(), height());
        }

        m_image = QImage(size(), QImage::Format_RGB32);
        goom_set_resolution(m_goom, width(), height());
        goom_set_screenbuffer(m_goom, m_image.bits());
    }

    for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; i++)
    {
        left[i] = left[i] * 32767.0;
        right[i] = right[i] * 32767.0;
    }
    goom_update(m_goom, m_out, 0, m_fps);
}

void GoomWidget::clearImage()
{
    m_image.fill(Qt::black);
    update();
}

void GoomWidget::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(readSettings()));

    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();

    QMenu *refreshRate = m_menu->addMenu(tr("Refresh Rate"));
    m_fpsGroup = new QActionGroup(this);
    m_fpsGroup->setExclusive(true);
    m_fpsGroup->addAction(tr("60 fps"))->setData(60);
    m_fpsGroup->addAction(tr("50 fps"))->setData(50);
    m_fpsGroup->addAction(tr("25 fps"))->setData(25);
    foreach(QAction *act, m_fpsGroup->actions())
    {
        act->setCheckable(true);
        refreshRate->addAction(act);
    }

}
