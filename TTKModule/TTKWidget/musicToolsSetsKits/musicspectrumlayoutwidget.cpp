#include "musicspectrumlayoutwidget.h"
#include "musicuiobject.h"
#include "musicimageutils.h"

#include <QPainter>
#include <QScrollArea>
#include <QSignalMapper>

MusicSpectrumLayoutItem::MusicSpectrumLayoutItem(QWidget *parent)
    : MusicClickedLabel(parent)
{
    setFixedSize(219, 123);
    m_isSelected = false;
}

MusicSpectrumLayoutItem::~MusicSpectrumLayoutItem()
{

}

void MusicSpectrumLayoutItem::addItem(const QString &item, const QString &tip)
{
    setPixmap(MusicUtils::Image::pixmapToRound(item, 10, 10));
    setToolTip(tip);
}

void MusicSpectrumLayoutItem::setCheck(bool check)
{
    m_isSelected = check;
    update();
}

bool MusicSpectrumLayoutItem::isChecked() const
{
    return m_isSelected;
}

void MusicSpectrumLayoutItem::paintEvent(QPaintEvent *event)
{
    MusicClickedLabel::paintEvent(event);

    if(m_isSelected)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 17, height() - 17, 17, 17, QPixmap(":/tiny/lb_selected"));
    }
}



MusicSpectrumLayoutWidget::MusicSpectrumLayoutWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

MusicSpectrumLayoutWidget::~MusicSpectrumLayoutWidget()
{
    qDeleteAll(m_items);
}

void MusicSpectrumLayoutWidget::popupMenu()
{
    m_menu->exec( mapToGlobal(QPoint(-m_containWidget->width() + width(), 0)));
}

void MusicSpectrumLayoutWidget::labelClicked(int index)
{
    if(m_exclusive)
    {
        foreach(MusicSpectrumLayoutItem *item, m_items)
        {
            item->setCheck(false);
        }
    }

    const QStringList &types = spectrumTypeList();
    bool state = m_items[index]->isChecked();

    state = !state;
    m_items[index]->setCheck(state);

    Q_EMIT stateChanged(state, types[index]);
    if(!state)
    {
        m_items[index]->setCheck(false);
    }
    m_menu->close();
}

void MusicSpectrumLayoutWidget::initWidget()
{
    m_exclusive = false;

    const QString &style = MusicUIObject::MQSSBorderStyle04 + MusicUIObject::MQSSBackgroundStyle17;
    setObjectName("mianWidget");
    setStyleSheet(QString("#mianWidget{%1}").arg(style));

    m_containWidget->setFixedSize(240, 340);
    m_containWidget->setObjectName("containWidget");
    m_containWidget->setStyleSheet(QString("#containWidget{%1}").arg(style));

    QVBoxLayout *layout = new QVBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea(m_containWidget);
    QWidget *containWidget = new QWidget(scrollArea);
    containWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle17);
    m_containLayout = new QVBoxLayout(containWidget);

    m_containLayout->setContentsMargins(5, 0, 0, 0);
    m_containLayout->setSpacing(20);
    containWidget->setLayout(m_containLayout);

    scrollArea->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(containWidget);
    scrollArea->viewport()->setStyleSheet(MusicUIObject::MQSSBackgroundStyle17);
    layout->addWidget(scrollArea);

    m_containWidget->setLayout(layout);
}

void MusicSpectrumLayoutWidget::addItems(const ItemInfos &items)
{
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), SLOT(labelClicked(int)));

    for(int i=0; i<items.count(); ++i)
    {
        const ItemInfo &info = items[i];
        MusicSpectrumLayoutItem *item = new MusicSpectrumLayoutItem(this);
        item->addItem(info.first, info.second);
        connect(item, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(item, i);
        m_containLayout->addWidget(item);
        m_items << item;
    }
}



MusicSpectrumNormalLayoutWidget::MusicSpectrumNormalLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    ItemInfos items;
    items << ItemInfo(":/spectrum/normal_1", tr("Analyzer"));
    items << ItemInfo(":/spectrum/normal_2", tr("EWave"));
    items << ItemInfo(":/spectrum/normal_3", tr("FlowWave"));
    items << ItemInfo(":/spectrum/normal_4", tr("Histogram"));
    items << ItemInfo(":/spectrum/normal_5", tr("Line"));
    items << ItemInfo(":/spectrum/normal_6", tr("SpaceWave"));
    addItems(items);
}

MusicSpectrumNormalLayoutWidget::~MusicSpectrumNormalLayoutWidget()
{

}

QStringList MusicSpectrumNormalLayoutWidget::spectrumTypeList() const
{
    return QStringList() << "normalanalyzer" << "normalewave" << "normalflowwave"
                         << "normalhistogram" << "normalline" << "normalspacewave";
}



MusicSpectrumPlusLayoutWidget::MusicSpectrumPlusLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    ItemInfos items;
    items << ItemInfo(":/spectrum/plus_1", tr("FoldWave"));
    items << ItemInfo(":/spectrum/plus_2", tr("Monowave"));
    items << ItemInfo(":/spectrum/plus_3", tr("Multiwave"));
    items << ItemInfo(":/spectrum/plus_4", tr("XRays"));
    items << ItemInfo(":/spectrum/plus_5", tr("PointXRays"));
    items << ItemInfo(":/spectrum/plus_6", tr("VolumeWave"));
    items << ItemInfo(":/spectrum/plus_7", tr("LightEnvelope"));
    addItems(items);
}

MusicSpectrumPlusLayoutWidget::~MusicSpectrumPlusLayoutWidget()
{

}

QStringList MusicSpectrumPlusLayoutWidget::spectrumTypeList() const
{
    return QStringList() << "plusfoldwave" << "plusmonowave" << "plusmultiwave" << "plusxrays"
                         << "pluspointxrays" << "plusvolumewave" << "lightenvelope";
}



MusicSpectrumFloridLayoutWidget::MusicSpectrumFloridLayoutWidget(QWidget *parent)
    : MusicSpectrumLayoutWidget(parent)
{
    m_exclusive = true;

    ItemInfos items;
    items << ItemInfo(":/spectrum/florid_1", tr("Goom"));
    items << ItemInfo(":/spectrum/florid_2", tr("ProjectM"));
    items << ItemInfo(":/spectrum/florid_3", tr("Ethereality"));
    items << ItemInfo(":/spectrum/florid_4", tr("Reverb"));
    items << ItemInfo(":/spectrum/florid_5", tr("Autism"));
    items << ItemInfo(":/spectrum/florid_6", tr("Bass"));
    items << ItemInfo(":/spectrum/florid_7", tr("Surround"));
    items << ItemInfo(":/spectrum/florid_8", tr("Ancient"));
    items << ItemInfo(":/spectrum/florid_9", tr("Electric"));
    addItems(items);
}

MusicSpectrumFloridLayoutWidget::~MusicSpectrumFloridLayoutWidget()
{

}

QStringList MusicSpectrumFloridLayoutWidget::spectrumTypeList() const
{
    return QStringList() << "floridgoom" << "floridprojectm" << "floridethereality" << "floridreverb" << "floridautism"
                         << "floridbass" << "floridsurround" << "floridancient" << "floridelectric";
}
