#include "mountainplugin.h"
#include "mountainmidwidget.h"
#include "mountainsidewidget.h"
#include "inlines.h"

#include <QMenu>
#include <QSettings>
#include <QBoxLayout>
#include <qmmp/qmmp.h>

MountainPlugin::MountainPlugin(QWidget *parent)
    : Visual(parent),
      m_type(Type::Complex)
{
    setWindowTitle(tr("Flow Mountain Widget"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    createMenu();
    readSettings();
}

void MountainPlugin::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Mountain");
    m_type = static_cast<Type>(settings.value("type", Type::Complex).toInt());
    settings.endGroup();

    for(QAction *act : m_typeActions->actions())
    {
        if(m_type == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }

    createMoudle();
}

void MountainPlugin::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Mountain");
    QAction *act = m_typeActions->checkedAction();
    settings.setValue("type", m_type = (act ? static_cast<Type>(act->data().toInt()) : Type::Complex));
    settings.endGroup();

    createMoudle();
}

void MountainPlugin::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void MountainPlugin::processData(float *left, float *)
{
    short dest[256];
    calc_freq(dest, left);

    float buffer[256];
    for(int i = 0; i < QMMP_VISUAL_NODE_SIZE / 2; ++i)
    {
        buffer[i] = dest[i] / ((QMMP_VISUAL_NODE_SIZE << 8) / (8.0 / 2));
    }

    m_container->addBuffer(buffer);
    m_container->update();
}

void MountainPlugin::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();

    m_typeActions = new QActionGroup(this);
    m_typeActions->setExclusive(true);
    m_typeActions->addAction(tr("Simple"))->setData(Type::Simple);
    m_typeActions->addAction(tr("Complex"))->setData(Type::Complex);

    QMenu *positionMenu = m_menu->addMenu(tr("Type"));
    for(QAction *act : m_typeActions->actions())
    {
        act->setCheckable(true);
        positionMenu->addAction(act);
    }
}

void MountainPlugin::createMoudle()
{
    if(m_container)
    {
        layout()->removeWidget(m_container);
        m_container->hide();
        m_container->deleteLater();
        m_container = nullptr;
    }

    switch(m_type)
    {
    case Type::Simple:
        m_container = new MountainSideWidget(this);
        break;
    case Type::Complex:
        m_container = new MountainMidWidget(this);
        break;
    default:
        break;
    }

    layout()->addWidget(m_container);
}
