#include "musicmessagefeedbackdialog.h"
#include "ui_musicmessagefeedbackdialog.h"
#include "musicfileutils.h"
#include "musicuiobject.h"
#include "musicmessagebox.h"

MusicMessageFeedbackDialog::MusicMessageFeedbackDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicMessageFeedbackDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->productRadioButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->fileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->programRadioButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->fileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->songRadioButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->fileButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->copyrightRadioButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->fileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->textEdit->setStyleSheet(MusicUIObject::MQSSTextEditStyle01 + MusicUIObject::MQSSScrollBarStyle01);
    m_ui->lineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    m_ui->fileButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->fileButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->productRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->programRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->songRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->copyrightRadioButton->setFocusPolicy(Qt::NoFocus);
    m_ui->fileButton->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_ui->fileButton, SIGNAL(clicked()), SLOT(openFileButtonClicked()));
}

MusicMessageFeedbackDialog::~MusicMessageFeedbackDialog()
{
    delete m_ui;
}

int MusicMessageFeedbackDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicMessageFeedbackDialog::openFileButtonClicked()
{
    m_path = MusicUtils::File::getOpenFilesDialog(this);
}

void MusicMessageFeedbackDialog::confirmButtonClicked()
{
    MusicMessageBox message;
    if((!m_ui->productRadioButton->isChecked() && !m_ui->programRadioButton->isChecked()) ||
         m_ui->textEdit->toPlainText().isEmpty() || m_ui->lineEdit->text().isEmpty() || m_path.isEmpty())
    {
        message.setText(tr("You entered is incorrect"));
        message.exec();
        return;
    }

    message.setText(tr("Operate Successfully!"));
    message.exec();

    close();
}
