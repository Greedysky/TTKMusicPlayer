#include "musiclrccolorwidget.h"
#include "musicuiobject.h"
#include "ui_musiclrccolorwidget.h"

#include <QColorDialog>

MusicLrcColorWidget::MusicLrcColorWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcColorWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->listWidget->setFrameShape(QFrame::NoFrame);
    ui->listWidget->setSpacing(1);

    ui->addButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->deleteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->modifyButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->upButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->downButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(ui->deleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
    connect(ui->modifyButton, SIGNAL(clicked()), SLOT(modifyButtonClicked()));
    connect(ui->upButton, SIGNAL(clicked()), SLOT(upButtonClicked()));
    connect(ui->downButton, SIGNAL(clicked()), SLOT(downButtonClicked()));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(accept()));
}

MusicLrcColorWidget::~MusicLrcColorWidget()
{
    delete ui;
}

QString MusicLrcColorWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcColorWidget::setColors(const QList<QColor> &colors)
{
    foreach(const QColor &color, colors)
    {
        QListWidgetItem *it = new QListWidgetItem(ui->listWidget);
        it->setBackgroundColor(color);
        ui->listWidget->addItem(it);
    }
}

QList<QColor> MusicLrcColorWidget::getColors()
{
    QList<QColor> colors;
    for(int i=0; i<ui->listWidget->count(); ++i)
    {
        colors << ui->listWidget->item(i)->backgroundColor();
    }

    QLinearGradient line;

    return colors;
}

void MusicLrcColorWidget::addButtonClicked()
{
    QColorDialog getColor(Qt::white, this);
    if(getColor.exec())
    {
        QColor color = getColor.selectedColor();
        QListWidgetItem *it = new QListWidgetItem(ui->listWidget);
        it->setBackgroundColor(color);
        ui->listWidget->addItem(it);
    }
}

void MusicLrcColorWidget::deleteButtonClicked()
{
    int index = ui->listWidget->currentRow();
    if(index >= 0)
    {
        delete ui->listWidget->takeItem(index);
    }
}

void MusicLrcColorWidget::modifyButtonClicked()
{
    QColorDialog getColor(Qt::white, this);
    if(getColor.exec())
    {
        QColor color = getColor.selectedColor();
        QListWidgetItem *it = ui->listWidget->currentItem();
        if(it)
        {
            it->setBackgroundColor(color);
        }
    }
}

void MusicLrcColorWidget::upButtonClicked()
{
    int index = ui->listWidget->currentRow();
    if(index >= 0)
    {
        QListWidgetItem *it = ui->listWidget->takeItem(index);
        if(--index < 0)
        {
            index = 0;
        }
        ui->listWidget->insertItem(index, it);
        ui->listWidget->setCurrentRow(index);
    }
}

void MusicLrcColorWidget::downButtonClicked()
{
    int index = ui->listWidget->currentRow();
    if(index >= 0)
    {
        QListWidgetItem *it = ui->listWidget->takeItem(index);
        if(++index >= ui->listWidget->count())
        {
            index = ui->listWidget->count();
        }
        ui->listWidget->insertItem(index, it);
        ui->listWidget->setCurrentRow(index);
    }
}

int MusicLrcColorWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
