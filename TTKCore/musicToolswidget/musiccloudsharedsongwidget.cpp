#include "musiccloudsharedsongwidget.h"
#include "musicdatadownloadthread.h"
#include "musicsourcedownloadthread.h"
#include "musicopenfilewidget.h"
#include "musicleftareawidget.h"
#include "musicnumberdefine.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicformats.h"
#include "musicnumberutils.h"
#include "musiccoreutils.h"
#include "musictoastlabel.h"

#include "qnconf.h"
#///QJson import
#include "qjson/parser.h"

#include <QMenu>
#include <QTimer>
#include <QPainter>
#include <QScrollBar>
#include <QBoxLayout>
#include <QFileDialog>
#include <QPushButton>

#define QN_BUCKET       "music"
#define QN_PRFIX        "bkdIdE5FTXFpalU3MmxKMG5OOFVLS0lWZ0tCdDRzOGtQemJ6QnN3TlN2VUc3SGp4"
#define QN_UA_URL       "VlQxWWhUSjJzWjFTSkZRRFFqdnlPK3FJZ0JxbmlrcFoydjVCRlZ2a3hRdlBuRFhmOUZObW1STmxqNVVEWUJsdA=="

MusicCloudSharedSongTableWidget::MusicCloudSharedSongTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 256);
    headerview->resizeSection(2, 45);

    m_uploading = false;
    m_openFileWidget = nullptr;
    m_currentUploadIndex = 0;

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03.arg(50));

    m_fileDialog = new MusicCloudFileManagerDialog(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_qnListData = new QNSimpleListData(m_networkManager, this);
    m_qnDeleteData = new QNSimpleDeleteData(m_networkManager, this);
    m_qnUploadData = new QNSimpleUploadData(m_networkManager, this);
    connect(m_qnListData, SIGNAL(receiveFinshed(QNDataItems)), SLOT(receiveDataFinshed(QNDataItems)));
    connect(m_qnDeleteData, SIGNAL(deleteFileFinished(bool)), SLOT(deleteFileFinished(bool)));
    connect(m_qnUploadData, SIGNAL(uploadFileFinished(QString)), SLOT(uploadFileFinished(QString)));
}

MusicCloudSharedSongTableWidget::~MusicCloudSharedSongTableWidget()
{
    delete m_fileDialog;
    delete m_openFileWidget;
    delete m_qnListData;
    delete m_qnDeleteData;
    delete m_qnUploadData;
    delete m_networkManager;
}

QString MusicCloudSharedSongTableWidget::getClassName()
{
    return staticMetaObject.className();
}

bool MusicCloudSharedSongTableWidget::getKey()
{
    QEventLoop loop;
    connect(this, SIGNAL(getKeyFinished()), &loop, SLOT(quit()));

    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(keyDownLoadFinished(QByteArray)));
    download->startToDownload(MusicCryptographicHash::decryptData(QN_UA_URL, URL_KEY));

    loop.exec();
    updateListToServer();

    return !QNConf::ACCESS_KEY.isEmpty() && !QNConf::SECRET_KEY.isEmpty();
}

void MusicCloudSharedSongTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudSharedSongTableWidget::keyDownLoadFinished(const QByteArray &data)
{
    QJson::Parser parser;
    bool ok;
    QVariant dt = parser.parse(data, &ok);
    if(ok)
    {
        QVariantMap value = dt.toMap();
        QNConf::ACCESS_KEY = value["key"].toString();
        QNConf::SECRET_KEY = value["secret"].toByteArray();
    }
    emit getKeyFinished();
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

    delete m_openFileWidget;
    m_openFileWidget = nullptr;

    setRowCount(count);
    for(int i=0; i<count; ++i)
    {
        QNDataItem dataItem = items[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), dataItem.m_name, Qt::ElideRight, 250));
        item->setToolTip(dataItem.m_name);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), MusicUtils::Number::size2Label(dataItem.m_size),
                                                              Qt::ElideRight, 40));
        item->setToolTip(item->text());
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

    m_currentUploadIndex = m_waitedFiles.count();
    emit updateLabelMessage(tr("List Update Finished!"));
}

void MusicCloudSharedSongTableWidget::uploadFileFinished(const QString &name)
{
    for(int i=0; i<m_waitedFiles.count(); ++i)
    {
        UploadData *data = &m_waitedFiles[i];
        if(data->m_path == m_waitedFiles[m_currentUploadIndex].m_path)
        {
            data->m_state = (data->m_name == name) ? UploadData::Successed : UploadData::Errored;
            m_fileDialog->updateItemProgress(100, *data);
            ++m_currentUploadIndex;
            startToUploadFile();
            break;
        }
    }
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

    if(--m_currentUploadIndex < 0)
    {
        m_currentUploadIndex = 0;
    }
    if(m_fileDialog->isVisible())
    {
        m_fileDialog->creatFilesManager(m_waitedFiles);
    }
}

