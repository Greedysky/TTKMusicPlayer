#include "musicspectrumwidget.h"
#include "ui_musicspectrumwidget.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicformats.h"

#include <QFileDialog>
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

    m_ui->analyzerBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->analyzer2Box->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->ewaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->gwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->histogramBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->xrayBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->fwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->monowaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->multiwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->volumeWaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->fspekBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->spekBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->spekBox->hide();

    m_ui->localFileButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->localFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->openFileButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->openFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->localFileButton, SIGNAL(clicked()), SLOT(localFileButtonClicked()));
    connect(m_ui->openFileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));

#ifdef Q_OS_UNIX
    m_ui->analyzerBox->setFocusPolicy(Qt::NoFocus);
    m_ui->analyzer2Box->setFocusPolicy(Qt::NoFocus);
    m_ui->ewaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->gwaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->histogramBox->setFocusPolicy(Qt::NoFocus);
    m_ui->xrayBox->setFocusPolicy(Qt::NoFocus);

    m_ui->fwaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->monowaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->multiwaveBox->setFocusPolicy(Qt::NoFocus);
    m_ui->fspekBox->setFocusPolicy(Qt::NoFocus);
    m_ui->volumeWaveBox->setFocusPolicy(Qt::NoFocus);

    m_ui->spekBox->setFocusPolicy(Qt::NoFocus);

    m_ui->localFileButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openFileButton->setFocusPolicy(Qt::NoFocus);
#endif

    QButtonGroup *group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(m_ui->analyzerBox, 0);
    group->addButton(m_ui->analyzer2Box, 1);
    group->addButton(m_ui->ewaveBox, 2);
    group->addButton(m_ui->gwaveBox, 3);
    group->addButton(m_ui->histogramBox, 4);
    group->addButton(m_ui->xrayBox, 5);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(spectrumTypeChanged(int)));

    QButtonGroup *group1 = new QButtonGroup(this);
    group1->setExclusive(false);
    group1->addButton(m_ui->fwaveBox, 0);
    group1->addButton(m_ui->monowaveBox, 1);
    group1->addButton(m_ui->multiwaveBox, 2);
    group1->addButton(m_ui->volumeWaveBox, 3);
    group1->addButton(m_ui->fspekBox, 4);
    connect(group1, SIGNAL(buttonClicked(int)), SLOT(spectrumPlusTypeChanged(int)));

    connect(m_ui->mainViewWidget, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    foreach(const MusicSpectrum &type, m_types)
    {
        showSpectrum(type.m_name, false);
    }
    delete m_ui;
}

QString MusicSpectrumWidget::getClassName()
{
    return staticMetaObject.className();
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
            adjustWidgetLayout(m_ui->spekLayout->count() - ITEM_DEFAULT_COUNT);
            break;
        default: break;
    }
}

void MusicSpectrumWidget::spectrumTypeChanged(int index)
{
    switch(index)
    {
        case 0:
            newSpectrumWidget(m_ui->analyzerBox, "analyzer", m_ui->spectrumAreaLayout);
            break;
        case 1:
            newSpectrumWidget(m_ui->analyzer2Box, "lineplus", m_ui->spectrumAreaLayout);
            break;
        case 2:
            newSpectrumWidget(m_ui->ewaveBox, "espewave", m_ui->spectrumAreaLayout);
            break;
        case 3:
            newSpectrumWidget(m_ui->gwaveBox, "gspewave", m_ui->spectrumAreaLayout);
            break;
        case 4:
            newSpectrumWidget(m_ui->histogramBox, "histogram", m_ui->spectrumAreaLayout);
            break;
        case 5:
            newSpectrumWidget(m_ui->xrayBox, "xray", m_ui->spectrumAreaLayout);
            break;
        default: break;
    }

    adjustWidgetLayout(m_ui->spectrumAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::spectrumPlusTypeChanged(int index)
{
    switch(index)
    {
        case 0:
            newSpectrumWidget(m_ui->fwaveBox, "fspewave", m_ui->spectrumPlusAreaLayout);
            break;
        case 1:
            newSpectrumWidget(m_ui->monowaveBox, "monowave", m_ui->spectrumPlusAreaLayout);
            break;
        case 2:
            newSpectrumWidget(m_ui->multiwaveBox, "multiwave", m_ui->spectrumPlusAreaLayout);
            break;
        case 3:
            newSpectrumWidget(m_ui->volumeWaveBox, "volumewave", m_ui->spectrumPlusAreaLayout);
            break;
        case 4:
            newSpekWidget(m_ui->fspekBox, "fspek", m_ui->spectrumPlusAreaLayout);
            fspekStateChanged();
            break;
        default: break;
    }

    adjustWidgetLayout(m_ui->spectrumPlusAreaLayout->count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    newSpekWidget(m_ui->spekBox, "qspek", m_ui->spekAreaLayout);

    int index = findSpectrumWidget("qspek");
    if(index != -1)
    {
        Spek *spek = MStatic_cast(Spek*, m_types[index].m_obj);
        spek->open( SoundCore::instance()->url() );
    }
}

void MusicSpectrumWidget::openFileButtonClicked()
{
    newSpekWidget(m_ui->spekBox, "qspek", m_ui->spekAreaLayout);

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters(MusicFormats::supportFormatsSpekString());
    if(dialog.exec())
    {
        QString path = dialog.selectedFiles().last();
        int index = findSpectrumWidget("qspek");
        if(index != -1 && !path.isEmpty())
        {
            Spek *spek = MStatic_cast(Spek*, m_types[index].m_obj);
            spek->open( path );
        }
    }
}

void MusicSpectrumWidget::closeEvent(QCloseEvent *event)
{
    emit resetFlag(MusicObject::TT_Spectrum);
    MusicAbstractMoveWidget::closeEvent(event);
}

void MusicSpectrumWidget::newSpectrumWidget(QCheckBox *box, const QString &name, QLayout *layout)
{
    if(box->isChecked())
    {
        int before = Visual::visuals()->count();
        showSpectrum(name, true);
        QList<Visual *> *vs = Visual::visuals();
        if(before == vs->count())
        {
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
    }
    else
    {
        int index = findSpectrumWidget(name);
        if(index != -1)
        {
            MusicSpectrum t = m_types.takeAt(index);
            layout->removeWidget(t.m_obj);
            showSpectrum(name, false);
        }
    }
}

void MusicSpectrumWidget::newSpekWidget(QCheckBox *box, const QString &name, QLayout *layout)
{
    if(box->isChecked())
    {
        if(findSpectrumWidget(name) == -1)
        {
            QPluginLoader loader;
            loader.setFileName(MusicUtils::Core::pluginPath("Spek", name));
            QObject *obj = loader.instance();
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
    }
    else
    {
        int index = findSpectrumWidget(name);
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

void MusicSpectrumWidget::showSpectrum(const QString &name, bool state)
{
    foreach(VisualFactory *v, Visual::factories())
    {
        if(v->properties().shortName.contains(name))
        {
            Visual::setEnabled(v, state);
        }
    }
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

void MusicSpectrumWidget::fspekStateChanged()
{
    if(m_ui->fspekBox->isChecked())
    {
        int index = findSpectrumWidget("fspek");
        if(index != -1)
        {
            Spek *spek = MStatic_cast(Spek*, m_types[index].m_obj);
            spek->open( SoundCore::instance()->url() );
        }
    }
}
