#include "musicttkfmradioplaywidget.h"
#include "ui_musicttkfmradioplaywidget.h"
#include "ui_musicttkfmradioinformationwidget.h"
#include "musiccoremplayer.h"
#include "musictoastlabel.h"
#include "musicmessagebox.h"
#include "ttkitemdelegate.h"

#include <QScrollBar>

MusicFMConfigManager::MusicFMConfigManager()
    : TTKAbstractXml()
{

}

bool MusicFMConfigManager::readBuffer(MusicFMCategoryList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("category");
    for(int i = 0; i < nodes.count(); ++i)
    {
        MusicFMCategory category;
        const QDomNode &node = nodes.item(i);
        category.m_category = node.toElement().attribute("value");

        const QDomNodeList &chnNodes = node.childNodes();
        for(int j = 0; j < chnNodes.count(); ++j)
        {
            const QDomElement &element = chnNodes.item(j).toElement();

            MusicFMChannel item;
            item.m_name = element.attribute("name");
            item.m_location = element.attribute("location");
            item.m_url = element.attribute("url");
            category.m_items << item;
        }
        items.append(category);
    }

    return true;
}

bool MusicFMConfigManager::writeBuffer(const MusicFMCategoryList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    const MusicFMCategory &item = items.back();

    createProcessingInstruction();
    QDomElement rootDom = createRoot(TTK_APP_NAME);
    QDomElement categoryDom = writeDomElement(rootDom, "category", {"value", item.m_category});

    for(const MusicFMChannel &channel : qAsConst(item.m_items))
    {
        writeDomMultiElement(categoryDom, "channel", {{"name", channel.m_name},
                                                      {"location", channel.m_location},
                                                      {"url", channel.m_url}});
    }

    save();
    return true;
}


MusicTTKFMRadioInformationWidget::MusicTTKFMRadioInformationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicTTKFMRadioInformationWidget),
      m_readOnly(true)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->button->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->button->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->button, SIGNAL(clicked()), SLOT(accept()));
}

MusicTTKFMRadioInformationWidget::~MusicTTKFMRadioInformationWidget()
{
    delete m_ui;
}

void MusicTTKFMRadioInformationWidget::setChannelInformation(const MusicFMChannel &channel)
{
    m_ui->pathEdit->setText(channel.m_url);
    m_ui->nameEdit->setText(channel.m_name);
    m_ui->locationEdit->setText(channel.m_location);
}

MusicFMChannel MusicTTKFMRadioInformationWidget::channelInformation() const
{
    return {m_ui->nameEdit->text(), m_ui->locationEdit->text(), m_ui->pathEdit->text()};
}

void MusicTTKFMRadioInformationWidget::setReadOnly(bool mode)
{
    m_readOnly = mode;
    m_ui->pathEdit->setReadOnly(mode);
    m_ui->nameEdit->setReadOnly(mode);
    m_ui->locationEdit->setReadOnly(mode);
}



