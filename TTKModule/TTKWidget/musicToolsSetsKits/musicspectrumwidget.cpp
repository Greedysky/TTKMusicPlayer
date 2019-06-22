#include "musicspectrumwidget.h"
#include "ui_musicspectrumwidget.h"
#include "musicuiobject.h"
#include "musicqmmputils.h"
#include "musicformats.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"
#include "musicsinglemanager.h"
#include "musicqmmputils.h"
#include "musictopareawidget.h"

#include <QButtonGroup>
#include <QPluginLoader>

///qmmp incldue
#include "florid.h"
#include "visualfactory.h"
#include "lightfactory.h"
#include "soundcore.h"

#define ITEM_DEFAULT_COUNT      3
#define ITEM_OFFSET             107

MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveWidget(false, parent),
      m_ui(new Ui::MusicSpectrumWidget)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    setStyleSheet(MusicUIObject::MMenuStyle02);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->mainViewWidget->setStyleSheet(MusicUIObject::MTabWidgetStyle01);
    m_ui->analyzerBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->ewaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->flowwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->histogramBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->lineBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->spacewaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->foldwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->monowaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->multiwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->xraysBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->pointxraysBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->volumeWaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->lightenvelopeBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->floridgoomBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->floridreverbBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->floridautismBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->floridbassBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->floridsurroundBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->floridancientBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->floridelectricBox->setStyleSheet(MusicUIObject::MRadioButtonStyle01);

    m_ui->lightspectrumBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->lightspectrumBox->hide();

    m_ui->localFileButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->localFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->openFileButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->openFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->localFileButton, SIGNAL(clicked()), SLOT(localFileButtonClicked()));
    connect(m_ui->openFileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));

#ifdef Q_OS_UNIX
    m_ui->analyzerBox->setFocusPolicy(Qt::NoFocus);
    m_ui->ewaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->flowwaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->histogramBox->setFocusPolicy(Qt::NoFocus);
    m_ui->lineBox->setFocusPolicy(Qt::NoFocus);
    m_ui->spacewaveBox->setFocusPolicy(Qt::NoFocus);

    m_ui->foldwaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->monowaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->multiwaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->xraysBox->setFocusPolicy(Qt::NoFocus);
    m_ui->pointxraysBox->setFocusPolicy(Qt::NoFocus);
    m_ui->volumeWaveBox->setFocusPolicy(Qt::NoFocus);

    m_ui->floridgoomBox->setFocusPolicy(Qt::NoFocus);
    m_ui->floridreverbBox->setFocusPolicy(Qt::NoFocus);
    m_ui->floridautismBox->setFocusPolicy(Qt::NoFocus);
    m_ui->floridbassBox->setFocusPolicy(Qt::NoFocus);
    m_ui->floridsurroundBox->setFocusPolicy(Qt::NoFocus);
    m_ui->floridancientBox->setFocusPolicy(Qt::NoFocus);
    m_ui->floridelectricBox->setFocusPolicy(Qt::NoFocus);

    m_ui->lightenvelopeBox->setFocusPolicy(Qt::NoFocus);
    m_ui->lightspectrumBox->setFocusPolicy(Qt::NoFocus);

    m_ui->localFileButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openFileButton->setFocusPolicy(Qt::NoFocus);
#endif

    QButtonGroup *group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(m_ui->analyzerBox, 0);
    group->addButton(m_ui->ewaveBox, 1);
    group->addButton(m_ui->flowwaveBox, 2);
    group->addButton(m_ui->histogramBox, 3);
    group->addButton(m_ui->lineBox, 4);
    group->addButton(m_ui->spacewaveBox, 5);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(spectrumTypeChanged(int)));

    QButtonGroup *group1 = new QButtonGroup(this);
    group1->setExclusive(false);
    group1->addButton(m_ui->foldwaveBox, 0);
    group1->addButton(m_ui->monowaveBox, 1);
    group1->addButton(m_ui->multiwaveBox, 2);
    group1->addButton(m_ui->xraysBox, 3);
    group1->addButton(m_ui->pointxraysBox, 4);
    group1->addButton(m_ui->volumeWaveBox, 5);
    group1->addButton(m_ui->lightenvelopeBox, 6);
    connect(group1, SIGNAL(buttonClicked(int)), SLOT(spectrumPlusTypeChanged(int)));

    QButtonGroup *group2 = new QButtonGroup(this);
    group2->addButton(m_ui->floridgoomBox, 0);
    group2->addButton(m_ui->floridreverbBox, 1);
    group2->addButton(m_ui->floridautismBox, 2);
    group2->addButton(m_ui->floridbassBox, 3);
    group2->addButton(m_ui->floridsurroundBox, 4);
    group2->addButton(m_ui->floridancientBox, 5);
    group2->addButton(m_ui->floridelectricBox, 6);
    connect(group2, SIGNAL(buttonClicked(int)), SLOT(spectrumFloridTypeChanged(int)));

    connect(m_ui->mainViewWidget, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    M_SINGLE_MANAGER_PTR->removeObject(getClassName());
    foreach(const MusicSpectrum &type, m_types)
    {
        MusicUtils::QMMP::enableVisualPlugin(type.m_name, false);
    }
    delete m_ui;
}

