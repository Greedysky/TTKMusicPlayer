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

static constexpr const char *LIGHT_WAVEFORM_MODULE = "lightwaveform";
static constexpr const char *LIGHT_SPECTROGRAM_MODULE = "lightspectrogram";

MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveResizePlainWidget(parent),
      m_ui(new Ui::MusicSpectrumWidget),
      m_spectrumLayout(nullptr)
{
    m_ui->setupUi(this);
    setMinimumSize(750, 650);
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    setStyleSheet(TTK::UI::MenuStyle02);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->mainViewWidget->setStyleSheet(TTK::UI::TabWidgetStyle01);

    m_ui->localFileButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->localFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->openFileButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->openFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->spectrumNormalLayoutButton->setStyleSheet(TTK::UI::ToolButtonStyle06);
    m_ui->spectrumPlusLayoutButton->setStyleSheet(TTK::UI::ToolButtonStyle06);
    m_ui->spectrumWaveLayoutButton->setStyleSheet(TTK::UI::ToolButtonStyle06);
    m_ui->spectrumFlowLayoutButton->setStyleSheet(TTK::UI::ToolButtonStyle06);
    m_ui->spectrumFloridLayoutButton->setStyleSheet(TTK::UI::ToolButtonStyle06);

#ifdef Q_OS_UNIX
    m_ui->spectrumNormalLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumPlusLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumWaveLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumFlowLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->spectrumFloridLayoutButton->setFocusPolicy(Qt::NoFocus);
    m_ui->localFileButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openFileButton->setFocusPolicy(Qt::NoFocus);
#endif
    /////////// Objects Mouse tracking
    setObjectsTracking({m_ui->background});

    connect(m_ui->localFileButton, SIGNAL(clicked()), SLOT(localFileButtonClicked()));
    connect(m_ui->openFileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));
    connect(m_ui->spectrumNormalLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumNormalTypeChanged(bool&,QString)));
    connect(m_ui->spectrumPlusLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumPlusTypeChanged(bool&,QString)));
    connect(m_ui->spectrumWaveLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumWaveTypeChanged(bool&,QString)));
    connect(m_ui->spectrumFlowLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumFlowTypeChanged(bool&,QString)));
    connect(m_ui->spectrumFloridLayoutButton, SIGNAL(stateChanged(bool&,QString)), SLOT(spectrumFloridTypeChanged(bool&,QString)));
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    TTKRemoveSingleWidget(className());
    for(const MusicSpectrum &type : qAsConst(m_types))
    {
        TTK::TTKQmmp::enabledVisualPlugin(type.m_module, false);
    }
    delete m_ui;
}

void MusicSpectrumWidget::spectrumNormalTypeChanged(bool &state, const QString &name)
{
    createSpectrumWidget(MusicSpectrum::Module::Normal, state, name, m_ui->spectrumNormalAreaLayout);
}

void MusicSpectrumWidget::spectrumPlusTypeChanged(bool &state, const QString &name)
{
    createSpectrumWidget(MusicSpectrum::Module::Plus, state, name, m_ui->spectrumPlusAreaLayout);
}

void MusicSpectrumWidget::spectrumWaveTypeChanged(bool &state, const QString &name)
{
    if(name == LIGHT_WAVEFORM_MODULE)
    {
        createLightWidget(MusicSpectrum::Module::Light, state, name, m_ui->spectrumWaveAreaLayout);
    }
    else
    {
        createSpectrumWidget(MusicSpectrum::Module::Wave, state, name, m_ui->spectrumWaveAreaLayout);
    }
}

void MusicSpectrumWidget::spectrumFlowTypeChanged(bool &state, const QString &name)
{
    createFlowWidget(MusicSpectrum::Module::Flow, state, name, m_ui->spectrumFlowAreaLayout);
}

void MusicSpectrumWidget::spectrumFloridTypeChanged(bool &state, const QString &name)
{
    createFloridWidget(MusicSpectrum::Module::Florid, state, name, m_ui->spectrumFloridAreaLayout);
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    bool state = true;
    createLightWidget(MusicSpectrum::Module::Light, state, LIGHT_SPECTROGRAM_MODULE, m_ui->spectrumLightAreaLayout);
}

void MusicSpectrumWidget::openFileButtonClicked()
{
    const QString &path = TTK::File::getOpenFileName(this, MusicFormats::supportSpekInputFormats());
    if(!path.isEmpty())
    {
        bool state = true;
        createLightWidget(MusicSpectrum::Module::Light, state, LIGHT_SPECTROGRAM_MODULE, m_ui->spectrumLightAreaLayout, path);
    }
}

