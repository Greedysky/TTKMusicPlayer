#include "musicpluginwidget.h"
#include "ui_musicpluginwidget.h"
#include "musicuiobject.h"
#include "musicitemdelegate.h"

#include "decoderfactory.h"
#include "outputfactory.h"
#include "visualfactory.h"
#include "effectfactory.h"

#include "decoder.h"
#include "effect.h"
#include "output.h"
#include "visual.h"

#include <QScrollBar>

/*! @brief The class of the plugin item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MusicPluginItem : public QTreeWidgetItem
{
public:
    MusicPluginItem(QTreeWidgetItem *parent, DecoderFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, DECODER)
    {
        initialize(Decoder::isEnabled(factory), true, factory->properties().name, factory->properties().hasSettings, path);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, EffectFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, EFFECT)
    {

        initialize(Effect::isEnabled(factory), false, factory->properties().name, factory->properties().hasSettings, path);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, VisualFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, VISUAL)
    {
        initialize(Visual::isEnabled(factory), false, factory->properties().name, factory->properties().hasSettings, path);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, OutputFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, OUTPUT)
    {
        initialize(Output::currentFactory() == factory, true, factory->properties().name, factory->properties().hasSettings, path);
        m_factory = factory;
    }

    enum PluginType
    {
        DECODER = QTreeWidgetItem::UserType,
        EFFECT,
        VISUAL,
        OUTPUT
    };

    bool hasSettings() const
    {
        switch(type())
        {
            case MusicPluginItem::DECODER:
                return TTKStatic_cast(DecoderFactory*, m_factory)->properties().hasSettings;
            case MusicPluginItem::EFFECT: return false;
            case MusicPluginItem::VISUAL: return false;
            case MusicPluginItem::OUTPUT:
                return TTKStatic_cast(OutputFactory*, m_factory)->properties().hasSettings;
            default: return false;
        }
    }

    void showSettingWidget() const
    {
        switch(type())
        {
            case MusicPluginItem::DECODER:
                TTKStatic_cast(DecoderFactory*, m_factory)->showSettings(treeWidget()); break;
            case MusicPluginItem::EFFECT: break;
            case MusicPluginItem::VISUAL: break;
            case MusicPluginItem::OUTPUT:
                TTKStatic_cast(OutputFactory*, m_factory)->showSettings(treeWidget()); break;
            default: break;
        }
    }

    void setEnabled(bool enabled)
    {
        switch(type())
        {
            case MusicPluginItem::DECODER:
                Decoder::setEnabled(TTKStatic_cast(DecoderFactory*, m_factory), enabled);
                break;
            case MusicPluginItem::EFFECT: break;
            case MusicPluginItem::VISUAL: break;
            case MusicPluginItem::OUTPUT:
                if(enabled)
                {
                    Output::setCurrentFactory(TTKStatic_cast(OutputFactory*, m_factory));
                }
                break;
            default: break;
        }
    }

    void initialize(bool state, bool enable, const QString &name, bool setting, const QString &path)
    {
        setData(0, MUSIC_CHECK_ROLE, state ? Qt::Checked : Qt::Unchecked);
        setData(0, MUSIC_ENABLE_ROLE, enable);
        setData(1, MUSIC_TEXT_ROLE, name);
        setData(2, MUSIC_TEXT_ROLE, path.section('/', -1));

        if(setting)
        {
            setIcon(3, QIcon(":/usermanager/btn_setting"));
        }

        const QColor &color = enable ? (state ? "#E67300" : "#000000") : "#BBBBBB";
        setData(1, Qt::TextColorRole, color);
        setData(2, Qt::TextColorRole, color);
    }

private:
    void *m_factory;

};



MusicPluginWidget::MusicPluginWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicPluginWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

#if TTK_QT_VERSION_CHECK(5,0,0)
    m_ui->treeWidget->header()->setSectionsMovable(false);
    m_ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Fixed);
#else
    m_ui->treeWidget->header()->setMovable(false);
    m_ui->treeWidget->header()->setResizeMode(0, QHeaderView::Fixed);
#endif
    m_ui->treeWidget->setHeaderLabels(QStringList() << QString() << tr("Description") << tr("Name") << QString());

    MusicCheckBoxDelegate *checkDelegate = new MusicCheckBoxDelegate(this);
    checkDelegate->showTextMode(true);
    checkDelegate->setTreeModel(true);
    m_ui->treeWidget->setItemDelegateForColumn(0, checkDelegate);

    MusicLabelDelegate *delegate_title = new MusicLabelDelegate(this);
    delegate_title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegate_title->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->treeWidget->setItemDelegateForColumn(1, delegate_title);

    MusicLabelDelegate *delegate_name = new MusicLabelDelegate(this);
    delegate_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegate_name->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->treeWidget->setItemDelegateForColumn(2, delegate_name);

    MusicLabelDelegate *delegate_setting = new MusicLabelDelegate(this);
    delegate_setting->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegate_setting->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->treeWidget->setItemDelegateForColumn(3, delegate_setting);

    m_ui->treeWidget->setColumnWidth(0, 65);
    m_ui->treeWidget->setColumnWidth(1, 210);
    m_ui->treeWidget->setColumnWidth(2, 120);
    m_ui->treeWidget->setColumnWidth(3, 70);

    m_ui->settingButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->treeWidget->setStyleSheet(MusicUIObject::MQSSGroupBoxStyle01 +
                                    MusicUIObject::MQSSSpinBoxStyle01 +
                                    MusicUIObject::MQSSSliderStyle06 +
                                    MusicUIObject::MQSSRadioButtonStyle01 +
                                    MusicUIObject::MQSSCheckBoxStyle01 +
                                    MusicUIObject::MQSSComboBoxStyle01 +
                                    MusicUIObject::MQSSPushButtonStyle15);
    m_ui->treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->treeWidget->verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);
    m_ui->treeWidget->setFocusPolicy(Qt::NoFocus);

    loadPluginsInfo();

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->settingButton, SIGNAL(clicked()), SLOT(pluginButtonClicked()));
    connect(m_ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(pluginItemChanged(QTreeWidgetItem*,int)));
}

MusicPluginWidget::~MusicPluginWidget()
{

}

void MusicPluginWidget::pluginItemChanged(QTreeWidgetItem *item, int column)
{
    if(column == 0 && (item->type() == MusicPluginItem::DECODER || item->type() == MusicPluginItem::OUTPUT))
    {
        if(item->type() == MusicPluginItem::OUTPUT)
        {
            QTreeWidgetItem *parent = item->parent();
            if(!parent)
            {
                return;
            }

            for(int i=0; i<parent->childCount(); ++i)
            {
                QTreeWidgetItem *it = parent->child(i);
                it->setData(column, MUSIC_CHECK_ROLE, Qt::Unchecked);
                it->setData(1, Qt::TextColorRole, "#000000");
                it->setData(2, Qt::TextColorRole, "#000000");
            }
        }

        const Qt::CheckState status = TTKStatic_cast(Qt::CheckState, item->data(column, MUSIC_CHECK_ROLE).toInt());
        item->setData(column, MUSIC_CHECK_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        TTKDynamic_cast(MusicPluginItem*, item)->setEnabled(status != Qt::Checked);

        const QColor &color = (status != Qt::Checked) ? "#E67300" : "#000000";
        item->setData(1, Qt::TextColorRole, color);
        item->setData(2, Qt::TextColorRole, color);
    }

    m_ui->settingButton->setEnabled(TTKDynamic_cast(MusicPluginItem*, item)->hasSettings());
}

void MusicPluginWidget::pluginButtonClicked()
{
    MusicPluginItem *item = TTKDynamic_cast(MusicPluginItem*, m_ui->treeWidget->currentItem());
    if(item)
    {
        item->showSettingWidget();
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
    for(DecoderFactory *factory : Decoder::factories())
    {
        new MusicPluginItem(item, factory,  Decoder::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Effects"));
    item->setFirstColumnSpanned(true);
    for(EffectFactory *factory : Effect::factories())
    {
        new MusicPluginItem(item, factory, Effect::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Visualization"));
    item->setFirstColumnSpanned(true);
    for(VisualFactory *factory : Visual::factories())
    {
        new MusicPluginItem(item, factory, Visual::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Output"));
    item->setFirstColumnSpanned(true);
    for(OutputFactory *factory : Output::factories())
    {
        new MusicPluginItem(item, factory, Output::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    m_ui->treeWidget->blockSignals(false);
}
