#include "musicfoundabstractwidget.h"
#include "musicuiobject.h"

#include <QBoxLayout>

MusicFoundAbstractWidget::MusicFoundAbstractWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_mainWindow = new QWidget(this);
    m_mainWindow->setObjectName("MainWindow");
    m_mainWindow->setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MBackgroundStyle17));
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MFontStyle05 + MusicUIObject::MFontStyle01);

    QVBoxLayout *mLayout = new QVBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);
}

MusicFoundAbstractWidget::~MusicFoundAbstractWidget()
{
    while(!m_resizeWidget.isEmpty())
    {
        delete m_resizeWidget.takeLast();
    }
    delete m_statusLabel;
    delete m_mainWindow;
}

QString MusicFoundAbstractWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicFoundAbstractWidget::setSongName(const QString &name)
{
    m_songNameFull = name;
}

void MusicFoundAbstractWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicFoundAbstractWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}
