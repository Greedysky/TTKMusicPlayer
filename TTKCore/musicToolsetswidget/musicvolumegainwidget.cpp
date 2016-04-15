#include "musicvolumegainwidget.h"
#include "ui_musicvolumegainwidget.h"
#include "musicbackgroundmanager.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"

MusicVolumeGainWidget::MusicVolumeGainWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicVolumeGainWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->addFileButton->setIcon(QIcon(":/toolSets/gainAddFile"));
    ui->addFileButton->setIconSize(QSize(40, 40));
    ui->addFileButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->addFileButton->setToolTip(tr("addFile"));

    ui->addFilesButton->setIcon(QIcon(":/toolSets/gainAddFiles"));
    ui->addFilesButton->setIconSize(QSize(40, 40));
    ui->addFilesButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->addFilesButton->setToolTip(tr("addFiles"));

    ui->rmFileButton->setIcon(QIcon(":/toolSets/gainRmFile"));
    ui->rmFileButton->setIconSize(QSize(40, 40));
    ui->rmFileButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->rmFileButton->setToolTip(tr("rmFile"));

    ui->rmFilesButton->setIcon(QIcon(":/toolSets/gainRmFiles"));
    ui->rmFilesButton->setIconSize(QSize(40, 40));
    ui->rmFilesButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->rmFilesButton->setToolTip(tr("rmFiles"));

    ui->analysisButton->setIcon(QIcon(":/toolSets/analysis"));
    ui->analysisButton->setIconSize(QSize(40, 40));
    ui->analysisButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->analysisButton->setToolTip(tr("analysis"));

    ui->applyButton->setIcon(QIcon(":/toolSets/apply"));
    ui->applyButton->setIconSize(QSize(50, 50));
    ui->applyButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->applyButton->setToolTip(tr("apply"));

}

MusicVolumeGainWidget::~MusicVolumeGainWidget()
{
    delete ui;
}

int MusicVolumeGainWidget::exec()
{
    if(!QFile::exists(MAKE_GAIN_AL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

