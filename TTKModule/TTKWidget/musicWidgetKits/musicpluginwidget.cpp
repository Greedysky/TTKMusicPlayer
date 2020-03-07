#include "musicpluginwidget.h"
#include "ui_musicpluginwidget.h"
#include "musicuiobject.h"
///qmmp incldue
#include "decoderfactory.h"
#include "outputfactory.h"
#include "visualfactory.h"
#include "effectfactory.h"
///qmmp incldue
#include "decoder.h"
#include "effect.h"
#include "output.h"
#include "visual.h"

#include <QScrollBar>

class MusicPluginItem : public QTreeWidgetItem
{
public:
    MusicPluginItem(QTreeWidgetItem *parent, DecoderFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/', -1), DECODER)
    {
        setCheckState(0, Decoder::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, EffectFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/', -1), EFFECT)
    {
        setCheckState(0, Effect::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, VisualFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/', -1), VISUAL)
    {
        setCheckState(0, Visual::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, OutputFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/', -1), OUTPUT)
    {
        setCheckState(0, (Output::currentFactory() == factory) ? Qt::Checked : Qt::Unchecked);
        m_factory = factory;
    }

    enum PluginType
    {
        DECODER = QTreeWidgetItem::UserType,
        EFFECT,
        VISUAL,
        OUTPUT
    };

    void setEnabled(bool enabled)
    {
        switch(type())
        {
            case MusicPluginItem::DECODER:
                Decoder::setEnabled(static_cast<DecoderFactory *>(m_factory), enabled);
                break;
            case MusicPluginItem::EFFECT: break;
            case MusicPluginItem::VISUAL: break;
            case MusicPluginItem::OUTPUT: break;
            default: break;
        }
    }

private:
    void *m_factory;
};



MusicPluginWidget::MusicPluginWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicPluginWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->treeWidget->header()->setSectionsMovable(false);
    m_ui->treeWidget->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->treeWidget->verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);

    loadPluginsInfo();

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(pluginItemChanged(QTreeWidgetItem*,int)));
}

MusicPluginWidget::~MusicPluginWidget()
{

}

void MusicPluginWidget::pluginItemChanged(QTreeWidgetItem *item, int column)
{
    if(column == 0 && item->type() >= MusicPluginItem::DECODER)
    {
        dynamic_cast<MusicPluginItem *>(item)->setEnabled(item->checkState(0) == Qt::Checked);
    }
}

int MusicPluginWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicPluginWidget::loadPluginsInfo()
{
    m_ui->treeWidget->blockSignals(true);

    QTreeWidgetItem *item = nullptr;
    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Decoders"));
    item->setFirstColumnSpanned(true);
    foreach(DecoderFactory *factory, Decoder::factories())
    {
        new MusicPluginItem(item, factory,  Decoder::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Effects"));
    item->setFirstColumnSpanned(true);
    foreach(EffectFactory *factory, Effect::factories())
    {
        new MusicPluginItem(item, factory, Effect::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Visualization"));
    item->setFirstColumnSpanned(true);
    foreach(VisualFactory *factory, Visual::factories())
    {
        new MusicPluginItem(item, factory, Visual::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Output"));
    item->setFirstColumnSpanned(true);
    foreach(OutputFactory *factory, Output::factories())
    {
        new MusicPluginItem(item, factory, Output::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    m_ui->treeWidget->blockSignals(false);
    m_ui->treeWidget->resizeColumnToContents(0);
    m_ui->treeWidget->resizeColumnToContents(1);
}
