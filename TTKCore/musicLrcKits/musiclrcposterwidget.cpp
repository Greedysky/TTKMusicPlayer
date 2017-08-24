#include "musiclrcposterwidget.h"
#include "ui_musiclrcposterwidget.h"
#include "musicuiobject.h"

MusicLrcPosterWidget::MusicLrcPosterWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPosterWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

}

MusicLrcPosterWidget::~MusicLrcPosterWidget()
{
    delete m_ui;
}

QString MusicLrcPosterWidget::getClassName()
{
    return staticMetaObject.className();
}

int MusicLrcPosterWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