void MusicSpectrumWidget::tabIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            adjustWidgetLayout(m_ui->spectrumAreaLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 1:
            adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 2:
            adjustWidgetLayout(m_ui->floridLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 3:
            adjustWidgetLayout(m_ui->lightLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        default:
            break;
    }
}

void MusicSpectrumWidget::spectrumTypeChanged(int index)
{
    const QStringList &types = spectrumTypeList();
    switch(index)
    {
        case 0:
            createSpectrumWidget(m_ui->analyzerBox, types[index], m_ui->spectrumAreaLayout);
            break;
        case 1:
            createSpectrumWidget(m_ui->ewaveBox, types[index], m_ui->spectrumAreaLayout);
            break;
        case 2:
            createSpectrumWidget(m_ui->flowwaveBox, types[index], m_ui->spectrumAreaLayout);
            break;
        case 3:
            createSpectrumWidget(m_ui->histogramBox, types[index], m_ui->spectrumAreaLayout);
            break;
        case 4:
            createSpectrumWidget(m_ui->lineBox, types[index], m_ui->spectrumAreaLayout);
            break;
        case 5:
            createSpectrumWidget(m_ui->spacewaveBox, types[index], m_ui->spectrumAreaLayout);
            break;
        default:
            break;
    }

    adjustWidgetLayout(m_ui->spectrumAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumPlusTypeChanged(int index)
{
    const QStringList &types = spectrumPlusTypeList();
    switch(index)
    {
        case 0:
            createSpectrumWidget(m_ui->foldwaveBox, types[index], m_ui->spectrumPlusAreaLayout);
            break;
        case 1:
            createSpectrumWidget(m_ui->monowaveBox, types[index], m_ui->spectrumPlusAreaLayout);
            break;
        case 2:
            createSpectrumWidget(m_ui->multiwaveBox, types[index], m_ui->spectrumPlusAreaLayout);
            break;
        case 3:
            createSpectrumWidget(m_ui->xraysBox, types[index], m_ui->spectrumPlusAreaLayout);
            break;
        case 4:
            createSpectrumWidget(m_ui->pointxraysBox, types[index], m_ui->spectrumPlusAreaLayout);
            break;
        case 5:
            createSpectrumWidget(m_ui->volumeWaveBox, types[index], m_ui->spectrumPlusAreaLayout);
            break;
        case 6:
            createLightWidget(m_ui->lightenvelopeBox, "lightenvelope", m_ui->spectrumPlusAreaLayout);
            break;
        default: break;
    }

    adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumFloridTypeChanged(int index)
{
    const QStringList &types = spectrumFloridTypeList();
    switch(index)
    {
        case 0:
            createFloridWidget(m_ui->floridgoomBox, types[index], m_ui->floridAreaLayout);
            break;
        case 1:
            createFloridWidget(m_ui->floridreverbBox, types[index], m_ui->floridAreaLayout);
            break;
        case 2:
            createFloridWidget(m_ui->floridautismBox, types[index], m_ui->floridAreaLayout);
            break;
        case 3:
            createFloridWidget(m_ui->floridbassBox, types[index], m_ui->floridAreaLayout);
            break;
        case 4:
            createFloridWidget(m_ui->floridsurroundBox, types[index], m_ui->floridAreaLayout);
            break;
        case 5:
            createFloridWidget(m_ui->floridancientBox, types[index], m_ui->floridAreaLayout);
            break;
        case 6:
            createFloridWidget(m_ui->floridelectricBox, types[index], m_ui->floridAreaLayout);
            break;
        default: break;
    }
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    createLightWidget(m_ui->lightspectrumBox, "lightspectrum", m_ui->lightAreaLayout);
}

void MusicSpectrumWidget::openFileButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters(MusicFormats::supportFormatsSpekString());
    if(dialog.exec())
    {
        const QString &path = dialog.selectedFiles().last();
        createLightWidget(m_ui->lightspectrumBox, "lightspectrum", m_ui->lightAreaLayout, path);
    }
}

QStringList MusicSpectrumWidget::spectrumTypeList()
{
    return QStringList() << "normalanalyzer" << "normalewave" <<"normalflowwave"
                         << "normalhistogram" << "normalline" <<"normalspacewave";
}

QStringList MusicSpectrumWidget::spectrumPlusTypeList()
{
    return QStringList() << "plusfoldwave" << "plusmonowave" <<"plusmultiwave"
                         << "plusxrays" << "pluspointxrays" <<"plusvolumewave";
}

QStringList MusicSpectrumWidget::spectrumFloridTypeList()
{
    return QStringList() << "floridgoom" << "floridreverb" << "floridautism"
                         << "floridbass" << "floridsurround" << "floridancient" << "floridelectric";
}

void MusicSpectrumWidget::createSpectrumWidget(QAbstractButton *box, const QString &name, QLayout *layout)
{
    if(box->isChecked())
    {
        const int before = Visual::visuals()->count();
        MusicUtils::QMMP::enableVisualPlugin(name, true);
        const QList<Visual*> *vs = Visual::visuals();
        if(before == vs->count())
        {
            showMessageBoxWidget(box);
            return;
        }

        if(!vs->isEmpty())
        {
            MusicSpectrum t;
            t.m_name = name;
            t.m_obj = vs->last();
            layout->addWidget(t.m_obj);
            m_types << t;
        }
        else
        {
            showMessageBoxWidget(box);
        }
    }
    else
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            MusicSpectrum t = m_types.takeAt(index);
            layout->removeWidget(t.m_obj);
            MusicUtils::QMMP::enableVisualPlugin(name, false);
        }
    }
}

void MusicSpectrumWidget::createLightWidget(QAbstractButton *box, const QString &name, QLayout *layout, const QString &url)
{
    if(box->isChecked())
    {
        if(findSpectrumWidget(name) == -1)
        {
            QPluginLoader loader;
            loader.setFileName(MusicUtils::QMMP::pluginPath("Light", name));
            const QObject *obj = loader.instance();
            LightFactory *decoderfac = nullptr;
            if(obj && (decoderfac = MObject_cast(LightFactory*, obj)))
            {
                Light *lightWidget = decoderfac->create(this);
                MusicSpectrum sp;
                sp.m_name = name;
                sp.m_obj = lightWidget;
                m_types << sp;
                layout->addWidget(lightWidget);
            }
        }

        const int index = findSpectrumWidget(name);
        Light *light = MStatic_cast(Light*, m_types[index].m_obj);
        light->open(url.isEmpty() ? SoundCore::instance()->path() : url );
    }
    else
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            MusicSpectrum t = m_types.takeAt(index);
            layout->removeWidget(t.m_obj);
            delete t.m_obj;
        }
    }
}

