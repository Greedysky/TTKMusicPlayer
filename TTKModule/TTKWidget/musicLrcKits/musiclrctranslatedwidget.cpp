#include "musiclrctranslatedwidget.h"
#include "musicfunctionuiobject.h"
#include "musicwidgetutils.h"

MusicLrcTranslatedWidget::MusicLrcTranslatedWidget(QWidget *parent)
    : QLabel(parent)
{
    setObjectName(MusicLrcTranslatedWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), TTK::UI::BackgroundStyle07));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->setSpacing(0);

    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(40);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(0, 0, 10, 0);

    m_titleName = new QLabel(topWidget);
    m_titleName->setAlignment(Qt::AlignCenter);
    m_titleName->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle04 + TTK::UI::FontStyle01);
    topWidgetLayout->addWidget(m_titleName);

    QPushButton *closeButton = new QPushButton(this);
    closeButton->setFixedSize(14, 14);
    closeButton->setStyleSheet(TTK::UI::BtnPClose);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    closeButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

    topWidgetLayout->addWidget(closeButton);
    topWidget->setLayout(topWidgetLayout);

    m_plainText = new QLabel(this);
    m_plainText->setWordWrap(true);
//    m_plainText->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_plainText, false);

    m_plainText->setObjectName("ContentsWidget");
    m_plainText->setStyleSheet(QString("#%1{ %2 %3 %4 }").arg(m_plainText->objectName(), TTK::UI::BackgroundStyle01, TTK::UI::ColorStyle12, TTK::UI::FontStyle03));

    QWidget *view = scrollArea->viewport();
    view->setObjectName("Viewport");
    view->setStyleSheet(QString("#%1{ %2 }").arg(view->objectName(), TTK::UI::BackgroundStyle01));

    mainLayout->addWidget(topWidget);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

MusicLrcTranslatedWidget::~MusicLrcTranslatedWidget()
{
    delete m_titleName;
    delete m_plainText;
}

void MusicLrcTranslatedWidget::setPlainText(const QString &name, const QString &text)
{
    m_titleName->setText(name);
    m_plainText->setText(text);
}

void MusicLrcTranslatedWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcTranslatedWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcTranslatedWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcTranslatedWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
