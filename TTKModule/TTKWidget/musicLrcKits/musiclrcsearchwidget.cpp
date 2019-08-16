#include "musiclrcsearchwidget.h"
#include "ui_musiclrcsearchwidget.h"
#include "musicuiobject.h"
#include "musiclrcsearchtablewidget.h"
#include "musicmessagebox.h"

MusicLrcSearchWidget::MusicLrcSearchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcSearchWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->label_checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->songSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    m_ui->lrcSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->lrcSearchDownload->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->label_checkBox->setFocusPolicy(Qt::NoFocus);
    m_ui->lrcSearchButton->setFocusPolicy(Qt::NoFocus);
    m_ui->lrcSearchDownload->setFocusPolicy(Qt::NoFocus);
    m_ui->closeButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->lrcSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->lrcSearchDownload->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->functionTopLabel->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MBackgroundStyle03);

    connect(m_ui->lrcSearchButton, SIGNAL(clicked()), SLOT(lrcSearchButtonClicked()));
    connect(m_ui->lrcSearchDownload, SIGNAL(clicked()), SLOT(lrcSearchDownloadClicked()));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->label_checkBox, SIGNAL(clicked(bool)), m_ui->tableWidget, SLOT(setSelectedAllItems(bool)));
    connect(m_ui->tableWidget, SIGNAL(resolvedSuccess()), SLOT(lrcSearchFinished()));
    connect(m_ui->tableWidget, SIGNAL(lrcDownloadStateChanged(QString)), SLOT(lrcDownloadStateChanged(QString)));
    connect(m_ui->tableWidget, SIGNAL(restartSearchQuery(QString)), SLOT(setCurrentSongName(QString)));
}

MusicLrcSearchWidget::~MusicLrcSearchWidget()
{
    delete m_ui;
}

void MusicLrcSearchWidget::setCurrentSongName(const QString &name) const
{
    if(name.isEmpty())
    {
        lrcSearchFinished();
        return;
    }
    m_ui->songSearchEdit->setText(name);
    lrcSearchButtonClicked();
}

void MusicLrcSearchWidget::lrcSearchFinished() const
{
    m_ui->stateLabel->setText(tr("lrc is searching finished!"));
}

void MusicLrcSearchWidget::lrcSearchButtonClicked() const
{
    m_ui->stateLabel->setText(tr("lrc is searching now!"));
    const QString &text = m_ui->songSearchEdit->text().trimmed();
    m_ui->tableWidget->startSearchQuery( text );
    m_ui->functionTopLabel->setText(tr("&nbsp;find <font color=#158FE1> %1 </font> result").arg(MusicUtils::Widget::elidedText(font(), text, Qt::ElideRight, 245)));
}

void MusicLrcSearchWidget::lrcSearchDownloadClicked()
{
    MIntList list = m_ui->tableWidget->getSelectedItems();
    list.removeOne(m_ui->tableWidget->rowCount() - 1);
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    foreach(const int row, list)
    {
        m_ui->tableWidget->musicDownloadLocal(row);
    }
    m_ui->stateLabel->setText(tr("lrc is downloading now!"));
}

void MusicLrcSearchWidget::lrcDownloadStateChanged(const QString &string)
{
    if(string == "DownloadLrc")
    {
       m_ui->stateLabel->setText(tr("lrc download finished!"));
    }
}

int MusicLrcSearchWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
