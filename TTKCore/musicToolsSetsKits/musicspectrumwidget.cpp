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

#define NEW_OPERATOR(widget, type)  \
    if(m_ui->widget->isChecked()) \
    { \
        showSpectrum(type, true); \
        QList<Visual *> *vs = Visual::visuals(); \
        if(!vs->isEmpty()) \
        { \
            MusicSpectrum t; \
            t.m_name = type; \
            t.m_obj = vs->last(); \
            m_ui->spectrumAreaLayout->addWidget(t.m_obj); \
            m_types << t; \
        } \
    } \
    else \
    { \
        int index = findSpectrumWidget(type); \
        if(index != -1) \
        { \
            MusicSpectrum t = m_types.takeAt(index); \
            m_ui->spectrumAreaLayout->removeWidget(t.m_obj); \
            showSpectrum(type, false); \
        } \
    }


MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveWidget(false, parent),
      m_ui(new Ui::MusicSpectrumWidget)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_spekWidget = nullptr;
    setStyleSheet(MusicUIObject::MMenuStyle02);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->analyzerBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->analyzer2Box->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->ewaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->fwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->gwaveBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->histogramBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);

    m_ui->localFileButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->localFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->openFileButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->openFileButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->localFileButton, SIGNAL(clicked()), SLOT(localFileButtonClicked()));
    connect(m_ui->openFileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));

    QButtonGroup *group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(m_ui->analyzerBox, 0);
    group->addButton(m_ui->analyzer2Box, 1);
    group->addButton(m_ui->ewaveBox, 2);
    group->addButton(m_ui->fwaveBox, 3);
    group->addButton(m_ui->gwaveBox, 4);
    group->addButton(m_ui->histogramBox, 5);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(spectrumTypeChanged(int)));

    connect(m_ui->mainViewWidget, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

MusicSpectrumWidget::~MusicSpectrumWidget()
{
    foreach(const MusicSpectrum &type, m_types)
    {
        showSpectrum(type.m_name, false);
    }
    delete m_spekWidget;
    delete m_ui;
}

QString MusicSpectrumWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSpectrumWidget::tabIndexChanged(int index)
{
    adjustWidgetLayout(index == 0 ? m_types.count() - ITEM_DEFAULT_COUNT : 0);
}

void MusicSpectrumWidget::spectrumTypeChanged(int index)
{
    switch(index)
    {
        case 0:
            NEW_OPERATOR(analyzerBox, "analyzer");
            break;
        case 1:
            NEW_OPERATOR(analyzer2Box, "lineplus");
            break;
        case 2:
            NEW_OPERATOR(ewaveBox, "ewave");
            break;
        case 3:
            NEW_OPERATOR(fwaveBox, "fwave");
            break;
        case 4:
            NEW_OPERATOR(gwaveBox, "gwave");
            break;
        case 5:
            NEW_OPERATOR(histogramBox, "histogram");
            break;
        default: break;
    }

    adjustWidgetLayout(m_types.count() - ITEM_DEFAULT_COUNT);
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::localFileButtonClicked()
{
    createSpekWidget();

    if(m_spekWidget)
    {
        m_spekWidget->open( SoundCore::instance()->url() );
    }
}

void MusicSpectrumWidget::openFileButtonClicked()
{
    createSpekWidget();

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters(MusicFormats::supportFormatsSpekString());
    if(dialog.exec())
    {
        QString path = dialog.selectedFiles().last();
        if(m_spekWidget && !path.isEmpty())
        {
            m_spekWidget->open( path );
        }
    }
}

void MusicSpectrumWidget::closeEvent(QCloseEvent *event)
{
    emit resetFlag(MusicObject::TT_Spectrum);
    MusicAbstractMoveWidget::closeEvent(event);
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

void MusicSpectrumWidget::createSpekWidget()
{
    if(!m_spekWidget)
    {
        QPluginLoader loader;
        loader.setFileName(MusicUtils::Core::pluginPath("Spek", "spek"));
        QObject *obj = loader.instance();
        SpekFactory *decoderfac = nullptr;
        if(obj && (decoderfac = MObject_cast(SpekFactory*, obj)) )
        {
            m_spekWidget = decoderfac->create(this);
            m_ui->spekAreaLayout->addWidget(m_spekWidget);
        }
    }
}
