#include "musiccloudfilemanagerdialog.h"
#include "ui_musiccloudfilemanagerdialog.h"
#include "musicitemdelegate.h"
#include "musicuiobject.h"

MusicCloudUploadTableWidget::MusicCloudUploadTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 300);
    headerview->resizeSection(2, 111);
    headerview->resizeSection(3, 50);

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

}

MusicCloudUploadTableWidget::~MusicCloudUploadTableWidget()
{
    delete m_progressBarDelegate;
}

QString MusicCloudUploadTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudUploadTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}


MusicCloudFileManagerDialog::MusicCloudFileManagerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicCloudFileManagerDialog)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->reuploadButton->setEnabled(false);
    ui->reuploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    connect(ui->reuploadButton, SIGNAL(clicked()), parent, SLOT(reuploadFileToServer()));

}

MusicCloudFileManagerDialog::~MusicCloudFileManagerDialog()
{
    delete ui;
}

QString MusicCloudFileManagerDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudFileManagerDialog::setReuploadState(bool state)
{
    ui->reuploadButton->setEnabled(state);
}

void MusicCloudFileManagerDialog::creatFileManager(const UploadData &data)
{
    int index = ui->uploadTableWidget->rowCount();
    ui->uploadTableWidget->setRowCount(index + 1);
    QTableWidgetItem *item = new QTableWidgetItem;
    ui->uploadTableWidget->setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), data.m_name, Qt::ElideRight, 260));
    item->setToolTip(data.m_name);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->uploadTableWidget->setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, (data.m_state == UploadData::Successed) ? 100 : 0);
    ui->uploadTableWidget->setItem(index, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon( getIconByDataState(data.m_state) );
    item->setTextAlignment(Qt::AlignCenter);
    ui->uploadTableWidget->setItem(index, 3, item);
}

void MusicCloudFileManagerDialog::creatFilesManager(const UploadDatas &datas)
{
    ui->uploadTableWidget->clear();
    foreach(const UploadData &data, datas)
    {
        creatFileManager(data);
    }
}

void MusicCloudFileManagerDialog::updateItemProgress(int percent, const UploadData &data)
{
    for(int i=0; i<ui->uploadTableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *it = ui->uploadTableWidget->item(i, 1);
        if(it && it->toolTip() == data.m_name)
        {
            ui->uploadTableWidget->item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            ui->uploadTableWidget->item(i, 3)->setIcon( getIconByDataState(data.m_state) );
        }
    }
}

int MusicCloudFileManagerDialog::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicCloudFileManagerDialog::show()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::show();
}

QIcon MusicCloudFileManagerDialog::getIconByDataState(UploadData::State state)
{
    switch(state)
    {
        case UploadData::Waited :    return QIcon(":/tiny/lb_wait");
        case UploadData::Successed : return QIcon(":/tiny/lb_right");
        case UploadData::Errored :   return QIcon(":/tiny/lb_error");
        default: return QIcon();
    }
}
