#include "musiclrccolorwidget.h"
#include "ui_musiclrccolorwidget.h"
#include "musiccolordialog.h"

MusicLrcColorWidget::MusicLrcColorWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcColorWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->listWidget->setSpacing(1);
    m_ui->listWidget->setFrameShape(QFrame::NoFrame);

    m_ui->addButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->modifyButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->upButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->downButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deleteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->modifyButton->setFocusPolicy(Qt::NoFocus);
    m_ui->upButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downButton->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
    connect(m_ui->modifyButton, SIGNAL(clicked()), SLOT(modifyButtonClicked()));
    connect(m_ui->upButton, SIGNAL(clicked()), SLOT(upButtonClicked()));
    connect(m_ui->downButton, SIGNAL(clicked()), SLOT(downButtonClicked()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(accept()));
}

MusicLrcColorWidget::~MusicLrcColorWidget()
{
    delete m_ui;
}

void MusicLrcColorWidget::setColors(const QList<QColor> &colors)
{
    for(const QColor &color : qAsConst(colors))
    {
        QListWidgetItem *it = new QListWidgetItem(m_ui->listWidget);
        it->setBackground(color);
        m_ui->listWidget->addItem(it);
    }
}

QList<QColor> MusicLrcColorWidget::colors() const
{
    QList<QColor> colors;
    for(int i = 0; i < m_ui->listWidget->count(); ++i)
    {
        colors << m_ui->listWidget->item(i)->background().color();
    }
    return colors;
}

void MusicLrcColorWidget::addButtonClicked()
{
    MusicColorDialog dialog(this);
    if(dialog.exec())
    {
        const QColor &color = dialog.color();
        QListWidgetItem *it = new QListWidgetItem(m_ui->listWidget);
        it->setBackground(color);
        m_ui->listWidget->addItem(it);
    }
}

void MusicLrcColorWidget::deleteButtonClicked()
{
    const int index = m_ui->listWidget->currentRow();
    if(index >= 0)
    {
        delete m_ui->listWidget->takeItem(index);
    }
}

void MusicLrcColorWidget::modifyButtonClicked()
{
    MusicColorDialog dialog(this);
    if(dialog.exec())
    {
        QListWidgetItem *it = m_ui->listWidget->currentItem();
        if(it)
        {
            it->setBackground(dialog.color());
        }
    }
}

void MusicLrcColorWidget::upButtonClicked()
{
    int index = m_ui->listWidget->currentRow();
    if(index > 0)
    {
        QListWidgetItem *it = m_ui->listWidget->takeItem(index);
        if(!it)
        {
            return;
        }

        if(--index < 0)
        {
            index = 0;
        }

        m_ui->listWidget->insertItem(index, it);
        m_ui->listWidget->setCurrentRow(index);
    }
}

void MusicLrcColorWidget::downButtonClicked()
{
    int index = m_ui->listWidget->currentRow();
    if(index >= 0)
    {
        QListWidgetItem *it = m_ui->listWidget->takeItem(index);
        if(!it)
        {
            return;
        }

        if(++index >= m_ui->listWidget->count())
        {
            index = m_ui->listWidget->count();
        }

        m_ui->listWidget->insertItem(index, it);
        m_ui->listWidget->setCurrentRow(index);
    }
}
