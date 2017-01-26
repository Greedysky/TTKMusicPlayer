#include "musicsongchecktoolswidget.h"
#include "ui_musicsongchecktoolswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"
#include "musicsongchecktoolscore.h"
#include "musicconnectionpool.h"
#include "musicsongssummariziedwidget.h"

MusicSongCheckToolsWidget::MusicSongCheckToolsWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongCheckToolsWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    renameWidgetInit();
    qualityWidgetInit();
    duplicateWidgetInit();

    switchToSelectedItemStyle(0);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicSongCheckToolsWidget::~MusicSongCheckToolsWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_renameCore;
    delete m_duplicateCore;
    delete m_qualityCore;
    delete m_ui;
}

QString MusicSongCheckToolsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsWidget::renameButtonClicked()
{
    switchToSelectedItemStyle(0);
}

void MusicSongCheckToolsWidget::renameButtonCheckClicked()
{
    if(m_ui->renameCheckButton->text() == tr("StartCheck"))
    {
        renameReCheckButtonClicked();
    }
    else if(m_ui->renameCheckButton->text() == tr("StopCheck"))
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameCheckButton->setText(tr("StartCheck"));
        m_renameCore->stopAndQuitThread();
    }
    else if(m_ui->renameCheckButton->text() == tr("ApplayCheck"))
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameReCheckButton->show();
    }
}

void MusicSongCheckToolsWidget::renameReCheckButtonClicked()
{
    m_ui->renameReCheckButton->hide();
    m_ui->renameLoadingLabel->start();
    m_ui->renameLoadingLabel->show();
    m_ui->renameCheckButton->setText(tr("StopCheck"));

    m_renameCore->stopAndQuitThread();
    MusicSongItems songs;
    emit getMusicLists(songs);
    m_renameCore->setRenameSongs(&songs);
    m_renameCore->start();
}

void MusicSongCheckToolsWidget::qualityButtonClicked()
{
    switchToSelectedItemStyle(1);
}

void MusicSongCheckToolsWidget::qualityButtonCheckClicked()
{

}

void MusicSongCheckToolsWidget::qualityReCheckButtonClicked()
{

}

void MusicSongCheckToolsWidget::duplicateButtonClicked()
{
    switchToSelectedItemStyle(2);
}

void MusicSongCheckToolsWidget::duplicateButtonCheckClicked()
{

}

void MusicSongCheckToolsWidget::duplicateReCheckButtonClicked()
{

}

void MusicSongCheckToolsWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSongCheckToolsWidget::renameWidgetInit()
{
    m_ui->renameSelectAllButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->renameCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->renameButton, SIGNAL(clicked()), SLOT(renameButtonClicked()));
    connect(m_ui->renameCheckButton, SIGNAL(clicked()), SLOT(renameButtonCheckClicked()));
    connect(m_ui->renameReCheckButton, SIGNAL(clicked()), SLOT(renameReCheckButtonClicked()));

    m_ui->renameSelectAllButton->setEnabled(false);
    m_ui->renameLoadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->renameLoadingLabel->hide();
    m_ui->renameReCheckButton->hide();

    m_renameCore = new MusicSongCheckToolsRenameCore(this);
}

void MusicSongCheckToolsWidget::qualityWidgetInit()
{
    m_ui->qualitySelectAllButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->qualityCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->qualityButton, SIGNAL(clicked()), SLOT(qualityButtonClicked()));
    connect(m_ui->qualityCheckButton, SIGNAL(clicked()), SLOT(qualityButtonCheckClicked()));
    connect(m_ui->qualityCheckButton, SIGNAL(clicked()), SLOT(qualityReCheckButtonClicked()));

    m_ui->qualitySelectAllButton->setEnabled(false);
    m_ui->qualityLoadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->qualityLoadingLabel->hide();
    m_ui->qualityReCheckButton->hide();

    m_qualityCore = new MusicSongCheckToolsQualityCore(this);
}

void MusicSongCheckToolsWidget::duplicateWidgetInit()
{
    m_ui->duplicateSelectAllButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->duplicateCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->duplicateButton, SIGNAL(clicked()), SLOT(duplicateButtonClicked()));
    connect(m_ui->duplicateCheckButton, SIGNAL(clicked()), SLOT(duplicateButtonCheckClicked()));
    connect(m_ui->duplicateCheckButton, SIGNAL(clicked()), SLOT(duplicateReCheckButtonClicked()));

    m_ui->duplicateSelectAllButton->setEnabled(false);
    m_ui->duplicateLoadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->duplicateLoadingLabel->hide();
    m_ui->duplicateReCheckButton->hide();

    m_duplicateCore = new MusicSongCheckToolsDuplicateCore(this);
}

void MusicSongCheckToolsWidget::switchToSelectedItemStyle(int index)
{
    m_ui->renameButton->setStyleSheet(MusicUIObject::MKGCheckTestRename);
    m_ui->qualityButton->setStyleSheet(MusicUIObject::MKGCheckTestQuality);
    m_ui->duplicateButton->setStyleSheet(MusicUIObject::MKGCheckTestDuplicate);

    m_ui->stackedWidget->setCurrentIndex(index);
    switch(index)
    {
        case 0: m_ui->renameButton->setStyleSheet(MusicUIObject::MKGCheckTestRenameClicked); break;
        case 1: m_ui->qualityButton->setStyleSheet(MusicUIObject::MKGCheckTestQualityClicked);break;
        case 2: m_ui->duplicateButton->setStyleSheet(MusicUIObject::MKGCheckTestDuplicateClicked);break;
        default: break;
    }
}
