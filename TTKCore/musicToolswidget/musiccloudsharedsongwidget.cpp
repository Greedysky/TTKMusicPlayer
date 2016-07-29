#include "musiccloudsharedsongwidget.h"
#include "musicdatadownloadthread.h"
#include "musicuploadfilewidget.h"
#include "musicnumberdefine.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicplayer.h"
#include "qnconf.h"

#include <QPainter>
#include <QBoxLayout>
#include <QTimer>
#include <QMenu>
#include <QFileDialog>
#include <QPushButton>

#define QN_BUCKET       "music"
#define QN_PRFIX        "http://o9zmxm4rh.bkt.clouddn.com"
#define QN_ACCESS_KEY   "L2GGQ-ttIlTScXVtXOdwPF2ftQAEiVK1qor5KCu3"
#define QN_SECRET_KEY   "FXiQ8EWibo-9tIlWRS3UAJOqv94pM1QViU2Gw25y"

MusicCloudSharedSongTableWidget::MusicCloudSharedSongTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 266);
    headerview->resizeSection(2, 35);

    QNConf::ACCESS_KEY = QN_ACCESS_KEY;
    QNConf::SECRET_KEY = QN_SECRET_KEY;

    m_uploading = false;
    m_uploadFileWidget = nullptr;

    m_fileDialog = new MusicCloudFileManagerDialog(this);
    m_timerToUpload = new QTimer(this);
    m_timerToUpload->setInterval(MT_S2MS);
    m_networkManager = new QNetworkAccessManager(this);
    m_qnListData = new QNSimpleListData(m_networkManager, this);
    m_qnDeleteData = new QNSimpleDeleteData(m_networkManager, this);
    m_qnUploadData = new QNSimpleUploadData(m_networkManager, this);
    connect(m_timerToUpload, SIGNAL(timeout()), SLOT(startToUploadFile()));
    connect(m_qnListData, SIGNAL(receiveFinshed(QNDataItems)), SLOT(receiveDataFinshed(QNDataItems)));
    connect(m_qnDeleteData, SIGNAL(deleteFileFinished(bool)), SLOT(deleteFileFinished(bool)));
    connect(m_qnUploadData, SIGNAL(uploadFileFinished(QString)), SLOT(uploadFileFinished(QString)));
    connect(this, SIGNAL(uploadDone()), &m_eventLoop, SLOT(quit()));

    QTimer::singleShot(MT_MS*100, this, SLOT(updateListToServer()));
}

MusicCloudSharedSongTableWidget::~MusicCloudSharedSongTableWidget()
{
    m_eventLoop.quit();
    m_timerToUpload->stop();
    delete m_fileDialog;
    delete m_timerToUpload;
    delete m_uploadFileWidget;
    delete m_qnListData;
    delete m_qnDeleteData;
    delete m_qnUploadData;
    delete m_networkManager;
}

QString MusicCloudSharedSongTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudSharedSongTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudSharedSongTableWidget::receiveDataFinshed(const QNDataItems &items)
{
    clear();
    int count = items.count();
    if(count == 0)
    {
        emit updateLabelMessage(tr("List Is Empty!"));
        createUploadFileWidget();
        return;
    }

    delete m_uploadFileWidget;
    m_uploadFileWidget = nullptr;

    setRowCount(count);
    for(int i=0; i<count; ++i)
    {
        QNDataItem dataItem = items[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), dataItem.m_name, Qt::ElideRight, 260));
        item->setToolTip(dataItem.m_name);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), MusicUtils::UNumber::size2Number(dataItem.m_size),
                                                              Qt::ElideRight, 30));
        item->setToolTip(MusicUtils::UNumber::size2Label(dataItem.m_size));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

        ///insert server datas into caches
        UploadData data;
        data.m_path = dataItem.m_name.trimmed();
        data.m_name = data.m_path;
        data.m_state = UploadData::Successed;
        if(!m_waitedFiles.contains(data))
        {
            m_waitedFiles << data;
        }
    }
    emit updateLabelMessage(tr("List Update Finished!"));
}

void MusicCloudSharedSongTableWidget::uploadFileFinished(const QString &name)
{
    for(int i=0; i<m_waitedFiles.count(); ++i)
    {
        UploadData *data = &m_waitedFiles[i];
        if(data->m_path == m_currentUploadData.m_path)
        {
            data->m_state = (data->m_name == name) ? UploadData::Successed : UploadData::Errored;
            m_fileDialog->updateItemProgress(100, *data);
            break;
        }
    }
    emit uploadDone();
}

void MusicCloudSharedSongTableWidget::deleteFileFinished(bool state)
{
    if(state)
    {
        updateListToServer();
    }
    else
    {
        MusicMessageBox message;
        message.setText(tr("Delete The Current File Error!"));
        message.exec();
    }
}

void MusicCloudSharedSongTableWidget::updateListToServer()
{
    emit updateLabelMessage(tr("List Updating"));
    m_qnListData->listDataToServer(QN_BUCKET);
}

void MusicCloudSharedSongTableWidget::deleteFileToServer()
{
    if(currentRow() < 0)
    {
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 1);
    if(it == nullptr)
    {
        return;
    }
    UploadData data;
    data.m_path = it->toolTip();
    data.m_name = data.m_path;
    data.m_state = UploadData::Successed;
    m_waitedFiles.removeOne(data);
    m_qnDeleteData->deleteDataToServer(QN_BUCKET, data.m_name);
}