void MusicSpectrumWidget::fullscreenByUser(QWidget *widget, bool state)
{
    if(state)
    {
        QWidget *parent = TTKObjectCast(QWidget*, widget->parent());
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

            TTK::TTKQmmp::enabledVisualPlugin(type.m_module, false);

            bool state = true;
            switch(type.m_type)
            {
                case MusicSpectrum::Module::Normal: createSpectrumWidget(MusicSpectrum::Module::Normal, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Module::Plus: createSpectrumWidget(MusicSpectrum::Module::Plus, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Module::Wave: createSpectrumWidget(MusicSpectrum::Module::Wave, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Module::Flow: createFlowWidget(MusicSpectrum::Module::Flow, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Module::Florid: createFloridWidget(MusicSpectrum::Module::Florid, state, type.m_module, m_spectrumLayout); break;
                case MusicSpectrum::Module::Light: createLightWidget(MusicSpectrum::Module::Light, state, type.m_module, m_spectrumLayout); break;
                default: break;
            }
#endif
        }
    }
}

void MusicSpectrumWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    MusicAbstractMoveResizePlainWidget::mouseDoubleClickEvent(event);
    if(event->buttons() == Qt::LeftButton)
    {
        isMaximized() ? showNormal() : showMaximized();
    }
}

void MusicSpectrumWidget::createSpectrumWidget(MusicSpectrum::Module spectrum, bool &state, const QString &name, QLayout *layout)
{
    if(state)
    {
        const int before = Visual::visuals()->count();
        TTK::TTKQmmp::enabledVisualPlugin(name, true);
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
            m_types << type;
            layout->addWidget(type.m_object);
            type.m_object->setStyleSheet(TTK::UI::MenuStyle02);

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
            TTK::TTKQmmp::enabledVisualPlugin(name, false);
        }
    }
}

void MusicSpectrumWidget::createFlowWidget(MusicSpectrum::Module spectrum, bool &state, const QString &name, QLayout *layout)
{
    createModuleWidget(spectrum, state, name, layout, false);
}

void MusicSpectrumWidget::createFloridWidget(MusicSpectrum::Module spectrum, bool &state, const QString &name, QLayout *layout)
{
    createModuleWidget(spectrum, state, name, layout, true);
}

void MusicSpectrumWidget::createModuleWidget(MusicSpectrum::Module spectrum, bool &state, const QString &name, QLayout *layout, bool florid)
{
    QString *module = florid ? &m_lastFloridName : &m_lastFlowName;
    if(*module == name)
    {
        state = true;
        return;
    }

    const int index = findSpectrumWidget(*module);
    if(index != -1)
    {
        const MusicSpectrum &type = m_types.takeAt(index);
        layout->removeWidget(type.m_object);
        TTK::TTKQmmp::enabledVisualPlugin(*module, false);
    }

    if(!state)
    {
        module->clear();
        return;
    }

    const int before = Visual::visuals()->count();
    TTK::TTKQmmp::enabledVisualPlugin(name, true);
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
        m_types << type;
        layout->addWidget(type.m_object);
        type.m_object->setStyleSheet(TTK::UI::MenuStyle02);

        if(florid)
        {
            TTKObjectCast(Florid*, type.m_object)->setPixmap(MusicTopAreaWidget::instance()->rendererPixmap());
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

void MusicSpectrumWidget::createLightWidget(MusicSpectrum::Module spectrum, bool &state, const QString &name, QLayout *layout, const QString &url)
{
    if(state)
    {
        if(findSpectrumWidget(name) == -1)
        {
            QPluginLoader loader;
            loader.setFileName(TTK::TTKQmmp::pluginPath("Light", name));
            const QObject *obj = loader.instance();
            LightFactory *factory = nullptr;
            if(obj && (factory = TTKObjectCast(LightFactory*, obj)))
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
        if(index == -1)
        {
            return;
        }

        Light *light = TTKObjectCast(Light*, m_types[index].m_object);
        if(!light)
        {
            return;
        }

        const QString &path = url.isEmpty() ? SoundCore::instance()->path() : url;
        if(LIGHT_WAVEFORM_MODULE == name)
        {
            light->open(path);
        }
        else if(LIGHT_SPECTROGRAM_MODULE == name)
        {
            const QString &suffix = TTK_FILE_SUFFIX(QFileInfo(path));
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
    MusicToastLabel::popup(tr("Spectrum init error"));
}
