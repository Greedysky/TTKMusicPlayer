#include "musicmessagebox.h"
#include "ui_musicmessagebox.h"

#include <QButtonGroup>

MusicMessageBox::MusicMessageBox(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicMessageBox)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->topTitleCloseButton, 0);
    buttonGroup->addButton(m_ui->confirmButton, 1);
    buttonGroup->addButton(m_ui->cancelButton, 2);
    QtButtonGroupConnect(buttonGroup, this, buttonClicked, TTK_SLOT);
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
        case 2: reject(); break;
        case 1: accept(); break;
        default: break;
    }
}
