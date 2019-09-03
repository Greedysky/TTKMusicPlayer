#include "musicsongdlnatransferwidget.h"
#include "ui_musicsongdlnatransferwidget.h"

#include "qdlna/dlnafinder.h"

MusicSongDlnaTransferWidget::MusicSongDlnaTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongDlnaTransferWidget)
{
    m_ui->setupUi(this);

    m_dlnaFinder = new DlnaFinder(this);
    connect(m_dlnaFinder, SIGNAL(finished()), SLOT(scanFinished()));
}

MusicSongDlnaTransferWidget::~MusicSongDlnaTransferWidget()
{
    delete m_dlnaFinder;
    delete m_ui;
}

void MusicSongDlnaTransferWidget::startToScan()
{
    m_dlnaFinder->find();
}

void MusicSongDlnaTransferWidget::scanFinished()
{
    m_dlnaFinder->clientNames();
}

int MusicSongDlnaTransferWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
