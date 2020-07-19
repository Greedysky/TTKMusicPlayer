#include "musicbackgroundremotewidget.h"
#include "musicdownloadqueuerequest.h"
#include "musicextractwrap.h"
#include "musicwidgetheaders.h"

#include <QDir>
#include <QButtonGroup>

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    m_backgroundList = new MusicBackgroundListWidget(this);
    hbox->addWidget(m_backgroundList);
    setLayout(hbox);

    m_currentIndex = -1;
    m_downloadRequest = nullptr;

    m_downloadQueue = new MusicDownloadQueueRequest(MusicObject::DownloadBigBackground, this);
    connect(m_downloadQueue, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
    abort();
    delete m_backgroundList;
    delete m_downloadQueue;
    delete m_downloadRequest;
}

void MusicBackgroundRemoteWidget::abort()
{
    m_downloadQueue->abort();
}

void MusicBackgroundRemoteWidget::downLoadFinished(const QString &data)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    outputRemoteSkin(image, data);
    if(!image.isValid())
    {
        image.m_pix = QPixmap(":/image/lb_noneImage");
    }
    m_backgroundList->updateItem(image, data);
}

void MusicBackgroundRemoteWidget::downLoadFinished(const MusicSkinRemoteGroups &data)
{
    m_groups = data;
}

void MusicBackgroundRemoteWidget::startToDownload(const QString &prefix)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    const QString &path = QString("%1%2").arg(CACHE_DIR_FULL).arg(m_groups[m_currentIndex].m_group);
    QDir().mkpath(path);

    m_backgroundList->clearAllItems();
    MusicDownloadQueueDatas datas;
    MusicSkinRemoteItems *items = &m_groups[m_currentIndex].m_items;
    for(int i=0; i<items->count(); i++)
    {
        m_backgroundList->createItem(":/image/lb_noneImage", false);
        MusicDownloadQueueData data;
        data.m_url = (*items)[i].m_url;
        data.m_savePath = QString("%1/%2%3").arg(path).arg(i).arg(prefix);
        datas << data;
    }

    m_downloadQueue->addImageQueue(datas);
    m_downloadQueue->startToDownload();
}



MusicBackgroundThunderWidget::MusicBackgroundThunderWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent)
{
    m_functionsWidget = nullptr;
    connect(m_backgroundList, SIGNAL(itemClicked(QString)), parent, SLOT(remoteBackgroundListWidgetItemClicked(QString)));
}

MusicBackgroundThunderWidget::~MusicBackgroundThunderWidget()
{
    delete m_functionsWidget;
}

void MusicBackgroundThunderWidget::initialize()
{
    if(!m_downloadRequest)
    {
        m_downloadRequest = new MusicDownloadBackgroundThunderRequest(this);
        connect(m_downloadRequest, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroups)), SLOT(downLoadFinished(MusicSkinRemoteGroups)));
        m_downloadRequest->startToDownload();
    }
}

