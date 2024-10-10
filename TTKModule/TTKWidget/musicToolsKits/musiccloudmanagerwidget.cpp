#include "musiccloudmanagerwidget.h"
#include "musiccloudfileinformationwidget.h"
#include "musicopenfilewidget.h"
#include "musicdatasourcerequest.h"
#include "musicdownloaddatarequest.h"
#include "musicfileutils.h"
#include "musicformats.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musiccloudtablewidget.h"

#include "qsync/qsyncconfig.h"
#include "qsync/qsynclistdata.h"
#include "qsync/qsyncuploaddata.h"
#include "qsync/qsyncdeletedata.h"
#include "qsync/qsyncdownloaddata.h"

static constexpr const char *OS_CLOUD_URL = "cloud";

Q_DECLARE_METATYPE(MusicCloudDataItem)

MusicCloudManagerTableWidget::MusicCloudManagerTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent),
      m_uploading(false),
      m_cancel(false),
      m_totalFileSzie(0),
      m_openFileWidget(nullptr)
{
    setColumnCount(5);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 10);
    headerView->resizeSection(1, 360);
    headerView->resizeSection(2, 110);
    headerView->resizeSection(3, 50);
    headerView->resizeSection(4, 120);

    TTK::Widget::setTransparent(this, 0);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    m_progressBarDelegate = new TTKProgressBarItemDelegate(this);
    m_progressBarDelegate->setStyleSheet(TTK::UI::ProgressBar01);
    setItemDelegateForColumn(2, m_progressBarDelegate);

    m_manager = new QNetworkAccessManager(this);
    m_syncListData = new QSyncListData(m_manager, this);
    m_syncDeleteData = new QSyncDeleteData(m_manager, this);
    m_syncUploadData = new QSyncUploadData(m_manager, this);
    m_syncDownloadData = new QSyncDownloadData(m_manager, this);

    connect(m_syncListData, SIGNAL(receiveFinshed(QSyncDataItemList)), SLOT(receiveDataFinshed(QSyncDataItemList)));
    connect(m_syncDeleteData, SIGNAL(deleteFileFinished(bool)), SLOT(deleteFileFinished(bool)));
    connect(m_syncUploadData, SIGNAL(uploadFileFinished(QString)), SLOT(uploadFileFinished(QString)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicCloudUploadTableWidget::className());
}

MusicCloudManagerTableWidget::~MusicCloudManagerTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    delete m_syncListData;
    delete m_syncDeleteData;
    delete m_syncUploadData;
    delete m_syncDownloadData;
    delete m_manager;
    delete m_openFileWidget;
    delete m_progressBarDelegate;
}

bool MusicCloudManagerTableWidget::queryCloudKey()
{
    if(!cloudConfigValid())
    {
        TTKSemaphoreLoop loop;
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

        MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startToRequest(QSyncUtils::makeDataBucketUrl() + OS_CLOUD_URL);
        loop.exec();
    }

    if(!cloudConfigValid())
    {
        return false;
    }

    updateListFromServer();
    return true;
}

void MusicCloudManagerTableWidget::resizeSection() const
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(1, 360 + width - WINDOW_WIDTH_MIN);

    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 31));
    }

    if(m_openFileWidget)
    {
        m_openFileWidget->adjustWidgetRect(this->width(), height());
    }
}

void MusicCloudManagerTableWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
    }
    else
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            QSyncConfig::NAME = value["key"].toString();
            QSyncConfig::KEY = value["secret"].toByteArray();
        }
    }

    Q_EMIT finished();
}

