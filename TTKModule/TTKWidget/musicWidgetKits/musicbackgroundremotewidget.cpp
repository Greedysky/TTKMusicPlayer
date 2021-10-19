#include "musicbackgroundremotewidget.h"
#include "musicdownloadqueuerequest.h"
#include "musicdownloadskin.h"
#include "musicextractwrapper.h"
#include "musicwidgetheaders.h"

#include <QDir>
#include <QAbstractItemView>
#include <QStyledItemDelegate>

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

void MusicBackgroundRemoteWidget::downLoadFinished(const QString &bytes)
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

void MusicBackgroundRemoteWidget::downLoadFinished(const MusicSkinRemoteGroups &bytes)
{
    m_groups = bytes;
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
    for(const MusicSkinRemoteItem &item : qAsConst(m_groups[m_currentIndex].m_items))
    {
        m_backgroundList->createItem(":/image/lb_none_image", false);
        MusicDownloadQueueData data;
        data.m_url = item.m_url;
        data.m_savePath = QString("%1/%2%3").arg(path).arg(item.m_index).arg(prefix);
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
    m_backgroundList->clearAllItems();
    if(!m_downloadRequest)
    {
        m_downloadRequest = new MusicDownloadThunderSkinRequest(this);
        connect(m_downloadRequest, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroups)), SLOT(downLoadFinished(MusicSkinRemoteGroups)));
        m_downloadRequest->startToDownload();
    }
}

QWidget* MusicBackgroundThunderWidget::createFunctionsWidget(bool revert, QWidget *object)
{
    if(!m_functionsWidget)
    {
        m_functionsWidget = new QWidget(object);
        m_functionsWidget->setGeometry(24, 70, 585, 25);
        m_functionsWidget->hide();

        QHBoxLayout *hbox = new QHBoxLayout(m_functionsWidget);
        hbox->setContentsMargins(9, 0, 0, 9);
        hbox->addStretch(1);

        m_typeBox = new QComboBox(m_functionsWidget);
        m_typeBox->setItemDelegate(new QStyledItemDelegate(m_typeBox));
        m_typeBox->setStyleSheet(MusicUIObject::MQSSComboBoxStyle01 + MusicUIObject::MQSSItemView01);
        m_typeBox->view()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
        m_typeBox->setFixedSize(100, 20);
        m_typeBox->hide();

        m_allButton = new QPushButton(tr("All"), m_functionsWidget);
        m_allButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02 + QString("QPushButton{%1}").arg(MusicUIObject::MQSSColorStyle08));
        m_allButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_allButton->setFixedSize(35, 20);
#ifdef Q_OS_UNIX
        m_allButton->setFocusPolicy(Qt::NoFocus);
#endif
        m_allButton->hide();

        m_functionsWidget->setLayout(hbox);
        connect(m_typeBox, SIGNAL(currentIndexChanged(int)), SLOT(currentTypeChanged(int)));
    }

    QHBoxLayout *layout = TTKStatic_cast(QHBoxLayout*, m_functionsWidget->layout());
    if(revert)
    {
        layout->removeWidget(m_typeBox);
        layout->addWidget(m_allButton);
        m_allButton->show();
        m_typeBox->hide();
    }
    else
    {
        layout->removeWidget(m_allButton);
        layout->addWidget(m_typeBox);
        m_allButton->hide();
        m_typeBox->show();
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
        MusicExtractWrapper::outputThunderSkin(image.m_pix, data);
    }
}

void MusicBackgroundThunderWidget::currentTypeChanged(int index)
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
    startToDownload(TTS_FILE);
}

void MusicBackgroundThunderWidget::downLoadFinished(const MusicSkinRemoteGroups &bytes)
{
    MusicBackgroundRemoteWidget::downLoadFinished(bytes);
    m_typeBox->blockSignals(true);
    for(int i=0; i<m_groups.count(); ++i)
    {
        QString title(m_groups[i].m_group);
        m_typeBox->addItem(title.remove(TTK_STRCAT(MUSIC_THUNDER_DIR, "/")));
    }
    m_typeBox->setCurrentIndex(-1);
    m_typeBox->blockSignals(false);
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
        m_downloadRequest = new MusicDownloadBingSkinRequest(this);
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

void MusicBackgroundDailyWidget::downLoadFinished(const MusicSkinRemoteGroups &bytes)
{
    MusicBackgroundRemoteWidget::downLoadFinished(bytes);
    startToDownload(TTS_FILE);
}
