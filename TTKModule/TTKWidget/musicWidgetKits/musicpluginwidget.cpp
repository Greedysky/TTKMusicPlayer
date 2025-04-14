#include "musicpluginwidget.h"
#include "ui_musicqmmppluginwidget.h"
#include "ui_musicserverpluginwidget.h"
#include "musicitemdelegate.h"
#include "musicsettingmanager.h"
#include "musicpluginproperty.h"

#include "qjson/parser.h"
#include "qjson/serializer.h"

#include <qmmp/decoderfactory.h>
#include <qmmp/effectfactory.h>
#include <qmmp/visualfactory.h>
#include <qmmp/inputsourcefactory.h>
#include <qmmp/outputfactory.h>
#include <qmmp/decoder.h>
#include <qmmp/effect.h>
#include <qmmp/visual.h>
#include <qmmp/inputsource.h>
#include <qmmp/output.h>

#include <QScrollBar>

/*! @brief The class of the qmmp plugin item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MusicQmmpPluginItem : public QTreeWidgetItem
{
public:
    MusicQmmpPluginItem(QTreeWidgetItem *parent, DecoderFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, PluginDecoder),
          m_factory(factory)
    {
        MusicPluginProperty property;
        property.m_name = factory->properties().name;
        property.m_hasSettings = factory->properties().hasSettings;
        property.m_description = factory->properties().description;
        property.m_type = path;
        initialize(Decoder::isEnabled(factory), true, property);
    }

    MusicQmmpPluginItem(QTreeWidgetItem *parent, EffectFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, PluginEffect),
          m_factory(factory)
    {
        MusicPluginProperty property;
        property.m_name = factory->properties().name;
        property.m_hasSettings = factory->properties().hasSettings;
        property.m_type = path;
        initialize(Effect::isEnabled(factory), false, property);
    }

    MusicQmmpPluginItem(QTreeWidgetItem *parent, VisualFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, PluginVisual),
          m_factory(factory)
    {
        MusicPluginProperty property;
        property.m_name = factory->properties().name;
        property.m_hasSettings = factory->properties().hasSettings;
        property.m_type = path;
        initialize(Visual::isEnabled(factory), false, property);
    }

    MusicQmmpPluginItem(QTreeWidgetItem *parent, InputSourceFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, PluginTransports),
          m_factory(factory)
    {
        MusicPluginProperty property;
        property.m_name = factory->properties().name;
        property.m_hasSettings = factory->properties().hasSettings;
        property.m_type = path;
        initialize(InputSource::isEnabled(factory), true, property);
    }

    MusicQmmpPluginItem(QTreeWidgetItem *parent, OutputFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, PluginOutput),
          m_factory(factory)
    {
        MusicPluginProperty property;
        property.m_name = factory->properties().name;
        property.m_hasSettings = factory->properties().hasSettings;
        property.m_type = path;
        initialize(Output::currentFactory() == factory, true, property);
    }

    enum Module
    {
        PluginDecoder = QTreeWidgetItem::UserType,
        PluginEffect,
        PluginVisual,
        PluginTransports,
        PluginOutput
    };

    inline bool hasSettings() const
    {
        switch(type())
        {
            case PluginDecoder: return TTKStaticCast(DecoderFactory*, m_factory)->properties().hasSettings;
            case PluginEffect: return false;
            case PluginVisual: return false;
            case PluginTransports: return TTKStaticCast(InputSourceFactory*, m_factory)->properties().hasSettings;
            case PluginOutput: return TTKStaticCast(OutputFactory*, m_factory)->properties().hasSettings;
            default: return false;
        }
    }

    inline void showSettingWidget() const
    {
        QDialog *dialog = nullptr;

        switch(type())
        {
            case PluginDecoder: dialog = TTKStaticCast(DecoderFactory*, m_factory)->createSettings(nullptr); break;
            case PluginEffect: break;
            case PluginVisual: break;
            case PluginTransports: dialog = TTKStaticCast(InputSourceFactory*, m_factory)->createSettings(nullptr); break;
            case PluginOutput: dialog = TTKStaticCast(OutputFactory*, m_factory)->createSettings(nullptr); break;
            default: break;
        }

        if(dialog)
        {
            dialog->exec();
            dialog->deleteLater();
        }
    }

    inline void setEnabled(bool enabled)
    {
        switch(type())
        {
            case PluginDecoder: Decoder::setEnabled(TTKStaticCast(DecoderFactory*, m_factory), enabled); break;
            case PluginEffect: break;
            case PluginVisual: break;
            case PluginTransports: InputSource::setEnabled(TTKStaticCast(InputSourceFactory*, m_factory), enabled); break;
            case PluginOutput:
            {
                if(enabled)
                {
                    Output::setCurrentFactory(TTKStaticCast(OutputFactory*, m_factory));
                }
                break;
            }
            default: break;
        }
    }

    inline void initialize(bool state, bool enabled, const MusicPluginProperty &property)
    {
        setData(0, TTK_CHECKED_ROLE, state ? Qt::Checked : Qt::Unchecked);
        setData(0, TTK_ENABLED_ROLE, enabled);
        setData(1, TTK_DISPLAY_ROLE, property.m_name);
        setData(2, TTK_DISPLAY_ROLE, property.m_type.section('/', -1));

        if(!property.m_description.isEmpty())
        {
            setToolTip(1, property.m_description);
            setToolTip(2, property.m_description);
        }

        if(property.m_hasSettings)
        {
            setIcon(3, QIcon(":/contextMenu/btn_setting"));
        }

        const QColor &color = enabled ? (state ? QColor(0xE6, 0x73, 0x00) : QColor(0x00, 0x00, 0x00)) : QColor(0xBB, 0xBB, 0xBB);
        setData(1, Qt::ForegroundRole, color);
        setData(2, Qt::ForegroundRole, color);
    }

private:
    void *m_factory;

};



MusicQmmpPluginWidget::MusicQmmpPluginWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicQmmpPluginWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

#if TTK_QT_VERSION_CHECK(5,0,0)
    m_ui->treeWidget->header()->setSectionsMovable(false);
    m_ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Fixed);
#else
    m_ui->treeWidget->header()->setMovable(false);
    m_ui->treeWidget->header()->setResizeMode(0, QHeaderView::Fixed);
#endif
    m_ui->treeWidget->setHeaderLabels({{}, tr("Description"), tr("Name"), {}});

    TTKCheckBoxItemDelegate *delegateCheck = new TTKCheckBoxItemDelegate(this);
    delegateCheck->setStyleSheet(TTK::UI::CheckBoxStyle01);
    delegateCheck->setModuleMode(TTKAbstractItemDelegate::DisplayMode | TTKAbstractItemDelegate::TreeMode);
    m_ui->treeWidget->setItemDelegateForColumn(0, delegateCheck);

    TTKLabelItemDelegate *delegateTitle = new TTKLabelItemDelegate(this);
    delegateTitle->setModuleMode(TTKAbstractItemDelegate::ElideMode);
    delegateTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegateTitle->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->treeWidget->setItemDelegateForColumn(1, delegateTitle);

    TTKLabelItemDelegate *delegateName = new TTKLabelItemDelegate(this);
    delegateName->setModuleMode(TTKAbstractItemDelegate::ElideMode);
    delegateName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegateName->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->treeWidget->setItemDelegateForColumn(2, delegateName);

    QItemDelegate *delegateSetting = new QItemDelegate(this);
    m_ui->treeWidget->setItemDelegateForColumn(3, delegateSetting);

    m_ui->treeWidget->setColumnWidth(0, 65);
    m_ui->treeWidget->setColumnWidth(1, 210);
    m_ui->treeWidget->setColumnWidth(2, 120);
    m_ui->treeWidget->setColumnWidth(3, 70);

    m_ui->settingButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    m_ui->treeWidget->setStyleSheet(TTK::UI::GroupBoxStyle01 + TTK::UI::SpinBoxStyle01 + TTK::UI::SliderStyle06 +
                                    TTK::UI::RadioButtonStyle01 + TTK::UI::CheckBoxStyle01 + TTK::UI::ComboBoxStyle01 +
                                    TTK::UI::PushButtonStyle12 + TTK::UI::LineEditStyle01 + TTK::UI::ScrollBarStyle01);
    m_ui->treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->treeWidget->verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);
    m_ui->treeWidget->setFocusPolicy(Qt::NoFocus);

    loadPluginsInfo();

    connect(m_ui->settingButton, SIGNAL(clicked()), SLOT(pluginButtonClicked()));
    connect(m_ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(pluginItemChanged(QTreeWidgetItem*,int)));
}

void MusicQmmpPluginWidget::pluginItemChanged(QTreeWidgetItem *item, int column)
{
    if(column == 0 && (item->type() == MusicQmmpPluginItem::PluginDecoder || item->type() == MusicQmmpPluginItem::PluginTransports || item->type() == MusicQmmpPluginItem::PluginOutput))
    {
        if(item->type() == MusicQmmpPluginItem::PluginOutput)
        {
            QTreeWidgetItem *parent = item->parent();
            if(!parent)
            {
                return;
            }

            for(int i = 0; i < parent->childCount(); ++i)
            {
                QTreeWidgetItem *it = parent->child(i);
                it->setData(column, TTK_CHECKED_ROLE, Qt::Unchecked);
                it->setData(1, Qt::ForegroundRole, QColor(0x00, 0x00, 0x00));
                it->setData(2, Qt::ForegroundRole, QColor(0x00, 0x00, 0x00));
            }
        }

        const Qt::CheckState status = TTKStaticCast(Qt::CheckState, item->data(column, TTK_CHECKED_ROLE).toInt());
        item->setData(column, TTK_CHECKED_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        TTKDynamicCast(MusicQmmpPluginItem*, item)->setEnabled(status != Qt::Checked);

        const QColor &color = (status != Qt::Checked) ? QColor(0xE6, 0x73, 0x00) : QColor(0x00, 0x00, 0x00);
        item->setData(1, Qt::ForegroundRole, color);
        item->setData(2, Qt::ForegroundRole, color);
    }

    MusicQmmpPluginItem *it = TTKDynamicCast(MusicQmmpPluginItem*, item);
    m_ui->settingButton->setEnabled(it ? it->hasSettings() : false);
}

void MusicQmmpPluginWidget::pluginButtonClicked()
{
    MusicQmmpPluginItem *item = TTKDynamicCast(MusicQmmpPluginItem*, m_ui->treeWidget->currentItem());
    if(item)
    {
        item->showSettingWidget();
    }
}

void MusicQmmpPluginWidget::loadPluginsInfo()
{
    m_ui->treeWidget->blockSignals(true);

    QTreeWidgetItem *item = nullptr;
    item = new QTreeWidgetItem(m_ui->treeWidget, {tr("Decoder")});
    item->setFirstColumnSpanned(true);
    item->setExpanded(true);
    for(DecoderFactory *factory : Decoder::factories())
    {
        new MusicQmmpPluginItem(item, factory, Decoder::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);

    item = new QTreeWidgetItem(m_ui->treeWidget, {tr("Effect")});
    item->setFirstColumnSpanned(true);
    item->setExpanded(true);
    for(EffectFactory *factory : Effect::factories())
    {
        new MusicQmmpPluginItem(item, factory, Effect::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);

    item = new QTreeWidgetItem(m_ui->treeWidget, {tr("Visualization")});
    item->setFirstColumnSpanned(true);
    item->setExpanded(true);
    for(VisualFactory *factory : Visual::factories())
    {
        new MusicQmmpPluginItem(item, factory, Visual::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);

    item = new QTreeWidgetItem(m_ui->treeWidget, {tr("Transport")});
    item->setFirstColumnSpanned(true);
    item->setExpanded(true);
    for(InputSourceFactory *factory : InputSource::factories())
    {
        new MusicQmmpPluginItem(item, factory, InputSource::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);

    item = new QTreeWidgetItem(m_ui->treeWidget, {tr("Output")});
    item->setFirstColumnSpanned(true);
    item->setExpanded(true);
    for(OutputFactory *factory : Output::factories())
    {
        new MusicQmmpPluginItem(item, factory, Output::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);

    m_ui->treeWidget->blockSignals(false);
}


static constexpr const char *QUERY_PLUGINS_URL = "resource/plugins";

MusicServerPluginTableWidget::MusicServerPluginTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
#ifdef Q_OS_UNIX
    headerView->resizeSection(1, 219);
#else
    headerView->resizeSection(1, 222);
#endif
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);

    addCellItems();
}

void MusicServerPluginTableWidget::save() const
{
    QByteArray bytes;
    QFile file(APPCACHE_DIR_FULL + QUERY_PLUGINS_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_INFO_STREAM("Load server unity plugins ok");
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_ERROR_STREAM("Load server unity plugins failed");
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantList datas = data.toList();
        for(int i = 0; i < datas.count(); ++i)
        {
            QVariant &var = datas[i];
            QTableWidgetItem *it = item(i, 0);

            if(var.isNull() || !it)
            {
                continue;
            }

            QVariantMap value = var.toMap();
            value["option"] = it->data(TTK_CHECKED_ROLE).toBool();
            var = value;
        }

        if(!file.open(QIODevice::WriteOnly))
        {
            TTK_ERROR_STREAM("Save server unity plugins failed");
            return;
        }

        QJson::Serializer s;
        s.setIndentMode(QJson::IndentFull);
        s.serialize(datas, &file, &ok);
        TTK_INFO_STREAM("Save server unity plugins" << (ok ? "ok" : "failed"));
    }
}

void MusicServerPluginTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem *it = nullptr;

    it = item(row, 0);
    const Qt::CheckState status = TTKStaticCast(Qt::CheckState, it->data(TTK_CHECKED_ROLE).toInt());
    it->setData(TTK_CHECKED_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);

    it = item(row, 1);
    it->setForeground(status == Qt::Checked ? Qt::gray : QColor(0xE6, 0x73, 0x00));
}

void MusicServerPluginTableWidget::checkedItemsStatus(bool checked)
{
    MusicFillItemTableWidget::checkedItemsStatus(checked);

    for(int i = 0; i < rowCount(); ++i)
    {
        item(i, 1)->setForeground(!checked ? Qt::gray : QColor(0xE6, 0x73, 0x00));
    }
}

void MusicServerPluginTableWidget::addCellItems()
{
    QByteArray bytes;
    QFile file(APPCACHE_DIR_FULL + QUERY_PLUGINS_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_INFO_STREAM("Load server unity plugins ok");
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_ERROR_STREAM("Load server unity plugins failed");
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        int index = 0;
        const QVariantList &datas = data.toList();
        setRowCount(datas.count());

        for(const QVariant &var : qAsConst(datas))
        {
            if(var.isNull())
            {
                continue;
            }

            const QVariantMap &value = var.toMap();
            const bool option = value["option"].toBool();
            const QString &name = value["name"].toString();
            const QString &tips = value["description"].toString() + TTK_SPACE + value["version"].toString();

            QTableWidgetItem *item = new QTableWidgetItem;
            item->setData(TTK_CHECKED_ROLE, option ? Qt::Checked : Qt::Unchecked);
            setItem(index, 0, item);

                              item = new QTableWidgetItem;
            if(G_SETTING_PTR->value(MusicSettingManager::UserPermission).toBool())
            {
                item->setToolTip(tips);
            }
            item->setText(name);
            item->setForeground(option ? QColor(0xE6, 0x73, 0x00) : Qt::gray);
            QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
            setItem(index++, 1, item);
        }
    }
}



MusicServerPluginWidget::MusicServerPluginWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicServerPluginWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->selectAllCheckButton->setStyleSheet(TTK::UI::CheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->selectAllCheckButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_ui->selectAllCheckButton, SIGNAL(clicked(bool)), m_ui->itemTableWidget, SLOT(checkedItemsStatus(bool)));
}

void MusicServerPluginWidget::confirmButtonClicked()
{
    m_ui->itemTableWidget->save();
    accept();
}
