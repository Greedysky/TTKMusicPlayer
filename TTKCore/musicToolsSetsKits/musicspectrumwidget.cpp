#include "musicspectrumwidget.h"
#include "ui_musicspectrumwidget.h"
#include "musicuiobject.h"
///qmmp incldue
#include "visual.h"
#include "visualfactory.h"

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
            m_ui->viewLayout->addWidget(t.m_obj); \
            m_types << t; \
        } \
    } \
    else \
    { \
        int index = findSpectrumWidget(type); \
        if(index != -1) \
        { \
            MusicSpectrum t = m_types.takeAt(index); \
            m_ui->viewLayout->removeWidget(t.m_obj); \
            showSpectrum(type, false); \
        } \
    }


MusicSpectrumWidget::MusicSpectrumWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSpectrumWidget)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

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

    QButtonGroup *group = new QButtonGroup(this);
    group->setExclusive(false);
    group->addButton(m_ui->analyzerBox, 0);
    group->addButton(m_ui->analyzer2Box, 1);
    group->addButton(m_ui->ewaveBox, 2);
    group->addButton(m_ui->fwaveBox, 3);
    group->addButton(m_ui->gwaveBox, 4);
    group->addButton(m_ui->histogramBox, 5);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(spectrumTypeChanged(int)));
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

    adjustWidgetLayout();
}

void MusicSpectrumWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSpectrumWidget::closeEvent(QCloseEvent *event)
{
    emit resetFlag(MusicObject::TT_Spectrum);
    MusicAbstractMoveWidget::closeEvent(event);
}

void MusicSpectrumWidget::adjustWidgetLayout()
{
    int offset = m_types.count() - ITEM_DEFAULT_COUNT;
    if(offset < 0)
    {
        offset = 0;
    }
    offset *= ITEM_OFFSET;

    setFixedHeight(offset + 403);
    m_ui->background->setFixedHeight(offset + 395);
    m_ui->backgroundMask->setFixedHeight(offset + 370);
    m_ui->verticalLayoutWidget->setFixedHeight(offset + 311);

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
