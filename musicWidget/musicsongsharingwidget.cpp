#include "musicsongsharingwidget.h"
#include "ui_musicsongsharingwidget.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicbgthememanager.h"

#include <QDesktopServices>

MusicSongSharingWidget::MusicSongSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongSharingWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->qqButton->setChecked(true);
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
    QString url;
    if(ui->qqButton->isChecked())
    {
        url = QString(QQ_SHARE).arg(ui->textEdit->toPlainText())
                               .arg(ui->sharedName->text()).arg(tr("QMusicPlayer"));
    }
    else if(ui->renrenButton->isChecked())
    {
        url = QString(RENREN_SHARE).arg(ui->textEdit->toPlainText());
    }
    else if(ui->qqspaceButton->isChecked())
    {
        url = QString(QQ_SPACE_SHARE).arg(tr("QMusicPlayer"))
                                     .arg(ui->textEdit->toPlainText());
    }
    else if(ui->qqblogButton->isChecked())
    {
        url = QString(QQ_MICBG_SHARE).arg(ui->textEdit->toPlainText());
    }
    else if(ui->sinaButton->isChecked())
    {
        url = QString(SINA_SHARE).arg(ui->textEdit->toPlainText());
    }

    url.replace('#', "%23");
    QDesktopServices::openUrl(QUrl(url));
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