void MusicCloudManagerTableWidget::receiveDataFinshed(const QSyncDataItemList &items)
{
    removeItems();
    m_totalFileSzie = 0;

    const int count = items.count();
    if(count == 0)
    {
        Q_EMIT updateLabelMessage(tr("List is empty"));
        createUploadFileModule();
        return;
    }

    for(const QSyncDataItem &item : qAsConst(items))
    {
        MusicCloudDataItem data;
        data.m_id = QString::number(TTKDateTime::currentTimestamp());
        data.m_path = item.m_name.trimmed();
        data.m_state = MusicCloudDataItem::State::Successed;
        data.m_data = item;
        m_totalFileSzie += item.m_size;

        addCellItem(data);
    }

    Q_EMIT updateLabelMessage(tr("List update finished"));
    Q_EMIT updataSizeLabel(m_totalFileSzie);
}

void MusicCloudManagerTableWidget::uploadFileFinished(const QString &time)
{
    if(time == TTK_NAN_STR)
    {
        Q_EMIT uploadFileError(m_currentDataItem);
    }

    const int row = FindUploadItemRow(time);
    if(row != -1)
    {
        QTableWidgetItem *it = item(row, 0);
        if(it)
        {
            MusicCloudDataItem data = it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>();
            data.m_state = MusicCloudDataItem::State::Successed;
            it->setData(TTK_DATA_ROLE, QVariant::fromValue<MusicCloudDataItem>(data));
            m_totalFileSzie += data.m_data.m_size;
        }
        Q_EMIT updataSizeLabel(m_totalFileSzie);
    }
    else
    {
        Q_EMIT uploadFileError(m_currentDataItem);
    }

    startToUploadFile();
}

void MusicCloudManagerTableWidget::deleteFileFinished(bool state)
{
    Q_EMIT updateLabelMessage(state ? tr("Delete current file success") : tr("Delete current file error"));
}

void MusicCloudManagerTableWidget::updateListFromServer()
{
    Q_EMIT updateLabelMessage(tr("List updating"));
    m_syncListData->request(SYNC_MUSIC_BUCKET);
}

void MusicCloudManagerTableWidget::deleteFileFromServer()
{
    if(m_uploading)
    {
        MusicToastLabel::popup(tr("Current mode is uploading files"));
        return;
    }

    if(!isValid())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    const int index = currentRow();
    QTableWidgetItem *it = item(index, 0);
    if(it == nullptr)
    {
        return;
    }

    const MusicCloudDataItem &data = it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>();
    removeRow(index);

    m_syncDeleteData->request(SYNC_MUSIC_BUCKET, data.m_data.m_name);
    m_totalFileSzie -= data.m_data.m_size;
    Q_EMIT updataSizeLabel(m_totalFileSzie);

    createUploadFileModule();
}

void MusicCloudManagerTableWidget::deleteFilesFromServer()
{
    if(m_uploading)
    {
        MusicToastLabel::popup(tr("Current mode is uploading files"));
        return;
    }

    selectAll();
    const TTKIntList deletedList(selectedIndexList());

    for(int i = deletedList.count() - 1; i >= 0; --i)
    {
        const int index = deletedList[i];
        QTableWidgetItem *it = item(index, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>();
        removeRow(index);

        m_syncDeleteData->request(SYNC_MUSIC_BUCKET, data.m_data.m_name);
        m_totalFileSzie -= data.m_data.m_size;
        Q_EMIT updataSizeLabel(m_totalFileSzie);
    }

    createUploadFileModule();
}

void MusicCloudManagerTableWidget::downloadFileFromServer()
{
    if(!isValid())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    const MusicCloudDataItem &data = it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>();
    const QString &format = QFileInfo(data.m_data.m_name).completeSuffix();
    const QString &baseName = QFileInfo(data.m_data.m_name).completeBaseName();
    const QString &url = m_syncDownloadData->downloadUrl(SYNC_MUSIC_BUCKET, data.m_data.m_name);

    int index = 1;
    QString downloadPath;
    QString fileName = baseName;

    do
    {
        downloadPath = QString("%1%2.%3").arg(TTK::String::musicDirPrefix(), fileName, format);
        if(!QFile::exists(downloadPath))
        {
            break;
        }

        fileName = baseName + QString("(%1)").arg(index++);
    } while(index < 99);

    MusicDownloadDataRequest *d = new MusicDownloadDataRequest(url, downloadPath, TTK::Download::Music, TTK::Record::CloudDownload, this);
    d->startToRequest();
}

void MusicCloudManagerTableWidget::cancelUploadFilesToServer()
{
    m_cancel = true;
}

void MusicCloudManagerTableWidget::uploadFilesToServer()
{
    const QStringList &files = TTK::File::getOpenFileNames(this, MusicFormats::supportMusicInputFormats());
    uploadFilesToServer(files);
}

void MusicCloudManagerTableWidget::uploadFileDirToServer()
{
    const QString &path = TTK::File::getExistingDirectory(this);
    if(path.isEmpty())
    {
        return;
    }

    uploadFilesToServer(TTK::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats()));
}

