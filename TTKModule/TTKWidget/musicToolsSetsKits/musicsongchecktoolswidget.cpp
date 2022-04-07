#include "musicsongchecktoolswidget.h"
#include "ui_musicsongchecktoolswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicsongchecktoolsthread.h"
#include "musictoastlabel.h"

MusicSongCheckToolsWidget::MusicSongCheckToolsWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongCheckToolsWidget)
{
    Q_UNUSED(qRegisterMetaType<MusicSongCheckToolsRenameList>("MusicSongCheckToolsRenameList"));
    Q_UNUSED(qRegisterMetaType<MusicSongCheckToolsDuplicateList>("MusicSongCheckToolsDuplicateList"));
    Q_UNUSED(qRegisterMetaType<MusicSongCheckToolsQualityList>("MusicSongCheckToolsQualityList"));

    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    initRenameWidget();
    initQualityWidget();
    initDuplicateWidget();

    switchToSelectedItemStyle(0);
}

MusicSongCheckToolsWidget::~MusicSongCheckToolsWidget()
{
    G_SINGLE_MANAGER_PTR->removeObject(className());
    delete m_renameCore;
    delete m_duplicateCore;
    delete m_qualityCore;
    delete m_ui;
}

void MusicSongCheckToolsWidget::renameButtonClicked()
{
    switchToSelectedItemStyle(0);
}

void MusicSongCheckToolsWidget::renameButtonCheckClicked()
{
    if(m_ui->renameCheckButton->text() == tr("Start"))
    {
        renameReCheckButtonClicked();
    }
    else if(m_ui->renameCheckButton->text() == tr("Stop"))
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameCheckButton->setText(tr("Start"));

        m_renameCore->setMode(MusicObject::Check);
        m_renameCore->stopAndQuitThread();
    }
    else if(m_ui->renameCheckButton->text() == tr("Apply"))
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameReCheckButton->show();

        m_renameCore->setItemList(m_ui->renameTableWidget->selectedItems());
        m_renameCore->setMode(MusicObject::Apply);
        m_renameCore->stopAndQuitThread();
        m_renameCore->start();
    }
}

void MusicSongCheckToolsWidget::renameReCheckButtonClicked()
{
    m_ui->renameReCheckButton->hide();
    m_ui->renameLoadingLabel->start();
    m_ui->renameLoadingLabel->show();
    m_ui->renameCheckButton->setText(tr("Stop"));
    m_ui->renameSelectAllButton->setChecked(false);

    m_ui->renameTableWidget->clear();
    m_renameCore->stopAndQuitThread();
    m_localSongs = m_ui->selectedAreaWidget->selectedSongItems();

    m_renameCore->setMode(MusicObject::Check);
    m_renameCore->setRenameSongs(&m_localSongs);
    m_renameCore->start();
}

void MusicSongCheckToolsWidget::renameCheckFinished(const MusicSongCheckToolsRenameList &items)
{
    if(m_renameCore->mode() == MusicObject::Check || items.isEmpty())
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameCheckButton->setText(tr("Apply"));
        m_ui->renameReCheckButton->show();
        m_ui->renameSelectAllButton->setEnabled(!items.isEmpty());

        m_ui->renameTableWidget->clear();
        m_ui->renameTableWidget->createAllItems(items);
    }
    else if(m_renameCore->mode() == MusicObject::Apply &&
           !m_ui->renameTableWidget->selectedItems().isEmpty())
    {
        MusicToastLabel::popup(tr("Rename apply finished"));
    }
}

void MusicSongCheckToolsWidget::qualityButtonClicked()
{
    switchToSelectedItemStyle(2);
}

void MusicSongCheckToolsWidget::qualityButtonCheckClicked()
{
    if(m_ui->qualityCheckButton->text() == tr("Start"))
    {
        qualityReCheckButtonClicked();
    }
    else if(m_ui->qualityCheckButton->text() == tr("Stop"))
    {
        m_ui->qualityLoadingLabel->stop();
        m_ui->qualityLoadingLabel->hide();
        m_ui->qualityCheckButton->setText(tr("Start"));
        m_qualityCore->stopAndQuitThread();
    }
    else if(m_ui->qualityCheckButton->text() == tr("Apply"))
    {
        m_ui->qualityLoadingLabel->stop();
        m_ui->qualityLoadingLabel->hide();
        m_ui->qualityReCheckButton->show();
    }
}

