#include "musicmessagebox.h"
#include "ui_musicmessagebox.h"
#include "musicuiobject.h"

#include <QPainter>
#include <QButtonGroup>

MusicMessageBox::MusicMessageBox(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicMessageBox)
{
    m_ui->setupUi(this);

    m_status = 0;
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

    QFont f = font();
    f.setFamily("Times New Roman");
    setFont(f);
    m_ui->textLabel->setFont(f);

    QButtonGroup *groupButton = new QButtonGroup(this);
    groupButton->addButton(m_ui->topTitleCloseButton, 0);
    groupButton->addButton(m_ui->confirmButton, 1);
    groupButton->addButton(m_ui->cancelButton, 2);
    connect(groupButton, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

}

MusicMessageBox::MusicMessageBox(const QString &text, QWidget *parent)
    : MusicMessageBox(parent)
{
    setText(text);
}

MusicMessageBox::MusicMessageBox(const QString &title, const QString &text, QWidget *parent)
    : MusicMessageBox(text, parent)
{
    setTitle(title);
}

MusicMessageBox::~MusicMessageBox()
{
    delete m_ui;
}

void MusicMessageBox::setTitle(const QString &text) const
{
    m_ui->topTitleName->setText(text);
}

QString MusicMessageBox::title() const
{
    return m_ui->topTitleName->text();
}

void MusicMessageBox::setText(const QString &text) const
{
    m_ui->textLabel->setText(text);
}

QString MusicMessageBox::text() const
{
    return m_ui->textLabel->text();
}

void MusicMessageBox::buttonClicked(int index)
{
    switch(index)
    {
        case 0:
        case 2: m_status = 0; break;
        case 1: m_status = 1; break;
        default: break;
    }
    close();
}

int MusicMessageBox::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveDialog::exec();
    return m_status;
}

void MusicMessageBox::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveDialog::show();
}
