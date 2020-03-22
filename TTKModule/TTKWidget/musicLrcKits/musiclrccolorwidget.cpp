#include "musiclrccolorwidget.h"
#include "ui_musiclrccolorwidget.h"
#include "musiccolordialog.h"
#include "musicuiobject.h"

MusicLrcColorWidget::MusicLrcColorWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcColorWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->listWidget->setFrameShape(QFrame::NoFrame);
    m_ui->listWidget->setSpacing(1);

    m_ui->addButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->modifyButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->upButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->downButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->confirmButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deleteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->modifyButton->setFocusPolicy(Qt::NoFocus);
    m_ui->upButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downButton->setFocusPolicy(Qt::NoFocus);
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
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
    foreach(const QColor &color, colors)
    {
        QListWidgetItem *it = new QListWidgetItem(m_ui->listWidget);
#if TTK_QT_VERSION_CHECK(5,13,0)
        it->setBackground(color);
#else
        it->setBackgroundColor(color);
#endif
        m_ui->listWidget->addItem(it);
    }
}

QList<QColor> MusicLrcColorWidget::getColors() const
{
    QList<QColor> colors;
    for(int i=0; i<m_ui->listWidget->count(); ++i)
    {
#if TTK_QT_VERSION_CHECK(5,13,0)
        colors << m_ui->listWidget->item(i)->background().color();
#else
        colors << m_ui->listWidget->item(i)->backgroundColor();
#endif
    }
    return colors;
}

void MusicLrcColorWidget::addButtonClicked()
{
    MusicColorDialog getColor(this);
    if(getColor.exec())
    {
        const QColor &color = getColor.color();
        QListWidgetItem *it = new QListWidgetItem(m_ui->listWidget);
#if TTK_QT_VERSION_CHECK(5,13,0)
        it->setBackground(color);
#else
        it->setBackgroundColor(color);
#endif
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
    MusicColorDialog getColor(this);
    if(getColor.exec())
    {
        const QColor &color = getColor.color();
        QListWidgetItem *it = m_ui->listWidget->currentItem();
        if(it)
        {
#if TTK_QT_VERSION_CHECK(5,13,0)
            it->setBackground(color);
#else
            it->setBackgroundColor(color);
#endif
        }
    }
}

void MusicLrcColorWidget::upButtonClicked()
{
    int index = m_ui->listWidget->currentRow();
    if(index >= 0)
    {
        QListWidgetItem *it = m_ui->listWidget->takeItem(index);
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
        if(++index >= m_ui->listWidget->count())
        {
            index = m_ui->listWidget->count();
        }
        m_ui->listWidget->insertItem(index, it);
        m_ui->listWidget->setCurrentRow(index);
    }
}

int MusicLrcColorWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