void MusicCloudSharedSongTableWidget::deleteFilesToServer()
{
    foreach(const UploadData &data, m_waitedFiles)
    {
        m_waitedFiles.removeOne(data);
        m_qnDeleteData->deleteDataToServer(QN_BUCKET, data.m_name);
    }

    m_currentUploadIndex = 0;
    if(m_fileDialog->isVisible())
    {
        m_fileDialog->creatFilesManager(m_waitedFiles);
    }
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

    QString url = m_qnUploadData->getDownloadUrl(MusicCryptographicHash::decryptData(QN_PRFIX, URL_KEY), it->toolTip());
    (new MusicDataDownloadThread( url, MusicUtils::Core::musicPrefix() + it->toolTip(),
         MusicDownLoadThreadAbstract::Download_Music, this))->startToDownload();
}

void MusicCloudSharedSongTableWidget::uploadFileToServer()
{
    QStringList path = QFileDialog::getOpenFileNames(this,
                       QString(), "./", "File (*.mp3)");
    if(!path.isEmpty())
    {
        foreach(const QString &file, path)
        {
            UploadData data;
            data.m_path = file.trimmed();
            data.m_name = QFileInfo(file).fileName().trimmed();
            data.m_state = UploadData::Waited;
            if(!m_waitedFiles.contains(data))
            {
                m_waitedFiles << data;
                if(m_fileDialog->isVisible())
                {
                    m_fileDialog->creatFileManager(data);
                }
            }
        }
        if(!m_uploading)
        {
            startToUploadFile();
        }
    }
}

void MusicCloudSharedSongTableWidget::uploadFilesToServer()
{
    QString path =  QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        foreach(const QFileInfo &file, MusicUtils::Core::findFile(path, MusicFormats::supportFormatsFilterString()))
        {
            UploadData data;
            data.m_path = file.absoluteFilePath().trimmed();
            data.m_name = file.fileName().trimmed();
            data.m_state = UploadData::Waited;
            if(!m_waitedFiles.contains(data))
            {
                m_waitedFiles << data;
                if(m_fileDialog->isVisible())
                {
                    m_fileDialog->creatFileManager(data);
                }
            }
        }
        if(!m_uploading)
        {
            startToUploadFile();
        }
    }
}

void MusicCloudSharedSongTableWidget::reuploadFileToServer()
{
    m_currentUploadIndex = 0;
    startToUploadFile();
}

void MusicCloudSharedSongTableWidget::openFileManagerDialog()
{
    m_fileDialog->creatFilesManager(m_waitedFiles);
    m_fileDialog->show();
}

void MusicCloudSharedSongTableWidget::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal != 0)
    {
        int value = MStatic_cast(int, (bytesSent*1.0/bytesTotal)*100);
        m_fileDialog->updateItemProgress(value, m_waitedFiles[m_currentUploadIndex]);
    }
}

void MusicCloudSharedSongTableWidget::uploadDone()
{
    m_uploading = false;
    m_currentUploadIndex = m_waitedFiles.count();
    updateListToServer();
    m_fileDialog->setReuploadState( uploadHasFailed() );

    QTimer::singleShot(MT_MS, MusicLeftAreaWidget::instance(), SLOT(cloudSharedSongUploadAllDone()));
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
    menu.addAction(tr("deleteFiles"), this, SLOT(deleteFilesToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("download"), this, SLOT(downloadFileToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("updateFiles"), this, SLOT(updateListToServer()))->setEnabled(!m_uploading);
    menu.exec(QCursor::pos());
}

void MusicCloudSharedSongTableWidget::createUploadFileWidget()
{
    if(m_openFileWidget == nullptr)
    {
        m_openFileWidget = new MusicOpenFileWidget(this);
        connect(m_openFileWidget, SIGNAL(uploadFileClicked()), SLOT(uploadFileToServer()));
        connect(m_openFileWidget, SIGNAL(uploadFilesClicked()), SLOT(uploadFilesToServer()));
        m_openFileWidget->adjustRect(width(), height());
    }
    m_openFileWidget->raise();
    m_openFileWidget->show();
}

void MusicCloudSharedSongTableWidget::startToUploadFile()
{
    if(m_waitedFiles.isEmpty() || m_currentUploadIndex < 0 || m_currentUploadIndex >= m_waitedFiles.count())
    {
        uploadDone();
        return;
    }

    m_uploading = true;
    emit updateLabelMessage(tr("Files Is Uploading!"));

    UploadData data = m_waitedFiles[m_currentUploadIndex];
    QFile file(data.m_path);
    if(!file.open(QIODevice::ReadOnly) || data.m_state == UploadData::Successed)
    {
        ++m_currentUploadIndex;
        startToUploadFile();
    }

    m_qnUploadData->uploadDataToServer(file.readAll(), QN_BUCKET, data.m_name, data.m_name);
    file.close();
}

bool MusicCloudSharedSongTableWidget::uploadHasFailed()
{
    foreach(const UploadData &data, m_waitedFiles)
    {
        if(data.m_state != UploadData::Successed)
        {
            return true;
        }
    }
    return false;
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
    button->setStyleSheet(MusicUIObject::MPushButtonStyle01);
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

void MusicCloudSharedSongWidget::getKey()
{
    if(!m_tableWidget->getKey())
    {
        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->setFontSize(15);
        toast->setFontMargin(20, 20);
        toast->setText(tr("Init Error!"));
        toast->popup(this);
    }
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
