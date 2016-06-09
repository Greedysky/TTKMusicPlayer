#include "musicsoundtouchwidget.h"
#include "ui_musicsoundtouchwidget.h"
#include "musicbackgroundmanager.h"
#include "musicmessagebox.h"
#include "musicobject.h"
#include "musicuiobject.h"

MusicSoundTouchWidget::MusicSoundTouchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSoundTouchWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

}

MusicSoundTouchWidget::~MusicSoundTouchWidget()
{
    delete ui;
}

int MusicSoundTouchWidget::exec()
{
    if(!QFile::exists(MAKE_SOUNDTOUCH_FULL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
