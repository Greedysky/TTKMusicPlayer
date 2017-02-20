#include "musicsonglistsharingwidget.h"
#include "ui_musicsonglistsharingwidget.h"
#include "musicuiobject.h"

MusicSongListSharingWidget::MusicSongListSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongListSharingWidget)
{
    m_ui->setupUi(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->label->setStyleSheet(MusicUIObject::MBackgroundStyle01 + MusicUIObject::MFontStyle05 + \
                             MusicUIObject::MFontStyle01 + MusicUIObject::MColorStyle03);
    m_ui->outputButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->inputButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    m_ui->outputButton->setIcon(QIcon(":/toolSets/btn_kukou_output"));
    m_ui->inputButton->setIcon(QIcon(":/toolSets/btn_kukou_input"));
}

MusicSongListSharingWidget::~MusicSongListSharingWidget()
{
    delete m_ui;
}

QString MusicSongListSharingWidget::getClassName()
{
    return staticMetaObject.className();
}

int MusicSongListSharingWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
