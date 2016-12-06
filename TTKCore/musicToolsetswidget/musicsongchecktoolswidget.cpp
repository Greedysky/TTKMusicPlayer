#include "musicsongchecktoolswidget.h"
#include "ui_musicsongchecktoolswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"

MusicSongCheckToolsWidget::MusicSongCheckToolsWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicSongCheckToolsWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->renameCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->qualityCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->duplicateCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(ui->renameButton, SIGNAL(clicked()), SLOT(renameButtonClicked()));
    connect(ui->qualityButton, SIGNAL(clicked()), SLOT(qualityButtonClicked()));
    connect(ui->duplicateButton, SIGNAL(clicked()), SLOT(duplicateButtonClicked()));

    switchToSelectedItemStyle(0);
}

MusicSongCheckToolsWidget::~MusicSongCheckToolsWidget()
{
    delete ui;
}

QString MusicSongCheckToolsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsWidget::renameButtonClicked()
{
    switchToSelectedItemStyle(0);
}

void MusicSongCheckToolsWidget::qualityButtonClicked()
{
    switchToSelectedItemStyle(1);
}

void MusicSongCheckToolsWidget::duplicateButtonClicked()
{
    switchToSelectedItemStyle(2);
}

void MusicSongCheckToolsWidget::show()
{
    setBackgroundPixmap(ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSongCheckToolsWidget::switchToSelectedItemStyle(int index)
{
    ui->renameButton->setStyleSheet(MusicUIObject::MKGCheckTestRename);
    ui->qualityButton->setStyleSheet(MusicUIObject::MKGCheckTestQuality);
    ui->duplicateButton->setStyleSheet(MusicUIObject::MKGCheckTestDuplicate);

    ui->stackedWidget->setCurrentIndex(index);
    switch(index)
    {
        case 0: ui->renameButton->setStyleSheet(MusicUIObject::MKGCheckTestRenameClicked); break;
        case 1: ui->qualityButton->setStyleSheet(MusicUIObject::MKGCheckTestQualityClicked);break;
        case 2: ui->duplicateButton->setStyleSheet(MusicUIObject::MKGCheckTestDuplicateClicked);break;
        default: break;
    }
}
