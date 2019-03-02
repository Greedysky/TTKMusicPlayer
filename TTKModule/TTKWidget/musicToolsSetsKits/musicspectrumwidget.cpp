#include "musicspectrumwidget.h"
#include "ui_musicspectrumwidget.h"
#include "musicuiobject.h"
#include "musicqmmputils.h"
#include "musicformats.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"
#include "musicsinglemanager.h"
#include "musicqmmputils.h"

#include <QButtonGroup>
#include <QPluginLoader>

///qmmp incldue
#include "visual.h"
#include "visualfactory.h"
#include "spekfactory.h"
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
    m_ui->envelopespekBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->flashgoomBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->flashmeterBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->spectrumspekBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->spectrumspekBox->hide();

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
    m_ui->envelopespekBox->setFocusPolicy(Qt::NoFocus);

    m_ui->spectrumspekBox->setFocusPolicy(Qt::NoFocus);
    m_ui->flashgoomBox->setFocusPolicy(Qt::NoFocus);
    m_ui->flashmeterBox->setFocusPolicy(Qt::NoFocus);

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
    group1->addButton(m_ui->envelopespekBox, 6);
    connect(group1, SIGNAL(buttonClicked(int)), SLOT(spectrumPlusTypeChanged(int)));

    QButtonGroup *group2 = new QButtonGroup(this);
    group2->setExclusive(false);
    group2->addButton(m_ui->flashgoomBox, 0);
    group2->addButton(m_ui->flashmeterBox, 1);
    connect(group2, SIGNAL(buttonClicked(int)), SLOT(spectrumDazzleTypeChanged(int)));

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
            adjustWidgetLayout(m_ui->dazzleLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        case 3:
            adjustWidgetLayout(m_ui->spekLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        default:
            break;
    }
}

void MusicSpectrumWidget::spectrumTypeChanged(int index)
{
    switch(index)
    {
        case 0:
            newSpectrumWidget(m_ui->analyzerBox, "normalanalyzer", m_ui->spectrumAreaLayout);
            break;
        case 1:
            newSpectrumWidget(m_ui->ewaveBox, "normalewave", m_ui->spectrumAreaLayout);
            break;
        case 2:
            newSpectrumWidget(m_ui->flowwaveBox, "normalflowwave", m_ui->spectrumAreaLayout);
            break;
        case 3:
            newSpectrumWidget(m_ui->histogramBox, "normalhistogram", m_ui->spectrumAreaLayout);
            break;
        case 4:
            newSpectrumWidget(m_ui->lineBox, "normalline", m_ui->spectrumAreaLayout);
            break;
        case 5:
            newSpectrumWidget(m_ui->spacewaveBox, "normalspacewave", m_ui->spectrumAreaLayout);
            break;
        default:
            break;
    }

    adjustWidgetLayout(m_ui->spectrumAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumPlusTypeChanged(int index)
{
    switch(index)
    {
        case 0:
            newSpectrumWidget(m_ui->foldwaveBox, "plusfoldwave", m_ui->spectrumPlusAreaLayout);
            break;
        case 1:
            newSpectrumWidget(m_ui->monowaveBox, "plusmonowave", m_ui->spectrumPlusAreaLayout);
            break;
        case 2:
            newSpectrumWidget(m_ui->multiwaveBox, "plusmultiwave", m_ui->spectrumPlusAreaLayout);
            break;
        case 3:
            newSpectrumWidget(m_ui->xraysBox, "plusxrays", m_ui->spectrumPlusAreaLayout);
            break;
        case 4:
            newSpectrumWidget(m_ui->pointxraysBox, "pluspointxrays", m_ui->spectrumPlusAreaLayout);
            break;
        case 5:
            newSpectrumWidget(m_ui->volumeWaveBox, "plusvolumewave", m_ui->spectrumPlusAreaLayout);
            break;
        case 6:
            newSpekWidget(m_ui->envelopespekBox, "envelopespek", m_ui->spectrumPlusAreaLayout);
            break;
        default: break;
    }

    adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumDazzleTypeChanged(int index)
{
    switch(index)
    {
        case 0:
            newSpectrumWidget(m_ui->flashgoomBox, "flashgoom", m_ui->dazzleAreaLayout);
            break;
        case 1:
            newSpectrumWidget(m_ui->flashmeterBox, "flashmeter", m_ui->dazzleAreaLayout);
            break;
        default: break;
    }

    adjustWidgetLayout(m_ui->dazzleAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    newSpekWidget(m_ui->spectrumspekBox, "spectrumspek", m_ui->spekAreaLayout);
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
        newSpekWidget(m_ui->spectrumspekBox, "spectrumspek", m_ui->spekAreaLayout, path);
    }
}

void MusicSpectrumWidget::newSpectrumWidget(QCheckBox *box, const QString &name, QLayout *layout)
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

void MusicSpectrumWidget::newSpekWidget(QCheckBox *box, const QString &name, QLayout *layout, const QString &url)
{
    if(box->isChecked())
    {
        if(findSpectrumWidget(name) == -1)
        {
            QPluginLoader loader;
            loader.setFileName(MusicUtils::QMMP::pluginPath("Spek", name));
            const QObject *obj = loader.instance();
            SpekFactory *decoderfac = nullptr;
            if(obj && (decoderfac = MObject_cast(SpekFactory*, obj)))
            {
                Spek *spekWidget = decoderfac->create(this);
                MusicSpectrum sp;
                sp.m_name = name;
                sp.m_obj = spekWidget;
                m_types << sp;
                layout->addWidget(spekWidget);
            }
        }

        const int index = findSpectrumWidget(name);
        Spek *spek = MStatic_cast(Spek*, m_types[index].m_obj);
        spek->open(url.isEmpty() ? SoundCore::instance()->path() : url );
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

void MusicSpectrumWidget::showMessageBoxWidget(QCheckBox *box)
{
    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->defaultLabel(this, tr("Spectrum Init Error!"));
    box->setChecked(false);
}
