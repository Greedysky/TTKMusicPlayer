#include "musicmessagebox.h"
#include "ui_musicmessagebox.h"
#include "musicuiobject.h"

#include <QButtonGroup>
#include <QPainter>

MusicMessageBox::MusicMessageBox(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicMessageBox)
{
    ui->setupUi(this);

    m_status = 0;
    ////////////////////////////////////////////////
    ui->topTitleCloseButton->setIcon(QIcon(":/share/lb_close"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *groupButton = new QButtonGroup(this);
    groupButton->addButton(ui->topTitleCloseButton, 0);
    groupButton->addButton(ui->confirmButton, 1);
    groupButton->addButton(ui->cancelButton, 2);
    connect(groupButton, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

}

MusicMessageBox::MusicMessageBox(const QString &text, QWidget *parent)
    : MusicMessageBox(parent)
{
    setText(text);
}

MusicMessageBox::MusicMessageBox(const QString &title, const QString &text,
                                 QWidget *parent)
    : MusicMessageBox(text, parent)
{
    setTitle(title);
}

MusicMessageBox::~MusicMessageBox()
{
    delete ui;
}

QString MusicMessageBox::getClassName()
{
    return staticMetaObject.className();
}

void MusicMessageBox::setTitle(const QString &text) const
{
    ui->topTitleName->setText(text);
}

QString MusicMessageBox::title() const
{
    return ui->topTitleName->text();
}

void MusicMessageBox::setText(const QString &text) const
{
    ui->textLabel->setText(text);
}

QString MusicMessageBox::text() const
{
    return ui->textLabel->text();
}

void MusicMessageBox::buttonClicked(int index)
{
    switch(index)
    {
        case 0:
        case 2: m_status = 1; break;
        case 1: m_status = 0; break;
    }
    close();
}

int MusicMessageBox::exec()
{
    QPixmap pix(20, 20);
    pix.fill(QColor(76, 108, 179));
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveDialog::exec();
    return m_status;
}

void MusicMessageBox::show()
{
    QPixmap pix(20, 20);
    pix.fill(QColor(76, 108, 179));
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveDialog::show();
}
