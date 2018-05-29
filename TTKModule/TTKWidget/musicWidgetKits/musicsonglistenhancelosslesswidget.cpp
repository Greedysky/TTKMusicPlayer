#include "musicsonglistenhancelosslesswidget.h"
#include "ui_musicsonglistenhancelosslesswidget.h"
#include "musicuiobject.h"

MusicSongListEnhanceLosslessWidget::MusicSongListEnhanceLosslessWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongListEnhanceLosslessWidget)
{
    m_ui->setupUi(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSongListEnhanceLosslessWidget::~MusicSongListEnhanceLosslessWidget()
{
    delete m_ui;
}

int MusicSongListEnhanceLosslessWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
