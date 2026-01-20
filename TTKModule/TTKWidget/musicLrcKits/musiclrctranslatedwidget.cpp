#include "musiclrctranslatedwidget.h"
#include "musicfunctionuiobject.h"
#include "musicwidgetutils.h"
#include "musictranslationrequest.h"
#include "musiclrcanalysis.h"
#include "musicgiflabelwidget.h"

MusicLrcTranslatedWidget::MusicLrcTranslatedWidget(QWidget *parent)
    : QLabel(parent),
      m_lrcAnalysis(nullptr)
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

    m_loadingLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Module::CicleBlue, this);
}

MusicLrcTranslatedWidget::~MusicLrcTranslatedWidget()
{
    delete m_titleName;
    delete m_plainText;
    delete m_loadingLabel;
}

void MusicLrcTranslatedWidget::setCurrentSongName(const QString &name)
{
    m_titleName->setText(name);
    m_loadingLabel->execute(true);

    MusicTranslationRequest *req = new MusicTranslationRequest(this);
    connect(req, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
    req->setHeader("data", m_lrcAnalysis->dataString());
    req->setHeader("name", m_lrcAnalysis->filePath());
    req->startToRequest();
}

void MusicLrcTranslatedWidget::downLoadFinished(const QString &bytes)
{
    QString text;
    m_loadingLabel->execute(false);

    if(bytes.startsWith(TTK_AUTHOR_NAME))
    {
        // The original and translated lrc have been merged
        text = bytes;
        text.remove(0, strlen(TTK_AUTHOR_NAME));
    }
    else
    {
        const QStringList &orts = m_lrcAnalysis->dataList();
        const QStringList &trts = bytes.split(TTK_LINEFEED);

        if(orts.count() > trts.count())
        {
            return;
        }

        for(int i = 0; i < orts.count(); ++i)
        {
            text += QString("<font color=#AAAAAA>%1</font><br/>").arg(orts[i].trimmed());
            text += QString("<font color=#FFFFFF>%1</font><br/>").arg(trts[i].trimmed());
        }
    }

    m_plainText->setText(text);
}

void MusicLrcTranslatedWidget::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    m_loadingLabel->move((width() - m_loadingLabel->width()) / 2, (height() - m_loadingLabel->height()) / 2);
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
