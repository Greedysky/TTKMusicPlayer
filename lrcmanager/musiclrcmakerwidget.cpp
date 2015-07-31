#include "musiclrcmakerwidget.h"
#include "ui_musiclrcmakerwidget.h"
#include "musicobject.h"

#include <QMessageBox>

MusicLrcMakerWidget::MusicLrcMakerWidget(QWidget *parent)
    : MusicMoveWidgetAbstract(parent),
    ui(new Ui::MusicLrcMakerWidget)
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
    ui->topTitleCloseButton->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    const QString textStyle = "QTextEdit{ border: 1px solid gray;} \
                               QTextEdit::hover{ border: 1px solid #AAAAFF;}";
    ui->artNameEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->songNameEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->authorNameEdit->setStyleSheet(MusicObject::MusicLoginEditStyle);
    ui->introductionTextEdit->setStyleSheet( textStyle );
    ui->lrcTextEdit->setStyleSheet( textStyle );

    ui->makeButton->setStyleSheet( MusicObject::MusicSettingMainButton);
    ui->saveButton->setStyleSheet( MusicObject::MusicSettingMainButton);
    ui->reviewButton->setStyleSheet( MusicObject::MusicSettingMainButton);
    ui->makeButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->saveButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->reviewButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->makeButton,SIGNAL(clicked()),SLOT(makeButtonClicked()));
    connect(ui->saveButton,SIGNAL(clicked()),SLOT(saveButtonClicked()));
    connect(ui->reviewButton,SIGNAL(clicked()),SLOT(reviewButtonClicked()));

    ui->saveButton->setEnabled(false);

}

void MusicLrcMakerWidget::makeButtonClicked()
{
    bool errorFlag = false;
    QString msg;
    if(ui->artNameEdit->text().isEmpty())
    {
        errorFlag = true;
        msg = "Art name is empty!";
    }
    if(ui->songNameEdit->text().isEmpty())
    {
        errorFlag = true;
        msg = "song name is empty!";
    }
    if(ui->lrcTextEdit->toPlainText().isEmpty())
    {
        errorFlag = true;
        msg = "lrc text is empty!";
    }
    if(errorFlag)
    {
        QMessageBox::warning(this, tr("QMusicPlayer"), msg);
        return;
    }

    ui->makeButton->setEnabled(false);
    setControlEnable(false);
}

void MusicLrcMakerWidget::saveButtonClicked()
{

}

void MusicLrcMakerWidget::reviewButtonClicked()
{
    ui->makeButton->setEnabled(true);
    setControlEnable(true);
    ui->artNameEdit->clear();
    ui->songNameEdit->clear();
    ui->authorNameEdit->clear();
    ui->lrcTextEdit->clear();
    ui->introductionTextEdit->clear();
}

void MusicLrcMakerWidget::setControlEnable(bool b)
{
    ui->artNameEdit->setEnabled(b);
    ui->songNameEdit->setEnabled(b);
    ui->authorNameEdit->setEnabled(b);
    ui->lrcTextEdit->setEnabled(b);
    ui->introductionTextEdit->setEnabled(b);
}

void MusicLrcMakerWidget::keyPressEvent(QKeyEvent* event)
{
    MusicMoveWidgetAbstract::keyPressEvent(event);
    if(!ui->makeButton->isEnabled())
    {
        qDebug()<<event->key();
    }
}

void MusicLrcMakerWidget::keyReleaseEvent(QKeyEvent* event)
{
    MusicMoveWidgetAbstract::keyReleaseEvent(event);
}
