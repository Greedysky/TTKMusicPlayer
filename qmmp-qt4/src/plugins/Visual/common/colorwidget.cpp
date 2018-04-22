#include "colorwidget.h"
#include "ui_colorwidget.h"

#include <QMouseEvent>
#include <QColorDialog>

const QString MPushButtonStyle04 = " \
        QPushButton{ border:1px solid #AAAAAA; background:#FFFFFF; color:#777777;} \
        QPushButton::hover{ border:1px solid #555555; color:#444444;} \
        QPushButton::disabled{ color:#999999;}";

const QString MToolButtonStyle03 = " background-color:transparent; \
        QToolButton::hover{ background-color:rgba(255, 255, 255, 20)}";


ColorWidget::ColorWidget(QWidget *parent)
    : QDialog(parent),
      m_ui(new Ui::ColorWidget)
{
    m_ui->setupUi(this);
    m_ui->background->setStyleSheet("background:#80B7F1");

    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    setAttribute(Qt::WA_TranslucentBackground);

    m_leftButtonPress = false;

    m_ui->topTitleCloseButton->setIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton));
    m_ui->topTitleCloseButton->setStyleSheet(MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip("Close");

    m_ui->listWidget->setFrameShape(QFrame::NoFrame);
    m_ui->listWidget->setSpacing(1);

    m_ui->addButton->setStyleSheet(MPushButtonStyle04);
    m_ui->deleteButton->setStyleSheet(MPushButtonStyle04);
    m_ui->modifyButton->setStyleSheet(MPushButtonStyle04);
    m_ui->upButton->setStyleSheet(MPushButtonStyle04);
    m_ui->downButton->setStyleSheet(MPushButtonStyle04);
    m_ui->confirmButton->setStyleSheet(MPushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(MPushButtonStyle04);

    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
    connect(m_ui->modifyButton, SIGNAL(clicked()), SLOT(modifyButtonClicked()));
    connect(m_ui->upButton, SIGNAL(clicked()), SLOT(upButtonClicked()));
    connect(m_ui->downButton, SIGNAL(clicked()), SLOT(downButtonClicked()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(accept()));
}

ColorWidget::~ColorWidget()
{
    delete m_ui;
}

QList<QColor> ColorWidget::readColorConfig(const QString &value)
{
    QList<QColor> colors;
    QStringList rgbs = value.split(';', QString::SkipEmptyParts);
    foreach(const QString &rgb, rgbs)
    {
        QStringList var = rgb.split(',');
        if(var.count() != 3)
        {
            continue;
        }
        colors << QColor(var[0].toInt(), var[1].toInt(), var[2].toInt());
    }
    return colors;
}

QString ColorWidget::writeColorConfig(const QList<QColor> &colors)
{
    QString value;
    foreach(const QColor &rgb, colors)
    {
        value.append(QString("%1,%2,%3").arg(rgb.red()).arg(rgb.green()).arg(rgb.blue()));
        value.append(";");
    }
    return value;
}

void ColorWidget::setColors(const QList<QColor> &colors)
{
    foreach(const QColor &color, colors)
    {
        QListWidgetItem *it = new QListWidgetItem(m_ui->listWidget);
        it->setBackgroundColor(color);
        m_ui->listWidget->addItem(it);
    }
}

QList<QColor> ColorWidget::getColors() const
{
    QList<QColor> colors;
    for(int i=0; i<m_ui->listWidget->count(); ++i)
    {
        colors << m_ui->listWidget->item(i)->backgroundColor();
    }
    return colors;
}

void ColorWidget::addButtonClicked()
{
    QColorDialog getColor(Qt::white, this);
    if(getColor.exec())
    {
        QColor color = getColor.selectedColor();
        QListWidgetItem *it = new QListWidgetItem(m_ui->listWidget);
        it->setBackgroundColor(color);
        m_ui->listWidget->addItem(it);
    }
}

void ColorWidget::deleteButtonClicked()
{
    int index = m_ui->listWidget->currentRow();
    if(index >= 0)
    {
        delete m_ui->listWidget->takeItem(index);
    }
}

void ColorWidget::modifyButtonClicked()
{
    QColorDialog getColor(Qt::white, this);
    if(getColor.exec())
    {
        QColor color = getColor.selectedColor();
        QListWidgetItem *it = m_ui->listWidget->currentItem();
        if(it)
        {
            it->setBackgroundColor(color);
        }
    }
}

void ColorWidget::upButtonClicked()
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

void ColorWidget::downButtonClicked()
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

void ColorWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_pressAt = event->globalPos();
    if(event->button() == Qt::LeftButton)
    {
        m_leftButtonPress = true;
    }
}

void ColorWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(!m_leftButtonPress)
    {
        event->ignore();
        return;
    }

    int xpos = event->globalX() - m_pressAt.x();
    int ypos = event->globalY() - m_pressAt.y();
    m_pressAt = event->globalPos();
    move(x() + xpos, y() + ypos);
}

void ColorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_pressAt = event->globalPos();
    m_leftButtonPress = false;
}

