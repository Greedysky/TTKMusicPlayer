#include "musicbackgroundremotewidget.h"
#include "musicdownloadqueuerequest.h"
#include "musicdownloadbingskinrequest.h"
#include "musicdownloadthunderskinrequest.h"
#include "musicdownloadbirdskinrequest.h"
#include "musicresultscategorypopwidget.h"
#include "musicextractmanager.h"

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent),
      m_currentIndex(-1),
      m_downloadRequest(nullptr)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    m_backgroundList = new MusicBackgroundListWidget(this);
    hbox->addWidget(m_backgroundList);
    setLayout(hbox);

    m_networkRequest = new MusicDownloadQueueRequest(TTK::Download::Background, this);
    connect(m_networkRequest, SIGNAL(downloadDataChanged(QString)), SLOT(downloadDataChanged(QString)));
    connect(m_backgroundList, SIGNAL(itemClicked(int,QString)), parent, SLOT(remoteListWidgetItemClicked(int,QString)));
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
    abort();
    delete m_backgroundList;
    delete m_networkRequest;
    delete m_downloadRequest;
}

void MusicBackgroundRemoteWidget::abort()
{
    m_networkRequest->abort();
}

void MusicBackgroundRemoteWidget::downloadDataChanged(const QString &bytes)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    outputRemoteSkin(image, bytes);

    if(!image.isValid())
    {
        image.m_pix = QPixmap(":/image/lb_none_image");
    }

    m_backgroundList->updateItem(image, bytes);
}

void MusicBackgroundRemoteWidget::startToRequest(MusicSkinRemoteGroup::Type type)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    QString prefix;
    switch(type)
    {
        case MusicSkinRemoteGroup::Bing: prefix = "Bing"; break;
        case MusicSkinRemoteGroup::Thunder: prefix = "Thunder"; break;
        case MusicSkinRemoteGroup::BirdPaper: prefix = "BirdPaper"; break;
        default: break;
    }

    const QString &path = QString("%1%2/%3").arg(CACHE_DIR_FULL, prefix, m_groups[m_currentIndex].m_name);
    QDir().mkpath(path);

    m_backgroundList->clearItems();

    MusicDownloadQueueDataList datas;
    for(const MusicSkinRemoteItem &item : qAsConst(m_groups[m_currentIndex].m_items))
    {
        m_backgroundList->addCellItem(":/image/lb_none_image", false);
        MusicDownloadQueueData data;
        data.m_url = item.m_url;
        data.m_path = QString("%1/%2%3").arg(path).arg(item.m_index).arg(TKM_FILE);
        datas << data;
    }

    m_networkRequest->addQueue(datas);
    m_networkRequest->startToRequest();
}



MusicBackgroundDailyWidget::MusicBackgroundDailyWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent),
      m_tryTimes(3)
{
    m_currentIndex = 0;
    m_backgroundList->setType(MusicBackgroundListWidget::DailyModule);
}

void MusicBackgroundDailyWidget::initialize()
{
    if(!m_downloadRequest)
    {
        m_downloadRequest = new MusicDownloadBingSkinRequest(this);
        connect(m_downloadRequest, SIGNAL(downloadDataChanged(MusicSkinRemoteGroupList)), SLOT(downloadFinished(MusicSkinRemoteGroupList)));
        m_downloadRequest->startToRequest();
    }
    else
    {
        startToRequest(MusicSkinRemoteGroup::Bing);
    }
}

void MusicBackgroundDailyWidget::outputRemoteSkin(MusicBackgroundImage &image, const QString &data)
{
    if(m_groups.isEmpty() || m_currentIndex < 0)
    {
        return;
    }

    for(const MusicSkinRemoteItem &item : qAsConst(m_groups[m_currentIndex].m_items))
    {
        if(QFileInfo(data).baseName().toInt() == item.m_index)
        {
            image.m_item.m_name = item.m_name;
            image.m_item.m_useCount = item.m_useCount;
            image.m_pix = QPixmap(data);
            break;
        }
    }
}

void MusicBackgroundDailyWidget::downloadFinished(const MusicSkinRemoteGroupList &bytes)
{
    if(bytes.isEmpty() && m_tryTimes-- > 0)
    {
        TTK_INFO_STREAM("Retry to request daily skin data");
        m_downloadRequest->startToRequest();
        return;
    }

    m_groups = bytes;
    startToRequest(MusicSkinRemoteGroup::Bing);
}



MusicBackgroundOnlineWidget::MusicBackgroundOnlineWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent),
      m_functionsWidget(nullptr),
      m_categoryButton(nullptr)
{
    m_backgroundList->setType(MusicBackgroundListWidget::OnlineModule);
}

MusicBackgroundOnlineWidget::~MusicBackgroundOnlineWidget()
{
    delete m_functionsWidget;
    qDeleteAll(m_downloadRequests);
}