void MusicCloudSharedSongTableWidget::downloadFileToServer()
{
    if(currentRow() < 0)
    {
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 1);
    if(it == nullptr)
    {
        return;
    }

    QString url = m_qnUploadData->getDownloadUrl(QN_PRFIX, it->toolTip());
    (new MusicDataDownloadThread( url, MUSIC_DIR_FULL + it->toolTip(),
         MusicDownLoadThreadAbstract::Download_Music, this))->startToDownload();
}

void MusicCloudSharedSongTableWidget::uploadFileToServer()
{
    QStringList path = QFileDialog::getOpenFileNames(this,
                       QString(), "./", "File (*.mp3)");
    if(!path.isEmpty())
    {
        foreach(QString file, path)
        {
            UploadData data;
            data.m_path = file.trimmed();
            data.m_name = QFileInfo(file).fileName().trimmed();
            data.m_state = UploadData::Waited;
            if(!m_waitedFiles.contains(data))
            {
                m_waitedFiles << data;
            }
        }
    }
    m_timerToUpload->stop();
    m_timerToUpload->start();
}

void MusicCloudSharedSongTableWidget::uploadFilesToServer()
{
    QString path =  QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        foreach(QFileInfo file, MusicUtils::UCore::findFile(path, MusicPlayer::supportFormatsFilterString()))
        {
            UploadData data;
            data.m_path = file.absoluteFilePath().trimmed();
            data.m_name = file.fileName().trimmed();
            data.m_state = UploadData::Waited;
            if(!m_waitedFiles.contains(data))
            {
                m_waitedFiles << data;
            }
        }
    }
    m_timerToUpload->stop();
    m_timerToUpload->start();
}

void MusicCloudSharedSongTableWidget::startToUploadFile()
{
    if(m_waitedFiles.isEmpty() || m_uploading)
    {
        return;
    }

    m_uploading = true;
    m_timerToUpload->stop();
    bool needUpload;
    foreach(UploadData data, m_waitedFiles)
    {
        if(data.m_state == UploadData::Waited)
        {
            needUpload = true;
            break;
        }
    }
    if(!needUpload)
    {
        m_currentUploadData = UploadData();
        return;
    }

    emit updateLabelMessage(tr("Files Is Uploading!"));
    foreach(UploadData data, m_waitedFiles)
    {
        m_currentUploadData = data;
        QFile file(data.m_path);
        if(!file.open(QIODevice::ReadOnly) || data.m_state == UploadData::Successed)
        {
            continue;
        }

        m_qnUploadData->uploadDataToServer(file.readAll(), QN_BUCKET, data.m_name, data.m_name);
        m_eventLoop.exec();
        file.close();
    }
    m_uploading = false;

    updateListToServer();
}

void MusicCloudSharedSongTableWidget::openFileManagerDialog()
{
    m_fileDialog->creatFileManager(m_waitedFiles);
    m_fileDialog->show();
}

void MusicCloudSharedSongTableWidget::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal != 0)
    {
        int value = MStatic_cast(int, (bytesSent*1.0/bytesTotal)*100);
        m_fileDialog->updateItemProgress(value, m_currentUploadData);
    }
}

void MusicCloudSharedSongTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu menu(this);

    QMenu uploadMenu(tr("upload"), &menu);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    uploadMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    uploadMenu.addAction(tr("uploadFile"), this, SLOT(uploadFileToServer()));
    uploadMenu.addAction(tr("uploadFiles"), this, SLOT(uploadFilesToServer()));

    menu.addMenu(&uploadMenu);
    menu.addAction(tr("deleteFile"), this, SLOT(deleteFileToServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("download"), this, SLOT(downloadFileToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("updateFiles"), this, SLOT(updateListToServer()))->setEnabled(!m_uploading);
    menu.exec(QCursor::pos());
}

void MusicCloudSharedSongTableWidget::createUploadFileWidget()
{
    if(m_uploadFileWidget == nullptr)
    {
        m_uploadFileWidget = new MusicUploadFileWidget(this);
        connect(m_uploadFileWidget, SIGNAL(uploadFileClicked()), SLOT(uploadFileToServer()));
        connect(m_uploadFileWidget, SIGNAL(uploadFilesClicked()), SLOT(uploadFilesToServer()));
        m_uploadFileWidget->adjustRect(width(), height());
    }
    m_uploadFileWidget->raise();
    m_uploadFileWidget->show();
}



MusicCloudSharedSongWidget::MusicCloudSharedSongWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tableWidget = new MusicCloudSharedSongTableWidget(this);

    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(5, 0, 0, 0);
    m_statusLabel = new QLabel(this);
    QPushButton *button = new QPushButton(this);
    button->setIcon(QIcon(":/desktopTool/btn_setting_hover"));
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    button->setFixedWidth(40);

    bottomLayout->addWidget(m_statusLabel);
    bottomLayout->addWidget(button);
    bottomWidget->setLayout(bottomLayout);

    layout->addWidget(m_tableWidget);
    layout->addWidget(bottomWidget);
    setLayout(layout);

    connect(m_tableWidget, SIGNAL(updateLabelMessage(QString)), m_statusLabel, SLOT(setText(QString)));
    connect(button, SIGNAL(clicked()), m_tableWidget, SLOT(openFileManagerDialog()));
}

MusicCloudSharedSongWidget::~MusicCloudSharedSongWidget()
{
    delete m_tableWidget;
    delete m_statusLabel;
}

QString MusicCloudSharedSongWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudSharedSongWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255, 50));
    painter.end();
}

void MusicCloudSharedSongWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
