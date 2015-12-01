#include "musicsongsharingwidget.h"
#include "ui_musicsongsharingwidget.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicbgthememanager.h"

MusicSongSharingWidget::MusicSongSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongSharingWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->qQButton->setChecked(true);
    ui->textEdit->setStyleSheet(MusicUIObject::MTextEditStyle01);

    connect(ui->textEdit, SIGNAL(textChanged()), SLOT(textAreaChanged()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->shareButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
}

MusicSongSharingWidget::~MusicSongSharingWidget()
{
    delete ui;
}

void MusicSongSharingWidget::setSongName(const QString &name)
{
    ui->sharedName->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 200));

    QString path = ART_DOWNLOAD_AL + name.split('-').front().trimmed() + SKN_FILE;
    ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path)
                                  ? path : ":/share/defaultArt").scaled(50, 50));

    ui->textEdit->setText(tr("I used to listen music #%1# by QMusicPlayer,").arg(name) +
                          tr("and recommend it to you! (From #QMusicPlayer#)"));
}

int MusicSongSharingWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicSongSharingWidget::confirmButtonClicked()
{
    int type = -1;
    if(ui->qQButton->isChecked()) type = 0;
    else if(ui->renRButton->isChecked()) type = 1;
    else if(ui->wechatButton->isChecked()) type = 2;
    else if(ui->sinaButton->isChecked()) type = 3;

    close();
}

void MusicSongSharingWidget::textAreaChanged()
{
    int count = ui->textEdit->toPlainText().length();
    if((count = 150 - count) > -1)
    {
        ui->textEditArea->setText(tr("You can enter %1 characters").arg(count));
    }
    ui->textEdit->setEnabled( count > -1 );
}
