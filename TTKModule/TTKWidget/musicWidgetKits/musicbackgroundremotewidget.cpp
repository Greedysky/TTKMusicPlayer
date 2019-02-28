#include "musicbackgroundremotewidget.h"
#include "musicdownloadqueuecache.h"
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

    m_listWidget = new MusicBackgroundListWidget(this);
    hbox->addWidget(m_listWidget);
    setLayout(hbox);

    m_currentIndex = -1;
    m_queryThread = nullptr;

    m_downloadQueue = new MusicDownloadQueueCache(MusicObject::DownloadBigBackground, this);
    connect(m_downloadQueue, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataChanged(QString)));

}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
    abort();
    delete m_listWidget;
    delete m_downloadQueue;
    delete m_queryThread;
}

void MusicBackgroundRemoteWidget::abort()
{
    m_downloadQueue->abort();
}

void MusicBackgroundRemoteWidget::downLoadDataChanged(const QString &data)
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
    m_listWidget->updateItem(image, data);
}

void MusicBackgroundRemoteWidget::downLoadDataChanged(const MusicSkinRemoteGroups &data)
{
    m_groups = data;
}

void MusicBackgroundRemoteWidget::startToDownload(const QString &prefix)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    QDir dir(".");
    const QString &path = QString("%1%2").arg(CACHE_DIR_FULL).arg(m_groups[m_currentIndex].m_group);
    dir.mkpath( path );

    m_listWidget->clearAllItems();
    MusicDownloadQueueDatas datas;
    MusicSkinRemoteItems *items = &m_groups[m_currentIndex].m_items;
    for(int i=0; i<items->count(); i++)
    {
        m_listWidget->createItem(":/image/lb_noneImage", false);
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
    connect(m_listWidget, SIGNAL(itemClicked(QString)), parent, SLOT(remoteBackgroundListWidgetItemClicked(QString)));
}

MusicBackgroundThunderWidget::~MusicBackgroundThunderWidget()
{
    delete m_functionsWidget;
}

void MusicBackgroundThunderWidget::init()
{
    if(!m_queryThread)
    {
        m_queryThread = new MusicDownloadBackgroundThunderThread(this);
        connect(m_queryThread, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroups)), SLOT(downLoadDataChanged(MusicSkinRemoteGroups)));
        m_queryThread->startToDownload();
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
        p->setStyleSheet(p->styleSheet() + QString("QPushButton{%1}").arg(MusicUIObject::MColorStyle08));

        m_functionsWidget->setLayout(hbox);
    }

    QHBoxLayout *ly = MStatic_cast(QHBoxLayout*, m_functionsWidget->layout());
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

void MusicBackgroundThunderWidget::downLoadDataChanged(const MusicSkinRemoteGroups &data)
{
    MusicBackgroundRemoteWidget::downLoadDataChanged(data);

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
    btn->setStyleSheet(MusicUIObject::MPushButtonStyle02);
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
        m_functionsItems[i]->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    }
    m_functionsItems[m_currentIndex]->setStyleSheet(MusicUIObject::MPushButtonStyle02 + QString("QPushButton{%1}").arg(MusicUIObject::MColorStyle08));
}



MusicBackgroundDailyWidget::MusicBackgroundDailyWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent)
{
    m_currentIndex = 0;
    connect(m_listWidget, SIGNAL(itemClicked(QString)), parent, SLOT(dailyBackgroundListWidgetItemClicked(QString)));
}

void MusicBackgroundDailyWidget::init()
{
    if(!m_queryThread)
    {
        m_queryThread = new MusicDownloadBackgroundBingThread(this);
        connect(m_queryThread, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroups)), SLOT(downLoadDataChanged(MusicSkinRemoteGroups)));
        m_queryThread->startToDownload();
    }
    else
    {
        startToDownload(JPG_FILE);
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

void MusicBackgroundDailyWidget::downLoadDataChanged(const MusicSkinRemoteGroups &data)
{
    MusicBackgroundRemoteWidget::downLoadDataChanged(data);

    startToDownload(JPG_FILE);
}
