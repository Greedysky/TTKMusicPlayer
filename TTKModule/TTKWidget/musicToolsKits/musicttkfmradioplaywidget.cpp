#include "musicttkfmradioplaywidget.h"
#include "ui_musicttkfmradioplaywidget.h"
#include "musiccoremplayer.h"
#include "musicitemdelegate.h"
#include "musictoastlabel.h"

#include <QScrollBar>

MusicFMConfigManager::MusicFMConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicFMConfigManager::readBuffer(MusicFMCategoryList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("category");
    for(int i = 0; i < nodes.count(); ++i)
    {
        MusicFMCategory category;
        const QDomNode &node = nodes.at(i);
        category.m_category = node.toElement().attribute("value");

        const QDomNodeList &chnNodes = node.childNodes();
        for(int j = 0; j < chnNodes.count(); ++j)
        {
            const QDomNode &chnNode = chnNodes.at(j);
            MusicFMCategoryItem item;
            item.m_name = chnNode.toElement().attribute("name");
            item.m_location = chnNode.toElement().attribute("location");
            item.m_url = chnNode.toElement().attribute("url");
            category.m_items << item;
        }
        items.append(category);
    }
}

void MusicFMConfigManager::writeBuffer(const MusicFMCategoryList &items)
{
    if(!toFile(FMRADIO_PATH_FULL) || items.isEmpty())
    {
        return;
    }

    const MusicFMCategory &item = items.back();

    createProcessingInstruction();
    QDomElement rootDom = createRoot(APP_NAME);
    QDomElement categoryDom = writeDomElement(rootDom, "category", {"value", item.m_category});

    for(const MusicFMCategoryItem &it : qAsConst(item.m_items))
    {
        writeDomMutilElement(categoryDom, "channel", {{"name", it.m_name},
                                                      {"location", it.m_location},
                                                      {"url", it.m_url}});
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}


MusicTTKFMRadioPlayWidget::MusicTTKFMRadioPlayWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicTTKFMRadioPlayWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_isPlaying = false;
    m_currentIndex = 0;

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));

    m_ui->playButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->previousButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->nextButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->addButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->infoButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

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

    m_ui->volumeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle10);
    m_ui->volumeSlider->setRange(0, 100);
    m_ui->volumeSlider->setValue(100);

    m_ui->itemTree->setHeaderLabels({QString(), QString()});
    m_ui->itemTree->setColumnWidth(0, 220);
    m_ui->itemTree->setColumnWidth(1, 100);

    m_ui->itemTree->header()->setVisible(false);
    m_ui->itemTree->setStyleSheet(MusicUIObject::MQSSColorStyle10);
    m_ui->itemTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->itemTree->verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);
    m_ui->itemTree->setFocusPolicy(Qt::NoFocus);

    MusicLabelDelegate *delegateTitle = new MusicLabelDelegate(this);
    delegateTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delegateTitle->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->itemTree->setItemDelegateForColumn(0, delegateTitle);

    initialize();
    MusicUtils::Widget::positionInCenter(this);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(radioPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(radioNext()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
    connect(m_ui->infoButton, SIGNAL(clicked()), SLOT(infoButtonClicked()));
    connect(m_ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
    connect(m_ui->itemTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(radioItemChanged(QTreeWidgetItem*,int)));
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
        const int index = valid(item);
        if(index < 0)
        {
            return;
        }

        m_currentIndex = index;
        startCoreModule();
    }
}

void MusicTTKFMRadioPlayWidget::mediaAutionPlayError(int code)
{
    if(DEFAULT_LOW_LEVEL == code)
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
        manager.fromFile(":/data/fmlist");
        manager.readBuffer(categorys);
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
    for(const MusicFMCategory &category : categorys)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_ui->itemTree, {category.m_category});
        m_ui->itemTree->addTopLevelItem(item);

        for(const MusicFMCategoryItem &channel : category.m_items)
        {
            QTreeWidgetItem *it = new QTreeWidgetItem(item, {channel.m_name, channel.m_location});
            it->setData(0, MUSIC_DATA_ROLE, index++);
        }
        m_items << category.m_items;
    }

    createCoreModule();

    /// fix current play volume temporary
    const int v = m_ui->volumeSlider->value();
    m_ui->volumeSlider->setValue(0);
    m_ui->volumeSlider->setValue(v);
    ///
    m_ui->gifLabel_1->setType(MusicGifLabelWidget::RadioBlue);
    m_ui->gifLabel_2->setType(MusicGifLabelWidget::RadioBlue);
    m_ui->gifLabel_3->setType(MusicGifLabelWidget::RadioBlue);
    m_ui->gifLabel_4->setType(MusicGifLabelWidget::RadioBlue);
    m_ui->gifLabel_5->setType(MusicGifLabelWidget::RadioBlue);
}

void MusicTTKFMRadioPlayWidget::createCoreModule()
{
    m_player = new MusicCoreMPlayer(this);

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(finished(int)), SLOT(mediaAutionPlayError(int)));
}

void MusicTTKFMRadioPlayWidget::startCoreModule()
{
    m_isPlaying = false;
    if(!m_player)
    {
        createCoreModule();
    }

    m_player->setMedia(MusicCoreMPlayer::RadioCategory, m_items[m_currentIndex].m_url);
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

int MusicTTKFMRadioPlayWidget::valid(QTreeWidgetItem *item) const
{
    if(!item)
    {
        return -1;
    }

    bool ok;
    const int index = item->data(0, MUSIC_DATA_ROLE).toInt(&ok);
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

    m_ui->positionLabel->setText(QString("%1").arg(MusicTime::msecTime2LabelJustified(position * MT_S2MS)));
}

void MusicTTKFMRadioPlayWidget::addButtonClicked()
{

}

void MusicTTKFMRadioPlayWidget::deleteButtonClicked()
{
    const int index = valid(m_ui->itemTree->currentItem());
    if(index < 0)
    {
        return;
    }

    const int size = m_items.count() - 1;
    if(index < size)
    {
        MusicToastLabel::popup(tr("The current channel can not be deleted"));
        return;
    }

    m_favItem.back().m_items.removeAt(index - size);
    MusicFMConfigManager manager;
    manager.writeBuffer(m_favItem);
}

void MusicTTKFMRadioPlayWidget::infoButtonClicked()
{
    const int index = valid(m_ui->itemTree->currentItem());
    if(index < 0)
    {
        return;
    }

    qDebug() << index;
}

void MusicTTKFMRadioPlayWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
