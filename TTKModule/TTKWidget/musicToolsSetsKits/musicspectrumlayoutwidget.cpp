#include "musicspectrumlayoutwidget.h"
#include "musicwidgetutils.h"
#include "musicimageutils.h"
#include "ttkclickedgroup.h"

#include <QScrollArea>

MusicSpectrumLayoutItem::MusicSpectrumLayoutItem(QWidget *parent)
    : TTKClickedLabel(parent),
      m_isSelected(false)
{
    setFixedSize(219, 123);
}

void MusicSpectrumLayoutItem::addCellItem(const QString &item, const QString &tip)
{
    setPixmap(TTK::Image::roundedPixmap(item, 10, 10));
    setToolTip(tip);
}

void MusicSpectrumLayoutItem::setChecked(bool checked)
{
    m_isSelected = checked;
    update();
}

bool MusicSpectrumLayoutItem::isChecked() const
{
    return m_isSelected;
}

void MusicSpectrumLayoutItem::paintEvent(QPaintEvent *event)
{
    TTKClickedLabel::paintEvent(event);
    if(m_isSelected)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(width() - 17, height() - 17, 17, 17, QPixmap(":/tiny/lb_selected"));
    }
}



MusicSpectrumLayoutWidget::MusicSpectrumLayoutWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

MusicSpectrumLayoutWidget::~MusicSpectrumLayoutWidget()
{
    qDeleteAll(m_items);
}

void MusicSpectrumLayoutWidget::popupMenu()
{
    m_menu->exec(mapToGlobal(QPoint(-m_containWidget->width() + width(), 0)));
}

void MusicSpectrumLayoutWidget::labelClicked(int index)
{
    if(m_exclusive)
    {
        for(MusicSpectrumLayoutItem *item : qAsConst(m_items))
        {
            item->setChecked(false);
        }
    }

    const QStringList &types = spectrumTypeList();
    bool state = !m_items[index]->isChecked();
    m_items[index]->setChecked(state);

    Q_EMIT stateChanged(state, types[index]);
    if(!state)
    {
        m_items[index]->setChecked(false);
    }
    m_menu->close();
}

void MusicSpectrumLayoutWidget::initialize()
{
    m_exclusive = false;

    const QString &style = TTK::UI::BorderStyle02 + TTK::UI::BackgroundStyle10;
    setObjectName(MusicSpectrumLayoutWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), style));

    m_containWidget->setFixedSize(240, 440);
    m_containWidget->setObjectName("ContainWidget");
    m_containWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_containWidget->objectName(), style));

    QVBoxLayout *layout = new QVBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea(m_containWidget);
    QWidget *containWidget = new QWidget(scrollArea);
    containWidget->setStyleSheet(TTK::UI::BackgroundStyle10);
    m_containLayout = new QVBoxLayout(containWidget);
    m_containLayout->setContentsMargins(5, 0, 0, 0);
    m_containLayout->setSpacing(20);
    containWidget->setLayout(m_containLayout);

    TTK::Widget::generateVScrollAreaStyle(scrollArea, containWidget);
    scrollArea->viewport()->setStyleSheet(TTK::UI::BackgroundStyle10);
    layout->addWidget(scrollArea);

    m_containWidget->setLayout(layout);
}

void MusicSpectrumLayoutWidget::addCellItems(const SpectrumInfoList &items)
{
    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(labelClicked(int)));

    for(int i = 0; i < items.count(); ++i)
    {
        const SpectrumInfo &info = items[i];
        MusicSpectrumLayoutItem *item = new MusicSpectrumLayoutItem(this);
        item->addCellItem(info.first, info.second);

        clickedGroup->addWidget(item);
        m_containLayout->addWidget(item);
        m_items << item;
    }
}



