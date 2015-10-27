#include "musiclrcsearchwidget.h"
#include "ui_musiclrcsearchwidget.h"
#include "musicuiobject.h"
#include "musiclrcsearchtablewidget.h"
#include "musicbgthememanager.h"
#include "musicmessagebox.h"

MusicLrcSearchWidget::MusicLrcSearchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcSearchWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->songSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->lrcSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->lrcSearchDownload->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    ui->lrcSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcSearchDownload->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->lrcSearchButton, SIGNAL(clicked()), SLOT(lrcSearchButtonClicked()));
    connect(ui->lrcSearchDownload, SIGNAL(clicked()), SLOT(lrcSearchDownloadClicked()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->label_checkBox, SIGNAL(clicked(bool)), ui->tableWidget,
                                SLOT(setSelectedAllItems(bool)));
    connect(ui->tableWidget, SIGNAL(lrcDownloadStateChanged(QString)),
                             SLOT(lrcDownloadStateChanged(QString)));
    connect(ui->tableWidget, SIGNAL(restartSearchQuery(QString)),
                             SLOT(setCurrentSongName(QString)));
}

MusicLrcSearchWidget::~MusicLrcSearchWidget()
{
    delete ui;
}

void MusicLrcSearchWidget::setCurrentSongName(const QString &name) const
{
    ui->songSearchEdit->setText(name);
    ui->tableWidget->setCurrentSongName(name);
    lrcSearchButtonClicked();
}

void MusicLrcSearchWidget::lrcSearchButtonClicked() const
{
    ui->stateLabel->setText(tr("lrc is searching now!"));
    QString text = ui->songSearchEdit->text().trimmed();
    ui->tableWidget->startSearchQuery( text );
    ui->functionTopLabel->setText(tr("&nbsp;find <font color=red> %1 </font> result")
                                  .arg(QFontMetrics(font()).elidedText(text, Qt::ElideRight, 245)));
}

void MusicLrcSearchWidget::lrcSearchDownloadClicked()
{
    ui->stateLabel->setText(tr("lrc is downloading now!"));
    MIntList list = ui->tableWidget->getSelectedItems();
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
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
