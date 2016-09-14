#include "musiclrcsearchwidget.h"
#include "ui_musiclrcsearchwidget.h"
#include "musicuiobject.h"
#include "musiclrcsearchtablewidget.h"
#include "musicmessagebox.h"

MusicLrcSearchWidget::MusicLrcSearchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcSearchWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->label_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->songSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->lrcSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->lrcSearchDownload->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    ui->lrcSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcSearchDownload->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->lrcSearchButton, SIGNAL(clicked()), SLOT(lrcSearchButtonClicked()));
    connect(ui->lrcSearchDownload, SIGNAL(clicked()), SLOT(lrcSearchDownloadClicked()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->label_checkBox, SIGNAL(clicked(bool)), ui->tableWidget,
                                SLOT(setSelectedAllItems(bool)));
    connect(ui->tableWidget, SIGNAL(resolvedSuccess()), SLOT(lrcSearchFinished()));
    connect(ui->tableWidget, SIGNAL(lrcDownloadStateChanged(QString)),
                             SLOT(lrcDownloadStateChanged(QString)));
    connect(ui->tableWidget, SIGNAL(restartSearchQuery(QString)),
                             SLOT(setCurrentSongName(QString)));
}

MusicLrcSearchWidget::~MusicLrcSearchWidget()
{
    delete ui;
}

QString MusicLrcSearchWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcSearchWidget::setCurrentSongName(const QString &name) const
{
    ui->songSearchEdit->setText(name);
    lrcSearchButtonClicked();
}

void MusicLrcSearchWidget::lrcSearchFinished() const
{
    ui->stateLabel->setText(tr("lrc is searching finished!"));
}

void MusicLrcSearchWidget::lrcSearchButtonClicked() const
{
    ui->stateLabel->setText(tr("lrc is searching now!"));
    QString text = ui->songSearchEdit->text().trimmed();
    ui->tableWidget->startSearchQuery( text );
    ui->functionTopLabel->setText(tr("&nbsp;find <font color=red> %1 </font> result")
                                  .arg(MusicUtils::Widget::elidedText(font(), text, Qt::ElideRight, 245)));
}

void MusicLrcSearchWidget::lrcSearchDownloadClicked()
{
    MusicObject::MIntList list = ui->tableWidget->getSelectedItems();
    list.removeOne(ui->tableWidget->rowCount() - 1);
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    foreach(int row, list)
    {
        ui->tableWidget->musicDownloadLocal(row);
    }
    ui->stateLabel->setText(tr("lrc is downloading now!"));
}

void MusicLrcSearchWidget::lrcDownloadStateChanged(const QString &string)
{
    if(string == "Lrc")
    {
       ui->stateLabel->setText(tr("lrc download finished!"));
    }
}

int MusicLrcSearchWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