MusicSpectrumNormalLayoutWidget::MusicSpectrumNormalLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    SpectrumInfoList items;
    items << SpectrumInfo(":/spectrum/normal_1", tr("Analyzer"));
    items << SpectrumInfo(":/spectrum/normal_2", tr("EWave"));
    items << SpectrumInfo(":/spectrum/normal_3", tr("FlowWave"));
    items << SpectrumInfo(":/spectrum/normal_4", tr("Histogram"));
    items << SpectrumInfo(":/spectrum/normal_5", tr("Line"));
    addCellItems(items);
}

QStringList MusicSpectrumNormalLayoutWidget::spectrumTypeList() const
{
    return {"normalanalyzer", "normalewave", "normalflowwave", "normalhistogram", "normalline"};
}



MusicSpectrumPlusLayoutWidget::MusicSpectrumPlusLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    SpectrumInfoList items;
    items << SpectrumInfo(":/spectrum/plus_1", tr("FoldWave"));
    items << SpectrumInfo(":/spectrum/plus_2", tr("XRays"));
    items << SpectrumInfo(":/spectrum/plus_3", tr("BlurXRays"));
    addCellItems(items);
}

QStringList MusicSpectrumPlusLayoutWidget::spectrumTypeList() const
{
    return {"plusfoldwave", "plusxrays", "plusblurxrays"};
}



MusicSpectrumFlowLayoutWidget::MusicSpectrumFlowLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    m_exclusive = true;

    SpectrumInfoList items;
    items << SpectrumInfo(":/spectrum/flow_1", tr("Goom"));
    items << SpectrumInfo(":/spectrum/flow_2", tr("Dazzle"));
    items << SpectrumInfo(":/spectrum/flow_3", tr("Sonique"));
    items << SpectrumInfo(":/spectrum/flow_4", tr("Jakdaw"));
    items << SpectrumInfo(":/spectrum/flow_5", tr("MadSpin"));
    items << SpectrumInfo(":/spectrum/flow_6", tr("Infinity"));
    items << SpectrumInfo(":/spectrum/flow_7", tr("Mountain"));
    items << SpectrumInfo(":/spectrum/flow_8", tr("Matrix"));
    items << SpectrumInfo(":/spectrum/flow_9", tr("VU Meter"));
    addCellItems(items);
}

QStringList MusicSpectrumFlowLayoutWidget::spectrumTypeList() const
{
    return {"flowgoom", "flowprojectm", "flowsonique", "flowjakdaw", "flowmadspin", "flowinfinity", "flowmountain", "flowmatrix", "flowvumeter"};
}



MusicSpectrumWaveLayoutWidget::MusicSpectrumWaveLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    SpectrumInfoList items;
    items << SpectrumInfo(":/spectrum/wave_1", tr("Voice"));
    items << SpectrumInfo(":/spectrum/wave_2", tr("Crest"));
    items << SpectrumInfo(":/spectrum/wave_3", tr("Volume"));
    items << SpectrumInfo(":/spectrum/wave_4", tr("WaveForm"));
    addCellItems(items);
}

QStringList MusicSpectrumWaveLayoutWidget::spectrumTypeList() const
{
    return {"wavevoice", "wavecrest", "wavevolume", "lightwaveform"};
}



MusicSpectrumFloridLayoutWidget::MusicSpectrumFloridLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    m_exclusive = true;

    SpectrumInfoList items;
    items << SpectrumInfo(":/spectrum/florid_1", tr("Reverb"));
    items << SpectrumInfo(":/spectrum/florid_2", tr("Autism"));
    items << SpectrumInfo(":/spectrum/florid_3", tr("Bass"));
    items << SpectrumInfo(":/spectrum/florid_4", tr("Surround"));
    items << SpectrumInfo(":/spectrum/florid_5", tr("Ancient"));
    items << SpectrumInfo(":/spectrum/florid_6", tr("Electric"));
    items << SpectrumInfo(":/spectrum/florid_7", tr("Picture"));
    addCellItems(items);
}

QStringList MusicSpectrumFloridLayoutWidget::spectrumTypeList() const
{
    return {"floridreverb", "floridautism", "floridbass", "floridsurround", "floridancient", "floridelectric", "floridpicture"};
}
