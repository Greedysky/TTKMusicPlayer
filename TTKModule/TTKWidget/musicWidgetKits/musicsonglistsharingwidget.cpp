#include "musicsonglistsharingwidget.h"
#include "ui_musicsonglistsharingwidget.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"
#include "musiclicenseobject.h"

#include <QClipboard>
#include <QApplication>
#include <QButtonGroup>

MusicSongListSharingWidget::MusicSongListSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongListSharingWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->mainLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01 + MusicUIObject::MQSSFontStyle05 + MusicUIObject::MQSSFontStyle01 + MusicUIObject::MQSSColorStyle03);
    m_ui->mainOutputButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->mainInputButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

    m_ui->mainOutputButton->setIcon(QIcon(":/toolSets/btn_kukou_output"));
    m_ui->mainInputButton->setIcon(QIcon(":/toolSets/btn_kukou_input"));

    m_ui->writeBackButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01 + MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSFontStyle02);
    m_ui->readBackButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01 + MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSFontStyle02);
#ifdef Q_OS_UNIX
    m_ui->mainOutputButton->setFocusPolicy(Qt::NoFocus);
    m_ui->mainInputButton->setFocusPolicy(Qt::NoFocus);
    m_ui->writeBackButton->setFocusPolicy(Qt::NoFocus);
    m_ui->readBackButton->setFocusPolicy(Qt::NoFocus);
#endif

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_ui->mainInputButton, 2);
    group->addButton(m_ui->mainOutputButton, 1);
    group->addButton(m_ui->writeBackButton, 0);
    group->addButton(m_ui->readBackButton, 0);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(switchToDiffWidget(int)));

    connect(m_ui->writeMainCopyButton, SIGNAL(clicked()), SLOT(writeMainCopyButtonClicked()));
    connect(m_ui->readMainButton, SIGNAL(clicked()), SLOT(readMainButtonClicked()));

    createWriteKey();
}

MusicSongListSharingWidget::~MusicSongListSharingWidget()
{
    delete m_ui;
}

int MusicSongListSharingWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSongListSharingWidget::switchToDiffWidget(int index)
{
    m_ui->stackedWidget->setCurrentIndex(index);
}

void MusicSongListSharingWidget::writeMainCopyButtonClicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_ui->writeMainLabel2->text());

    MusicToastLabel *toast = new MusicToastLabel(this);
    toast->defaultLabel(this, tr("Copy Finished!"));
}

void MusicSongListSharingWidget::readMainButtonClicked()
{
    close();
}

void MusicSongListSharingWidget::createWriteKey()
{
    MusicLicenseObject core;
    m_ui->writeMainLabel2->setText(core.getCharacteristicStringNormal());
}
