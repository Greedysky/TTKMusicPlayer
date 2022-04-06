#include "musicspectrumwidget.h"
#include "ui_musicspectrumwidget.h"
#include "musicqmmputils.h"
#include "musicformats.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"
#include "musictopareawidget.h"
#include "musicfileutils.h"

#include <QPluginLoader>

#include <qmmp/florid.h>
#include <qmmp/visualfactory.h>
#include <qmmp/lightfactory.h>
#include <qmmp/soundcore.h>

#define ITEM_DEFAULT_COUNT      3
#define ITEM_OFFSET             107

MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveWidget(false, parent),
      m_ui(new Ui::MusicSpectrumWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_spectrumLayout = nullptr;
    m_ui->mainViewWidget->setStyleSheet(MusicUIObject::MQSSTabWidgetStyle01);

    m_ui->localFileButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->localFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->openFileButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->openFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->spectrumNormalLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumPlusLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumWaveLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumFlowLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);
    m_ui->spectrumFloridLayoutButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle06);

#ifdef Q_OS_UNIX
    m_ui->spectrumNormalLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumPlusLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumWaveLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumFlowLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumFloridLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->localFileButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openFileButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->localFileButton, SIGNAL(clicked()), SLOT(localFileButtonClicked()));
    connect(m_ui->openFileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));
    connect(m_ui->spectrumNormalLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumNormalTypeChanged(bool&,QString)));
    connect(m_ui->spectrumPlusLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumPlusTypeChanged(bool&,QString)));
    connect(m_ui->spectrumWaveLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumWaveTypeChanged(bool&,QString)));
    connect(m_ui->spectrumFlowLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumFlowTypeChanged(bool&,QString)));
    connect(m_ui->spectrumFloridLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumFloridTypeChanged(bool&,QString)));
    connect(m_ui->mainViewWidget, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    G_SINGLE_MANAGER_PTR->removeObject(className());
    for(const MusicSpectrum &type : qAsConst(m_types))
    {
        MusicUtils::QMMP::enabledVisualPlugin(type.m_module, false);
    }
    delete m_ui;
}

void MusicSpectrumWidget::tabIndexChanged(int index)
{
    switch(index)
    {
        case 0: adjustWidgetLayout(m_ui->spectrumNormalAreaLayout->count() - ITEM_DEFAULT_COUNT); break;
        case 1: adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT); break;
        case 2: adjustWidgetLayout(m_ui->spectrumWaveAreaLayout->count() - ITEM_DEFAULT_COUNT); break;
        case 3: adjustWidgetLayout(m_ui->spectrumFlowAreaLayout->count() - ITEM_DEFAULT_COUNT); break;
        case 4: adjustWidgetLayout(m_ui->spectrumFloridAreaLayout->count() - ITEM_DEFAULT_COUNT); break;
        case 5: adjustWidgetLayout(m_ui->spectrumLightAreaLayout->count() - ITEM_DEFAULT_COUNT); break;
        default: break;
    }
}