void MusicSongCheckToolsWidget::qualityReCheckButtonClicked()
{
    m_ui->qualityReCheckButton->hide();
    m_ui->qualityLoadingLabel->start();
    m_ui->qualityLoadingLabel->show();
    m_ui->qualityCheckButton->setText(tr("Stop"));

    m_qualityCore->stopAndQuitThread();
    m_localSongs = m_ui->selectedAreaWidget->selectedSongItems();

    m_qualityCore->setQualitySongs(&m_localSongs);
    m_qualityCore->start();
}

void MusicSongCheckToolsWidget::qualityCheckFinished(const MusicSongCheckToolsQualityList &items)
{
    m_ui->qualityLoadingLabel->stop();
    m_ui->qualityLoadingLabel->hide();
    m_ui->qualityCheckButton->setText(tr("Apply"));
    m_ui->qualityReCheckButton->show();

    m_ui->qualityTableWidget->clear();
    m_ui->qualityTableWidget->createAllItems(items);
}

void MusicSongCheckToolsWidget::duplicateButtonClicked()
{
    switchToSelectedItemStyle(1);
}

void MusicSongCheckToolsWidget::duplicateButtonCheckClicked()
{
    if(m_ui->duplicateCheckButton->text() == tr("Start"))
    {
        duplicateReCheckButtonClicked();
    }
    else if(m_ui->duplicateCheckButton->text() == tr("Stop"))
    {
        m_ui->duplicateLoadingLabel->stop();
        m_ui->duplicateLoadingLabel->hide();
        m_ui->duplicateCheckButton->setText(tr("Start"));

        m_duplicateCore->setMode(MusicObject::Check);
        m_duplicateCore->stopAndQuitThread();
    }
    else if(m_ui->duplicateCheckButton->text() == tr("Apply"))
    {
        m_ui->duplicateLoadingLabel->stop();
        m_ui->duplicateLoadingLabel->hide();
        m_ui->duplicateReCheckButton->show();

        m_duplicateCore->setItemList(m_ui->duplicateTableWidget->selectedItems());
        m_duplicateCore->setMode(MusicObject::Apply);
        m_duplicateCore->stopAndQuitThread();
        m_duplicateCore->start();
    }
}

void MusicSongCheckToolsWidget::duplicateReCheckButtonClicked()
{
    m_ui->duplicateReCheckButton->hide();
    m_ui->duplicateLoadingLabel->start();
    m_ui->duplicateLoadingLabel->show();
    m_ui->duplicateCheckButton->setText(tr("Stop"));
    m_ui->duplicateSelectAllButton->setChecked(false);

    m_qualityCore->stopAndQuitThread();
    m_localSongs = m_ui->selectedAreaWidget->selectedSongItems();

    m_duplicateCore->setMode(MusicObject::Check);
    m_duplicateCore->setDuplicateSongs(&m_localSongs);
    m_duplicateCore->start();
}

void MusicSongCheckToolsWidget::duplicateCheckFinished(const MusicSongCheckToolsDuplicateList &items)
{
    if(m_duplicateCore->mode() == MusicObject::Check || items.isEmpty())
    {
        m_ui->duplicateLoadingLabel->stop();
        m_ui->duplicateLoadingLabel->hide();
        m_ui->duplicateCheckButton->setText(tr("Apply"));
        m_ui->duplicateReCheckButton->show();
        m_ui->duplicateSelectAllButton->setEnabled(!items.isEmpty());

        m_ui->duplicateTableWidget->clear();
        m_ui->duplicateTableWidget->createAllItems(items);
    }
    else if(m_duplicateCore->mode() == MusicObject::Apply &&
           !m_ui->duplicateTableWidget->selectedItems().isEmpty())
    {
        MusicToastLabel::popup(tr("Duplicate apply finished"));
    }
}

void MusicSongCheckToolsWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSongCheckToolsWidget::initRenameWidget()
{
    m_ui->renameSelectAllButton->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->renameCheckButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

    connect(m_ui->renameButton, SIGNAL(clicked()), SLOT(renameButtonClicked()));
    connect(m_ui->renameCheckButton, SIGNAL(clicked()), SLOT(renameButtonCheckClicked()));
    connect(m_ui->renameReCheckButton, SIGNAL(clicked()), SLOT(renameReCheckButtonClicked()));
    connect(m_ui->renameSelectAllButton, SIGNAL(clicked(bool)), m_ui->renameTableWidget, SLOT(selectedAllItems(bool)));

#ifdef Q_OS_UNIX
    m_ui->renameCheckButton->setFocusPolicy(Qt::NoFocus);
    m_ui->renameSelectAllButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->renameSelectAllButton->setEnabled(false);
    m_ui->renameLoadingLabel->setType(MusicGifLabelWidget::CicleBlue);
    m_ui->renameLoadingLabel->hide();
    m_ui->renameReCheckButton->hide();

    m_renameCore = new MusicSongCheckToolsRenameThread(this);
    connect(m_renameCore, SIGNAL(finished(MusicSongCheckToolsRenameList)), SLOT(renameCheckFinished(MusicSongCheckToolsRenameList)));
}

void MusicSongCheckToolsWidget::initQualityWidget()
{
    m_ui->qualityCheckButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

    connect(m_ui->qualityButton, SIGNAL(clicked()), SLOT(qualityButtonClicked()));
    connect(m_ui->qualityCheckButton, SIGNAL(clicked()), SLOT(qualityButtonCheckClicked()));
    connect(m_ui->qualityReCheckButton, SIGNAL(clicked()), SLOT(qualityReCheckButtonClicked()));

#ifdef Q_OS_UNIX
    m_ui->qualityCheckButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->qualityLoadingLabel->setType(MusicGifLabelWidget::CicleBlue);
    m_ui->qualityLoadingLabel->hide();
    m_ui->qualityReCheckButton->hide();

    m_qualityCore = new MusicSongCheckToolsQualityThread(this);
    connect(m_qualityCore, SIGNAL(finished(MusicSongCheckToolsQualityList)), SLOT(qualityCheckFinished(MusicSongCheckToolsQualityList)));
}

void MusicSongCheckToolsWidget::initDuplicateWidget()
{
    m_ui->duplicateSelectAllButton->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->duplicateCheckButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

    connect(m_ui->duplicateButton, SIGNAL(clicked()), SLOT(duplicateButtonClicked()));
    connect(m_ui->duplicateCheckButton, SIGNAL(clicked()), SLOT(duplicateButtonCheckClicked()));
    connect(m_ui->duplicateReCheckButton, SIGNAL(clicked()), SLOT(duplicateReCheckButtonClicked()));
    connect(m_ui->duplicateSelectAllButton, SIGNAL(clicked(bool)), m_ui->duplicateTableWidget, SLOT(selectedAllItems(bool)));

#ifdef Q_OS_UNIX
    m_ui->duplicateSelectAllButton->setFocusPolicy(Qt::NoFocus);
    m_ui->duplicateCheckButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->duplicateSelectAllButton->setEnabled(false);
    m_ui->duplicateLoadingLabel->setType(MusicGifLabelWidget::CicleBlue);
    m_ui->duplicateLoadingLabel->hide();
    m_ui->duplicateReCheckButton->hide();

    m_duplicateCore = new MusicSongCheckToolsDuplicateThread(this);
    connect(m_duplicateCore, SIGNAL(finished(MusicSongCheckToolsDuplicateList)), SLOT(duplicateCheckFinished(MusicSongCheckToolsDuplicateList)));
}

void MusicSongCheckToolsWidget::switchToSelectedItemStyle(int index)
{
    m_ui->renameButton->setStyleSheet(MusicUIObject::MQSSCheckTestRename);
    m_ui->qualityButton->setStyleSheet(MusicUIObject::MQSSCheckTestQuality);
    m_ui->duplicateButton->setStyleSheet(MusicUIObject::MQSSCheckTestDuplicate);

    m_ui->stackedWidget->setCurrentIndex(index);
    switch(index)
    {
        case 0: m_ui->renameButton->setStyleSheet(MusicUIObject::MQSSCheckTestRenameClicked); break;
        case 1: m_ui->duplicateButton->setStyleSheet(MusicUIObject::MQSSCheckTestDuplicateClicked); break;
        case 2: m_ui->qualityButton->setStyleSheet(MusicUIObject::MQSSCheckTestQualityClicked); break;
        default: break;
    }
}