void MusicBackgroundOnlineWidget::initialize()
{
    m_backgroundList->clearItems();

    if(!m_downloadRequests.isEmpty())
    {
        return;
    }

    MusicAbstractDownloadSkinRequest *req = nullptr;

    req = new MusicDownloadThunderSkinRequest(this);
    connect(req, SIGNAL(downloadDataChanged(MusicSkinRemoteGroupList)), SLOT(downloadFinished(MusicSkinRemoteGroupList)));
    m_downloadRequests << req;

    req = new MusicDownloadBirdSkinRequest(this);
    connect(req, SIGNAL(downloadDataChanged(MusicSkinRemoteGroupList)), SLOT(downloadFinished(MusicSkinRemoteGroupList)));
    m_downloadRequests << req;

    MusicResultsCategoryList categories;
    MusicCategoryConfigManager manager(TTK_APP_NAME);
    if(manager.fromFile(MusicCategoryConfigManager::Category::SkinList))
    {
        manager.readBuffer(categories);
    }

    for(int i = 0; i < categories.count(); ++i)
    {
        for(const MusicResultsCategoryItem &item : qAsConst(categories[i].m_items))
        {
            MusicSkinRemoteGroup group;
            group.m_id = item.m_key;
            group.m_name = item.m_value;

            if(i == 0 /*MusicSkinRemoteGroup::Thunder*/)
            {
                group.m_type = MusicSkinRemoteGroup::Thunder;
            }
            else if(i == 1 /*MusicSkinRemoteGroup::BirdPaper*/)
            {
                group.m_type = MusicSkinRemoteGroup::BirdPaper;
            }

            m_groups << group;
        }
    }
}

QWidget* MusicBackgroundOnlineWidget::createFunctionsWidget(QWidget *container, QWidget *parent)
{
    if(m_functionsWidget)
    {
        return m_functionsWidget;
    }

    m_functionsWidget = new QWidget(parent);
    m_functionsWidget->setGeometry(24, 70, 585, 25);

    const QRect &rect = container->geometry();
    container->setGeometry(QRect(rect.x(), rect.y() + m_functionsWidget->height(), rect.width(), rect.height() - m_functionsWidget->height()));

    QHBoxLayout *hbox = new QHBoxLayout(m_functionsWidget);
    hbox->setContentsMargins(9, 0, 0, 9);

    m_categoryButton = new MusicResultsCategoryPopWidget(MusicCategoryConfigManager::Category::SkinList, m_functionsWidget);
    m_categoryButton->setCategory(TTK_APP_NAME, this);
    hbox->addWidget(m_categoryButton);
    hbox->addStretch(1);

    m_functionsWidget->setLayout(hbox);
    return m_functionsWidget;
}

void MusicBackgroundOnlineWidget::outputRemoteSkin(MusicBackgroundImage &image, const QString &data)
{
    if(m_groups.isEmpty() || m_currentIndex < 0)
    {
        return;
    }

    const int index = QFileInfo(data).baseName().toInt();
    const MusicSkinRemoteGroup &group = m_groups[m_currentIndex];
    const MusicSkinRemoteItemList &items = group.m_items;

    if(index < 0 || index >= items.count())
    {
        return;
    }

    const MusicSkinRemoteItem &item = items[index];
    image.m_item.m_name = item.m_name;
    image.m_item.m_useCount = item.m_useCount;

    switch(group.m_type)
    {
        case MusicSkinRemoteGroup::Thunder:
        {
            MusicExtractManager::outputThunderSkin(image.m_pix, data);
            break;
        }
        case MusicSkinRemoteGroup::BirdPaper:
        {
            image.m_pix = QPixmap(data);
            break;
        }
        default: break;
    }
}

void MusicBackgroundOnlineWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(!m_categoryButton)
    {
        return;
    }

    int index = -1;
    MusicSkinRemoteGroup::Type type;
    bool hasItems = false;

    for(int i = 0; i < m_groups.count(); ++i)
    {
        const MusicSkinRemoteGroup &group = m_groups[i];
        if(group.m_id == category.m_key && group.m_name == category.m_value)
        {
            index = i;
            type = group.m_type;
            hasItems = !group.m_items.isEmpty();
            break;
        }
    }

    m_categoryButton->closeMenu();

    if(index == -1)
    {
        return;
    }

    if(m_currentIndex != index)
    {
        m_networkRequest->abort();
    }

    m_currentIndex = index;

    if(hasItems)
    {
        startToRequest(type);
    }
    else
    {
        switch(type)
        {
            case MusicSkinRemoteGroup::Thunder:
            {
                m_downloadRequests[0]->startToRequest();
                break;
            }
            case MusicSkinRemoteGroup::BirdPaper:
            {
                m_downloadRequests[1]->startToRequest(category.m_key);
                break;
            }
            default: break;
        }
    }
}


void MusicBackgroundOnlineWidget::downloadFinished(const MusicSkinRemoteGroupList &bytes)
{
    if(bytes.isEmpty())
    {
        return;
    }

    const MusicSkinRemoteGroup &first = bytes.first();
    switch(first.m_type)
    {
        case MusicSkinRemoteGroup::Thunder:
        {
            for(MusicSkinRemoteGroup &group : m_groups)
            {
                if(group.m_type != MusicSkinRemoteGroup::Thunder)
                {
                    continue;
                }

                for(const MusicSkinRemoteGroup &byte : qAsConst(bytes))
                {
                    if(group.m_name == byte.m_name)
                    {
                        group.m_items = byte.m_items;
                        break;
                    }
                }
            }

            startToRequest(MusicSkinRemoteGroup::Thunder);
            break;
        }
        case MusicSkinRemoteGroup::BirdPaper:
        {
            m_groups[m_currentIndex] = first;
            startToRequest(MusicSkinRemoteGroup::BirdPaper);
            break;
        }
        default: break;
    }
}
