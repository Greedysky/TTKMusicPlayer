#include "musicinputdialog.h"
#include "ui_musicinputdialog.h"

MusicInputDialog::MusicInputDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicInputDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->input->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->button->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->button->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->button, SIGNAL(clicked()), SLOT(accept()));
}

MusicInputDialog::~MusicInputDialog()
{
    delete m_ui;
}

QString MusicInputDialog::text() const
{
    return m_ui->input->editor()->text();
}

void MusicInputDialog::setPlaceholderText(const QString &text)
{
    m_ui->input->editor()->setPlaceholderText(text);
}

int MusicInputDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