MusicTTKFMRadioPlayWidget::MusicTTKFMRadioPlayWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicTTKFMRadioPlayWidget),
      m_isPlaying(false),
      m_currentIndex(0)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));

    m_ui->playButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->previousButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->nextButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->addButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->infoButton->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton->setFocusPolicy(Qt::NoFocus);
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deleteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->infoButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->playButton->setIconSize(QSize(31, 31));
    m_ui->previousButton->setIconSize(QSize(31, 31));
    m_ui->nextButton->setIconSize(QSize(31, 31));

    m_ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->infoButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->volumeSlider->setStyleSheet(TTK::UI::SliderStyle10);
    m_ui->volumeSlider->setRange(0, 100);
    m_ui->volumeSlider->setValue(100);

    m_ui->itemTree->setHeaderLabels({{}, {}});
    m_ui->itemTree->setColumnWidth(0, 220);
    m_ui->itemTree->setColumnWidth(1, 100);

    m_ui->itemTree->header()->setVisible(false);
    m_ui->itemTree->setStyleSheet(TTK::UI::ColorStyle02);
    m_ui->itemTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->itemTree->verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);
    m_ui->itemTree->setFocusPolicy(Qt::NoFocus);

    TTKLabelItemDelegate *delegateTitle = new TTKLabelItemDelegate(this);
    delegateTitle->setModuleMode(TTKAbstractItemDelegate::DisplayMode);
    delegateTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegateTitle->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->itemTree->setItemDelegateForColumn(0, delegateTitle);

    TTKLabelItemDelegate *delegateName = new TTKLabelItemDelegate(this);
    delegateName->setModuleMode(TTKAbstractItemDelegate::DisplayMode);
    delegateName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegateName->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->itemTree->setItemDelegateForColumn(1, delegateName);

    initialize();
    TTK::Widget::adjustWidgetPosition(this);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(radioPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(radioNext()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
    connect(m_ui->infoButton, SIGNAL(clicked()), SLOT(infoButtonClicked()));
    connect(m_ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
    connect(m_ui->itemTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(radioItemChanged(QTreeWidgetItem*,int)));
}

MusicTTKFMRadioPlayWidget::~MusicTTKFMRadioPlayWidget()
{
    delete m_player;
    delete m_ui;
}

void MusicTTKFMRadioPlayWidget::radioItemChanged(QTreeWidgetItem *item, int column)
{
    if(item && column == 0)
    {
        const int index = indexValid(item);
        if(index < 0)
        {
            return;
        }

        m_currentIndex = index;
        startCoreModule();
    }
}

void MusicTTKFMRadioPlayWidget::mediaPlayFinished(int code)
{
    if(TTK_LOW_LEVEL == code)
    {
        startCoreModule();
    }
}

void MusicTTKFMRadioPlayWidget::radioPlay()
{
    if(!m_player)
    {
        return;
    }

    m_isPlaying = !m_isPlaying;
    m_ui->playButton->setIcon(QIcon(m_isPlaying ? ":/functions/btn_pause_hover" : ":/functions/btn_play_hover"));
    m_player->play();
}

void MusicTTKFMRadioPlayWidget::radioPrevious()
{
    if(--m_currentIndex < 0)
    {
        m_currentIndex = 0;
    }

    startCoreModule();
}

void MusicTTKFMRadioPlayWidget::radioNext()
{
    if(++m_currentIndex >= m_items.count())
    {
        m_currentIndex = 0;
    }

    startCoreModule();
}

void MusicTTKFMRadioPlayWidget::radioVolume(int num)
{
    if(m_player)
    {
        m_player->setVolume(num);
    }
}

void MusicTTKFMRadioPlayWidget::closeEvent(QCloseEvent *event)
{
    delete m_player;
    m_player = nullptr;
    QWidget::closeEvent(event);
}

void MusicTTKFMRadioPlayWidget::initialize()
{
    MusicFMCategoryList categorys;
    {
        MusicFMConfigManager manager;
        if(manager.fromFile(":/data/fmlist"))
        {
            manager.readBuffer(categorys);
        }
    }

    {
        MusicFMConfigManager manager;
        if(manager.fromFile(FMRADIO_PATH_FULL))
        {
            categorys.takeLast();
            manager.readBuffer(m_favItem);
            categorys << m_favItem;
        }
    }

    int index = 0;
    for(const MusicFMCategory &category : qAsConst(categorys))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_ui->itemTree);
        item->setData(0, TTK_DISPLAY_ROLE, category.m_category);
        m_ui->itemTree->addTopLevelItem(item);

        for(const MusicFMChannel &channel : qAsConst(category.m_items))
        {
            QTreeWidgetItem *it = new QTreeWidgetItem(item);
            it->setData(0, TTK_DATA_ROLE, index++);
            it->setData(0, TTK_DISPLAY_ROLE, channel.m_name);
            it->setData(1, TTK_DISPLAY_ROLE, channel.m_location);
        }
        m_items << category.m_items;
    }

    createCoreModule();

    /// fix current play volume temporary
    const int v = m_ui->volumeSlider->value();
    m_ui->volumeSlider->setValue(0);
    m_ui->volumeSlider->setValue(v);
    ///
    m_ui->gifLabel_1->setType(MusicGifLabelWidget::Module::RadioBlue);
    m_ui->gifLabel_2->setType(MusicGifLabelWidget::Module::RadioBlue);
    m_ui->gifLabel_3->setType(MusicGifLabelWidget::Module::RadioBlue);
    m_ui->gifLabel_4->setType(MusicGifLabelWidget::Module::RadioBlue);
    m_ui->gifLabel_5->setType(MusicGifLabelWidget::Module::RadioBlue);
}

