#include "musicsongdailywidget.h"
#include "musicwidgetheaders.h"
#include "musiccibarequest.h"

MusicSongDailyWidget::MusicSongDailyWidget(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle10);

    m_container = new QLabel(this);
    m_container->setScaledContents(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_container);
    setLayout(layout);

    m_title = new QLabel(this);
    m_title->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_title->move(50, 50);

    m_networkRequest = new MusicCiBaRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadRawDataChanged(QByteArray)), this, SLOT(downLoadFinished(QByteArray)));

    m_networkRequest->startToRequest();
}

MusicSongDailyWidget::~MusicSongDailyWidget()
{
    delete m_container;
    delete m_networkRequest;
}

void MusicSongDailyWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    QPixmap pix;
    pix.loadFromData(bytes);
    m_container->setPixmap(pix);

    m_title->setText(m_networkRequest->note() + TTK_LINEFEED + m_networkRequest->content());
    TTK_INFO_STREAM(m_networkRequest->note() << m_networkRequest->content());
    TTK_INFO_STREAM(m_networkRequest->note() + TTK_LINEFEED + m_networkRequest->content());
}

void MusicSongDailyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QFont font = m_title->font();
    font.setPixelSize(20 + G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() / 1000.0);
    m_title->setFont(font);
}