QWidget* MusicBackgroundThunderWidget::createFunctionsWidget(bool revert, QWidget *object)
{
    if(!m_functionsWidget)
    {
        m_functionsWidget = new QWidget(object);
        m_functionsWidget->setGeometry(24, 70, 585, 20);
        m_functionsWidget->hide();

        QHBoxLayout *hbox = new QHBoxLayout(m_functionsWidget);
        hbox->setContentsMargins(9, 0, 0, 9);
        QButtonGroup *button = new QButtonGroup(m_functionsWidget);
        connect(button, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

        QStringList names;
        for(int i=1; i<=9; ++i)
        {
            names << QString::number(i);
        }
        for(int i=0; i<names.count(); ++i)
        {
            button->addButton(createButton(names[i]), i);
        }

        hbox->addStretch(1);
        QPushButton *p = createButton(tr("ALL"));
        p->setStyleSheet(p->styleSheet() + QString("QPushButton{%1}").arg(MusicUIObject::MQSSColorStyle08));

        m_functionsWidget->setLayout(hbox);
    }

    QHBoxLayout *ly = TTKStatic_cast(QHBoxLayout*, m_functionsWidget->layout());
    int count = ly->count();
    if(revert)
    {
        while(count > 0)
        {
            --count;
            ly->removeWidget(m_functionsItems[count]);
            ly->removeItem(ly->itemAt(count));
            m_functionsItems[count]->hide();
        }
        ly->addStretch(1);

        m_functionsItems[m_functionsItems.count() - 1]->show();
        ly->addWidget(m_functionsItems[m_functionsItems.count() - 1]);
    }
    else
    {
        while(count > 0)
        {
            --count;
            ly->removeItem(ly->itemAt(count));
        }
        m_functionsItems[m_functionsItems.count() - 1]->hide();

        for(int i=0; i<m_functionsItems.count() - 1; ++i)
        {
            m_functionsItems[i]->show();
            ly->addWidget(m_functionsItems[i]);
        }
        ly->addStretch(1);
    }

    return m_functionsWidget;
}

void MusicBackgroundThunderWidget::outputRemoteSkin(MusicBackgroundImage &image, const QString &data)
{
    if(m_groups.isEmpty() || m_currentIndex < 0)
    {
        return;
    }

    const int index = QFileInfo(data).baseName().toInt();
    MusicSkinRemoteItems *items = &m_groups[m_currentIndex].m_items;
    if(index >= 0 || index < items->count())
    {
        MusicSkinRemoteItem *item = &(*items)[index];
        image.m_item.m_name = item->m_name;
        image.m_item.m_useCount = item->m_useCount;
        MusicExtractWrap::outputThunderSkin(image.m_pix, data);
    }
}

void MusicBackgroundThunderWidget::buttonClicked(int index)
{
    if(index < 0 || index >= m_groups.count())
    {
        return;
    }

    if(m_currentIndex != index)
    {
        m_downloadQueue->abort();
    }

    m_currentIndex = index;
    buttonStyleChanged();

    startToDownload(TTS_FILE);
}

void MusicBackgroundThunderWidget::downLoadFinished(const MusicSkinRemoteGroups &data)
{
    MusicBackgroundRemoteWidget::downLoadFinished(data);

    for(int i=0; i<m_groups.count(); ++i)
    {
        MusicSkinRemoteGroup *item = &m_groups[i];
        m_functionsItems[i]->setText(item->m_group);
    }

    //Hide left items if the number just not enough
    for(int i=m_groups.count(); i<m_functionsItems.count(); ++i)
    {
        m_functionsItems[i]->hide();
    }
}

QPushButton* MusicBackgroundThunderWidget::createButton(const QString &name)
{
    QPushButton *btn = new QPushButton(name, m_functionsWidget);
    btn->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02);
    btn->setCursor(QCursor(Qt::PointingHandCursor));
    btn->setFixedSize(35, 20);
    btn->hide();
#ifdef Q_OS_UNIX
    btn->setFocusPolicy(Qt::NoFocus);
#endif
    m_functionsItems << btn;

    return btn;
}

void MusicBackgroundThunderWidget::buttonStyleChanged()
{
    for(int i=0; i<m_functionsItems.count() - 1; ++i)
    {
        m_functionsItems[i]->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02);
    }
    m_functionsItems[m_currentIndex]->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02 + QString("QPushButton{%1}").arg(MusicUIObject::MQSSColorStyle08));
}



MusicBackgroundDailyWidget::MusicBackgroundDailyWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent)
{
    m_currentIndex = 0;
    connect(m_backgroundList, SIGNAL(itemClicked(QString)), parent, SLOT(dailyBackgroundListWidgetItemClicked(QString)));
}

void MusicBackgroundDailyWidget::initialize()
{
    if(!m_downloadRequest)
    {
        m_downloadRequest = new MusicDownloadBackgroundBingRequest(this);
        connect(m_downloadRequest, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroups)), SLOT(downLoadFinished(MusicSkinRemoteGroups)));
        m_downloadRequest->startToDownload();
    }
    else
    {
        startToDownload(TTS_FILE);
    }
}

void MusicBackgroundDailyWidget::outputRemoteSkin(MusicBackgroundImage &image, const QString &data)
{
    if(m_groups.isEmpty() || m_currentIndex < 0)
    {
        return;
    }

    const int index = QFileInfo(data).baseName().toInt();
    MusicSkinRemoteItems *items = &m_groups[m_currentIndex].m_items;
    if(index >= 0 || index < items->count())
    {
        MusicSkinRemoteItem *item = &(*items)[index];
        image.m_item.m_name = item->m_name;
        image.m_item.m_useCount = item->m_useCount;
        image.m_pix = QPixmap(data);
    }
}

void MusicBackgroundDailyWidget::downLoadFinished(const MusicSkinRemoteGroups &data)
{
    MusicBackgroundRemoteWidget::downLoadFinished(data);
    startToDownload(TTS_FILE);
}