void MusicTTKFMRadioPlayWidget::createCoreModule()
{
    m_player = new MusicCoreMPlayer(this);

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(finished(int)), SLOT(mediaPlayFinished(int)));
}

void MusicTTKFMRadioPlayWidget::startCoreModule()
{
    m_isPlaying = false;
    if(!m_player)
    {
        createCoreModule();
    }

    m_player->setMedia(MusicCoreMPlayer::Module::Radio, m_items[m_currentIndex].m_url);
    radioPlay();

    m_ui->titleWidget->setText(m_items[m_currentIndex].m_name);
    m_ui->gifLabel_1->start();
    m_ui->gifLabel_2->start();
    m_ui->gifLabel_3->start();
    m_ui->gifLabel_4->start();
    m_ui->gifLabel_5->start();

    positionChanged(0);
    m_isPlaying = true;
}

int MusicTTKFMRadioPlayWidget::indexValid(QTreeWidgetItem *item) const
{
    if(!item)
    {
        return -1;
    }

    bool ok = false;
    const int index = item->data(0, TTK_DATA_ROLE).toInt(&ok);
    if(!ok || index < 0 || index >= m_items.count())
    {
        return -1;
    }

    return index;
}

void MusicTTKFMRadioPlayWidget::positionChanged(qint64 position)
{
    if(!m_player)
    {
        return;
    }

    m_ui->positionLabel->setText(QString("%1").arg(TTKTime::formatDuration(position * TTK_DN_S2MS)));
}

void MusicTTKFMRadioPlayWidget::addButtonClicked()
{
    MusicTTKFMRadioInformationWidget w;
    w.setReadOnly(false);
    if(w.exec())
    {
        const MusicFMChannel &channel = w.channelInformation();

        m_items << channel;
        m_favItem.back().m_items << channel;
        QTreeWidgetItem *item = m_ui->itemTree->topLevelItem(m_ui->itemTree->topLevelItemCount() - 1);
        if(!item)
        {
            return;
        }

        QTreeWidgetItem *it = new QTreeWidgetItem(item);
        it->setData(0, TTK_DATA_ROLE, m_items.count() - 1);
        it->setData(0, TTK_DISPLAY_ROLE, channel.m_name);
        it->setData(1, TTK_DISPLAY_ROLE, channel.m_location);

        MusicFMConfigManager manager;
        if(manager.load(FMRADIO_PATH_FULL))
        {
            manager.writeBuffer(m_favItem);
            MusicToastLabel::popup(tr("Add current channel success"));
        }
    }
}

void MusicTTKFMRadioPlayWidget::deleteButtonClicked()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec())
    {
       return;
    }

    QTreeWidgetItem *item = m_ui->itemTree->currentItem();
    const int index = indexValid(item);
    if(index < 0)
    {
        return;
    }

    const int offset = m_items.count() - m_favItem.back().m_items.count();
    if(index < offset)
    {
        MusicToastLabel::popup(tr("The current channel can not be deleted"));
        return;
    }

    for(int i = index - offset + 1; i < item->parent()->childCount(); ++i)
    {
        item->parent()->child(i)->setData(0, TTK_DATA_ROLE, offset + i - 1);
    }

    item->parent()->removeChild(item);
    m_items.removeAt(index);
    m_favItem.back().m_items.removeAt(index - offset);

    if(m_currentIndex == index)
    {
        radioPrevious();
    }

    MusicFMConfigManager manager;
    if(manager.load(FMRADIO_PATH_FULL))
    {
        manager.writeBuffer(m_favItem);
        MusicToastLabel::popup(tr("Delete current channel success"));
    }
}

void MusicTTKFMRadioPlayWidget::infoButtonClicked()
{
    const int index = indexValid(m_ui->itemTree->currentItem());
    if(index < 0)
    {
        return;
    }

    MusicTTKFMRadioInformationWidget widget;
    widget.setChannelInformation(m_items[index]);
    widget.exec();
}