void MusicCloudManagerTableWidget::reuploadFilesToServer(const QStringList &items)
{
    uploadFilesToServer(items);
}

void MusicCloudManagerTableWidget::uploadProgress(const QString &time, qint64 percent, qint64 total)
{
    if(total != 0)
    {
        const int value = TTKStaticCast(int, (percent * 1.0 / total) * 100);
        const int row = FindUploadItemRow(time);
        if(row != -1)
        {
            QTableWidgetItem *it = item(row, 2);
            if(it)
            {
                it->setData(TTK_PROGRESS_ROLE, value);
            }
        }
    }
}

void MusicCloudManagerTableWidget::uploadDone()
{
    m_uploading = false;
    m_currentDataItem.clear();
    updateListFromServer();
}

void MusicCloudManagerTableWidget::showFileInformationWidget()
{
    if(!isValid())
    {
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    MusicCloudFileInformationWidget widget;
    MusicCloudDataItem data(it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>());
    widget.setFileInformation(&data.m_data);
    widget.exec();
}

void MusicCloudManagerTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    QMenu uploadMenu(tr("Upload"), &menu);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    if(m_currentDataItem.isValid() && m_currentDataItem.m_state == MusicCloudDataItem::State::Waited)
    {
        uploadMenu.addAction(tr("Cancel Upload"), this, SLOT(cancelUploadFilesToServer()));
    }

    uploadMenu.addAction(tr("Upload File"), this, SLOT(uploadFilesToServer()));
    uploadMenu.addAction(tr("Upload Files"), this, SLOT(uploadFileDirToServer()));
    TTK::Widget::adjustMenuPosition(&uploadMenu);

    menu.addMenu(&uploadMenu);
    menu.addAction(tr("Delete File"), this, SLOT(deleteFileFromServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("Delete Files"), this, SLOT(deleteFilesFromServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("Download"), this, SLOT(downloadFileFromServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("Update List"), this, SLOT(updateListFromServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformationWidget()));

    TTK::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

bool MusicCloudManagerTableWidget::cloudConfigValid() const
{
    return !QSyncConfig::NAME.isEmpty() && !QSyncConfig::KEY.isEmpty();
}

void MusicCloudManagerTableWidget::addCellItem(const MusicCloudDataItem &data)
{
    const int row = rowCount();
    setRowCount(row + 1);

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_DATA_ROLE, QVariant::fromValue<MusicCloudDataItem>(data));
    setItem(row, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_data.m_name);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 20));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row, 1, item);

                      item = new QTableWidgetItem;
    item->setData(TTK_PROGRESS_ROLE, data.m_data.m_hash.isEmpty() ? 0 : 100);
    setItem(row, 2, item);

                      item = new QTableWidgetItem;
    item->setToolTip(TTK::Number::sizeByteToLabel(data.m_data.m_size));
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(3) - 5));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, 3, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_data.m_putTime);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(4) - 5));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, 4, item);
}