void MusicSpectrumWidget::spectrumNormalTypeChanged(bool &state, const QString &name)
{
    createSpectrumWidget(MusicSpectrum::Normal, state, name, m_ui->spectrumNormalAreaLayout);
    adjustWidgetLayout(m_ui->spectrumNormalAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumPlusTypeChanged(bool &state, const QString &name)
{
    createSpectrumWidget(MusicSpectrum::Plus, state, name, m_ui->spectrumPlusAreaLayout);
    adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumWaveTypeChanged(bool &state, const QString &name)
{
    if(name == "lightenvelope")
    {
        createLightWidget(MusicSpectrum::Light, state, name, m_ui->spectrumWaveAreaLayout);
    }
    else
    {
        createSpectrumWidget(MusicSpectrum::Wave, state, name, m_ui->spectrumWaveAreaLayout);
    }
    adjustWidgetLayout(m_ui->spectrumWaveAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumFlowTypeChanged(bool &state, const QString &name)
{
    createFlowWidget(MusicSpectrum::Flow, state, name, m_ui->spectrumFlowAreaLayout);
}

void MusicSpectrumWidget::spectrumFloridTypeChanged(bool &state, const QString &name)
{
    createFloridWidget(MusicSpectrum::Florid, state, name, m_ui->spectrumFloridAreaLayout);
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    bool state = true;
    createLightWidget(MusicSpectrum::Light, state, "lightspectrum", m_ui->spectrumLightAreaLayout);
}

void MusicSpectrumWidget::openFileButtonClicked()
{
    const QString &path = MusicUtils::File::openFileDialog(this, MusicFormats::supportSpekInputFormats());
    if(!path.isEmpty())
    {
        bool state = true;
        createLightWidget(MusicSpectrum::Light, state, "lightspectrum", m_ui->spectrumLightAreaLayout, path);
    }
}

void MusicSpectrumWidget::fullscreenByUser(QWidget *widget, bool state)
{
    if(state)
    {
        QWidget *parent = TTKStatic_cast(QWidget*, widget->parent());
        if(parent)
        {
            m_spectrumLayout = parent->layout();
            widget->setParent(nullptr);
            widget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            widget->showFullScreen();
        }
    }
    else
    {
        if(m_spectrumLayout)
        {
#ifdef Q_OS_WIN
            widget->showNormal();
            widget->setWindowFlags(Qt::Window);
            m_spectrumLayout->addWidget(widget);
#else
            MusicSpectrum type;
            for(int i = 0; i < m_types.count(); ++i)
            {
                if(m_types[i].m_object == widget)
                {
                    type = m_types.takeAt(i);
                    break;
                }
            }

            if(type.m_module.isEmpty())
            {
                return;
            }

            MusicUtils::QMMP::enabledVisualPlugin(type.m_module, false);

            bool state = true;
            switch(type.m_type)
            {
                case MusicSpectrum::Normal: createSpectrumWidget(MusicSpectrum::Normal, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Plus: createSpectrumWidget(MusicSpectrum::Plus, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Wave: createSpectrumWidget(MusicSpectrum::Wave, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Flow: createFlowWidget(MusicSpectrum::Flow, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Florid: createFloridWidget(MusicSpectrum::Florid, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Light: createLightWidget(MusicSpectrum::Light, state, type.m_module, m_spectrumLayout); break;
                default: break;
            }
#endif
        }
    }
}

void MusicSpectrumWidget::createSpectrumWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout)
{
    if(state)
    {
        const int before = Visual::visuals()->count();
        MusicUtils::QMMP::enabledVisualPlugin(name, true);
        const QList<Visual*> *vs = Visual::visuals();
        if(before == vs->count())
        {
            showMessageBoxWidget();
            state = false;
            return;
        }

        if(!vs->isEmpty())
        {
            MusicSpectrum type;
            type.m_module = name;
            type.m_object = vs->back();
            type.m_type = spectrum;
            layout->addWidget(type.m_object);
            m_types << type;
            type.m_object->setStyleSheet(MusicUIObject::MQSSMenuStyle02);

            connect(type.m_object, SIGNAL(fullscreenByUser(QWidget*,bool)), SLOT(fullscreenByUser(QWidget*,bool)));
        }
        else
        {
            showMessageBoxWidget();
            state = false;
        }
    }
    else
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            const MusicSpectrum &type = m_types.takeAt(index);
            layout->removeWidget(type.m_object);
            MusicUtils::QMMP::enabledVisualPlugin(name, false);
        }
    }
}

void MusicSpectrumWidget::createFlowWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout)
{
    createModuleWidget(spectrum, state, name, layout, false);
}

void MusicSpectrumWidget::createFloridWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout)
{
    createModuleWidget(spectrum, state, name, layout, true);
}

void MusicSpectrumWidget::createModuleWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout, bool florid)
{
    QString *module = florid ? &m_lastFloridName : &m_lastFlowName;
    const int index = findSpectrumWidget(*module);
    if(index != -1)
    {
        const MusicSpectrum &type = m_types.takeAt(index);
        layout->removeWidget(type.m_object);
        MusicUtils::QMMP::enabledVisualPlugin(*module, false);
    }

    if(!state)
    {
        module->clear();
        return;
    }

    const int before = Visual::visuals()->count();
    MusicUtils::QMMP::enabledVisualPlugin(name, true);
    const QList<Visual*> *vs = Visual::visuals();
    if(before == vs->count())
    {
        showMessageBoxWidget();
        state = false;
        return;
    }

    if(!vs->isEmpty())
    {
        *module = name;
        MusicSpectrum type;
        type.m_module = name;
        type.m_object = vs->back();
        type.m_type = spectrum;
        layout->addWidget(type.m_object);
        m_types << type;
        type.m_object->setStyleSheet(MusicUIObject::MQSSMenuStyle02);

        if(florid)
        {
            TTKStatic_cast(Florid*, type.m_object)->setPixmap(MusicTopAreaWidget::instance()->rendererPixmap());
            connect(MusicTopAreaWidget::instance(), SIGNAL(backgroundPixmapChanged(QPixmap)), type.m_object, SLOT(setPixmap(QPixmap)));
        }
        connect(type.m_object, SIGNAL(fullscreenByUser(QWidget*,bool)), SLOT(fullscreenByUser(QWidget*,bool)));
    }
    else
    {
        showMessageBoxWidget();
        state = false;
    }
}

void MusicSpectrumWidget::createLightWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout, const QString &url)
{
    if(state)
    {
        if(findSpectrumWidget(name) == -1)
        {
            QPluginLoader loader;
            loader.setFileName(MusicUtils::QMMP::pluginPath("Light", name));
            const QObject *obj = loader.instance();
            LightFactory *factory = nullptr;
            if(obj && (factory = TTKObject_cast(LightFactory*, obj)))
            {
                Light *lightWidget = factory->create(this);
                MusicSpectrum type;
                type.m_module = name;
                type.m_object = lightWidget;
                type.m_type = spectrum;
                m_types << type;
                layout->addWidget(lightWidget);
            }
        }

        const int index = findSpectrumWidget(name);
        Light *light = TTKStatic_cast(Light*, m_types[index].m_object);
        if(light)
        {
            const QString &path = url.isEmpty() ? SoundCore::instance()->path() : url;
            const QString &suffix = FILE_SUFFIX(QFileInfo(path));
            for(QString &filter : MusicFormats::supportSpekInputFilterFormats())
            {
                if(filter.remove(0, 2) == suffix)   // remove *.
                {
                    light->open(path);
                    return;
                }
            }

            MusicToastLabel::popup(tr("Unsupported file format"));
        }
    }
    else
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            const MusicSpectrum &type = m_types.takeAt(index);
            layout->removeWidget(type.m_object);
            delete type.m_object;
        }
    }
}

void MusicSpectrumWidget::adjustWidgetLayout(int offset)
{
    if(offset < 0)
    {
        offset = 0;
    }
    offset *= ITEM_OFFSET;

    setFixedHeight(offset + 418);
    m_ui->background->setFixedHeight(offset + 418);
    m_ui->backgroundMask->setFixedHeight(offset + 389);
    m_ui->mainViewWidget->setFixedHeight(offset + 390);

    setBackgroundPixmap(m_ui->background, size());
}

int MusicSpectrumWidget::findSpectrumWidget(const QString &name)
{
    if(name.isEmpty())
    {
        return -1;
    }

    for(int i = 0; i < m_types.count(); ++i)
    {
        if(m_types[i].m_module.contains(name))
        {
            return i;
        }
    }

    return -1;
}

void MusicSpectrumWidget::showMessageBoxWidget()
{
    MusicToastLabel::popup(tr("Spectrum init error!"));
}
