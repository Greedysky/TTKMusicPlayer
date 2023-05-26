#include "musiclrcsearchwidget.h"
#include "ui_musiclrcsearchwidget.h"
#include "musiclrcsearchtablewidget.h"
#include "musictoastlabel.h"

MusicLrcSearchWidget::MusicLrcSearchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcSearchWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->label_checkBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->songSearchEdit->setStyleSheet(TTK::UI::LineEditStyle01);
    m_ui->lrcSearchButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->lrcSearchDownload->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->closeButton->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->label_checkBox->setFocusPolicy(Qt::NoFocus);
    m_ui->lrcSearchButton->setFocusPolicy(Qt::NoFocus);
    m_ui->lrcSearchDownload->setFocusPolicy(Qt::NoFocus);
    m_ui->closeButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->lrcSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->lrcSearchDownload->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->functionTopLabel->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::BackgroundStyle09);

    connect(m_ui->lrcSearchButton, SIGNAL(clicked()), SLOT(lrcSearchButtonClicked()));
    connect(m_ui->lrcSearchDownload, SIGNAL(clicked()), SLOT(lrcSearchDownloadClicked()));
    connect(m_ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->label_checkBox, SIGNAL(clicked(bool)), m_ui->tableWidget, SLOT(checkedItemsState(bool)));
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
    m_ui->stateLabel->setText(tr("Lrc search finished"));
}

void MusicLrcSearchWidget::lrcSearchButtonClicked() const
{
    m_ui->stateLabel->setText(tr("Lrc is searching now"));
    const QString &text = m_ui->songSearchEdit->text().trimmed();
    m_ui->tableWidget->startSearchQuery(text);
    m_ui->functionTopLabel->setText(tr("&nbsp;find <font color=#158FE1> %1 </font> result").arg(TTK::Widget::elidedText(font(), text, Qt::ElideRight, 245)));
}

void MusicLrcSearchWidget::lrcSearchDownloadClicked()
{
    TTKIntList list = m_ui->tableWidget->checkedIndexList();
    list.removeOne(m_ui->tableWidget->rowCount() - 1);

    if(list.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    for(const int row : qAsConst(list))
    {
        m_ui->tableWidget->downloadQuery(row);
    }
    m_ui->stateLabel->setText(tr("Lrc is downloading now"));
}

void MusicLrcSearchWidget::lrcDownloadStateChanged(const QString &string)
{
    if(string == DOWNLOAD_KEY_LRC)
    {
       m_ui->stateLabel->setText(tr("Lrc download finished"));
    }
}