void MusicCloudManagerTableWidget::uploadFilesToServer(const QStringList &paths)
{
    if(paths.isEmpty())
    {
        return;
    }

    delete m_openFileWidget;
    m_openFileWidget = nullptr;

    for(const QString &path : qAsConst(paths))
    {
        MusicCloudDataItem item;
        const QFileInfo fin(path);
        item.m_id = QString::number(TTKDateTime::currentTimestamp());
        item.m_path = path;
        item.m_state = MusicCloudDataItem::State::Waited;
        item.m_data.m_name = fin.fileName().trimmed();
        item.m_data.m_putTime = fin.lastModified().toString(TTK_DATE_TIMEM_FORMAT);
        item.m_data.m_size = fin.size();

        TTK::Core::sleep(TTK_DN_MS);

        addCellItem(item);
    }

    if(!m_uploading)
    {
        startToUploadFile();
    }

    createUploadFileModule();
}

void MusicCloudManagerTableWidget::createUploadFileModule()
{
    if(rowCount() != 0)
    {
        return;
    }

    if(m_openFileWidget == nullptr)
    {
        m_openFileWidget = new MusicOpenFileWidget(this);
        connect(m_openFileWidget, SIGNAL(uploadFileClicked()), SLOT(uploadFilesToServer()));
        connect(m_openFileWidget, SIGNAL(uploadDirClicked()), SLOT(uploadFileDirToServer()));
        m_openFileWidget->adjustWidgetRect(width(), height());
    }

    m_openFileWidget->raise();
    m_openFileWidget->show();
}

void MusicCloudManagerTableWidget::startToUploadFile()
{
    m_uploading = true;
    Q_EMIT updateLabelMessage(tr("Files is uploading..."));

    if(m_cancel)
    {
        m_cancel = false;
        uploadDone();
        return;
    }

    m_currentDataItem = FindWaitedItemRow();
    if(!m_currentDataItem.isValid())
    {
        uploadDone();
        return;
    }

    m_syncUploadData->request(m_currentDataItem.m_id, SYNC_MUSIC_BUCKET, m_currentDataItem.m_data.m_name, m_currentDataItem.m_path);
}

int MusicCloudManagerTableWidget::FindUploadItemRow(const QString &time) const
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>();
        if(data.m_id == time)
        {
            return i;
        }
    }
    return -1;
}

MusicCloudDataItem MusicCloudManagerTableWidget::FindWaitedItemRow() const
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(TTK_DATA_ROLE).value<MusicCloudDataItem>();
        if(data.m_state == MusicCloudDataItem::State::Waited)
        {
            return data;
        }
    }
    return MusicCloudDataItem();
}



MusicCloudManagerWidget::MusicCloudManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle10 + TTK::UI::ColorStyle02);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    //
    QWidget *topWidget = new QWidget(mainWidget);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *pLabel = new QLabel(tr("Personal Cloud"), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(TTK::UI::ColorStyle01);

    QLabel *iLabel = new QLabel(tr("Sharing of cloud disk songs"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);
    iLabel->setStyleSheet(TTK::UI::ColorStyle03);

    QLabel *sLabel = new QLabel(tr("Size"), topWidget);

    m_sizeValueBar = new QProgressBar(topWidget);
    m_sizeValueBar->setRange(0, 100);
    m_sizeValueBar->setValue(0);
    m_sizeValueBar->setStyleSheet(TTK::UI::ProgressBar01);

    m_sizeValueLabel = new QLabel("0.0M/40.0G", topWidget);

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(sLabel);
    topWidgetLayout->addWidget(m_sizeValueBar);
    topWidgetLayout->addWidget(m_sizeValueLabel);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    //
    QWidget *functionWidget = new QWidget(mainWidget);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);

    QPushButton *uploadButton = new QPushButton(tr("Upload"), functionWidget);
    uploadButton->setFixedSize(70, 30);
    uploadButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    uploadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("Download"), functionWidget);
    downloadButton->setFixedSize(70, 30);
    downloadButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *deleteButton = new QPushButton(tr("Delete"), functionWidget);
    deleteButton->setFixedSize(70, 30);
    deleteButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    deleteButton->setCursor(QCursor(Qt::PointingHandCursor));

    QLabel *statusLabel = new QLabel(functionWidget);

    functionWidgetLayout->addWidget(uploadButton);
    functionWidgetLayout->addWidget(downloadButton);
    functionWidgetLayout->addWidget(deleteButton);
    functionWidgetLayout->addStretch(1);
    functionWidgetLayout->addWidget(statusLabel);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);
