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
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->QQButton->setChecked(true);
}

MusicSongSharingWidget::~MusicSongSharingWidget()
{
    delete ui;
}

int MusicSongSharingWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicSongSharingWidget::setSongName(const QString &name)
{
    ui->sharedName->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 200));

    QString path = ART_DOWNLOAD_AL + name.split('-').front().trimmed() + SKN_FILE;
    ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path)
                                  ? path : ":/share/defaultArt").scaled(50, 50));
}
