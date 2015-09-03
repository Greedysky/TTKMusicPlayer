#include "musicmessagebox.h"
#include "ui_musicmessagebox.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"

#include <QButtonGroup>

MusicMessageBox::MusicMessageBox(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
    ui(new Ui::MusicMessageBox)
{
    ui->setupUi(this);
    drawWindowRoundedRect(this);
    //set window radius
    m_status = 0;
    ////////////////////////////////////////////////
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *groupButton = new QButtonGroup(this);
    groupButton->addButton(ui->topTitleCloseButton, 0);
    groupButton->addButton(ui->confirmButton, 1);
    groupButton->addButton(ui->cancelButton, 2);
    connect(groupButton, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

}

MusicMessageBox::~MusicMessageBox()
{
    delete ui;
}


void MusicMessageBox::setText(const QString &text) const
{
    ui->textLabel->setText(text);
}

void MusicMessageBox::buttonClicked(int index)
{
    switch(index)
    {
        case 0: case 2: m_status = 1; break;
        case 1: m_status = 0; break;
    }
    close();
}

int MusicMessageBox::exec()
{
    QPixmap pix(M_BG_MANAGER.getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveDialog::exec();
    return m_status;
}
