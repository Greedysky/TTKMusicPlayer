#include "musiclrctranslatedwidget.h"
#include "musicfunctionuiobject.h"
#include "musicwidgetutils.h"

MusicLrcTranslatedWidget::MusicLrcTranslatedWidget(QWidget *parent)
    : QLabel(parent)
{
    setObjectName("MainWidnow");
    setStyleSheet(QString("#MainWidnow{%1}").arg(MusicUIObject::MQSSBackgroundStyle14));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(40);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(0, 0, 10, 0);

    m_titleName = new QLabel(topWidget);
    m_titleName->setAlignment(Qt::AlignCenter);
    m_titleName->setStyleSheet(MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSFontStyle04 + MusicUIObject::MQSSFontStyle01);
    topWidgetLayout->addWidget(m_titleName);

    QPushButton *closeButton = new QPushButton(this);
    closeButton->setFixedSize(14, 14);
    closeButton->setStyleSheet(MusicUIObject::MQSSBtnPClose);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    closeButton->setFocusPolicy(Qt::NoFocus);
#endif

    topWidgetLayout->addWidget(closeButton);
    topWidget->setLayout(topWidgetLayout);

    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

    m_plainText = new QLabel(this);
    m_plainText->setWordWrap(true);
    m_plainText->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, m_plainText, false);

    m_plainText->setObjectName("contentsWidget");
    m_plainText->setStyleSheet(QString("#contentsWidget{%1%2%3}").arg(MusicUIObject::MQSSBackgroundStyle01, MusicUIObject::MQSSColorStyle06, MusicUIObject::MQSSFontStyle03));

    QWidget *view = scrollArea->viewport();
    view->setObjectName("viewport");
    view->setStyleSheet(QString("#viewport{%1}").arg(MusicUIObject::MQSSBackgroundStyle01));

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
