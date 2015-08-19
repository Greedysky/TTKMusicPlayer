#include "musiclrcsearchwidget.h"
#include "ui_musiclrcsearchwidget.h"
#include "musicuiobject.h"
#include "musiclrcsearchtablewidget.h"

MusicLrcSearchWidget::MusicLrcSearchWidget(QWidget *parent)
    : MusicMoveDialogAbstract(parent),
      ui(new Ui::MusicLrcSearchWidget)
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

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->songSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->lrcSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->lrcSearchDownload->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);

    ui->lrcSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcSearchDownload->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->lrcSearchButton,SIGNAL(clicked()),SLOT(lrcSearchButtonClicked()));
    connect(ui->lrcSearchDownload,SIGNAL(clicked()),SLOT(lrcSearchDownloadClicked()));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->closeButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->tableWidget,SIGNAL(lrcDownloadStateChanged(QString)),
                            SLOT(lrcDownloadStateChanged(QString)));
}

MusicLrcSearchWidget::~MusicLrcSearchWidget()
{
    delete ui;
}

void MusicLrcSearchWidget::setCurrentSongName(const QString &name)
{
    ui->songSearchEdit->setText(name);
    ui->tableWidget->setCurrentSongName(name);
    lrcSearchButtonClicked();
}

void MusicLrcSearchWidget::lrcSearchButtonClicked()
{
    ui->stateLabel->setText(tr("lrc is searching now!"));
    ui->tableWidget->startSearchQuery(ui->songSearchEdit->text().trimmed());
}

void MusicLrcSearchWidget::lrcSearchDownloadClicked()
{
    ui->stateLabel->setText(tr("lrc is downloading now!"));
    int row = ui->tableWidget->currentRow();
    if(row >= 0)
        ui->tableWidget->musicDownloadLocal(row);
}

void MusicLrcSearchWidget::lrcDownloadStateChanged(const QString& string)
{
    if(string == "Lrc")
       ui->stateLabel->setText(tr("lrc download finished!"));
}