void MusicSpectrumWidget::createFloridWidget(QAbstractButton *box, const QString &name, QLayout *layout)
{
    foreach(const QString &name, spectrumFloridTypeList())
    {
        const int index = findSpectrumWidget(name);
        if(index != -1)
        {
            MusicSpectrum t = m_types.takeAt(index);
            layout->removeWidget(t.m_obj);
            MusicUtils::QMMP::enableVisualPlugin(name, false);
        }
    }

    const int before = Visual::visuals()->count();
    MusicUtils::QMMP::enableVisualPlugin(name, true);
    const QList<Visual*> *vs = Visual::visuals();
    if(before == vs->count())
    {
        showMessageBoxWidget(box);
        return;
    }

    if(!vs->isEmpty())
    {
        MusicSpectrum t;
        t.m_name = name;
        t.m_obj = vs->last();
        MStatic_cast(Florid*, t.m_obj)->setPixmap(MusicTopAreaWidget::instance()->getRendererPixmap());
        layout->addWidget(t.m_obj);
        m_types << t;
    }
    else
    {
        showMessageBoxWidget(box);
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
    for(int i=0; i<m_types.count(); ++i)
    {
        if(m_types[i].m_name.contains(name))
        {
            return i;
        }
    }

    return -1;
}

void MusicSpectrumWidget::showMessageBoxWidget(QAbstractButton *box)
{
    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->defaultLabel(this, tr("Spectrum Init Error!"));
    box->setChecked(false);
}
