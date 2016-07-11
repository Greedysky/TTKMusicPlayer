#include "musiccloudfilemanagerdialog.h"
#include "ui_musiccloudfilemanagerdialog.h"
#include "musicbackgroundmanager.h"
#include "musicitemdelegate.h"

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

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

}

MusicCloudFileManagerDialog::~MusicCloudFileManagerDialog()
{
    delete ui;
}

QString MusicCloudFileManagerDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudFileManagerDialog::creatFileManager(const UploadDatas &datas)
{
    ui->uploadTableWidget->clear();
    ui->uploadTableWidget->setRowCount(datas.count());
    for(int i=0; i<datas.count(); ++i)
    {
        UploadData data = datas[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        ui->uploadTableWidget->setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), data.m_name, Qt::ElideRight, 260));
        item->setToolTip(data.m_name);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->uploadTableWidget->setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setData(MUSIC_PROCS_ROLE, (data.m_state == UploadData::Successed) ? 100 : 0);
        ui->uploadTableWidget->setItem(i, 2, item);

                          item = new QTableWidgetItem;
        item->setIcon( getIconByDataState(data.m_state) );
        item->setTextAlignment(Qt::AlignCenter);
        ui->uploadTableWidget->setItem(i, 3, item);
    }
}

int MusicCloudFileManagerDialog::exec()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

QIcon MusicCloudFileManagerDialog::getIconByDataState(UploadData::State state)
{
    switch(state)
    {
        case UploadData::Waited :    return QIcon(":/usermanager/wait");
        case UploadData::Successed : return QIcon(":/usermanager/right");
        case UploadData::Errored :   return QIcon(":/usermanager/error");
        default: return QIcon();
    }
}