#ifdef Q_OS_UNIX
    uploadButton->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
    deleteButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(uploadButton, SIGNAL(clicked(bool)), SLOT(uploadFilesToServer()));
    connect(downloadButton, SIGNAL(clicked(bool)), SLOT(downloadFileFromServer()));
    connect(deleteButton, SIGNAL(clicked(bool)), SLOT(deleteFileFromServer()));
    //
    QWidget *labelWidget = new QWidget(mainWidget);
    labelWidget->setStyleSheet(TTK::UI::BackgroundStyle09);
    QHBoxLayout *labelWidgetLayout = new QHBoxLayout(labelWidget);
    functionWidgetLayout->setContentsMargins(10, 20, 10, 10);

    QLabel *label1 = new QLabel(tr("SongName"), labelWidget);
    label1->setAlignment(Qt::AlignCenter);
    label1->setStyleSheet(TTK::UI::FontStyle01);
    labelWidgetLayout->addWidget(label1, 100);
    m_resizeWidgets << label1;

    QLabel *label2 = new QLabel(tr("FileSize"), labelWidget);
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet(TTK::UI::FontStyle01);
    labelWidgetLayout->addWidget(label2, 1);

    QLabel *label3 = new QLabel(tr("UploadTime"), labelWidget);
    label3->setAlignment(Qt::AlignCenter);
    label3->setStyleSheet(TTK::UI::FontStyle01);
    labelWidgetLayout->addWidget(label3, 1);
    labelWidgetLayout->addStretch(3);

    labelWidget->setLayout(labelWidgetLayout);
    mainLayout->addWidget(labelWidget);

    m_tableWidget = new MusicCloudManagerTableWidget(mainWidget);

    connect(m_tableWidget, SIGNAL(updateLabelMessage(QString)), statusLabel, SLOT(setText(QString)));
    connect(m_tableWidget, SIGNAL(updataSizeLabel(qint64)), SLOT(updataSizeLabel(qint64)));

    mainLayout->addWidget(m_tableWidget);
}

MusicCloudManagerWidget::~MusicCloudManagerWidget()
{
    delete m_sizeValueLabel;
    delete m_sizeValueBar;
    delete m_tableWidget;
}

void MusicCloudManagerWidget::initialize() const
{
    m_tableWidget->queryCloudKey();
}

void MusicCloudManagerWidget::resizeWidget()
{
    m_tableWidget->resizeSection();

    if(!m_resizeWidgets.isEmpty())
    {
        const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        m_resizeWidgets[0]->setFixedWidth(480 + width - WINDOW_WIDTH_MIN);
    }
}

void MusicCloudManagerWidget::updataSizeLabel(qint64 size)
{
    m_sizeValueLabel->setText(QString("%1/40.0G").arg(TTK::Number::sizeByteToLabel(size)));
    m_sizeValueBar->setValue(size * TTK_RN_MAX / (40 * TTK_SN_GB2B));
}

void MusicCloudManagerWidget::downloadFileFromServer()
{
    m_tableWidget->downloadFileFromServer();
}

void MusicCloudManagerWidget::deleteFileFromServer()
{
    m_tableWidget->deleteFileFromServer();
}

void MusicCloudManagerWidget::uploadFilesToServer()
{
    m_tableWidget->uploadFilesToServer();
}

void MusicCloudManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWidget();
}
