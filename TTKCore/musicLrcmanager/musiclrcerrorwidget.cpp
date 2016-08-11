#include "musiclrcerrorwidget.h"
#include "ui_musiclrcerrorwidget.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"

#include <QButtonGroup>

#define MAX_LEGNTH 30

MusicLrcErrorWidget::MusicLrcErrorWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcErrorWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->radioButton1->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->radioButton2->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->radioButton3->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->radioButton4->setStyleSheet(MusicUIObject::MRadioButtonStyle01);

    ui->textEdit->setStyleSheet(MusicUIObject::MTextEditStyle01 + MusicUIObject::MScrollBarStyle01);
    ui->textEdit->setEnabled(false);
    ui->radioButton1->setChecked(true);
    connect(ui->textEdit, SIGNAL(textChanged()), SLOT(textAreaChanged()));

    ui->pushButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->pushButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioButton1, 0);
    buttonGroup->addButton(ui->radioButton2, 1);
    buttonGroup->addButton(ui->radioButton3, 2);
    buttonGroup->addButton(ui->radioButton4, 3);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

}

MusicLrcErrorWidget::~MusicLrcErrorWidget()
{
    delete ui;
}

QString MusicLrcErrorWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcErrorWidget::buttonClicked(int index)
{
    if(index == 3)
    {
        ui->textEdit->setEnabled(true);
        ui->textEdit->setText( QString() );
    }
}

void MusicLrcErrorWidget::textAreaChanged()
{
    QString text = ui->textEdit->toPlainText();
    int length = text.count();
    if(length > MAX_LEGNTH)
    {
        QTextCursor textCursor = ui->textEdit->textCursor();
        int position = textCursor.position();
        text.remove(position - (length - MAX_LEGNTH), length - MAX_LEGNTH);
        ui->textEdit->setText(text);
        textCursor.setPosition(position - (length - MAX_LEGNTH));
        ui->textEdit->setTextCursor(textCursor);
    }
    else
    {
        QString style = QString("<html><head/><body><p><span style=' font-size:10pt;'>%1"
             "</span><span style=' font-size:10pt; color:#ff0000;'> %2 "
             "</span><span style=' font-size:10pt;'>%3</span></p></body></html>")
             .arg(tr("you can now input")).arg(MAX_LEGNTH - length).arg(tr("word"));
        ui->label->setText(style);
    }
}

void MusicLrcErrorWidget::confirmButtonClicked()
{
    MusicMessageBox message;
    message.setText(tr("report error finished"));
    message.exec();

    close();
}

int MusicLrcErrorWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
