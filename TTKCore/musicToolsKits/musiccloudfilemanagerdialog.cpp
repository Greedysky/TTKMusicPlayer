#include "musiccloudfilemanagerdialog.h"
#include "ui_musiccloudfilemanagerdialog.h"
#include "musiccloudtablewidget.h"
#include "musicitemdelegate.h"
#include "musicuiobject.h"

MusicCloudFileManagerDialog::MusicCloudFileManagerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicCloudFileManagerDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->reuploadButton->setEnabled(false);
    m_ui->reuploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->downloadedButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->reuploadButton, SIGNAL(clicked()), parent, SLOT(reuploadFileToServer()));
    connect(m_ui->downloadedButton, SIGNAL(clicked()), SLOT(downloadStateChanged()));
}

MusicCloudFileManagerDialog::~MusicCloudFileManagerDialog()
{
    delete m_ui;
}

QString MusicCloudFileManagerDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudFileManagerDialog::setReuploadState(bool state)
{
    m_ui->reuploadButton->setEnabled(state);
}

void MusicCloudFileManagerDialog::creatFileManager(const MusicUploadData &data)
{
    int index = m_ui->uploadedTableWidget->rowCount();
    m_ui->uploadedTableWidget->setRowCount(index + 1);
    QTableWidgetItem *item = new QTableWidgetItem;
    m_ui->uploadedTableWidget->setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), data.m_name, Qt::ElideRight, 260));
    item->setToolTip(data.m_name);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ui->uploadedTableWidget->setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, (data.m_state == MusicUploadData::Successed) ? 100 : 0);
    m_ui->uploadedTableWidget->setItem(index, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon( getIconByDataState(data.m_state) );
    item->setTextAlignment(Qt::AlignCenter);
    m_ui->uploadedTableWidget->setItem(index, 3, item);
}

void MusicCloudFileManagerDialog::creatFilesManager(const MusicUploadDatas &datas)
{
    m_ui->uploadedTableWidget->clear();
    foreach(const MusicUploadData &data, datas)
    {
        creatFileManager(data);
    }
}

void MusicCloudFileManagerDialog::updateItemProgress(int percent, const MusicUploadData &data)
{
    for(int i=0; i<m_ui->uploadedTableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *it = m_ui->uploadedTableWidget->item(i, 1);
        if(it && it->toolTip() == data.m_name)
        {
            m_ui->uploadedTableWidget->item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            m_ui->uploadedTableWidget->item(i, 3)->setIcon( getIconByDataState(data.m_state) );
        }
    }
}

void MusicCloudFileManagerDialog::downloadStateChanged()
{
    if(m_ui->downloadedButton->text() == tr("Downloaded"))
    {
        m_ui->downloadedButton->setText(tr("Uploaded"));
        m_ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        m_ui->downloadedButton->setText(tr("Downloaded"));
        m_ui->stackedWidget->setCurrentIndex(0);
    }
}

int MusicCloudFileManagerDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicCloudFileManagerDialog::show()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::show();
}

QIcon MusicCloudFileManagerDialog::getIconByDataState(MusicUploadData::State state)
{
    switch(state)
    {
        case MusicUploadData::Waited :    return QIcon(":/tiny/lb_wait");
        case MusicUploadData::Successed : return QIcon(":/tiny/lb_right");
        case MusicUploadData::Errored :   return QIcon(":/tiny/lb_error");
        default: return QIcon();
    }
}
