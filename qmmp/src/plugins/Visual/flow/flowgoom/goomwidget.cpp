#include "goomwidget.h"

#include <QMenu>
#include <QTimer>
#include <QPainter>
#include <QSettings>
#include <QActionGroup>
#include <math.h>
#include <qmmp/qmmp.h>

GoomWidget::GoomWidget(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle("Flow Goom Widget");
    setMinimumSize(150, 150);

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
    m_timer->setInterval(1000 / settings.value("refresh_rate", 30).toInt());
    settings.endGroup();

    if(m_update)
    {
        return;
    }

    m_update = true;
    for(QAction *act : m_fpsGroup->actions())
    {
        if(m_timer->interval() == 1000 / act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }
}

void GoomWidget::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Goom");
    QAction *act = m_fpsGroup->checkedAction();
    settings.setValue("refresh_rate", act ? act->data().toInt() : 30);
    settings.endGroup();
    readSettings();
}

void GoomWidget::hideEvent(QHideEvent *e)
{
    Visual::hideEvent(e);
    clearImage();
}

void GoomWidget::showEvent(QShowEvent *e)
{
    Visual::showEvent(e);
}

void GoomWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawImage(0, 0, m_image);
}

void GoomWidget::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void GoomWidget::processData(float *left, float *right)
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

    short buf[2][QMMP_VISUAL_NODE_SIZE];
    for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        buf[0][i] = left[i] * 32767.0;
        buf[1][i] = right[i] * 32767.0;
    }

    goom_update(m_goom, buf, 0, 1000 / m_timer->interval());
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

    QMenu *refreshRate = m_menu->addMenu(tr("Refresh Rate"));
    m_fpsGroup = new QActionGroup(this);
    m_fpsGroup->setExclusive(true);
    m_fpsGroup->addAction(tr("60 fps"))->setData(60);
    m_fpsGroup->addAction(tr("50 fps"))->setData(50);
    m_fpsGroup->addAction(tr("40 fps"))->setData(40);
    m_fpsGroup->addAction(tr("30 fps"))->setData(30);
    m_fpsGroup->addAction(tr("20 fps"))->setData(20);

    for(QAction *act : m_fpsGroup->actions())
    {
        act->setCheckable(true);
        refreshRate->addAction(act);
    }
}
