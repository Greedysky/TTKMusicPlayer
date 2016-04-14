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

