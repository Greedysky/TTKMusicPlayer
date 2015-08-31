#include "musicmessagebox.h"
#include "ui_musicmessagebox.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"

MusicMessageBox::MusicMessageBox(QWidget *parent)
    : MusicMoveDialogAbstract(parent),
    ui(new Ui::MusicMessageBox)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius

    ////////////////////////////////////////////////
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

}

MusicMessageBox::~MusicMessageBox()
{
    delete ui;
}


void MusicMessageBox::setText(const QString &text) const
{
    ui->textLabel->setText(text);
}

int MusicMessageBox::exec()
{
    QPixmap pix(M_BG_MANAGER.getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicMoveDialogAbstract::exec();
}
